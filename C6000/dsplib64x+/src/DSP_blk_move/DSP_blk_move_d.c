/* ======================================================================= */
/* DSP_blk_move_d.c -- Move a block of memory                              */
/*                     Driver code implementation                          */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#include "DSP_blk_move_cn.h"
#include "DSP_blk_move.h"

/* ======================================================================== */
/*  Kernel-specific alignments                                              */
/* ======================================================================== */
#pragma DATA_ALIGN(x_cn, 8);
#pragma DATA_ALIGN(r_cn, 8);

#pragma DATA_ALIGN(x_i, 8);
#pragma DATA_ALIGN(r_i, 8);

/* ======================================================================== */
/*  Parameters of fixed dataset.                                            */
/* ======================================================================== */
#define NX (1024)
#define PAD (16)

/* ======================================================================== */
/*  Initialized arrays with fixed test data.                                */
/* ======================================================================== */
short x[NX + 2*PAD] =
{
     0x032C, -0x544D, -0x6A9D,  0x09C4,  0x5226,  0x6A85,  0x6EDE,  0x1463,
    -0x158A,  0x75BC,  0x7407,  0x160B, -0x0E16, -0x3F82,  0x5F6D, -0x567B,

     0x453E, -0x321D,  0x2DBB,  0x77C7, -0x57B9, -0x4ADD,  0x10B7, -0x113F,
    -0x1A92,  0x7CA7,  0x4A14,  0x719D,  0x3E61,  0x1858, -0x0D4C,  0x3D94,
    -0x32B8, -0x14B2,  0x1B6A,  0x7A1D,  0x0D5B,  0x1D2C,  0x55D8, -0x5459,
     0x5030, -0x5CBD,  0x0180,  0x059B,  0x4D97,  0x49E3,  0x68A4, -0x3629,
    -0x7AA5, -0x582E,  0x1D05,  0x3A65, -0x4734,  0x3D6B, -0x6CE3,  0x4A99,
     0x1D54, -0x50B4,  0x62F5, -0x75A7, -0x6AFA,  0x5D26,  0x3C1A, -0x793D,
    -0x6493,  0x4282, -0x0020,  0x1BE0,  0x1A7F,  0x621B, -0x1C86, -0x254E,
    -0x12FB, -0x4E5B,  0x3409,  0x223E,  0x3856, -0x4179, -0x60C9, -0x102C,
     0x5EB6, -0x097B,  0x7A6E,  0x1450, -0x41CD, -0x133A,  0x0358, -0x2677,
    -0x0BA0,  0x6732,  0x19A5, -0x4961, -0x1E47, -0x6657,  0x57C6,  0x1E70,
     0x1F12,  0x59F9,  0x13A4,  0x16D9,  0x26E6, -0x4FE0,  0x39FA, -0x6AF4,
    -0x2829, -0x673F,  0x7460,  0x57A6, -0x5BB4,  0x424C, -0x6068,  0x2CC9,
     0x0F61, -0x4030, -0x00AD,  0x6AE1, -0x4FD3,  0x56ED,  0x2FDA, -0x759C,
    -0x726F,  0x78E3,  0x42DF, -0x63D3, -0x1F88, -0x71D0,  0x07E8, -0x6D4A,
     0x21E5, -0x6C4D, -0x0072,  0x0EEE,  0x749D, -0x4622, -0x2AF7, -0x3A2B,
     0x3DC4,  0x05D0, -0x00C7,  0x059F,  0x1C52,  0x36EE,  0x58B6, -0x7864,
    -0x145A, -0x3F1F,  0x4107,  0x1B89,  0x2024,  0x4771,  0x3F8F,  0x4298,
     0x231F,  0x6C44, -0x79AD,  0x58DA, -0x1706,  0x37E1,  0x6B2A,  0x6447,
    -0x7C33,  0x080C,  0x426E,  0x1F1C, -0x2095,  0x64D2, -0x7634,  0x4BC4,
     0x6720,  0x6260, -0x12BD,  0x040C, -0x3F04,  0x2FFF,  0x14F4,  0x00CC,
     0x6007,  0x6E3D, -0x532B,  0x7FD1,  0x3538, -0x3B46, -0x4D28, -0x21D9,
     0x087B, -0x7872,  0x68AC,  0x7E95, -0x1851, -0x3B44, -0x0754,  0x126C,
     0x3DE7, -0x754D,  0x360C, -0x2589, -0x2234, -0x2DAF, -0x7E1F,  0x022A,
    -0x4C8A, -0x34A6,  0x2A23, -0x5A1A,  0x4B95, -0x0317,  0x7430, -0x7091,
    -0x0CC1, -0x0A3E, -0x7AFE,  0x645A, -0x50BD,  0x0909,  0x6090,  0x34AE,
    -0x56B3, -0x3AA9,  0x438B,  0x1C64,  0x62BE, -0x5E6A,  0x7A07,  0x0D08,
     0x6089, -0x5D4D,  0x0B27,  0x4A2C,  0x79F5,  0x5A86,  0x4061, -0x4411,
    -0x6674,  0x19FC,  0x7F77, -0x4EBE,  0x0E21, -0x5D11,  0x72BE,  0x2232,
     0x4040,  0x3DA5, -0x1736,  0x10DD, -0x192A, -0x768A,  0x770A,  0x1261,
    -0x788B, -0x2778,  0x4989, -0x4999,  0x7905,  0x6F1E,  0x184B, -0x3C6D,
     0x1543, -0x6F94, -0x3588, -0x0D80,  0x57D4, -0x728A, -0x5E27,  0x4092,
    -0x59A4, -0x2322, -0x762C,  0x684B, -0x0AA5,  0x5F2B,  0x5F6B, -0x5CA5,
    -0x01CC, -0x1F8D, -0x35A1,  0x432A, -0x35C6, -0x0E0F, -0x7FEF,  0x3D0B,
     0x3D0B,  0x2B67, -0x7BC7,  0x48D5, -0x1FE3,  0x0ADA,  0x4F02, -0x69DF,
    -0x6226,  0x4E99,  0x52AC, -0x322F, -0x4CF2,  0x34FE, -0x26B0,  0x512C,
    -0x7CE4,  0x59F0, -0x4137, -0x7CBE, -0x7F4B, -0x5471, -0x1A85, -0x0F38,
    -0x0883,  0x3513, -0x0813,  0x322B, -0x1C8F,  0x2643,  0x59E8,  0x7410,
     0x7F60, -0x1D73, -0x27DB,  0x48FD, -0x1AAB, -0x7DD7, -0x0BD5, -0x1CA4,
     0x524B, -0x71B5,  0x5C6E,  0x4C92,  0x6100,  0x4ED3,  0x7244, -0x5594,
    -0x12C8, -0x6F88,  0x6EDB,  0x3284, -0x36A5,  0x33E6,  0x05AC, -0x1E10,
     0x13B5,  0x49C1,  0x1399, -0x721F,  0x5033,  0x430D,  0x3B48,  0x6B6D,
     0x18FB, -0x2711, -0x0233, -0x5DBB,  0x6FB3,  0x2341,  0x3640, -0x027C,
    -0x5C31, -0x4715,  0x795B,  0x5C52, -0x3241,  0x1380,  0x50AE, -0x0365,
    -0x42AD,  0x3917, -0x4178,  0x2082, -0x7CCF, -0x222D,  0x1931,  0x46E0,
     0x4CAF, -0x1DED, -0x6482,  0x7B64, -0x340E, -0x623A, -0x4B9A, -0x2D4B,
    -0x1563, -0x5F28, -0x7754, -0x44D3, -0x030A,  0x6E31, -0x67CF,  0x2F75,
     0x1AD2, -0x12C6,  0x120C,  0x5FB5, -0x05E8, -0x0B97, -0x5006, -0x0F69,
    -0x1FB6, -0x3C3D, -0x52BB,  0x6BC9, -0x072F, -0x3830, -0x2D48, -0x18C2,
    -0x468B,  0x7BF6, -0x1B51, -0x661A, -0x5D29, -0x4040, -0x5215,  0x4C15,
    -0x0EC2, -0x7A14, -0x1ECD,  0x6F4B, -0x5E69,  0x1DDF, -0x0699, -0x4161,
     0x58FA,  0x0753,  0x6610,  0x2F78, -0x7A70, -0x6633,  0x2110,  0x28DA,
     0x5F31,  0x55CD,  0x1281, -0x5205,  0x098E,  0x492E,  0x3965, -0x7CB9,
    -0x78B8,  0x4BB0,  0x1939, -0x0051, -0x413D,  0x6F22,  0x64FA,  0x794F,
     0x3F0E, -0x74DB,  0x4DCC,  0x134F,  0x0CC4,  0x2409, -0x703D, -0x1AF2,
     0x1627, -0x675A, -0x7110,  0x1F6F, -0x5E9A, -0x1D42, -0x6DB1,  0x3F74,
     0x5754, -0x3225, -0x6460, -0x092E,  0x3074, -0x10E6, -0x4F2A, -0x2C2D,
    -0x065C,  0x3EDF,  0x401E,  0x3C20, -0x61B1, -0x5813,  0x702D, -0x123F,
    -0x008A,  0x6CD8, -0x1B28,  0x74FF, -0x7B9C, -0x54B9, -0x1964, -0x279E,
     0x256E, -0x1803,  0x7A2A, -0x0577,  0x1B8D, -0x18D7,  0x478C, -0x3BF5,
    -0x00C8, -0x3B13, -0x3200, -0x2FDE, -0x7FBA, -0x7F6B, -0x0FE3, -0x7BB2,
     0x6E39, -0x0589,  0x345B,  0x55B3,  0x5BCC, -0x6206, -0x3C67,  0x365E,
     0x7415, -0x6350, -0x654E, -0x29C6, -0x19E9,  0x1605, -0x1B3B, -0x3C2A,
     0x6D1D, -0x0887, -0x0CC9,  0x02C9,  0x57B3, -0x062D, -0x26E8,  0x43E1,
    -0x5767, -0x77A5,  0x09F8,  0x3AD6, -0x5D81, -0x0587,  0x0BC9,  0x40C0,
     0x5AF4, -0x233A, -0x551C, -0x3408, -0x5BBB, -0x4208, -0x5D3A, -0x244E,
    -0x063C,  0x46AF,  0x39AB, -0x0808, -0x57CC, -0x0174, -0x397A, -0x36C4,
    -0x0F45, -0x7F6C, -0x657A, -0x0DB3,  0x4F3E, -0x60A8,  0x3DB8,  0x3DCC,
    -0x4896,  0x21D3, -0x7B2B, -0x510F, -0x27AD, -0x6B87,  0x6ADE, -0x583B,
     0x4AF4, -0x068F,  0x2FA3, -0x6D68, -0x3E87, -0x558F, -0x5147,  0x58E1,
     0x0293,  0x4E33,  0x630D,  0x0B46, -0x2737,  0x35F9,  0x229E,  0x05FE,
    -0x39FB, -0x0AE1, -0x1981, -0x02B7, -0x65B6, -0x65D3, -0x4154,  0x2B3E,
    -0x5427,  0x4E5E,  0x4CDC,  0x1D8D, -0x0354, -0x44E0, -0x3027,  0x4F93,
    -0x1C6A,  0x1E77,  0x277D,  0x1B54, -0x6C1E,  0x41C5, -0x2215,  0x23A7,
     0x18CA,  0x4DED, -0x6CE7, -0x41B3,  0x1C9E, -0x39BE, -0x19B8, -0x73E3,
     0x21F3,  0x7D44,  0x2981, -0x31F5,  0x61AD, -0x1D62, -0x0057,  0x7F3D,
     0x1C49, -0x7AA5,  0x494B, -0x1122,  0x7F2E,  0x17D0,  0x53AE, -0x3706,
    -0x1D95, -0x2ED4, -0x31B2,  0x4C05, -0x6E4E, -0x69D4,  0x7F50, -0x54A8,
    -0x2016, -0x6616,  0x1C09,  0x354B, -0x4CCD, -0x0CC9,  0x5634,  0x6A34,
     0x4AB1, -0x057B, -0x751B, -0x7908,  0x6AF3, -0x5730, -0x0221, -0x5B96,
    -0x46AA, -0x087F,  0x3858,  0x032E,  0x6D33, -0x7B3D,  0x67CA, -0x65A2,
     0x1C3D,  0x4F3D,  0x3AE9, -0x7C64, -0x3430, -0x0949,  0x3C51,  0x55DF,
     0x12A9,  0x3B8D,  0x270F,  0x0D96,  0x71BE,  0x367D,  0x118A, -0x5E8C,
    -0x5F59,  0x05B9,  0x0AFA, -0x3D74,  0x1063,  0x52BE,  0x09F5, -0x0503,
     0x47D4, -0x7520, -0x4FCF, -0x4B1F,  0x67D0,  0x190C, -0x29EE,  0x46BD,
     0x7E0E, -0x724F,  0x5519, -0x67DE, -0x1520, -0x1BB2,  0x13D2,  0x7EC1,
    -0x7B06, -0x096F, -0x48AC,  0x5B9F, -0x3AD1,  0x4193,  0x0BEB,  0x2EA8,
    -0x618D, -0x0FEC, -0x7DF9, -0x6CA4,  0x47F9, -0x7CC2, -0x2EF8, -0x243B,
    -0x582F, -0x2221,  0x1FFB, -0x2D8F, -0x5831,  0x2595, -0x401E,  0x41AE,
     0x0C19,  0x37E5, -0x1A5D, -0x2748,  0x2C39,  0x1CB2,  0x5C2E,  0x7C1A,
     0x2004,  0x470A, -0x5806, -0x5415, -0x4AD3, -0x0030, -0x6C90,  0x0531,
    -0x2B16, -0x3BE2, -0x5DB4, -0x7BEA, -0x6496,  0x11FB,  0x56B0, -0x66D4,
    -0x3071,  0x493D, -0x332C, -0x5A8E, -0x014D,  0x2D20, -0x61F0, -0x02A2,
    -0x1B30, -0x4B72, -0x78BE,  0x0B97,  0x6BED, -0x6B86, -0x3ACA, -0x6262,
    -0x17DA,  0x6164, -0x56E4,  0x031A, -0x21E9, -0x6E9F,  0x4DA9,  0x100A,
    -0x13E5,  0x41B9, -0x0DFE, -0x6682,  0x3023,  0x0D7B, -0x0A04,  0x7E2F,
    -0x3074, -0x24EE, -0x292E, -0x1073, -0x2848, -0x0FB7,  0x42E7, -0x1E6D,
     0x3BDC,  0x20B9, -0x5156,  0x6F10,  0x15C6,  0x7618,  0x5736,  0x2698,
    -0x0706,  0x2E61,  0x48C9, -0x221D,  0x7704,  0x534E,  0x379F,  0x34C3,
    -0x5AB5, -0x1DE2, -0x2CA9,  0x4A71, -0x3EB5, -0x40E7,  0x6272, -0x3398,
    -0x3385,  0x5BCE,  0x3202, -0x1779,  0x34D1,  0x6482, -0x28D3,  0x4DAC,
     0x5D21,  0x51CD, -0x587A, -0x4375,  0x3344,  0x28D4,  0x71F1,  0x56EC,
    -0x7CB1,  0x511D, -0x1AF3,  0x3115,  0x4EC3, -0x2A8C, -0x592D,  0x4D88,
    -0x2605, -0x2820,  0x7D74, -0x721B, -0x5B43, -0x58F5, -0x59F0, -0x44F9,
     0x0749,  0x4937, -0x5096,  0x413A, -0x5157, -0x48EC, -0x23E2, -0x6862,
    -0x4F48, -0x2B7A, -0x6985,  0x1E89,  0x66A0,  0x45CE, -0x355F,  0x6217,
    -0x2CD2,  0x270C, -0x4317,  0x6996, -0x3724, -0x1D75,  0x483E,  0x2D1A,
     0x7ADE,  0x09E6,  0x0177, -0x351B, -0x3BF0,  0x7E5C,  0x274E,  0x7FD3,
    -0x2AE7, -0x2492, -0x0BF8,  0x2299,  0x78A3,  0x5023,  0x2C6F,  0x11F7,
    -0x4F0C, -0x1106,  0x7923,  0x3FA5, -0x1C34, -0x5D01,  0x71F8,  0x772C,
     0x4BD7, -0x3E08, -0x21AA,  0x6B65, -0x1B72,  0x7D21,  0x2250, -0x5D2C,
    -0x701B,  0x1A73,  0x477A, -0x1B69, -0x5B70,  0x34F1, -0x60E6,  0x3F2C,
    -0x59B8,  0x00EA,  0x7ED2,  0x2CAF,  0x5CDA, -0x3358, -0x07B4,  0x01DD,
    -0x0CA2, -0x636A,  0x2313,  0x3B8D,  0x7D71,  0x4B38, -0x30DE, -0x4328,
     0x67C1, -0x13F9,  0x3747, -0x6B66,  0x37D0, -0x1F65, -0x6D0E,  0x6EA6,
     0x3A34,  0x6220,  0x108A,  0x3839,  0x6142, -0x0F79,  0x22DB,  0x1F0B,
     0x016A,  0x6179, -0x594C,  0x789F, -0x45DE,  0x0177, -0x35A6, -0x58F2,
     0x25BA, -0x6BE0,  0x40D7, -0x56F3, -0x6510, -0x7CE0, -0x5D3E,  0x4D5D,
    -0x4662,  0x2AB2, -0x775B, -0x489C, -0x0FAF,  0x7C39,  0x4E85,  0x4C1B,
    -0x4BD6, -0x0B22, -0x0B4F, -0x69E2, -0x511C,  0x14B1,  0x1B6F,  0x38FF,
    -0x7AB4,  0x5C45, -0x676B, -0x695D, -0x71FA, -0x00C6, -0x7D49,  0x3CDF,
     0x0B00,  0x42B5,  0x56F1, -0x65FD,  0x3B63, -0x37C9,  0x02F1,  0x4199,
    -0x12AA, -0x4833,  0x6C51,  0x2309, -0x2B88, -0x03EF,  0x62E0, -0x0DAB,
     0x4E5D, -0x6809,  0x63EE,  0x4AB4, -0x2715,  0x37DD,  0x4E2D, -0x64D0,
    -0x160F,  0x07CB,  0x5B53, -0x6FF0,  0x6AC8,  0x0A72,  0x5BF6, -0x11C0,
    -0x7C9C, -0x3435, -0x3E22, -0x6D96, -0x0866, -0x3228,  0x5F42, -0x5901,
    -0x3BF9, -0x7A7C,  0x5625, -0x6518,  0x4165,  0x4311,  0x3356,  0x0B0E,
     0x709B, -0x30F3, -0x59C8,  0x1680, -0x3D7C,  0x294E, -0x291B,  0x525E,
    -0x046A,  0x34E4, -0x313E, -0x1AB1,  0x7665, -0x5D84, -0x1ADB, -0x1A4A,

    -0x7FB1,  0x0E2F, -0x4BEF, -0x6EAE, -0x00E2,  0x7E31,  0x7FC1, -0x7F65,
     0x1B0C,  0x415B, -0x6F0C,  0x5C78,  0x2DEE,  0x6900, -0x7E52,  0x3998
};

short x_cn[NX + 2*PAD];
short r_cn[NX + 2*PAD];

short x_i[NX + 2*PAD];
short r_i[NX + 2*PAD];

/* ======================================================================== */
/*  Generate pointers to skip beyond array padding                          */
/* ======================================================================== */
short *const ptr_x_cn = x_cn + PAD;
short *const ptr_r_cn = r_cn + PAD;

short *const ptr_x_i = x_i + PAD;
short *const ptr_r_i = r_i + PAD;

/* ======================================================================== */
/*  Prototypes for timing functions.                                        */
/* ======================================================================== */
clock_t time_cn(int nx);
clock_t time_i(int nx);

/* ======================================================================== */
/*  MAIN -- Top level driver for the test.                                  */
/* ======================================================================== */
int main()
{   
    clock_t t_overhead, t_start, t_stop;
    clock_t t_cn, t_i;
    int i, nx, fail = 0;

    /* -------------------------------------------------------------------- */
    /*  Compute the overhead of calling clock() twice to get timing info.   */
    /* -------------------------------------------------------------------- */
    t_start = clock();
    t_stop = clock();
    t_overhead = t_stop - t_start;

    for(nx = 8, i = 1; nx <= NX; i++, nx += 8) {
        /* -------------------------------------------------------------------- */
        /*  Copy vector to all inputs.                                          */
        /* -------------------------------------------------------------------- */
        memcpy(x_cn, x, sizeof(x_cn));
        memcpy(x_i, x, sizeof(x_i));

        /* -------------------------------------------------------------------- */
        /*  Force uninitialized arrays to fixed values.                         */
        /* -------------------------------------------------------------------- */
        memset(r_cn, 0xA5, sizeof(r_cn));
        memset(r_i, 0xA5, sizeof(r_i));

        /* -------------------------------------------------------------------- */
        /*  Call the individual timing routines, and subtract off overhead.     */
        /* -------------------------------------------------------------------- */
        t_cn = time_cn(nx) - t_overhead;
        t_i = time_i(nx) - t_overhead;

        /* -------------------------------------------------------------------- */
        /*  Print timing results.                                               */
        /* -------------------------------------------------------------------- */
        printf("DSP_blk_move\tIter#: %d\t", i);

        /* -------------------------------------------------------------------- */
        /*  Check the results arrays, and report any failures.                  */
        /* -------------------------------------------------------------------- */
        if (memcmp(x_i, x_cn, sizeof(x_cn))) {
            fail++;
            printf("Result Failure (x_i)\t");
        }
        else
            printf("Result Successful (x_i)\t");

        if (memcmp(r_i, r_cn, sizeof(r_cn))) {
            fail++;
            printf("Result Failure (r_i)");
        }
        else
            printf("Result Successful (r_i)");

        printf("\tNX = %d\tnatC: %d\tintC: %d\n", nx, t_cn, t_i);
    }
    return (fail);
}

/* ======================================================================== */
/*  TIME_CN  -- Measure elapsed time for natural C version.                 */
/* ======================================================================== */
clock_t time_cn(int nx)
{
    clock_t t_start, t_stop;

    t_start = clock();
    DSP_blk_move_cn(ptr_x_cn, ptr_r_cn, nx);
    DSP_blk_move_cn(ptr_x_cn, NULL, nx);
    t_stop = clock();
    return t_stop - t_start;
}

/* ======================================================================== */
/*  TIME_I   -- Measure elapsed time for intrinsic C version.               */
/* ======================================================================== */
clock_t time_i(int nx)
{
    clock_t t_start, t_stop;

    t_start = clock();
    DSP_blk_move(ptr_x_i, ptr_r_i, nx);
    DSP_blk_move(ptr_x_i, NULL, nx);
    t_stop = clock();
    return t_stop - t_start;
}

/* ======================================================================== */
/*  End of file:  DSP_blk_move_d.c                                          */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2007 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */

