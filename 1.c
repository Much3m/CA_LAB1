#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
enum OPCODE {   // opcode + func3의 값을 Instruction을 구분하는 요소로 사용.
	ADD = 51,
	ADDI = 19,
	LW = 3,
	SW= 35,
	BEQ = 99,
	HALT = 134,
    LUI = 55  // FIXME: TODO
};

enum OPCODE2 {   // opcode + func3의 값을 Instruction을 구분하는 요소로 사용.
	ADD2 = 51,
	ADDI2 = 19,
	LW2 = 5,
	SW2= 37,
	BEQ2 = 99,
	BLT2 = 103,
	HALT2 = 134,
    LUI2 = 55  // FIXME: TODO
};
// NOTE:
// add  0110011(51), 000, 0000000       R-Type      
// addi 0010011(19), 000, n.a           I-Type       
// lw   0000011(3), 010, n.a            I-Type      
// sw   0100011(35), 010, n.a           S-Type      
// beq  1100011(99), 000, n.a           SB-Type     
// blt  1100011(99), 100, n.a           SB-Type


void print_bin(unsigned int n, int count) { // unsigned int값을 32-bit Binary로 출력하는 코드
	if (count != 0)
		print_bin(n/2,count -1);
	printf("%u",n%2);
}

void my_print_register() { 
	printf("Current Register Values \n"); // Register에 저장되어 있는 값을 출력
	int i = 0;
	while (i < 32) {
		if (registers[i] != 0)
			printf("| \033[31mx%02d : %02d \033[0m", i, registers[i]); // 0이 아닌 값이 있으면 빨간색 출력
		else
			printf("| x%02d : %02d ", i, registers[i]);
		i++;
		if (i % 8 == 0 && i != 0)
			printf("\n");
	}
	printf("Current Data_Memory Values \n"); // Data Memory에 저장되어 있는 값을 출력
	i = 0;
	while (i < 40) {
		if (data_mem[i] != 0)
			printf("| \033[31mMEM%02d : %02d \033[0m", i, data_mem[i]); // 0이 아닌 값이 있으면 빨간색 출력
		else
			printf("| MEM%02d : %02d ", i, data_mem[i]);
		i++;
		if (i % 8 == 0 && i != 0)
			printf("\n");
	}
}

// void execute_instructions(void) {
// 	// flag for end-of-program
// 	int running = 1;
//     int step = 0;
// 	printf("\033[31mInitial values\n\033[0m");
// 	my_print_register();
// 	printf("========================================================================================================\n");
// 	while (running) {
// 		//Fetch 
// 		int instruction = inst_mem[pc];
// 		int opcode = (instruction & 0x7F); //TODO
//         int funct3 = ((instruction >> 12) & 0x7); //TODO

// 		printf("\n\033[35mSTEP : %d\n",step++); // 현재 Step을 출력
//         printf("\033[36mPC : %d\n",pc); // 현재 PC값을 출력
// 		// int funct3 = (instruction >> 12) & 0x7; //TODO
// 		int rs1,    // source reg.1
// 			rs2,    // source reg.2
// 			rd,     // destination reg.
// 			imm,    // immediate
// 			addr;   // base address
		
