/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  DSPLIB  DSP Signal Processing Library                                   */
/*                                                                          */
/*  This library contains proprietary intellectual property of Texas        */
/*  Instruments, Inc.  The library and its source code are protected by     */
/*  various copyrights, and portions may also be protected by patents or    */
/*  other legal protections.                                                */
/*                                                                          */
/*  This software is licensed for use with Texas Instruments TMS320         */
/*  family DSPs.  This license was provided to you prior to installing      */
/*  the software.  You may review this license by consulting the file       */
/*  TI_license.PDF which accompanies the files in this library.             */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      DSP_fft16x16_imre -- fft16x16_imre                                  */
/*                                                                          */
/*      USAGE                                                               */
/*           This routine is C-callable and can be called as:               */
/*                                                                          */
/*          void fft16x16_imre (                                            */
/*              const short * ptr_w,                                        */
/*              int npoints,                                                */
/*              short * ptr_x,                                              */
/*              short * ptr_y                                               */
/*          );                                                              */
/*                                                                          */
/*            ptr_w   =  input twiddle factors                              */
/*            npoints =  number of points                                   */
/*            ptr_x   =  transformed data reversed                          */
/*            ptr_y   =  linear transformed data                            */
/*                                                                          */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The following code performs a mixed radix FFT for "npoints" which   */
/*      is either a multiple of 4 or 2. It uses logN4 - 1 stages of radix4  */
/*      transform and performs either a radix2 or radix4 transform on the   */
/*      last stage depending on "npoints". If "npoints" is a multiple of 4, */
/*      then this last stage is also a radix4 transform, otherwise it is a  */
/*      radix2 transform.                                                   */
/*                                                                          */
/*   input to this routine is in Imaginary/Real order instead of the        */
/*      traditional Real/Imaginary order.                                   */
/*                                                                          */
/* int gen_twiddle_fft16x16(short *w, int n)                                */
/*                                                                          */
/*    int i, j, k;                                                          */
/*     double M = 32767.5;                                                  */
/*                                                                          */
/*    for (j = 1, k = 0; j < n >> 2; j = j << 2)                            */
/*    {                                                                     */
/*        for (i = 0; i < n >> 2; i += j << 1)                              */
/*        {                                                                 */
/*                                                                          */
/*          w[k + 11] = d2s(M * cos(6.0 * PI * (i + j) / n));               */
/*          w[k + 10] = -d2s(M * sin(6.0 * PI * (i + j) / n));              */
/*          w[k +  9] = d2s(M * cos(6.0 * PI * (i    ) / n));               */
/*          w[k +  8] = -d2s(M * sin(6.0 * PI * (i    ) / n));              */
/*                                                                          */
/*          w[k +  7] = d2s(M * cos(4.0 * PI * (i + j) / n));               */
/*          w[k +  6] = -d2s(M * sin(4.0 * PI * (i + j) / n));              */
/*          w[k +  5] = d2s(M * cos(4.0 * PI * (i    ) / n));               */
/*          w[k +  4] = -d2s(M * sin(4.0 * PI * (i    ) / n));              */
/*                                                                          */
/*          w[k +  3] = d2s(M * cos(2.0 * PI * (i + j) / n));               */
/*          w[k +  2] = -d2s(M * sin(2.0 * PI * (i + j) / n));              */
/*          w[k +  1] = d2s(M * cos(2.0 * PI * (i    ) / n));               */
/*          w[k +  0] = -d2s(M * sin(2.0 * PI * (i    ) / n));              */
/*                                                                          */
/*          k += 12;                                                        */
/*                                                                          */
/*                                                                          */
/*      }                                                                   */
/*    }                                                                     */
/*                                                                          */
/*    return k;                                                             */
/*                                                                          */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      This code works for  both "npoints" a multiple of 2 or 4.           */
/*      The arrays 'x[]', 'y[]', and 'w[]' all must be aligned on a         */
/*      double-word boundary for the "optimized" implementations.           */
/*                                                                          */
/*      The input and output data are complex, with the real/imaginary      */
/*      components stored in adjacent locations in the array.  The real     */
/*      components are stored at even array indices, and the imaginary      */
/*      components are stored at odd array indices.                         */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The following C code represents an implementation of the Cooley     */
/*      Tukey radix 4 DIF FFT. It accepts the inputs in normal order and    */
/*      produces the outputs in digit reversed order. The natural C code    */
/*      shown in this file on the other hand, accepts the inputs in nor-    */
/*      mal order and produces the outputs in normal order.                 */
/*                                                                          */
/*      Several transformations have been applied to the original Cooley    */
/*      Tukey code to produce the natural C code description shown here.    */
/*      In order to understand these it would first be educational to       */
/*      understand some of the issues involved in the conventional Cooley   */
/*      Tukey FFT code.                                                     */
/*                                                                          */
/*      void radix4(int n, short x[], short wn[])                           */
/*      {                                                                   */
/*          int    n1,  n2,  ie,   ia1,  ia2, ia3;                          */
/*          int    i0,  i1,  i2,    i3,    i, j,     k;                     */
/*          short  co1, co2, co3,  si1,  si2, si3;                          */
/*          short  xt0, yt0, xt1,  yt1,  xt2, yt2;                          */
/*          short  xh0, xh1, xh20, xh21, xl0, xl1,xl20,xl21;                */
/*                                                                          */
/*          n2 = n;                                                         */
/*          ie = 1;                                                         */
/*          for (k = n; k > 1; k >>= 2)                                     */
/*          {                                                               */
/*              n1 = n2;                                                    */
/*              n2 >>= 2;                                                   */
/*              ia1 = 0;                                                    */
/*                                                                          */
/*              for (j = 0; j < n2; j++)                                    */
/*              {                                                           */
/*                   ia2 = ia1 + ia1;                                       */
/*                   ia3 = ia2 + ia1;                                       */
/*                                                                          */
/*                   co1 = wn[2 * ia1    ];                                 */
/*                   si1 = wn[2 * ia1 + 1];                                 */
/*                   co2 = wn[2 * ia2    ];                                 */
/*                   si2 = wn[2 * ia2 + 1];                                 */
/*                   co3 = wn[2 * ia3    ];                                 */
/*                   si3 = wn[2 * ia3 + 1];                                 */
/*                   ia1 = ia1 + ie;                                        */
/*                                                                          */
/*                   for (i0 = j; i0< n; i0 += n1)                          */
/*                   {                                                      */
/*                       i1 = i0 + n2;                                      */
/*                       i2 = i1 + n2;                                      */
/*                       i3 = i2 + n2;                                      */
/*                                                                          */
/*                                                                          */
/*                       xh0  = x[2 * i0    ] + x[2 * i2    ];              */
/*                       xh1  = x[2 * i0 + 1] + x[2 * i2 + 1];              */
/*                       xl0  = x[2 * i0    ] - x[2 * i2    ];              */
/*                       xl1  = x[2 * i0 + 1] - x[2 * i2 + 1];              */
/*                                                                          */
/*                       xh20 = x[2 * i1    ] + x[2 * i3    ];              */
/*                       xh21 = x[2 * i1 + 1] + x[2 * i3 + 1];              */
/*                       xl20 = x[2 * i1    ] - x[2 * i3    ];              */
/*                       xl21 = x[2 * i1 + 1] - x[2 * i3 + 1];              */
/*                                                                          */
/*                       x[2 * i0    ] = xh0 + xh20;                        */
/*                       x[2 * i0 + 1] = xh1 + xh21;                        */
/*                                                                          */
/*                       xt0  = xh0 - xh20;                                 */
/*                       yt0  = xh1 - xh21;                                 */
/*                       xt1  = xl0 + xl21;                                 */
/*                       yt2  = xl1 + xl20;                                 */
/*                       xt2  = xl0 - xl21;                                 */
/*                       yt1  = xl1 - xl20;                                 */
/*                                                                          */
/*                       x[2 * i1    ] = (xt1 * co1 + yt1 * si1) >> 15;     */
/*                       x[2 * i1 + 1] = (yt1 * co1 - xt1 * si1) >> 15;     */
/*                       x[2 * i2    ] = (xt0 * co2 + yt0 * si2) >> 15;     */
/*                       x[2 * i2 + 1] = (yt0 * co2 - xt0 * si2) >> 15;     */
/*                       x[2 * i3    ] = (xt2 * co3 + yt2 * si3) >> 15;     */
/*                       x[2 * i3 + 1] = (yt2 * co3 - xt2 * si3) >> 15;     */
/*                   }                                                      */
/*             }                                                            */
/*                                                                          */
/*             ie <<= 2;                                                    */
/*         }                                                                */
/*     }                                                                    */
/*                                                                          */
/*      The conventional Cooley Tukey FFT, is written using three loops.    */
/*      The outermost loop "k" cycles through the stages. There are log     */
/*      N to the base 4 stages in all. The loop "j" cycles through the      */
/*      groups of butterflies with different twiddle factors, loop "i"      */
/*      reuses the twiddle factors for the different butterflies within     */
/*      a stage. It is interesting to note the following:                   */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*      Stage#     #Groups     # Butterflies with common     #Groups*Bflys  */
/*                               twiddle factors                            */
/* ------------------------------------------------------------------------ */
/*       1         N/4          1                            N/4            */
/*       2         N/16         4                            N/4            */
/*       ..                                                                 */
/*       logN      1            N/4                          N/4            */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/*      The following statements can be made based on above observations:   */
/*                                                                          */
/*      a) Inner loop "i0" iterates a veriable number of times. In          */
/*      particular the number of iterations quadruples every time from      */
/*      1..N/4. Hence software pipelining a loop that iterates a vraiable   */
/*      number of times is not profitable.                                  */
/*                                                                          */
/*      b) Outer loop "j" iterates a variable number of times as well.      */
/*      However the number of iterations is quartered every time from       */
/*      N/4 ..1. Hence the behaviour in (a) and (b) are exactly opposite    */
/*      to each other.                                                      */
/*                                                                          */
/*      c) If the two loops "i" and "j" are colaesced together then they    */
/*      will iterate for a fixed number of times namely N/4. This allows    */
/*      us to combine the "i" and "j" loops into 1 loop. Optimized impl-    */
/*      ementations will make use of this fact.                             */
/*                                                                          */
/*      In addition the Cooley Tukey FFT accesses three twiddle factors     */
/*      per iteration of the inner loop, as the butterflies that re-use     */
/*      twiddle factors are lumped together. This leads to accessing the    */
/*      twiddle factor array at three points each sepearted by "ie". Note   */
/*      that "ie" is initially 1, and is quadrupled with every iteration.   */
/*      Therfore these three twiddle factors are not even contiguous in     */
/*      the array.                                                          */
/*                                                                          */
/*      In order to vectorize the FFT, it is desirable to access twiddle    */
/*      factor array using double word wide loads and fetch the twiddle     */
/*      factors needed. In order to do this a modified twiddle factor       */
/*      array is created, in which the factors WN/4, WN/2, W3N/4 are        */
/*      arranged to be contiguous. This eliminates the seperation between   */
/*      twiddle factors within a butterfly. However this implies that as    */
/*      the loop is traversed from one stage to another, that we maintain   */
/*      a redundant version of the twiddle factor array. Hence the size     */
/*      of the twiddle factor array increases as compared to the normal     */
/*      Cooley Tukey FFT.  The modified twiddle factor array is of size     */
/*      "2 * N" where the conventional Cooley Tukey FFT is of size"3N/4"    */
/*      where N is the number of complex points to be transformed. The      */
/*      routine that generates the modified twiddle factor array was        */
/*      presented earlier. With the above transformation of the FFT,        */
/*      both the input data and the twiddle factor array can be accessed    */
/*      using double-word wide loads to enable packed data processing.      */
/*                                                                          */
/*      The final stage is optimised to remove the multiplication as        */
/*      w0 = 1.  This stage also performs digit reversal on the data,       */
/*      so the final output is in natural order.                            */
/*                                                                          */
/*      The fft() code shown here performs the bulk of the computation      */
/*      in place. However, because digit-reversal cannot be performed       */
/*      in-place, the final result is written to a separate array, y[].     */
/*                                                                          */
/*      The actual twiddle factors for the FFT are cosine, - sine. The      */
/*      twiddle factors stored in the table are csine and sine, hence       */
/*      the sign of the "sine" term is comprehended during multipli-        */
/*      cation as shown above.                                              */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      The optimized implementations are written for LITTLE ENDIAN.        */
/*                                                                          */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#pragma CODE_SECTION(DSP_fft16x16_imre_i, ".text:intrinsic");

#include "DSP_fft16x16_imre.h"

#if 0
# define DIG_REV(i, m, j) ((j) = (_shfl(_rotl(_bitr(_deal(i)), 16)) >> (m)))
#else
# define DIG_REV(i, m, j)                                                   \
    do {                                                                    \
        unsigned _ = (i);                                                   \
        _ = ((_ & 0x33333333) <<  2) | ((_ & ~0x33333333) >>  2);           \
        _ = ((_ & 0x0F0F0F0F) <<  4) | ((_ & ~0x0F0F0F0F) >>  4);           \
        _ = ((_ & 0x00FF00FF) <<  8) | ((_ & ~0x00FF00FF) >>  8);           \
        _ = ((_ & 0x0000FFFF) << 16) | ((_ & ~0x0000FFFF) >> 16);           \
        (j) = _ >> (m);                                                     \
    } while (0)
#endif

static inline void radix_2(short *restrict ptr_x, short *restrict ptr_y, int npoints);
static inline void radix_4(short *restrict ptr_x, short *restrict ptr_y, int npoints);

void DSP_fft16x16_imre_i (
    const short * restrict ptr_w,
    int npoints,
    short * restrict ptr_x,
    short * restrict ptr_y
)
{
    const short * restrict w;
    const short * restrict w2;
    short * restrict x, * restrict x2, * restrict x0;

    int i, j, l1, l2, h2;
    int predj, tw_offset, stride, fft_jmp, radix;
    int xh0_0_xh1_0, xh0_1_xh1_1, xl0_0_xl1_0, xl0_1_xl1_1;
    int xh20_0_xh21_0, xh20_1_xh21_1, xl20_0_xl21_0, xl20_1_xl21_1;

    int x_1o_x_0o, x_3o_x_2o, mxt0_0_myt0_0, mxt0_1_myt0_1;
    int xl1_0_xl0_0, xl1_1_xl0_1, yt2_0_xt1_0, yt2_1_xt1_1, yt1_0_xt2_0;
    int yt1_1_xt2_1, xt1_0_yt1_0, xt1_1_yt1_1, xt2_0_yt2_0, xt2_1_yt2_1;
    int xl1_1_0, xl1_3_2, xh2_1_0, xh2_3_2, xl2_1_0, xl2_3_2;

    double co11si11_co10si10, co21si21_co20si20, co31si31_co30si30;
    double x_2301, x_l1_23_x_l1_01, x_l2_23_x_l2_01, x_h2_23_x_h2_01;

    long long xh0_0_xh1_0_xl0_0_xl1_0, xh0_1_xh1_1_xl0_1_xl1_1;
    long long xh20_0_xh21_0_xl20_0_xl21_0, xh20_1_xh21_1_xl20_1_xl21_1;
    long long yt2_0_xt1_0_yt1_0_xt2_0, yt2_1_xt1_1_yt1_1_xt2_1;
    long long xt1_0_yt1_0_xt2_0_yt2_0, xt1_1_yt1_1_xt2_1_yt2_1;

    /*---------------------------------------------------------------------*/
    /* Determine the magnitude od the number of points to be transformed.  */
    /* Check whether we can use a radix4 decomposition or a mixed radix    */
    /* transformation, by determining modulo 2.                            */
    /*---------------------------------------------------------------------*/
    radix = _norm(npoints) & 1 ? 2 : 4;

    /*----------------------------------------------------------------------*/
    /* The stride is quartered with every iteration of the outer loop. It   */
    /* denotes the seperation between any two adjacent inputs to the butter */
    /* -fly. This should start out at N/4, hence stride is initially set to */
    /* N. For every stride, 6*stride twiddle factors are accessed. The      */
    /* "tw_offset" is the offset within the current twiddle factor sub-     */
    /* table. This is set to zero, at the start of the code and is used to  */
    /* obtain the appropriate sub-table twiddle pointer by offseting it     */
    /* with the base pointer "ptr_w".                                       */
    /*----------------------------------------------------------------------*/
    stride = npoints;
    tw_offset = 0;
    fft_jmp = 6 * stride;

    _nassert(stride > 4);
    #pragma MUST_ITERATE(1,,1);
    while (stride > 4) {
        /*-----------------------------------------------------------------*/
        /* At the start of every iteration of the outer loop, "j" is set   */
        /* to zero, as "w" is pointing to the correct location within the  */
        /* twiddle factor array. For every iteration of the inner loop     */
        /* 6 * stride twiddle factors are accessed. For eg,                */
        /*                                                                 */
        /* #Iteration of outer loop  # twiddle factors    #times cycled    */
        /*  1                          6 N/4               1               */
        /*  2                          6 N/16              4               */
        /*  ...                                                            */
        /*-----------------------------------------------------------------*/
        j = 0;
        fft_jmp >>= 2;

        /*-----------------------------------------------------------------*/
        /* Set up offsets to access "N/4", "N/2", "3N/4" complex point or  */
        /* "N/2", "N", "3N/2" half word                                    */
        /*-----------------------------------------------------------------*/
        h2 = stride >> 1;
        l1 = stride;
        l2 = stride + (stride >> 1);

        /*-----------------------------------------------------------------*/
        /*  Reset "x" to point to the start of the input data array.       */
        /* "tw_offset" starts off at 0, and increments by "6 * stride"     */
        /*  The stride quarters with every iteration of the outer loop     */
        /*-----------------------------------------------------------------*/
        x = ptr_x;
        w = ptr_w + tw_offset;
        tw_offset += fft_jmp;
        stride >>= 2;

        /*----------------------------------------------------------------*/
        /* The following loop iterates through the different butterflies, */
        /* within a given stage. Recall that there are logN to base 4     */
        /* stages. Certain butterflies share the twiddle factors. These   */
        /* are grouped together. On the very first stage there are no     */
        /* butterflies that share the twiddle factor, all N/4 butter-     */
        /* flies have different factors. On the next stage two sets of    */
        /* N/8 butterflies share the same twiddle factor. Hence after     */
        /* half the butterflies are performed, j the index into the       */
        /* factor array resets to 0, and the twiddle factors are reused.  */
        /* When this happens, the data pointer 'x' is incremented by the  */
        /* fft_jmp amount. In addition the following code is unrolled to  */
        /* perform "2" radix4 butterflies in parallel.                    */
        /*----------------------------------------------------------------*/
        _nassert(npoints >= 16);
        #pragma MUST_ITERATE(2,,2);
        for (i = 0; i < (npoints >> 3); i++) {
            /*------------------------------------------------------------*/
            /* Read the first 12 twiddle factors, six of which are used   */
            /* for one radix 4 butterfly and six of which are used for    */
            /* next one.                                                  */
            /*------------------------------------------------------------*/
            w2 = w + j;

            // si10 = w[j+0];    si11 = w[j+2];
            // co10 = w[j+1];    co11 = w[j+3];
            co11si11_co10si10 = _amemd8_const(w2);

            // si20 = w[j+4];    co21 = w[j+7];
            // si21 = w[j+6];    co20 = w[j+5];
            co21si21_co20si20 = _amemd8_const(&w2[4]);

            // si30 = w[j+8];    co31 = w[j+11];
            // co30 = w[j+9];    si31 = w[j+10];
            co31si31_co30si30 = _amemd8_const(&w2[8]);

            /*------------------------------------------------------------*/
            /* Read in the first complex input for the butterflies.       */
            /* 1st complex input to 1st butterfly: x[0] + jx[1]           */
            /* 1st complex input to 2nd butterfly: x[2] + jx[3]           */
            /*------------------------------------------------------------*/

            // x_1 = x[0]; Im    x_2 = x[3]; Re
            // x_0 = x[1]; Re    x_3 = x[2]; Im
            x_2301 = _amemd8(&x[0]);

            /*------------------------------------------------------------*/
            /* Read in the complex inputs for the butterflies. Each of the*/
            /* successive complex inputs of the butterfly are seperated   */
            /* by a fixed amount known as stride. The stride starts out   */
            /* at N/4, and quarters with every stage.                     */
            /*------------------------------------------------------------*/

            // x_l1_1 = x[l1  ]; Im    x_l1_3 = x[l1+2]; Im
            // x_l1_0 = x[l1+1]; Re    x_l1_2 = x[l1+3]; Re
            x_l1_23_x_l1_01 = _amemd8(&x[l1]);

            // x_l2_1 = x[l2  ]; Im    x_l2_3 = x[l2+2]; Im
            // x_l2_0 = x[l2+1]; Re    x_l2_2 = x[l2+3]; Re
            x_l2_23_x_l2_01 = _amemd8(&x[l2]);

            // x_h2_1 = x[h2  ]; Im    x_h2_3 = x[h2+2]; Im
            // x_h2_0 = x[h2+1]; Re    x_h2_2 = x[h2+3]; Re
            x_h2_23_x_h2_01 = _amemd8(&x[h2]);

            /*-----------------------------------------------------------*/
            /* Two butterflies are evaluated in parallel. The following  */
            /* results will be shown for one butterfly only, although    */
            /* both are being evaluated in parallel.                     */
            /*-----------------------------------------------------------*/

            // xh0_0 = x_0 + x_l1_0;    xl0_0 = x_0 - x_l1_0;
            // xh1_0 = x_1 + x_l1_1;    xl1_0 = x_1 - x_l1_1;
            xh0_0_xh1_0_xl0_0_xl1_0 = _addsub2(_lo(x_2301), _lo(x_l1_23_x_l1_01));

            // xh0_1 = x_2 + x_l1_2;    xl0_1 = x_2 - x_l1_2;
            // xh1_1 = x_3 + x_l1_3;    xl1_1 = x_3 - x_l1_3;
            xh0_1_xh1_1_xl0_1_xl1_1 = _addsub2(_hi(x_2301), _hi(x_l1_23_x_l1_01));

            // xh20_0 = x_h2_0 + x_l2_0;    xl20_0 = x_h2_0 - x_l2_0;
            // xh21_0 = x_h2_1 + x_l2_1;    xl21_0 = x_h2_1 - x_l2_1;
            xh20_0_xh21_0_xl20_0_xl21_0 = _addsub2(_lo(x_h2_23_x_h2_01), _lo(x_l2_23_x_l2_01));

            // xh20_1 = x_h2_2 + x_l2_2;    xl20_1 = x_h2_2 - x_l2_2;
            // xh21_1 = x_h2_3 + x_l2_3;    xl21_1 = x_h2_3 - x_l2_3;
            xh20_1_xh21_1_xl20_1_xl21_1 = _addsub2(_hi(x_h2_23_x_h2_01), _hi(x_l2_23_x_l2_01));

            xh0_0_xh1_0 = _hill(xh0_0_xh1_0_xl0_0_xl1_0);
            xh0_1_xh1_1 = _hill(xh0_1_xh1_1_xl0_1_xl1_1);
            xl0_0_xl1_0 = _loll(xh0_0_xh1_0_xl0_0_xl1_0);
            xl0_1_xl1_1 = _loll(xh0_1_xh1_1_xl0_1_xl1_1);

            xh20_0_xh21_0 = _hill(xh20_0_xh21_0_xl20_0_xl21_0);
            xh20_1_xh21_1 = _hill(xh20_1_xh21_1_xl20_1_xl21_1);
            xl20_0_xl21_0 = _loll(xh20_0_xh21_0_xl20_0_xl21_0);
            xl20_1_xl21_1 = _loll(xh20_1_xh21_1_xl20_1_xl21_1);

            /*-----------------------------------------------------------*/
            /* Derive output pointers using the input pointer "x"        */
            /*-----------------------------------------------------------*/
            x0 = x;
            x2 = x0;

            /*-----------------------------------------------------------*/
            /* When the twiddle factors are not to be re-used, j is      */
            /* incremented by 12, to reflect the fact that 12 half words */
            /* are consumed in every iteration. The input data pointer   */
            /* increments by 4. Note that within a stage, the stride     */
            /* does not change and hence the offsets for the other three */
            /* legs, 0, h2, l1, l2.                                      */
            /*-----------------------------------------------------------*/
            j += 12;
            x += 4;

            predj = (j - fft_jmp);
            if (!predj) x += fft_jmp;
            if (!predj) j = 0;

            /*----------------------------------------------------------*/
            /* These four partial results can be re-written to show     */
            /* the underlying DIF structure similar to radix2 as        */
            /* follows:                                                 */
            /*                                                          */
            /* X(4k)  = (x(n)+x(n + N/2)) + (x(n+N/4)+ x(n + 3N/4))     */
            /* X(4k+1)= (x(n)-x(n + N/2)) -j(x(n+N/4) - x(n + 3N/4))    */
            /* x(4k+2)= (x(n)+x(n + N/2)) - (x(n+N/4)+ x(n + 3N/4))     */
            /* X(4k+3)= (x(n)-x(n + N/2)) +j(x(n+N/4) - x(n + 3N/4))    */
            /*                                                          */
            /* which leads to the real and imaginary values as follow:  */
            /*                                                          */
            /* y0r = x0r + x2r +  x1r +  x3r    =  xh0 + xh20           */
            /* y0i = x0i + x2i +  x1i +  x3i    =  xh1 + xh21           */
            /* y1r = x0r - x2r + (x1i -  x3i)   =  xl0 + xl21           */
            /* y1i = x0i - x2i - (x1r -  x3r)   =  xl1 - xl20           */
            /* y2r = x0r + x2r - (x1r +  x3r)   =  xh0 - xh20           */
            /* y2i = x0i + x2i - (x1i +  x3i    =  xh1 - xh21           */
            /* y3r = x0r - x2r - (x1i -  x3i)   =  xl0 - xl21           */
            /* y3i = x0i - x2i + (x1r -  x3r)   =  xl1 + xl20           */
            /* ---------------------------------------------------------*/

            /*----------------------------------------------------------*/
            /* "y0x0" and "y1x1" are the first four outputs of the      */
            /*  radix4 butterfly. These are computed on the C64x        */
            /* using _avg2.                                             */
            /*----------------------------------------------------------*/
            // x0[0] = (xh1_0 + xh21_0 + 1) >> 1; Im
            // x0[1] = (xh0_0 + xh20_0 + 1) >> 1; Re
            // x0[2] = (xh1_1 + xh21_1 + 1) >> 1; Im
            // x0[3] = (xh0_1 + xh20_1 + 1) >> 1; Re
            x_1o_x_0o = _avg2(xh0_0_xh1_0, xh20_0_xh21_0);
            x_3o_x_2o = _avg2(xh0_1_xh1_1, xh20_1_xh21_1);

            // xt0_0 = xh0_0 - xh20_0;    yt0_0 = xh1_0 - xh21_0;
            // xt0_1 = xh0_1 - xh20_1;    yt0_1 = xh1_1 - xh21_1;
            mxt0_0_myt0_0 = _sub2(xh0_0_xh1_0, xh20_0_xh21_0);
            mxt0_1_myt0_1 = _sub2(xh0_1_xh1_1, xh20_1_xh21_1);

            xl1_0_xl0_0 = _rotl(xl0_0_xl1_0, 16);
            xl1_1_xl0_1 = _rotl(xl0_1_xl1_1, 16);

            // yt2_0 = xl1_0 + xl20_0;    yt1_0 = xl1_0 - xl20_0;
            // xt1_0 = xl0_0 + xl21_0;    xt2_0 = xl0_0 - xl21_0;
            yt2_0_xt1_0_yt1_0_xt2_0 = _addsub2(xl1_0_xl0_0, xl20_0_xl21_0);

            // yt2_1 = xl1_1 + xl20_1;    yt1_1 = xl1_1 - xl20_1;
            // xt1_1 = xl0_1 + xl21_1;    xt2_1 = xl0_1 - xl21_1;
            yt2_1_xt1_1_yt1_1_xt2_1 = _addsub2(xl1_1_xl0_1, xl20_1_xl21_1);

            yt2_0_xt1_0 = _hill(yt2_0_xt1_0_yt1_0_xt2_0);
            yt2_1_xt1_1 = _hill(yt2_1_xt1_1_yt1_1_xt2_1);
            yt1_0_xt2_0 = _loll(yt2_0_xt1_0_yt1_0_xt2_0);
            yt1_1_xt2_1 = _loll(yt2_1_xt1_1_yt1_1_xt2_1);

            xt1_0_yt1_0_xt2_0_yt2_0 = _dpackx2(yt1_0_xt2_0, yt2_0_xt1_0);
            xt1_1_yt1_1_xt2_1_yt2_1 = _dpackx2(yt1_1_xt2_1, yt2_1_xt1_1);

            xt1_0_yt1_0 = _hill(xt1_0_yt1_0_xt2_0_yt2_0);
            xt1_1_yt1_1 = _hill(xt1_1_yt1_1_xt2_1_yt2_1);
            xt2_0_yt2_0 = _loll(xt1_0_yt1_0_xt2_0_yt2_0);
            xt2_1_yt2_1 = _loll(xt1_1_yt1_1_xt2_1_yt2_1);

            /*---------------------------------------------------------*/
            /* Perform twiddle factor multiplies of three terms,top    */
            /* term does not have any multiplies. Note the twiddle     */
            /* factors for a normal FFT are C + j (-S). Since the      */
            /* factors that are stored are C + j S, this is            */
            /* corrected for in the multiplies.                        */
            /*                                                         */
            /* Y1 = (xt1 + jyt1) (c + js) = (xc + ys) + (yc -xs)       */
            /*                                                         */
            /* Following equations REQUIRE SIN = -SIN in twiddle       */
            /* table                                                   */
            /*---------------------------------------------------------*/

            // x2[l1  ] = (-co20 * yt0_0 - si20 * xt0_0 + 0x8000) >> 16; //Im
            // x2[l1+1] = (-co20 * xt0_0 + si20 * yt0_0 + 0x8000) >> 16; //Re
            // x2[l1+2] = (-co21 * yt0_1 - si21 * xt0_1 + 0x8000) >> 16; //Im
            // x2[l1+3] = (-co21 * xt0_1 + si21 * yt0_1 + 0x8000) >> 16; //Re
            xl1_1_0 = _cmpyr(mxt0_0_myt0_0, _sub2(0x00000000, _lo(co21si21_co20si20)));
            xl1_3_2 = _cmpyr(mxt0_1_myt0_1, _sub2(0x00000000, _hi(co21si21_co20si20)));

            // x2[h2  ] = (co10 * yt1_0 + si10 * xt1_0 + 0x8000) >> 16;
            // x2[h2+1] = (co10 * xt1_0 - si10 * yt1_0 + 0x8000) >> 16;
            // x2[h2+2] = (co11 * yt1_1 + si11 * xt1_1 + 0x8000) >> 16;
            // x2[h2+3] = (co11 * xt1_1 - si11 * yt1_1 + 0x8000) >> 16;
            xh2_1_0 = _cmpyr(xt1_0_yt1_0, _lo(co11si11_co10si10));
            xh2_3_2 = _cmpyr(xt1_1_yt1_1, _hi(co11si11_co10si10));

            // x2[l2  ] = (co30 * yt2_0 + si30 * xt2_0 + 0x8000) >> 16;
            // x2[l2+1] = (co30 * xt2_0 - si30 * yt2_0 + 0x8000) >> 16;
            // x2[l2+2] = (co31 * yt2_1 + si31 * xt2_1 + 0x8000) >> 16;
            // x2[l2+3] = (co31 * xt2_1 - si31 * yt2_1 + 0x8000) >> 16;
            xl2_1_0 = _cmpyr(xt2_0_yt2_0, _lo(co31si31_co30si30));
            xl2_3_2 = _cmpyr(xt2_1_yt2_1, _hi(co31si31_co30si30));

            _amemd8(&x2[0]) = _itod(x_3o_x_2o, x_1o_x_0o);
            _amemd8(&x2[l1]) = _itod(xl1_3_2, xl1_1_0);
            _amemd8(&x2[h2]) = _itod(xh2_3_2, xh2_1_0);
            _amemd8(&x2[l2]) = _itod(xl2_3_2, xl2_1_0);
        }
    }

    if (radix == 2)
        radix_2(ptr_x, ptr_y, npoints);
    else if (radix == 4)
        radix_4(ptr_x, ptr_y, npoints);

    return;
}

void radix_2(
    short *restrict ptr_x,
    short *restrict ptr_y,
    int npoints
)
{
    short * restrict x2, * restrict x0;
    short * restrict y0, * restrict y1, * restrict y2, * restrict y3;
    short n0, j0;

    int i, j, l1, h2;
    int y_10, y_32, y_54, y_76, y_98, y_BA, y_DC, y_FE;

    double x_2301, x_6745, x_AB89, x_EFCD;

    /*-----------------------------------------------------------------*/
    /* The following code performs either a standard radix4 pass or a  */
    /* radix2 pass. Two pointers are used to access the input data.    */
    /* The input data is read "N/4" complex samples apart or "N/2"     */
    /* words apart using pointers "x0" and "x2". This produces out-    */
    /* puts that are 0, N/4, N/2, 3N/4 for a radix4 FFT, and 0, N/8    */
    /* N/2, 3N/8 for radix 2.                                          */
    /*-----------------------------------------------------------------*/
    y0 = ptr_y;
    y2 = ptr_y + (int)npoints;
    x0 = ptr_x;
    x2 = ptr_x + (int)(npoints >> 1);

    /*----------------------------------------------------------------*/
    /* The pointers are set at the following locations which are half */
    /* the offsets of a radix4 FFT.                                   */
    /*----------------------------------------------------------------*/
    y1 = y0 + (int)(npoints >> 2);
    y3 = y2 + (int)(npoints >> 2);
    l1 = _norm(npoints) + 1;
    j0 = 8;
    n0 = npoints >> 1;

    /*--------------------------------------------------------------------*/
    /* The following code reads data indentically for either a radix 4    */
    /* or a radix 2 style decomposition. It writes out at different       */
    /* locations though. It checks if either half the points, or a        */
    /* quarter of the complex points have been exhausted to jump to       */
    /* pervent double reversal.                                           */
    /*--------------------------------------------------------------------*/
    j = 0;

//    _nassert((int)(n0) % 4 == 0);
    _nassert((int)(ptr_x) % 8 == 0);
    _nassert((int)(ptr_y) % 8 == 0);
    _nassert((int)(x0) % 8 == 0);
    _nassert((int)(x2) % 8 == 0);
    _nassert((int)(y0) % 8 == 0);
    _nassert(npoints >= 16);
    #pragma MUST_ITERATE(2,,2);
    for (i = 0; i < npoints; i += 8) {
        /*----------------------------------------------------------------*/
        /* Digit reverse the index starting from 0. The increment to "j"  */
        /* is either by 4, or 8.                                          */
        /*----------------------------------------------------------------*/
        DIG_REV(j, l1, h2);

        /*----------------------------------------------------------------*/
        /* Read in the input data. These are transformed as a radix2.     */
        /*----------------------------------------------------------------*/
        x_2301 = _amemd8(&x0[0]);
        x_6745 = _amemd8(&x0[4]);
        x0 += 8;
        x_AB89 = _amemd8(&x2[0]);
        x_EFCD = _amemd8(&x2[4]);
        x2 += 8;

        /*-------------------------------------------------------------*/
        /* Perform radix2 style decomposition.                         */
        /*-------------------------------------------------------------*/
        // n00 = x_0 + x_2;    n01 = x_1 + x_3;
        // y_10 = n00_n01 = _add2(x_01, x_23)
        y_10 = _add2(_lo(x_2301), _hi(x_2301));

        // n20 = x_0 - x_2;    n21 = x_1 - x_3;
        // y_98 = n20_n21 = _sub2(x_01, x_23)
        y_98 = _sub2(_lo(x_2301), _hi(x_2301));

        // n10 = x_4 + x_6;    n11 = x_5 + x_7;
        // y_54 = n10_n11 = _add2(x_45, x_67)
        y_54 = _add2(_lo(x_6745), _hi(x_6745));

        // n30 = x_4 - x_6;    n31 = x_5 - x_7;
        // y_DC = n30_n31 = _sub2(x_45, x_67)
        y_DC = _sub2(_lo(x_6745), _hi(x_6745));

        // n02 = x_8 + x_a;    n03 = x_9 + x_b;
        // y_32 = n03_n02 = _add2(x_89, x_AB)
        y_32 = _add2(_lo(x_AB89), _hi(x_AB89));

        // n12 = x_c + x_e;    n13 = x_d + x_f;
        // y_76 = n12_n13 = _add2(x_CD, x_EF)
        y_76 = _add2(_lo(x_EFCD), _hi(x_EFCD));

        // n22 = x_8 - x_a;    n23 = x_9 - x_b;
        // y_BA = n22_n23 = _sub2(x_89, x_AB)
        y_BA = _sub2(_lo(x_AB89), _hi(x_AB89));

        // n32 = x_c - x_e;    n33 = x_d - x_f;
        // y_FE = n32_n33 = _sub2(x_CD, x_EF)
        y_FE = _sub2(_lo(x_EFCD), _hi(x_EFCD));
/*
        y0[2*h2+0] = n01;  //Im
        y0[2*h2+1] = n00;  //Re
        y0[2*h2+2] = n03;  //Im
        y0[2*h2+3] = n02;  //Re
        y1[2*h2+0] = n11;  //Im
        y1[2*h2+1] = n10;  //Re
        y1[2*h2+2] = n13;  //Im
        y1[2*h2+3] = n12;  //Re
        y2[2*h2+0] = n21;  //Im
        y2[2*h2+1] = n20;  //Re
        y2[2*h2+2] = n23;  //Im
        y2[2*h2+3] = n22;  //Re
        y3[2*h2+0] = n31;  //Im
        y3[2*h2+1] = n30;  //Re
        y3[2*h2+2] = n33;  //Im
        y3[2*h2+3] = n32;  //Re
*/
        /*------------------------------------------------------------*/
        /*  Store out the results of all four butterflies as double   */
        /*  words.                                                    */
        /*------------------------------------------------------------*/
        _amemd8(&y0[2 * h2]) = _itod(y_32, y_10);
        _amemd8(&y1[2 * h2]) = _itod(y_76, y_54);
        _amemd8(&y2[2 * h2]) = _itod(y_BA, y_98);
        _amemd8(&y3[2 * h2]) = _itod(y_FE, y_DC);

        j += j0;
        if (j == n0) {
            j += n0;
            x0 += (int)npoints >> 1;
            x2 += (int)npoints >> 1;
        }
    }
}

void radix_4(
    short *restrict ptr_x,
    short *restrict ptr_y,
    int npoints
)
{
    short * restrict x2, * restrict x0;
    short * restrict y0, * restrict y1, * restrict y2, * restrict y3;
    short n0, j0;

    int i, j, l1, h2;
    int xh0_0_xh1_0, xh0_1_xh1_1, xh0_2_xh1_2, xh0_3_xh1_3;
    int xl0_0_xl1_0, xl0_1_xl1_1, xl1_1_xl0_1, xl0_2_xl1_2, xl0_3_xl1_3, xl1_3_xl0_3;
    int n10_n11, n30_n31, n12_n13, n32_n33;

    double x_2301, x_6745, x_AB89, x_EFCD;

    long long n00_n01_n20_n21, n10_n31_n30_n11, n02_n03_n22_n23, n12_n33_n32_n13;

    /*-----------------------------------------------------------------*/
    /* The following code performs either a standard radix4 pass or a  */
    /* radix2 pass. Two pointers are used to access the input data.    */
    /* The input data is read "N/4" complex samples apart or "N/2"     */
    /* words apart using pointers "x0" and "x2". This produces out-    */
    /* puts that are 0, N/4, N/2, 3N/4 for a radix4 FFT, and 0, N/8    */
    /* N/2, 3N/8 for radix 2.                                          */
    /*-----------------------------------------------------------------*/
    y0 = ptr_y;
    y2 = ptr_y + (int)npoints;
    x0 = ptr_x;
    x2 = ptr_x + (int)(npoints >> 1);

    /*----------------------------------------------------------------*/
    /* The pointers are set at the following locations which are half */
    /* the offsets of a radix4 FFT.                                   */
    /*----------------------------------------------------------------*/
    y1 = y0 + (int)(npoints >> 1);
    y3 = y2 + (int)(npoints >> 1);
    l1 = _norm(npoints) + 2;
    j0 = 4;
    n0 = npoints >> 2;

    /*--------------------------------------------------------------------*/
    /* The following code reads data indentically for either a radix 4    */
    /* or a radix 2 style decomposition. It writes out at different       */
    /* locations though. It checks if either half the points, or a        */
    /* quarter of the complex points have been exhausted to jump to       */
    /* pervent double reversal.                                           */
    /*--------------------------------------------------------------------*/
    j = 0;

    _nassert((int)(n0) % 4 == 0);
    _nassert((int)(x0) % 8 == 0);
    _nassert((int)(x2) % 8 == 0);
    _nassert((int)(y0) % 8 == 0);
    _nassert(npoints >= 16);
    #pragma MUST_ITERATE(2,,2);

    for (i = 0; i < npoints; i += 8) {
        /*----------------------------------------------------------------*/
        /* Digit reverse the index starting from 0. The increment to "j"  */
        /* is either by 4, or 8.                                          */
        /*----------------------------------------------------------------*/
        DIG_REV(j, l1, h2);

        /*----------------------------------------------------------------*/
        /* Read in the input data. These are transformed as a radix4.     */
        /*----------------------------------------------------------------*/
        x_2301 = _amemd8(&x0[0]);
        x_6745 = _amemd8(&x0[4]);
        x0 += 8;
        x_AB89 = _amemd8(&x2[0]);
        x_EFCD = _amemd8(&x2[4]);
        x2 += 8;


        /*-------------------------------------------------------------*/
        /* Perform radix4 style decomposition.                         */
        /*-------------------------------------------------------------*/
        // xh0_0 = x_0 + x_4;    xh1_0 = x_1 + x_5;
        xh0_0_xh1_0 = _add2(_lo(x_2301), _lo(x_6745));

        // xl0_0 = x_0 - x_4;    xl1_0 = x_1 - x_5;
        xl0_0_xl1_0 = _sub2(_lo(x_2301), _lo(x_6745));

        // xh0_1 = x_2 + x_6;    xh1_1 = x_3 + x_7;
        xh0_1_xh1_1 = _add2(_hi(x_2301), _hi(x_6745));

        // xl0_1 = x_2 - x_6;    xl1_1 = x_3 - x_7;
        xl0_1_xl1_1 = _sub2(_hi(x_2301), _hi(x_6745));

        // n00 = xh0_0 + xh0_1;    n20 = xh0_0 - xh0_1;
        // n01 = xh1_0 + xh1_1;    n21 = xh1_0 - xh1_1;
        n00_n01_n20_n21 = _addsub2(xh0_0_xh1_0, xh0_1_xh1_1);

        // n10 = xl0_0 + xl1_1;    n30 = xl0_0 - xl1_1;
        // n31 = xl1_0 + xl0_1;    n11 = xl1_0 - xl0_1;
        xl1_1_xl0_1 = _rotl(xl0_1_xl1_1, 16);
        n10_n31_n30_n11 = _addsub2(xl0_0_xl1_0, xl1_1_xl0_1);

        n10_n11 = _packhl2(_hill(n10_n31_n30_n11), _loll(n10_n31_n30_n11));
        n30_n31 = _packhl2(_loll(n10_n31_n30_n11), _hill(n10_n31_n30_n11));


        /*-------------------------------------------------------------*/
        /* Perform radix4 style decomposition.                         */
        /*-------------------------------------------------------------*/
        // xh0_2 = x_8 + x_c;    xh1_2 = x_9 + x_d;
        xh0_2_xh1_2 = _add2(_lo(x_AB89), _lo(x_EFCD));

        // xl0_2 = x_8 - x_c;    xl1_2 = x_9 - x_d;
        xl0_2_xl1_2 = _sub2(_lo(x_AB89), _lo(x_EFCD));

        // xh0_3 = x_a + x_e;    xh1_3 = x_b + x_f;
        xh0_3_xh1_3 = _add2(_hi(x_AB89), _hi(x_EFCD));

        // xl0_3 = x_a - x_e;    xl1_3 = x_b - x_f;
        xl0_3_xl1_3 = _sub2(_hi(x_AB89), _hi(x_EFCD));

        // n02 = xh0_2 + xh0_3;    n22 = xh0_2 - xh0_3;
        // n03 = xh1_2 + xh1_3;    n23 = xh1_2 - xh1_3;
        n02_n03_n22_n23 = _addsub2(xh0_2_xh1_2, xh0_3_xh1_3);

        // n12 = xl0_2 + xl1_3;    n32 = xl0_2 - xl1_3;
        // n33 = xl1_2 + xl0_3;    n13 = xl1_2 - xl0_3;
        xl1_3_xl0_3 = _rotl(xl0_3_xl1_3, 16);
        n12_n33_n32_n13 = _addsub2(xl0_2_xl1_2, xl1_3_xl0_3);

        n12_n13 = _packhl2(_hill(n12_n33_n32_n13), _loll(n12_n33_n32_n13));
        n32_n33 = _packhl2(_loll(n12_n33_n32_n13), _hill(n12_n33_n32_n13));


        /*-----------------------------------------------------------------*/
        /* Points that are read from succesive locations map to y, y[N/4]  */
        /* y[N/2], y[3N/4] in a radix4 scheme, y, y[N/8], y[N/2],y[5N/8]   */
        /*-----------------------------------------------------------------*/
        _amemd8(&y0[2 * h2]) = _itod(_hill(n02_n03_n22_n23), _hill(n00_n01_n20_n21));
        _amemd8(&y1[2 * h2]) = _itod(n12_n13, n10_n11);
        _amemd8(&y2[2 * h2]) = _itod(_loll(n02_n03_n22_n23), _loll(n00_n01_n20_n21));
        _amemd8(&y3[2 * h2]) = _itod(n32_n33, n30_n31);
/*
        y0[2*h2+0] = n01;  //Im
        y0[2*h2+1] = n00;  //Re
        y0[2*h2+2] = n03;  //Im
        y0[2*h2+3] = n02;  //Re
        y1[2*h2+0] = n11;  //Im
        y1[2*h2+1] = n10;  //Re
        y1[2*h2+2] = n13;  //Im
        y1[2*h2+3] = n12;  //Re
        y2[2*h2+0] = n21;  //Im
        y2[2*h2+1] = n20;  //Re
        y2[2*h2+2] = n23;  //Im
        y2[2*h2+3] = n22;  //Re
        y3[2*h2+0] = n31;  //Im
        y3[2*h2+1] = n30;  //Re
        y3[2*h2+2] = n33;  //Im
        y3[2*h2+3] = n32;  //Re
*/
        j += j0;
        if (j == n0) {
            j  += n0;
            x0 += (int)npoints >> 1;
            x2 += (int)npoints >> 1;
        }
    }
}

/* ======================================================================== */
/*  End of file: DSP_fft16x16_imre.c                                        */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2007 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */

