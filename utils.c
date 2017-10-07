#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

//sign extends a bitfield with given size
/* You may find implementing this function helpful */
int bitSigner( unsigned int field, unsigned int size){
    /* YOUR CODE HERE */
    if ((field >> (size - 1)) == 1) {
        return (int)((field << (32- size) >> (32-size)) | (0xFFFFFFFF << (32-size)));
    }
    return (int)(field << (32-size) >> (32-size));
}

int get_branch_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned eleventh = instruction.sbtype.imm5 << 31 >> 31 << 10;
    unsigned twelth = instruction.sbtype.imm7 << 25 >> 31 << 11;
    unsigned onetofour = instruction.sbtype.imm5 << 27 >> 28;
    unsigned fivetoten = instruction.sbtype.imm7 << 26 >> 22;
    return (twelth | eleventh | fivetoten | onetofour);
}

int get_jump_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    return bitSigner(instruction.ujtype.imm, 20);
}

int get_store_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    return bitSigner((instruction.stype.imm7 << 5) | instruction.stype.imm5, 12);
}

void handle_invalid_instruction(Instruction instruction) {
    printf("Invalid Instruction: 0x%08x\n", instruction.bits); 
}

void handle_invalid_read(Address address) {
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}

