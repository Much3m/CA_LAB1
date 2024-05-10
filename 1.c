#include <stdio.h>
#include <math.h>
#include <stdlib.h>
void print_register_values();
void my_print_register();

// registers
unsigned int registers[32];

// program counter
unsigned int pc = 0;

// instruction/data memory
#define INST_MEM_SIZE 32*1024
#define DATA_MEM_SIZE 32*1024
unsigned int inst_mem[INST_MEM_SIZE]; //instruction memory
unsigned int data_mem[DATA_MEM_SIZE]; //data memory

// example instruction set
enum OPCODE {    
	ADD = 51,
	ADDI = 19,
	LW = 5,
	SW = 37,
	BEQ = 99,
	BLT = 103,
	HALT = 134  // FIXME: TODO
};
// NOTE:
// add  0110011(51), 000, 0000000       R-Type      
// addi 0010011(19), 000, n.a           I-Type       
// lw   0000011(3), 010, n.a            I-Type      
// sw   0100011(35), 010, n.a           S-Type      
// beq  1100011(99), 000, n.a           SB-Type     
// blt  1100011(99), 100, n.a           SB-Type

void print_binary(unsigned int n, int count) {
	if (count != 0)
		print_binary(n/2,count -1);
	printf("%u",n%2);
}


void parse_instructions(void) {
	int temp = 0;
	int running = 1;
	while (running) {
		int instruction = inst_mem[temp];
		print_binary(instruction,31);
		printf("\n");
		int opcode = (((instruction) & 0x7F) + ((instruction >> 12) & 0x7)); 
		int rs1,    // source reg.1
			rs2,    // source reg.2
			rd,     // destination reg.
			imm,    // immediate
			addr;   // base address

		switch (opcode) {
			case ADD:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("ADD Instruction\nrd = %d, rs1 = %d, rs2 = %d\n",rd,rs1,rs2);
				temp++;
				break;
			case ADDI:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
				printf("ADDI Type Instruction\n rd = %d, rs1 = %d, imm = %d\n",rd,rs1,imm);
				temp++;
				break;
			case LW:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
				printf("LW Type Instruction\n rd = %d, rs1 = %d, imm = %d\n",rd,rs1,imm);
				temp++;
				break;
			case SW:
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				imm = ((instruction >> 7) & 0x1F) + (((instruction >> 25) & 0x7F) << 5);
				printf("SW Type Instruction\n rs1 = %d, imm = %d\n",rs1,imm);
				temp++;
				break;
			case BEQ:
				imm = ((instruction >> 8) & 0xF) + (((instruction >> 25) & 0x3F) << 4) +\
				 (((instruction >> 7) & 0x1) << 10) + (((instruction >> 31) & 0x1) << 11); 
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("BEQ Type Instruction\n rs1 = %d, rs2 = %d, imm = %d \n",rs1,rs2,imm);
				temp++;
				break;
			case BLT:
				imm = ((instruction >> 8) & 0xF) + (((instruction >> 25) & 0x3F) << 4) +\
				 (((instruction >> 7) & 0x1) << 10) + (((instruction >> 31) & 0x1) << 11); 
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("BLT Type Instruction\n rs1 = %d, rs2 = %d, imm = %d \n",rs1,rs2,imm);
				temp++;
				break;
			case HALT:
				running = 0;
				printf(" ** END OF THE PROGRAM ** \n");
				break;
			default:
				printf("Unknown instruction \n");
				running = 0;
				break;
		}
		printf("\n\n");
	}



}

