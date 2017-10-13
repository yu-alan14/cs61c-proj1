#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"
#include "utils.h"
#include "riscv.h"

void execute_rtype(Instruction, Processor *);
void execute_itype_except_load(Instruction, Processor *);
void execute_branch(Instruction, Processor *);
void execute_jal(Instruction, Processor *);
void execute_load(Instruction, Processor *, Byte *);
void execute_store(Instruction, Processor *, Byte *);
void execute_ecall(Processor *, Byte *);
void execute_lui(Instruction, Processor *);

void execute_instruction(Instruction instruction,Processor *processor,Byte *memory) {    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) { // What do we switch on?
        case 0x33:
            execute_rtype(instruction, processor);
            break;
        case 0x03:
            execute_load(instruction, processor, memory);
            break;
        case 0x13:
            execute_itype_except_load(instruction, processor);
            break;
        case 0x73:
            execute_ecall(processor, memory);
            break;        
        case 0x23:
            execute_store(instruction, processor, memory);
            break;
        case 0x63:
            execute_branch(instruction, processor);
            break;
        case 0x37:
            execute_lui(instruction, processor);
            break;
        case 0x6f:
            execute_jal(instruction, processor);
            break;
        default: // undefined opcode
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_rtype(Instruction instruction, Processor *processor) {
    switch(instruction.rtype.funct3) { // What do we switch on?
        /* YOUR CODE HERE */
        case 0x0:
        	switch(instruction.rtype.funct7) {
        		case 0x00:
        			;
        			int add1 = processor->R[instruction.rtype.rs1];
        			int add2 = processor->R[instruction.rtype.rs2];
        			processor->R[instruction.rtype.rd] = add1 + add2;
        			break;
        		case 0x01:
        			;
        			int mul1 = processor->R[instruction.rtype.rs1];
        			int mul2 = processor->R[instruction.rtype.rs2];
        			processor->R[instruction.rtype.rd] = (mul1 * mul2);
        			//Remember to only take lower 32 bits ?
        			break;
        		case 0x20:
        			;
        			int sub1 = processor->R[instruction.rtype.rs1];
        			int sub2 = processor->R[instruction.rtype.rs2];
        			processor->R[instruction.rtype.rd] = sub1 - sub2;
        			break;
        		default: 
        			handle_invalid_instruction(instruction);
        			exit(-1);
        			break;
        	}
        	break;
        case 0x1:
        	switch(instruction.rtype.funct7) {
        		case 0x00:
        			;
        			//left logical shift
        			unsigned int shift1 = processor->R[instruction.rtype.rs1];
        			unsigned int shift2 = processor->R[instruction.rtype.rs2];
        			processor->R[instruction.rtype.rd] = shift1 << shift2;
        			break;
        		case 0x01:
        			;
        			int mulh1 = processor->R[instruction.rtype.rs1];
        			int mulh2 = processor->R[instruction.rtype.rs2];
        			processor->R[instruction.rtype.rd] = (mulh1 * mulh2);
        			//Remember to only take upper 32 bits?
        			break;
        		default:
        			handle_invalid_instruction(instruction);
        			exit(-1);
        			break;
        	}
        	break;
        case 0x2:
        	;
        	unsigned int slt1 = processor->R[instruction.rtype.rs1];
        	unsigned int slt2 = processor->R[instruction.rtype.rs2];
        	if (slt1 < slt2) {
        		processor->R[instruction.rtype.rd] = 1;
        	} else {
        		processor->R[instruction.rtype.rd] = 0;
        	}
        	break;
   		case 0x4:
   			switch(instruction.rtype.funct7) {
   				case 0x00:
   					;
   					unsigned int xor1 = processor->R[instruction.rtype.rs1];
   					unsigned int xor2 = processor->R[instruction.rtype.rs2];
   					processor->R[instruction.rtype.rd] = xor1 * xor2;
   					break;
   				case 0x01:
   					;
   					int div1 = processor->R[instruction.rtype.rs1];
   					int div2 = processor->R[instruction.rtype.rs2];
   					processor->R[instruction.rtype.rd] = div1 / div2;
   					break;
   				default:
   					handle_invalid_instruction(instruction);
   					exit(-1);
   					break;
   			}
   			break;
   		case 0x5:
   			switch(instruction.rtype.funct7) {
   				case 0x00:
   					;
   					// logical shift right
   					unsigned int srl1 = processor->R[instruction.rtype.rs1];
   					unsigned int srl2 = processor->R[instruction.rtype.rs2];
   					processor->R[instruction.rtype.rd] = srl1 >> srl2;
   					break;
   				case 0x20:
   					;
   					// arithmetic right shift
   					unsigned int sra1 = processor->R[instruction.rtype.rs1];
   					unsigned int sra2 = processor->R[instruction.rtype.rs2];
   					processor->R[instruction.rtype.rd] = sra1 >> sra2;
   					break;
   				default:
   					handle_invalid_instruction(instruction);
   					exit(-1);
   					break;
   			}
   			break;
   		case 0x6:
   			switch(instruction.rtype.funct7) {
   				case 0x00:
   					;
   					unsigned int or1 = processor->R[instruction.rtype.rs1];
   					unsigned int or2 = processor->R[instruction.rtype.rs2];
   					processor->R[instruction.rtype.rd] = or1 | or2;
   					break;
   				case 0x01:
   					;
   					int rem1 = processor->R[instruction.rtype.rs1];
   					int rem2 = processor->R[instruction.rtype.rs2];
   					processor->R[instruction.rtype.rd] = rem1 % rem2;
   					break;
   				default:
   					handle_invalid_instruction(instruction);
   					exit(-1);
   					break;
   			}
   			break;
   		case 0x7:
   			;
   			unsigned int and1 = processor->R[instruction.rtype.rs1];
   			unsigned int and2 = processor->R[instruction.rtype.rs2];
   			processor->R[instruction.rtype.rd] = and1 & and2;
   			break;
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
    processor->PC += 4; //Checks next line of instruction, look at JAL not sure
}

void execute_itype_except_load(Instruction instruction, Processor *processor) {
    int shiftOp;
    shiftOp = -1;
    switch(instruction.itype.funct3) { // What do we switch on?
        /* YOUR CODE HERE */
        case 0x0:
        	;
        	unsigned int addi1 = processor->R[instruction.itype.rs1];
        	unsigned int addi2 = bitSigner(instruction.itype.imm, 12);
        	processor->R[instruction.itype.rd] = addi1 + addi2;
        	break;
        case 0x1:
        	;
        	unsigned int slli1 = processor->R[instruction.itype.rs1];
        	unsigned int slli2 = bitSigner(instruction.itype.imm, 12);
        	processor->R[instruction.itype.rd] = slli1 << slli2;
        	break;
        case 0x2:
        	;
        	unsigned int slti1 = processor->R[instruction.itype.rs1];
        	unsigned int slti2 = bitSigner(instruction.itype.imm, 12);
        	if (slti1 < slti2) {
        		processor->R[instruction.itype.rd] = 1;
        	} else {
        		processor->R[instruction.itype.rd] = 2;
        	}
        	break;
        case 0x4:
        	;
        	unsigned int xori1 = processor->R[instruction.itype.rs1];
        	unsigned int xori2 = bitSigner(instruction.itype.imm, 12);
        	processor->R[instruction.itype.rd] = xori1 ^ xori2;
        	break;
        case 0x5:
        	switch(instruction.itype.imm >> 5) {
        		case 0x00:
        			;
        			//logical shift right should be good??
        			unsigned int srli1 = processor->R[instruction.itype.rs1];
        			unsigned int srli2 = bitSigner(instruction.itype.imm, 12);
        			processor->R[instruction.itype.rd] = srli1 >> srli2;
        			break;
        		case 0x20:
        			;
        			//arithmetic shift left idk??
        			unsigned int srai1 = processor->R[instruction.itype.rs1];
        			unsigned int srai2 = bitSigner(instruction.itype.imm, 12);
        			processor->R[instruction.itype.rd] = srai1 >> srai2;
        			break;
        		default:
        			handle_invalid_instruction(instruction);
        			break;
        	}
        	break;
        case 0x6:
        	;
        	unsigned int ori1 = processor->R[instruction.itype.rs1];
        	unsigned int ori2 = bitSigner(instruction.itype.imm, 12);
        	processor->R[instruction.itype.rd] = ori1 | ori2;
        	break;
        case 0x7:
        	;
        	unsigned int andi1 = processor->R[instruction.itype.rs1];
        	unsigned int andi2 = bitSigner(instruction.itype.imm, 12);
        	processor->R[instruction.itype.rd] = andi1 & andi2;
        	break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
    processor->PC += 4;
}

void execute_ecall(Processor *p, Byte *memory) {
    switch(0) { // What do we switch on?
        /* YOUR CODE HERE */
        default: // undefined ecall
            printf("Illegal ecall number %d\n", -1); // What stores the ecall arg?
            exit(-1);
            break;
    }
    p->PC += 4;
}

void execute_branch(Instruction instruction, Processor *processor) {
    int branchaddr;
    branchaddr = 0;
    /* Remember that the immediate portion of branches
       is counting half-words, so make sure to account for that. */
    switch(0) { // What do we switch on?
        /* YOUR CODE HERE */
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
    processor->PC += 4;
}

void execute_load(Instruction instruction, Processor *processor, Byte *memory) {
    switch(0) { // What do we switch on?
        /* YOUR CODE HERE */
        default:
            handle_invalid_instruction(instruction);
            break;
    }
    processor->PC += 4;
}

void execute_store(Instruction instruction, Processor *processor, Byte *memory) {
    switch(0) { // What do we switch on?
        /* YOUR CODE HERE */
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
    processor->PC += 4;
}

void execute_jal(Instruction instruction, Processor *processor) {
    /* Remember that the immediate and offset are counting half-words.
	   So make sure to plan accordingly to accomodate that. */
    int nextPC;
    nextPC = 0;
    /* YOUR CODE HERE */
}

void execute_lui(Instruction instruction, Processor *processor) {
    int imm;
    imm = 0;
    /* YOUR CODE HERE */  
    processor->PC += 4;
}

/* Checks that the address is aligned correctly */
int check(Address address,Alignment alignment) {
    if(address>0 && address < MEMORY_SPACE){
        if(alignment == LENGTH_BYTE){
            return 1;
        }
        else if( alignment == LENGTH_HALF_WORD ){
            return address%2 == 0;
        }
        else if (alignment == LENGTH_WORD){
            return address%4 ==0;
        }
    }
    
    return 0;
    
}

void store(Byte *memory,Address address,Alignment alignment,Word value, int check_align) {
    if((check_align && !check(address,alignment)) || (address >= MEMORY_SPACE)) {
        handle_invalid_write(address);
    }
    /* YOUR CODE HERE */
}

Word load(Byte *memory,Address address,Alignment alignment, int check_align) {
    if((check_align && !check(address,alignment)) || (address >= MEMORY_SPACE)) {
    handle_invalid_read(address);
    }
    
    /* YOUR CODE HERE */
    uint32_t data = 0; // initialize our return value to zero
    return data;
}