// 		switch (opcode) {
// 			case ADD:
// 				rd = (instruction >> 7) & 0x1F; // bit shift이후 Masking하여 원하는 값 추출
// 				rs1 = (instruction >> 15) & 0x1F;
// 				rs2 = (instruction >> 20) & 0x1F;
//                 printf("\033[32madd x%02d, x%02d, x%02d\n\033[0m",rd, rs1,rs2); // Binarycode를 Assembly로 변환하여 출력
// 				registers[rd] = registers[rs1] + registers[rs2]; // add 명령어 수행
// 				pc++;
// 				break;
// 			case ADDI:
// 				rd = (instruction >> 7) & 0x1F;
// 				rs1 = (instruction >> 15) & 0x1F;
// 				imm = (instruction >> 20) & 0xFFF;
//                 if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
//                 printf("\033[32maddi x%02d, x%02d, %d\n\033[0m",rd, rs1,imm);
// 				registers[rd] = registers[rs1] + imm; // addi 명령어 수행
// 				pc++;
// 				break;
// 			case LW:
// 				rd = (instruction >> 7) & 0x1F;
// 				rs1 = (instruction >> 15) & 0x1F;
// 				imm = (instruction >> 20) & 0xFFF;
//                 if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
// 				registers[rd] = data_mem[registers[rs1] + imm/4]; // LW 명령어 수행. data mem 1개가 4바이트 덩어리기 때문에 imm 값을 4로 나눠줌.
// 				pc++;
// 				break;
//             case LUI : 
//                 rd = (instruction >> 7) & 0x1F;
//                 imm = (((instruction >> 21) & 0x3FF) << 1) | (((instruction >> 20) & 0x1) << 11) | \
//                     (((instruction >> 12) & 0xFF) << 12) | (((instruction >> 31) & 0x1) << 20);
//                 imm = imm << 11;
//                 registers[rd] = imm;
//                 pc++;
//                 break;
// 			case SW:
// 				rs1 = (instruction >> 15) & 0x1F;
// 				rs2 = (instruction >> 20) & 0x1F;
// 				imm = ((instruction >> 7) & 0x1F) | (((instruction >> 25) & 0x7F) << 5);
//                 if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
// 				printf("\033[32msw x%02d, %d(x%02d)\n\033[0m",rs2,imm,rs1);
// 				data_mem[registers[rs1] + imm/4] = registers[rs2]; // SW 명령어 수행. data mem 1-unit이 4바이트 덩어리기 때문에 imm 값을 4로 나눠줌.
// 				pc++;
// 				break;
// 			case BEQ:
// 				imm = (((instruction >> 8) & 0xF) << 1) | (((instruction >> 25) & 0x3F) << 5) |\
// 				 (((instruction >> 7) & 0x1) << 11) | (((instruction >> 31) & 0x1) << 12);
//                 if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
// 				rs1 = ((instruction >> 15) & 0x1F);
// 				rs2 = ((instruction >> 20) & 0x1F);
// 				printf("\033[32mbeq x%02d, x%02d, %d\n\033[0m",rs1,rs2,imm);
// 				if (funct3 == 0) {
//                     if(registers[rs1] == registers[rs2]) { pc += (imm)/4; } // PC값이 4byte단위기 때문에 imm<<1 값을 4로 나눠서 PC에 더해줌
// 				    else pc++;
//                 }
//                 else if (funct3 == 4) {
//                     if(registers[rs1] < registers[rs2]) { pc += (imm)/4; } // PC값이 4byte단위기 때문에 imm<<1 값을 4로 나눠서 PC에 더해줌
//                     else pc++;
//                 }
// 				break;
// 			case HALT:
// 				running = 0;
// 				printf("\033[32mEND\n\033[0m");
// 				printf(" ** END OF THE PROGRAM ** \n");
// 				break;
// 			default:
// 				printf("Unknown instruction \n");
// 				running = 0;
// 				break;
// 		}
// 		printf("\n========================================================================================================\n");
// 	}
//     my_print_register();
// }



