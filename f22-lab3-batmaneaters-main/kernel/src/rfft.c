/** @file   rfft.c
 *
 *  @brief  real FFT function for ARM Cortex-M4
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 12 September 2022
 *  @author CMU 14-642
**/

#include <rfft.h>
#include <unistd.h>
#include <stdint.h>

/* internal function prototypes */
void cfft(int16_t *pIn);
void fft_bf(int16_t *pIn, uint32_t len);
extern void bit_reverse(uint16_t *p, const uint16_t len, const uint16_t *table);
void rfft_split(int16_t *pIn, int16_t *pOut);

/**    
 * @brief real FFT function
 * @param *pIn  - ptr to input buffer holding FFT_SIZE real-valued inputs
 * @param *pOut - ptr to output buffer holding 2*FFT_SIZE real/imag inputs
 * @return none   
 */
void rfft(int16_t *pIn, int16_t *pOut) {
    cfft(pIn);
    bit_reverse((uint16_t*) pIn, BIT_REV_LEN, bitRevTable);
    rfft_split(pIn, pOut);
}

/**    
 * @brief complex FFT function
 * @param *pIn  - ptr to input/output buffer holding FFT_SIZE real-valued inputs
 * @note  output will have FFT_SIZE/2 complex values
 * @return none   
 */
void cfft(int16_t *pIn) {    
    uint32_t i;
    uint32_t n2;
    int16_t p0, p1, p2, p3;
    int32_t T, S, R;
    int32_t coeff, out1, out2;
    const int16_t *pC = twiddleCoef;
    int16_t *pSi = pIn;
    int16_t *pSl = pIn + CFFT_LEN;
    
    n2 = CFFT_LEN >> 1; 

    for (i = n2; i > 0; i--) {
        coeff = (*(int32_t * ) (pC));
        pC += 2;

        T = (*(int32_t * ) (pSi));
        T = __SHADD16(T, 0);

        S = (*(int32_t * ) (pSl));
        S = __SHADD16(S, 0);

        R = __QSUB16(T, S);

        (*(int32_t * ) (pSi)) = __SHADD16(T, S);
        pSi += 2;

        out1 = __SMUAD(coeff, R) >> 16;
        out2 = __SMUSDX(coeff, R);

        (*(int32_t * ) (pSl)) = (int32_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);
        pSl += 2;
    } 

    fft_bf(pIn, n2);
    fft_bf(pIn+CFFT_LEN, n2);
			
    for (i = 0; i < CFFT_LEN >> 1; i++) {
        p0 = pIn[4*i+0];
        p1 = pIn[4*i+1];
        p2 = pIn[4*i+2];
        p3 = pIn[4*i+3];
        
        p0 <<= 1;
        p1 <<= 1;
        p2 <<= 1;
        p3 <<= 1;
        
        pIn[4*i+0] = p0;
        pIn[4*i+1] = p1;
        pIn[4*i+2] = p2;
        pIn[4*i+3] = p3;
    }
}

/**
 * @brief FFT butterfly implementation
 * @param *pIn  - ptr to input/output buffer
 * @return none
 */
void fft_bf(int16_t *pIn, uint32_t len) {
    int32_t R, S, T, U;
    int32_t C1, C2, C3, out1, out2;
    uint32_t n1, n2, ic, i0, j, k;
    uint32_t modifier = 2;

    int16_t *ptr1, *pSi0, *pSi1, *pSi2, *pSi3;
    int32_t xaya, xbyb, xcyc, xdyd;

    n2 = len >> 2;
    n1 = len;
    ic = 0;
    j = n2;

    pSi0 = pIn;
    pSi1 = pSi0 + (n2 << 1);
    pSi2 = pSi1 + (n2 << 1);
    pSi3 = pSi2 + (n2 << 1);

    do {
        T = (*(int32_t * ) (pSi0));
        T = __SHADD16(T, 0);
        T = __SHADD16(T, 0);

        S = (*(int32_t * ) (pSi2));
        S = __SHADD16(S, 0);
        S = __SHADD16(S, 0);

        R = __QADD16(T, S);
        S = __QSUB16(T, S);

        T = (*(int32_t * ) (pSi1));
        T = __SHADD16(T, 0);
        T = __SHADD16(T, 0);

        U = (*(int32_t * ) (pSi3));
        U = __SHADD16(U, 0);
        U = __SHADD16(U, 0);

        T = __QADD16(T, U);

        (*(int32_t * ) (pSi0)) = __SHADD16(R, T);
        pSi0 += 2;

        R = __QSUB16(R, T);

        C2 = (*(int32_t * ) (twiddleCoef + (ic << 2)));

        out1 = __SMUAD(C2, R) >> 16;
        out2 = __SMUSDX(C2, R);

        T = (*(int32_t * ) (pSi1));
        T = __SHADD16(T, 0);
        T = __SHADD16(T, 0);

        (*(int32_t * ) (pSi1)) = (int32_t) ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);
        pSi1 += 2;

        U = (*(int32_t * ) (pSi3));
        U = __SHADD16(U, 0);
        U = __SHADD16(U, 0);

        T = __QSUB16(T, U);
        R = __QASX(S, T);
        S = __QSAX(S, T);
        
        C1 = (*(int32_t * ) (twiddleCoef + (ic << 1)));

        out1 = __SMUAD(C1, S) >> 16;
        out2 = __SMUSDX(C1, S);        
        
        (*(int32_t * ) (pSi2)) = ((out2) & 0xFFFF0000) | ((out1) & 0x0000FFFF);
        pSi2 += 2;

        C3 = (*(int32_t * ) (twiddleCoef + ((ic << 2) + (ic << 1))));

        out1 = __SMUAD(C3, R) >> 16;
        out2 = __SMUSDX(C3, R);

        (*(int32_t * ) (pSi3)) = ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);
        pSi3 += 2;

        ic += modifier;

    } while(--j);

    modifier <<= 2;

    for(k = len >> 2; k > 4; k >>= 2) {
        n1 = n2;
        n2 >>= 2;
        ic = 0;

        for(j = 0; j <= (n2 - 1); j++) {
            C1 = (*(int32_t * ) (twiddleCoef + (ic << 1)));
            C2 = (*(int32_t * ) (twiddleCoef + (ic << 2)));
            C3 = (*(int32_t * ) (twiddleCoef + ((ic << 1) + (ic << 2))));

            ic += modifier;
          
            pSi0 = pIn + (j << 1);
            pSi1 = pSi0 + (n2 << 1);
            pSi2 = pSi1 + (n2 << 1);
            pSi3 = pSi2 + (n2 << 1);

            for(i0 = j; i0 < len; i0 += n1) {
                T = (*(int32_t * ) (pSi0));
                S = (*(int32_t * ) (pSi2));
                R = __QADD16(T, S);
                S = __QSUB16(T, S);
                T = (*(int32_t * ) (pSi1));
                U = (*(int32_t * ) (pSi3));
                T = __QADD16(T, U);
                out1 = __SHADD16(R, T);
                out1 = __SHADD16(out1, 0);
                (*(int32_t * ) (pSi0)) = out1;
                pSi0 += (n1 << 1);

                R = __SHSUB16(R, T);
                out1 = __SMUAD(C2, R) >> 16;
                out2 = __SMUSDX(C2, R);
                T = (*(int32_t * ) (pSi1));
                (*(int32_t * ) (pSi1)) = ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);
                pSi1 += (n1 << 1);

                U = (*(int32_t * ) (pSi3));
                T = __QSUB16(T, U);
                R = __SHASX(S, T);
                S = __SHSAX(S, T);
                out1 = __SMUAD(C1, S) >> 16;
                out2 = __SMUSDX(C1, S);

                (*(int32_t * ) (pSi2)) = ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);
                pSi2 += (n1 << 1);

                out1 = __SMUAD(C3, R) >> 16;
                out2 = __SMUSDX(C3, R);

                (*(int32_t * ) (pSi3)) = ((out2) & 0xFFFF0000) | (out1 & 0x0000FFFF);
                pSi3 += (n1 << 1);
            }
        }
        modifier <<= 2;
    }

    j = len >> 2;
    ptr1 = &pIn[0];

    do {
        xaya = *(*(int32_t **) & (ptr1))++;
        xbyb = *(*(int32_t **) & (ptr1))++;
        xcyc = *(*(int32_t **) & (ptr1))++;
        xdyd = *(*(int32_t **) & (ptr1))++;

        R = __QADD16(xaya, xcyc);
        T = __QADD16(xbyb, xdyd);
        ptr1 -= 8;

        *(*(int32_t **) & (ptr1))++ = __SHADD16(R, T);
        T = __QADD16(xbyb, xdyd);
        *(*(int32_t **) & (ptr1))++ = __SHSUB16(R, T);
        S = __QSUB16(xaya, xcyc);
        U = __QSUB16(xbyb, xdyd);
        *(*(int32_t **) & (ptr1))++ = __SHSAX(S, U);
        *(*(int32_t **) & (ptr1))++ = __SHASX(S, U);
    } while(--j);
}

/**
 * @brief computes real FFT output
 * @param *pIn  - ptr to input buffer of length FFT_SIZE
 * @param *pOut - ptr to output buffer of length 2*FFT_SIZE
 * @return none
 */
void rfft_split(int16_t *pIn, int16_t *pOut) {
    uint32_t i;
    int32_t outR, outI;
    int16_t *pCoefA, *pCoefB;
    int16_t *pSrc1, *pSrc2, *pD1, *pD2;

    pCoefA = (int16_t*)realCoefA;
    pCoefB = (int16_t*)realCoefB;

    pSrc1 = &pIn[2];
    pSrc2 = &pIn[FFT_SIZE - 2];

    i = 1;
    pD1 = pOut + 2;
    pD2 = pOut + (FFT_SIZE << 1) - 2;

    for(i = CFFT_LEN - 1; i > 0; i--) {
        outR = __SMUSD(*(*(int32_t **) & (pSrc1)), *(*(int32_t **) & (pCoefA)));
        outR = __SMLAD(*(*(int32_t **) & (pSrc2)), *(*(int32_t **) & (pCoefB)), outR) >> 16;

        outI = __SMUSDX(*(*(int32_t **) & (pSrc2))--, *(*(int32_t **) & (pCoefB)));
        outI = __SMLADX(*(*(int32_t **) & (pSrc1))++, *(*(int32_t **) & (pCoefA)), outI);

        *pD1++ = (int16_t) outR;
        *pD1++ = outI >> 16;

        pD2[0] = (int16_t) outR;
        pD2[1] = -(outI >> 16);
        pD2 -= 2;

        pCoefA += 2;
        pCoefB += 2;
    }

    pOut[FFT_SIZE] = (pIn[0] - pIn[1]) >> 1;
    pOut[FFT_SIZE + 1] = 0;

    pOut[0] = (pIn[0] + pIn[1]) >> 1;
    pOut[1] = 0;
}

/**
 * @brief vector absolute value -- pOut[i]=|pIn[i]|
 * @param *pIn  - ptr to input buffer
 * @param *pOut - ptr to output buffer
 * @param len   - buffer length
 * @return none
 * 
 * @note assumes len is a multiple of 4
 */
void rfft_abs(int16_t *pIn, int16_t *pOut, uint32_t len) {
    uint32_t loops;

    int32_t *simd;

    int16_t in1;                                     /* Input value1 */
    int16_t in2;                                     /* Input value2 */

    loops = len >> 2; // 4x loop unrolling

    simd = ((int32_t *) (pOut));

    while(loops > 0) {
        in1 = *pIn++;
        in2 = *pIn++;
        *simd++ = (((uint32_t)(in1 > 0 ? in1 : (int16_t)__QSUB16(0, in1))) & 0x0000ffff) | 
                 ((((uint32_t)(in2 > 0 ? in2 : (int16_t)__QSUB16(0, in2))) << 16) & 0xffff0000);

        in1 = *pIn++;
        in2 = *pIn++;
        *simd++ = (((uint32_t)(in1 > 0 ? in1 : (int16_t)__QSUB16(0, in1))) & 0x0000ffff) | 
                 ((((uint32_t)(in2 > 0 ? in2 : (int16_t)__QSUB16(0, in2))) << 16) & 0xffff0000);

        loops--;
    }
}

