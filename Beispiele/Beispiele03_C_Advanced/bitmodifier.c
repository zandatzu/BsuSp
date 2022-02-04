#include <stdio.h>

/* bitmodifier.c */
typedef union {
    struct {
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char reserved:4;
    } bits;
    unsigned char byte;
} HW_Register;

int main() {
   HW_Register reg;

    reg.byte = 0;    /* init */

   reg.bits.b1 = 1; /* set bit 1 */
   reg.bits.b3 = 1; /* set bit 3 */

   printf("reg=%x\n", reg.byte);
   return 0;
}