void step_execution(void) {
	// flag for end-of-program
	int running = 1;
    int step = 0;
	printf("\033[31mInitial values\n\033[0m");
	my_print_register();
	printf("========================================================================================================\n");
	while (running) {
		//Fetch 
		int instruction = inst_mem[pc];
		int opcode; 
        if ((instruction & 0x7F) == 0b0110111)
            opcode = instruction & 0x7F;
        else
            opcode = (((instruction) & 0x7F) + ((instruction >> 12) & 0x7)); // Opcde + func3 값을 합하여 각 명령어를 구분.

		// printf("OP CODE : %d = %d + %d\n", opcode,((instruction) & 0x7F),((instruction >> 12) & 0x7));
		printf("\n\033[35mSTEP : %d\n",step++); // 현재 Step을 출력
        printf("\033[36mPC : %d\n",pc); // 현재 PC값을 출력
		// int funct3 = (instruction >> 12) & 0x7; //TODO
		int rs1,    // source reg.1
			rs2,    // source reg.2
			rd,     // destination reg.
			imm,    // immediate
			addr;   // base address
		
		switch (opcode) {
			case ADD2:
				rd = (instruction >> 7) & 0x1F; // bit shift이후 Masking하여 원하는 값 추출
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("\033[32madd x%02d, x%02d, x%02d\n\033[0m",rd, rs1,rs2); // Binarycode를 Assembly로 변환하여 출력
				printf("\033[0;33mx%02d = %d, x%02d = %d, x%02d = %d\n\033[0m",rd,registers[rd],rs1,registers[rs1],rs2,registers[rs2]); // 명령어 실행 전 Target Register의 값 출력
				registers[rd] = registers[rs1] + registers[rs2]; // add 명령어 수행
				pc++;
				printf("\033[31mResult : x%02d = %d\033[0m\n",rd,registers[rd]); // 명령어 수행 후 Target Register의 값
				break;
			case ADDI2:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32maddi x%02d, x%02d, %d\n\033[0m",rd, rs1,imm);
				printf("\033[0;33mx%02d = %d , x%02d = %d\n\033[0m",rd,registers[rd],rs1,registers[rs1]);
				registers[rd] = registers[rs1] + imm; // addi 명령어 수행
				pc++;
				printf("\033[31mResult : x%02d = %d\033[0m\n",rd,registers[rd]);
				break;
			case LW2:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32mlw x%02d ,%d(x%02d)\n\033[0m",rd,imm,rs1);
				printf("\033[0;33mx%02d = %d, data_mem[%d] = %d\033[0m\n",rd, registers[rd], registers[rs1] + imm/4, data_mem[registers[rs1] + imm/4]);
				registers[rd] = data_mem[registers[rs1] + imm/4]; // LW 명령어 수행. data mem 1개가 4바이트 덩어리기 때문에 imm 값을 4로 나눠줌.
				printf("\033[31mResult : x%02d = %d\033[0m\n",rd, registers[rd]);
				pc++;
				break;
            case LUI2 : 
                rd = (instruction >> 7) & 0x1F;
                imm = (((instruction >> 21) & 0x3FF) << 1) | (((instruction >> 20) & 0x1) << 11) | \
                    (((instruction >> 12) & 0xFF) << 12) | (((instruction >> 31) & 0x1) << 20);
                imm = imm << 11;
                printf("\033[32mlui x%02d, %d\n\033[0m",rd,imm);
                printf("\033[0;33mx%02d = %d\n",rd, registers[rd]);
                registers[rd] = imm;
                printf("\033[31mResult : x%02d = %d\033[0m\n",rd, registers[rd]);
                pc++;
                break;
			case SW2:
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				imm = ((instruction >> 7) & 0x1F) | (((instruction >> 25) & 0x7F) << 5);
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32msw x%02d, %d(x%02d)\n\033[0m",rs2,imm,rs1);
				printf("\033[0;33mx%02d = %d, data_mem[%d] = %d\n\033[0m",rs2, registers[rs2], registers[rs1] + imm/4, data_mem[registers[rs1] + imm/4]);
				data_mem[registers[rs1] + imm/4] = registers[rs2]; // SW 명령어 수행. data mem 1-unit이 4바이트 덩어리기 때문에 imm 값을 4로 나눠줌.
				printf("\033[31mResult : data_mem[%d] = %d\033[0m\n",registers[rs1] + imm/4, data_mem[registers[rs1] + imm/4]);
				pc++;
				break;
			case BEQ2:
				imm = (((instruction >> 8) & 0xF) << 1) | (((instruction >> 25) & 0x3F) << 5) |\
				 (((instruction >> 7) & 0x1) << 11) | (((instruction >> 31) & 0x1) << 12);
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				rs1 = ((instruction >> 15) & 0x1F);
				rs2 = ((instruction >> 20) & 0x1F);
				printf("\033[32mbeq x%02d, x%02d, %d\n\033[0m",rs1,rs2,imm);
				if(registers[rs1] == registers[rs2]) { pc += (imm)/4; } // PC값이 4byte단위기 때문에 imm<<1 값을 4로 나눠서 PC에 더해줌
				else pc++;
				break;
			case BLT2:
				imm = (((instruction >> 8) & 0xF) << 1) | (((instruction >> 25) & 0x3F) << 5) |\
				 (((instruction >> 7) & 0x1) << 11) | (((instruction >> 31) & 0x1) << 12);
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("\033[32mblt x%02d, x%02d, %d\n\033[0m",rs1,rs2,imm);
				if(registers[rs1] < registers[rs2]) { pc += (imm)/4; } // PC값이 4byte단위기 때문에 imm<<1 값을 4로 나눠서 PC에 더해줌
				else pc++;
				break;
			case HALT2:
				running = 0;
				printf("\033[32mEND\n\033[0m");
				printf(" ** END OF THE PROGRAM ** \n");
				break;
			default:
				printf("Unknown instruction \n");
				running = 0;
				break;
		}
		printf("\n");
		my_print_register();
		printf("\n========================================================================================================\n");
	}
}

void parse_instructions(void) { // execute_instructions 함수와 유사하지만, 연산을 수행하지 않고 모든 명령어를 순회하며 Assembly를 출력하는 함수.
	int temp = 0;
	int running = 1;
	while (running) {
		int instruction = inst_mem[temp];
		// print_bin(instruction,31);
		// printf("\n");
		int opcode; 
        if ((instruction & 0x7F) == 0b0110111)
            opcode = instruction & 0x7F;
        else
            opcode = (((instruction) & 0x7F) + ((instruction >> 12) & 0x7)); // Opcde + func3 값을 합하여 각 명령어를 구분.
		int rs1,    // source reg.1
			rs2,    // source reg.2
			rd,     // destination reg.
			imm,    // immediate
			addr;   // base address

		switch (opcode) {
			case ADD2:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("\033[32madd x%02d, x%02d, x%02d\n\033[0m",rd, rs1,rs2); // Assembly 출력.
				temp++;
				break;
			case ADDI2:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32maddi x%02d, x%02d, %d\n\033[0m",rd, rs1,imm);
				temp++;
				break;
			case LW2:
				rd = (instruction >> 7) & 0x1F;
				rs1 = (instruction >> 15) & 0x1F;
				imm = (instruction >> 20) & 0xFFF;
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32mlw x%02d, %d(x%02d)\n\033[0m",rd,imm,rs1);
				temp++;
				break;
            case LUI2 : 
                rd = (instruction >> 7) & 0x1F;
                imm = (((instruction >> 21) & 0x3FF) << 1) | (((instruction >> 20) & 0x1) << 11) | \
                    (((instruction >> 12) & 0xFF) << 12) | (((instruction >> 31) & 0x1) << 20);
                imm = imm << 11;
                printf("\033[32mlui x%02d, %d\n\033[0m",rd,imm);
                temp++;
                break;
			case SW2:
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				imm = ((instruction >> 7) & 0x1F) | (((instruction >> 25) & 0x7F) << 5);
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32msw x%02d, %d(x%02d)\n\033[0m",rs2,imm,rs1);
				temp++;
				break;
			case BEQ2:
				imm = (((instruction >> 8) & 0xF) << 1) | (((instruction >> 25) & 0x3F) << 5) |\
				 (((instruction >> 7) & 0x1) << 11) | (((instruction >> 31) & 0x1) << 12); 
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				printf("\033[32mbeq x%02d, x%02d, %d\n\033[0m",rs1,rs2,(imm));
				temp++; // 조건문과 무관하게 PC를 1 증가 시킴
				break;
			case BLT2:
				imm = (((instruction >> 8) & 0xF) << 1) | (((instruction >> 25) & 0x3F) << 5) |\
				 (((instruction >> 7) & 0x1) << 11) | (((instruction >> 31) & 0x1) << 12);
                if (((instruction >> 31) & 0x1) == 0x1) { imm = (imm | 0xFFFFF000); }
				rs1 = (instruction >> 15) & 0x1F;
				rs2 = (instruction >> 20) & 0x1F;
				printf("\033[32mblt x%02d, x%02d, %d\n\033[0m",rs1,rs2,(imm));
				temp++; // 조건문과 무관하게 PC를 1 증가 시킴
				break;
			case HALT2:
				running = 0;
				printf("\033[32mEND\n\033[0m");
				break;
			default:
				printf("\033[32mUnknown instruction \n");
				running = 0;
				break;
		}
		printf("\n");
	}
}

int main() {
	////////     수정금지 영역 시작 (프로그램 B의 주석을 해제하는 수정만 가능)    ///////////
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
	inst_mem[0] = 0b00000000000001000010110000100011; //sw zero, 24(x8)
    inst_mem[1] = 0b00000000000001000010101000100011; //sw zero, 20(x8)
    inst_mem[2] = 0b00000001010001000010011100000011; //lw x14, 20(x8) : curr i
    inst_mem[3] = 0b00000000000000000000011110110111; //lui x15, 0
    inst_mem[4] = 0b00000000010001111000011110010011;  //addi x15, x15, 4
    inst_mem[5] = 0b00000010111001111100000001100011; //blt x15, x14, 32 : 4 < i 
    inst_mem[6] = 0b00000001100001000010011110000011; //lw x15, 24(x8)
    inst_mem[7] = 0b00000000010001111000011110010011; //addi x15, x15, 4
    inst_mem[8] = 0b00000000111101000010110000100011; //sw x15, 24(x8)
    inst_mem[9] = 0b00000001010001000010011110000011; //lw x15, 20(x8)
    inst_mem[10] = 0b00000000000101111000011110010011; //addi x15, x15, 1
    inst_mem[11] = 0b00000000111101000010101000100011; //sw x15, 20(x8)
    inst_mem[12] = 0b11111100000000000000110011100011; //beq zero, zero, -36
    inst_mem[13] = 0b00000001100001000010011110000011; //lw x15, 24(x8)
    inst_mem[14] = 0b00000000101001111000011110010011; //addi x15, x15, 10
    inst_mem[15] = 0b00000000111101000010111000100011; //sw x15, 28(x8)
    inst_mem[16] = 0b11111111111111111111111111111111; //END

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
	execute_instructions();
	return 0;
}