int main() {
	////////     수정금지 영역 시작 (프로그램B의 주석을 해제하는 수정만 가능)    ///////////
	// Instructions for Program A
	// ISA: ADD, ADDI, LW, BEQ, HALT
	// inst_mem[0] = 0b00000000011000101000001110110011; 
	// inst_mem[1] = 0b00000000010100101000001010010011; 
	// inst_mem[2] = 0b00000000010100101000001100110011; 
	// inst_mem[3] = 0b00000000011000101000001110110011; 
	// inst_mem[4] = 0b00010000010100110000000001100011; 
	// inst_mem[5] = 0b00000000000010010010001100000011; 
	// inst_mem[6] = 0b11111111111111111111111111111111; // END

	// // Instructions for Program B 
	// ISA: ADD, ADDI, LW, BEQ, HALT + (SW, BLT)
	inst_mem[0] = 0b00000000000001000010110000100011; 
	inst_mem[1] = 0b00000000000001000010101000100011; 
	inst_mem[2] = 0b00000001010001000010011100000011; 
	inst_mem[3] = 0b00000000010001111000011110010011; 
	inst_mem[4] = 0b00000010111101110100000001100011; 
	inst_mem[5] = 0b00000001100001000010011110000011; 
	inst_mem[6] = 0b00000000010001111000011110010011; 
	inst_mem[7] = 0b00000000111101000010110000100011; 
	inst_mem[8] = 0b00000001010001000010011110000011; 
	inst_mem[9] = 0b00000000000101111000011110010011; 
	inst_mem[10] = 0b00000000111101000010101000100011;
	inst_mem[11] = 0b11111100000000000000111011100011;
	inst_mem[12] = 0b00000001100001000010011110000011;
	inst_mem[13] = 0b00000000101001111000011110010011;
	inst_mem[14] = 0b00000000111101000010111000100011;
	inst_mem[15] = 0b11111111111111111111111111111111; //END
	
	// EXIT

	// Given Data for Program A and B
	data_mem[0] = 0b00000000000000000000000000000000; // 0
	data_mem[1] = 0b00000000000000000000000000000001; // 1
	data_mem[2] = 0b00000000000000000000000000000010; // 2
	data_mem[3] = 0b00000000000000000000000000000011; // 3
	data_mem[4] = 0b00000000000000000000000000000100; // 4
	data_mem[5] = 0b00000000000000000000000000000101; // 5
	data_mem[6] = 0b00000000000000000000000000000110; // 6
	data_mem[7] = 0b00000000000000000000000000000111; // 7 

	// Given Register values for Program A and B
	registers[5] = 10;
	registers[6] = 20;
	registers[7] = 30;

	////////               수정금지 영역 끝             ///////////
	// parse_instructions();

	// flag for end-of-program
	int running = 1;
	my_print_register(); printf("\n\n");

	while (running) {
		//Fetch 
		int instruction = inst_mem[pc];
		int opcode = (((instruction) & 0x7F) + ((instruction >> 12) & 0x7)); 

		// printf("OP CODE : %d = %d + %d\n", opcode,((instruction) & 0x7F),((instruction >> 12) & 0x7));
		printf("\033[36mPC : %d\n",pc);
		// int funct3 = (instruction >> 12) & 0x7; //TODO
		int rs1,    // source reg.1
			rs2,    // source reg.2
			rd,     // destination reg.
			imm,    // immediate
			addr;   // base address
		
		switch (opcode) {
			case ADD:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				// printf("ADD Type Instruction\n rd = %d, rs1 = %d, rs2 = %d\n",rd,rs1,rs2);
				printf("\033[32mDecoded Instruction : add x%02d, x%02d, x%02d\n\033[0m",rd, rs1,rs2);
				printf("Before add Type Instruction\nx%02d = %d , x%02d = %d , x%02d = %d\n",rd,registers[rd],rs1,registers[rs1],rs2,registers[rs2]);
				registers[rd] = registers[rs1] + registers[rs2];
				pc++;
				printf("After add Type Instruction\nx%02d = %d\n",rd,registers[rd]);
				break;
			case ADDI:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
				printf("\033[32mDecoded Instruction : addi x%02d, x%02d, %d\n\033[0m",rd, rs1,imm);
				printf("Before addi Type Instruction\nx%02d = %d , x%02d = %d\n",rd,registers[rd],rs1,registers[rs1]);
				registers[rd] = registers[rs1] + imm;
				pc++;
				printf("After addi Type Instruction\nx%02d = %d\n",rd,registers[rd]);
				break;
			case LW:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
				printf("\033[32mDecoded Instruction : lw x%02d ,%d(x%02d)\n\033[0m",rd,imm,rs1);
				printf("Before LW Type Instruction\n x%02d = %d, data_mem[%d] = %d\n",rd, registers[rd], registers[rs1] + imm/4, data_mem[registers[rs1] + imm/4]);
				registers[rd] = data_mem[registers[rs1] + imm/4];
				printf("After LW Type Result\nx%02d = %d\n",rd, registers[rd]);
				pc++;
				break;
			case SW:
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				imm = ((instruction >> 7) & 0x1F) + (((instruction >> 25) & 0x7F) << 5);
				printf("\033[32mDecoded Instruction : sw x%02d ,%d(x%02d)\n\033[0m",rs2,imm,rs1);
				printf("Before SW Type Instruction\n x%02d = %d, data_mem[%d] = %d\n",rs2, registers[rs2], registers[rs1] + imm/4, data_mem[registers[rs1] + imm/4]);
				data_mem[registers[rs1] + imm/4] = registers[rs2];
				printf("After SW Type Result\ndata_mem[%d] = %d\n",registers[rs1] + imm/4, data_mem[registers[rs1] + imm/4]);
				pc++;
				break;
			case BEQ:
				imm = ((instruction >> 8) & 0xF) + (((instruction >> 25) & 0x3F) << 4) +\
				 (((instruction >> 7) & 0x1) << 10) + (((instruction >> 31) & 0x1) << 11); 
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("\033[32mDecoded Instruction : beq x%02d , x%02d, %d\n\033[0m",rs1,rs2,(imm<<1)/4);
				if(registers[rs1] == registers[rs2]) { pc += (imm<<1)/4;}
				else pc++;
				// pc++;
				break;
			case BLT:
				imm = ((instruction >> 8) & 0xF) + (((instruction >> 25) & 0x3F) << 4) +\
				 (((instruction >> 7) & 0x1) << 10) + (((instruction >> 31) & 0x1) << 11); 
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("\033[32mDecoded Instruction : blt x%02d , x%02d, %d\n\033[0m",rs1,rs2,(imm<<1)/4);
				if(registers[rs1] < registers[rs2]) { pc += (imm<<1)/4;}
				else pc++;
				// pc++;
				break;
			case HALT:
				running = 0;
				printf("\033[32mDecoded Instruction : Halt\n\033[0m");
				printf(" ** END OF THE PROGRAM ** \n");
				break;
			default:
				printf("Unknown instruction \n");
				running = 0;
				break;
		}
		printf("\n\n");
		my_print_register();
		printf("\n\n");
	}

	// Print final register values
	
	return 0;
}

void print_register_values(){
	printf("Register values:\n");
	int i;
	for (i = 0; i < 32; i++) {
		printf("x%02d: %d\n", i, registers[i]);
	}
}

void my_print_register() { 
	printf("Current Register Values \n");
	int i = 0;
	while (i < 32) {
		if (registers[i] != 0)
			printf("| \033[31mx%02d : %02d \033[0m", i, registers[i]);
		else
			printf("| x%02d : %02d ", i, registers[i]);
		i++;
		if (i % 8 == 0 && i != 0)
			printf("\n");
	}
	printf("Current Data_Memory Values \n");
	i = 0;
	while (i < 40) {
		if (data_mem[i] != 0)
			printf("| \033[31mMEM%02d : %02d \033[0m", i, data_mem[i]);
		else
			printf("| MEM%02d : %02d ", i, data_mem[i]);
		i++;
		if (i % 8 == 0 && i != 0)
			printf("\n");
	}
}
