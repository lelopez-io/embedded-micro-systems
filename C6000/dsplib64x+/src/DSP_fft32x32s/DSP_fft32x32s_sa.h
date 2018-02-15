/* ======================================================================= */
/* DSP_fft32x32s_sa.h -- 32x32s Mixed Radix FFT                            */
/*                       SA Implementation                                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef DSP_FFT32X32S_SA_H_
#define DSP_FFT32X32S_SA_H_ 1

void DSP_fft32x32s (
    const int * restrict ptr_w,
    int npoints,
    int * restrict ptr_x,
    int * restrict ptr_y
);

#endif

/* ======================================================================== */
/*  End of file:  DSP_fft32x32s_sa.h                                        */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

