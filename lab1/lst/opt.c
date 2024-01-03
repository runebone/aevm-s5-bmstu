#define len 9
#define enroll 2
#define elem_sz 4

unsigned _x[] = { 1, 2, 3, 4, 8, 6, 7, 5, 4 };

void _start() {
    unsigned *x1 = _x; // la x1, _x
    unsigned *x20 = x1 + len; // addi x20, x1, elem_sz*len
    unsigned x31 = x1[0]; // lw x31, 0(x1)
    x1 += 1; // addi x1, x1, elem_sz*1
    
    do {
        // lp
        unsigned x2 = x1[0]; // lw x2, 0(x1)
        unsigned x3 = x1[1]; // lw x3, 4(x1)

        x1 += enroll; // add x1, x1, elem_sz*enroll

        if (x2 < x31) { // bltu x2, x31, lt1
        } else {
            x31 = x2; // add x31, x0, x2
        }
        // lt1
        if (x3 < x31) { // bltu x3, x31, lt2
        } else {
            x31 = x3; // add x31, x0, x3
        }
        // lt2
    } while (x1 != x20); // bne x1, x20, lp

    for (;;); // lp2: j lp2
}
