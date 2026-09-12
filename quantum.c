#include "cpu_structure.c"
#include <stdint.h>

// Qubit
typedef struct
{
    // 0 and 1 probs
    float probx;
    float proby;

    uint8_t result; // Final result
    float random;   // Random number
} QUbit;

// QUbit function
QUbit probs(float x, float y)
{
    QUbit p = {0};
    p.probx = x; p.proby = y;

    ALUResult r2 = alu(0, 0, ALU_RAND); // Gerenate the random number using the ALU function
    p.random = (float)r2.result / 256.0f;
    p.result = (p.random < p.probx)? 0 : 1;

    printf("ramdom: %f\n", p.random);

    return p;
}

// Run testing programm
int main()
{
    srand((unsigned)time(NULL)); // Random number (seed = time)

    QUbit prbs = probs(0.7f, 0.3f);
    printf("probx: %f\nproby: %f\nresult: %d", prbs.probx, prbs.proby, prbs.result);

    return 0;
}
