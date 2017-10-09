#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

//sign extends a bitfield with given size
/* You may find implementing this function helpful */
int bitSigner( unsigned int field, unsigned int size){
    /* YOUR CODE HERE */
    if ((field >> (size - 1)) == 1) {
        return (int)((field << (32-size) >> (32-size)) | (0xFFFFFFFF << size));
    }
    return (int)(field << (32-size) >> (32-size));
}

/* Remember that the offsets should return the offset in BYTES */

int get_branch_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int eleventh = instruction.sbtype.imm5 << 31 >> 31 << 11;
    unsigned int twelth = instruction.sbtype.imm7 << 25 >> 31 << 12;
    unsigned int onetofour = instruction.sbtype.imm5 >> 1 << 1;
    unsigned int fivetoten = instruction.sbtype.imm7 << 26 >> 26 << 5;
    unsigned int immediate = (twelth | eleventh | fivetoten | onetofour);
    return immediate;  
}

int get_jump_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int twenty = instruction.ujtype.imm << 12 >> 31 << 20;
    unsigned int onetoten = instruction.ujtype.imm >> 9 << 21 >> 21 << 1;
    unsigned int eleven = instruction.ujtype.imm << 23 >> 31 << 11;
    unsigned int twelvetonineteen = instruction.ujtype.imm << 24 >> 24 << 12;
    unsigned int immediate = (twenty | twelvetonineteen | eleven | onetoten);;
    return immediate;
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

