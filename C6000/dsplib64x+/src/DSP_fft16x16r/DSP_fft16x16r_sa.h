/* ======================================================================= */
/* DSP_fft16x16r_sa.h -- 16x16r Mixed Radix FFT                            */
/*                    SA Implementation                                    */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef DSP_FFT16X16R_SA_H_
#define DSP_FFT16X16R_SA_H_ 1

void DSP_fft16x16r (
    int n,
    short * restrict ptr_x,
    const short * restrict ptr_w,
    short * restrict ptr_y,
    int radix,
    int offset,
    int nmax
);

#endif

/* ======================================================================== */
/*  End of file:  DSP_fft16x16r_sa.h                                        */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

