#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define MASK 0xFF // architecture 8-bit

uint8_t x = 0b00001011;       // num: 11
uint8_t y = 0b00000111;       // num: 7

// CPU strutcure
typedef struct
{
    // CPU Registers
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    
    // Special Registers
    uint8_t PC;        // Program Counter
    uint8_t SP;        // Stack Pointer
    
    // Flags
    uint8_t z;
    uint8_t CARRY;
    uint8_t SIGN;
    uint8_t OVERFLOW;
    
    // Memory
    uint8_t memory[256]; // 256 bytes of data
} CPU;

// AlU structure
typedef struct
{
    uint8_t result;
    
    uint8_t zero;
    uint8_t carry;
    uint8_t sign;
    uint8_t overflow;
} ALUResult;

// ALU functions
typedef enum
{
    ALU_ADD, ALU_SUB, ALU_DIV,               // Operations
    ALU_MOD, ALU_MUL, ALU_INC, ALU_DEC,             
    ALU_AND, ALU_OR, ALU_XOR, ALU_NOT,       // Logic Operators
    ALU_SHL, ALU_SHR,                        // Shift Logic
    ALU_RAND                                 // Special
} ALU0p;



// CPU functions [DEPRECATED]
// Operations
uint8_t ADD(uint8_t x, uint8_t y) {return (x + y) & MASK;}               // ADD: sum a bin code 
uint8_t SUB(uint8_t x, uint8_t y) {return (x - y) & MASK;}               // SUB: subtract a bin code
uint8_t DIV(uint8_t x, uint8_t y) {return y ? ((x / y) & MASK) : 0;}     // SIV: divide a bin code
uint8_t MOD(uint8_t x, uint8_t y) {return y ? ((x % y) & MASK) : 0;}     // MOD: calculate divide result 
uint8_t MUL(uint8_t x, uint8_t y) {return (x * y) & MASK;}               // MUL: multiplicate a bin code

uint8_t INC(uint8_t n) {return (n+1) & MASK;}                            // INC: increment a bin code (+1)
uint8_t DEC(uint8_t n) {return (n-1) & MASK;}                            // DEC: decrese a bin code (-1)

// Logic Operators
uint8_t AND(uint8_t x, int y) {return x & y;}                            // AND: gives 1 if both bits are 1
uint8_t OR(uint8_t x, int y) {return x | y;}                             // OR: bit* if one 1 return 1 else 0
uint8_t XOR(uint8_t x, int y) {return x ^ y;}                            // XOR: if (x == y) {0}
uint8_t NOT(uint8_t n) {return (~n) & MASK;}                             // NOT: inverts every bit

// Shift Logic
uint8_t SHL(uint8_t n) {return (n << 1) & MASK;}                         // SHL: shift bytes left deleating offscreen ones
uint8_t SHR(uint8_t n) {return (n >> 1) & MASK;}                         // SHR: shift bytes right deleating offscreen ones

// Special Logic
uint8_t RAND() {return (uint8_t)(rand() & MASK);}                        // RAND: random number based on the seed


// ALU function
ALUResult alu(uint8_t A, uint8_t B, ALU0p opcode)
{
    ALUResult r = {0};
    switch (opcode)
    {
        // Operations
        case ALU_ADD: {uint16_t temp = (uint16_t)A + (uint16_t)B;
            r.result = temp & MASK; r.carry = temp > MASK;
            r.overflow = (~(A ^ B) & (A ^ r.result) & 0x80) != 0; break;}    // ADD operation
        case ALU_SUB: {r.result = A - B; r.carry = A < B;
            r.overflow = ((A ^ B) & (A ^ r.result) & 0x80) != 0; break;}     // SUB operation
        case ALU_DIV: {if (B != 0) {r.result = A / B;}
            else {r.result = 0;} break;}                                     // DIV operation
        case ALU_MOD: {if (B != 0) {r.result = A % B;}
            else {r.result = 0;} break;}                                     // MOD operation
        case ALU_MUL: {uint16_t temp = (uint16_t)A * (uint16_t)B;
            r.result = temp & MASK; r.carry = temp > MASK; break;}           // MUL operation
        case ALU_INC: {uint16_t temp = (uint16_t)A + 1;
            r.result = temp & MASK; r.carry = temp > MASK; break;}           // INC operation
        case ALU_DEC: {r.result = A - 1; r.carry = A == 0; break;}           // DEC operation
        
        // Logic Operators
        case ALU_AND: {r.result = A & B; break;}                             // AND operator
        case ALU_OR: {r.result = A | B; break;}                              // OR operator
        case ALU_XOR: {r.result = A ^ B; break;}                             // XOR operator
        case ALU_NOT: {r.result = ~A; break;}                                // NOT operator
        
        // Shift Logic
        case ALU_SHL: {r.carry = (A & 0x80) != 0;
            r.result = A << 1; break;}                                       // SHL logic
        case ALU_SHR: {r.carry = (A & 0x01) != 0;
            r.result = A >> 1; break;}                                       // SHR logic
            
        // Special
        case ALU_RAND: {r.result = (uint8_t)(rand() & MASK);
        r.carry = 0; r.overflow = 0; break;}                                 // RAND special logic
    }

    r.zero = (r.result == 0);
    r.sign = (r.result & 0x80) != 0;
    
    return r;
}



// Deprecated testing function
int deprecated()
{
    printf("add: %u\n", ADD(x, y)); // so: (11 + 7) == (0b1011 + 0b0111)
    printf("sub: %u\n", SUB(x, y));
    printf("div: %u\n", DIV(x, y));
    printf("mod: %u\n", MOD(x, y));
    printf("mul: %u\n", MUL(x, y));
    
    printf("inc: %u\n", INC(x));
    printf("dec: %u\n", DEC(x));
    
    printf("and: %u\n", AND(x, y));
    printf("or: %u\n", OR(x, y));
    printf("xor: %u\n", XOR(x, y));
    printf("not: %u\n", NOT(x));
    
    printf("shl: %u\n", SHL(x));
    printf("shr: %u\n", SHR(x));

    printf("rand: %u\n", RAND());
    return 0;
}

// Main testing function
//int main()
int test()
{
    printf("DEPRECATED:\n");
    deprecated(); // print deprecated CPU functions
    printf("\nALU:\n");
    ALUResult r = alu(x, y, ALU_ADD); // run ALU commands
    printf("%u\n", r.result);
    
    return 0;
}
