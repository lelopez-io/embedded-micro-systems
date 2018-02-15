/* ======================================================================== */
/* DSP_maxidx_c.h -- Maximum Vector Value                                   */
/*                   Driver Code Implementation                             */
/*                                                                          */
/* Rev 0.0.1                                                                */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#include "DSP_maxidx_cn.h"
#include "DSP_maxidx.h"

/* kernel specific input arrays */
#pragma DATA_ALIGN(Input, 8);
/* implementation specific output arrays */
#pragma DATA_ALIGN(Max_c, 8);
#pragma DATA_ALIGN(Max_i, 8);

#define N (128)
#define nInputs (N)
#define PAD (8)

short Input[N] =
{
  0x429e, 0x4aab, 0x2444, 0x0589, 0x0bbd, 0xfc91, 0xb1a7, 0xbae7,
  0xcb1c, 0xf4a0, 0xe041, 0xa21e, 0x0e65, 0x4db2, 0xeff4, 0x05cf,
  0xed66, 0x3e8a, 0xe11c, 0x1292, 0xe176, 0x0529, 0xe129, 0xe628,
  0x19f5, 0x038b, 0x34f2, 0x2b68, 0x34e1, 0x118a, 0xcf36, 0x2d18,
  0x0c28, 0x346f, 0x0cc0, 0xb5f4, 0xcdfa, 0x307a, 0xd0df, 0x96d2,
  0xc3b8, 0x11f7, 0xd42c, 0xbe45, 0x0154, 0x2fae, 0xc9cc, 0x5ae8,
  0x0c9b, 0xe65d, 0x2135, 0x1877, 0xef80, 0x5e91, 0xf4b0, 0xb56c,
  0x0121, 0x3be5, 0x0bdc, 0x2a7c, 0xfa5e, 0xdbcd, 0xb379, 0xb1f0,
  0x1035, 0x166b, 0x0175, 0x4545, 0x38ce, 0xed13, 0x7fad, 0xda9f,
  0x2062, 0x004a, 0x1226, 0xbb35, 0xb1ef, 0xdaaa, 0xfe8b, 0xbabd,
  0x9ebb, 0xe52b, 0x096e, 0xfc4e, 0xfa24, 0xce40, 0xe887, 0xe5ee,
  0xf511, 0x1f03, 0x21c4, 0xc635, 0x412a, 0x41c2, 0xe4dc, 0xd6fd,
  0xd448, 0xe2cf, 0x0cbe, 0xee51, 0xff33, 0xe038, 0xa22a, 0x4f86,
  0xdaac, 0x098a, 0xf195, 0xd26a, 0xd377, 0xbf09, 0x1f2e, 0x926c,
  0x232e, 0x539c, 0x3fae, 0x1c5d, 0xf8b3, 0xde14, 0xf1cb, 0x0037,
  0x1551, 0xe461, 0x0a4c, 0x33fe, 0x201f, 0x1ec0, 0x1422, 0x26e8,
};

/* results, padded with guard to show array overflow */
int Max_c;
int Max_i;

clock_t time_c(int n);
clock_t time_i(int n);

/****************************************************************************/
/* TOP LEVEL DRIVER FOR THE TEST.                                           */
/****************************************************************************/
int main()
{
    clock_t t_overhead, t_start, t_stop, t_c, t_i;
    int i, n, fail = 0;    /* failure status */

   /*************************************************************************/
   /* COMPUTE THE OVERHEAD OF CALLING CLOCK TWICE TO GET TIMING INFO.       */
   /*************************************************************************/
    t_start = clock();
    t_stop = clock();
    t_overhead = t_stop - t_start;

    for(n = 32, i = 1; n <= N; i++, n += 16) {
        Max_c = 0x7373;
        Max_i = 0x7373;

        /********************************************************************/
        /* CALL THE INDIVIDIUAL TIMING ROUTINES.                            */
        /********************************************************************/
        t_c = time_c(n) - t_overhead;
        t_i = time_i(n) - t_overhead;

        /********************************************************************/
        /* PRINT TIMING RESULTS                                             */
        /********************************************************************/ 
        printf("DSP_maxidx\tIter#: %d\t", i);

        /********************************************************************/
        /* CHECK THE RESULTS ARRAY.                                         */
        /********************************************************************/
        if (Max_i != Max_c) {
            fail++;
            printf("Result Failure (max_i)");
        }
        else
            printf("Result Successful (max_i)");

        printf("\tN = %d\tnatC: %d\tintC: %d\n", n, t_c, t_i);
    }

    return (fail);
}

/****************************************************************************/
/* COMPUTE THE ELAPSED TIME OF THE COMPILED NATURAL C ROUTINE               */
/****************************************************************************/
clock_t time_c(int n)
{
    clock_t t_start, t_stop;

    t_start = clock();
    Max_c = DSP_maxidx_cn(Input, n);
    t_stop = clock();
    return t_stop - t_start;
}

/****************************************************************************/
/* COMPUTE THE ELAPSED TIME OF THE COMPILED INTRINSIC C ROUTINE             */
/****************************************************************************/
clock_t time_i(int n)
{
    clock_t t_start, t_stop;

    t_start = clock();
    Max_i = DSP_maxidx(Input, n);
    t_stop = clock();
    return t_stop - t_start;
}

/****************************************************************************/
/*                  END OF DRIVER FILE                                      */
/****************************************************************************/

