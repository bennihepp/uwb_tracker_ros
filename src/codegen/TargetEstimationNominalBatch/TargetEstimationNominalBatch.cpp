//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TargetEstimationNominalBatch.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 08-Feb-2016 14:02:33
//

// Include Files
#include "rt_nonfinite.h"
#include "TargetEstimationNominalBatch.h"

// Variable Definitions
static boolean_T initialized_not_empty;
static float x[6];
static float P[36];

// Function Declarations
static float norm(const float b_x[3]);
static void xgetrf(float A[16], int ipiv[4], int *info);

// Function Definitions

//
// Arguments    : const float b_x[3]
// Return Type  : float
//
static float norm(const float b_x[3])
{
  float y;
  float scale;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  scale = 1.17549435E-38F;
  for (k = 0; k < 3; k++) {
    absxk = (float)fabs((double)b_x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0F + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * (float)sqrt((double)y);
}

//
// Arguments    : float A[16]
//                int ipiv[4]
//                int *info
// Return Type  : void
//
static void xgetrf(float A[16], int ipiv[4], int *info)
{
  int i2;
  int j;
  int c;
  int iy;
  int ix;
  float smax;
  int jy;
  float s;
  int b_j;
  int ijA;
  for (i2 = 0; i2 < 4; i2++) {
    ipiv[i2] = 1 + i2;
  }

  *info = 0;
  for (j = 0; j < 3; j++) {
    c = j * 5;
    iy = 0;
    ix = c;
    smax = (float)fabs((double)A[c]);
    for (jy = 2; jy <= 4 - j; jy++) {
      ix++;
      s = (float)fabs((double)A[ix]);
      if (s > smax) {
        iy = jy - 1;
        smax = s;
      }
    }

    if (A[c + iy] != 0.0F) {
      if (iy != 0) {
        ipiv[j] = (j + iy) + 1;
        ix = j;
        iy += j;
        for (jy = 0; jy < 4; jy++) {
          smax = A[ix];
          A[ix] = A[iy];
          A[iy] = smax;
          ix += 4;
          iy += 4;
        }
      }

      i2 = (c - j) + 4;
      for (iy = c + 1; iy + 1 <= i2; iy++) {
        A[iy] /= A[c];
      }
    } else {
      *info = j + 1;
    }

    iy = c;
    jy = c + 4;
    for (b_j = 1; b_j <= 3 - j; b_j++) {
      smax = A[jy];
      if (A[jy] != 0.0F) {
        ix = c + 1;
        i2 = (iy - j) + 8;
        for (ijA = 5 + iy; ijA + 1 <= i2; ijA++) {
          A[ijA] += A[ix] * -smax;
          ix++;
        }
      }

      jy += 4;
      iy += 4;
    }
  }

  if ((*info == 0) && (!(A[15] != 0.0F))) {
    *info = 4;
  }
}

//
// UNTITLED Summary of this function goes here
//    Detailed explanation goes here
//  model
// Arguments    : const float rangeArray[4]
//                const Params *params
//                const float pos_init[3]
//                float x_out[6]
// Return Type  : void
//
void TargetEstimationNominalBatch(const float rangeArray[4], const Params
  *params, const float pos_init[3], float x_out[6])
{
  int i0;
  static const signed char iv0[36] = { 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0,
    0, 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 10 };

  int ia;
  float o[12];
  float a[12];
  float b[12];
  int i1;
  static const signed char b_a[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  float n[6];
  float c_a[36];
  static const float d_a[36] = { 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0166666675F,
    0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0166666675F, 0.0F, 0.0F, 1.0F, 0.0F,
    0.0F, 0.0F, 0.0166666675F, 0.0F, 0.0F, 1.0F };

  int jp;
  float temp;
  static const float b_b[36] = { 1.0F, 0.0F, 0.0F, 0.0166666675F, 0.0F, 0.0F,
    0.0F, 1.0F, 0.0F, 0.0F, 0.0166666675F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,
    0.0166666675F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F };

  static const float fv0[36] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.2F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.2F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.2F };

  float pos[3];
  float InitialRangeArray[4];
  int K_size_idx_0;
  int K_size_idx_1;
  float K_data[36];
  float H[24];
  float c[24];
  int iter;
  float h[4];
  float b_H[24];
  float res[4];
  int cntUWB;
  float b_pos[3];
  float h_to_hn;
  float hn_to_hs;
  float S[16];
  static const float fv1[16] = { 0.03F, 0.0F, 0.0F, 0.0F, 0.0F, 0.03F, 0.0F,
    0.0F, 0.0F, 0.0F, 0.03F, 0.0F, 0.0F, 0.0F, 0.0F, 0.03F };

  float A[16];
  int ipiv[4];
  int br;
  int ar;
  int ib;
  int kBcol;
  float b_c[4];
  float c_data[6];
  int ic;
  int outl;
  float b_c_data[36];
  float B[36];
  static const signed char iv1[36] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };

  if (!initialized_not_empty) {
    initialized_not_empty = true;
    for (i0 = 0; i0 < 36; i0++) {
      P[i0] = iv0[i0];
    }

    P[14] = 0.0F;
    for (ia = 0; ia < 3; ia++) {
      for (i0 = 0; i0 < 3; i0++) {
        P[(i0 + 6 * (3 + ia)) + 3] = 0.0F;
      }

      x[ia] = pos_init[ia];
    }

    x[3] = 0.0F;
    x[4] = 0.0F;
    x[5] = 0.0F;
  }

  for (i0 = 0; i0 < 12; i0++) {
    o[i0] = 0.0F;
    a[i0] = 0.0F;
    b[i0] = 0.0F;
  }

  a[0] = params->UWB1p0;
  b[0] = params->UWB1p1;
  a[1] = params->UWB2p0;
  b[1] = params->UWB2p1;
  a[2] = params->UWB3p0;
  b[2] = params->UWB3p1;
  for (i0 = 0; i0 < 3; i0++) {
    o[i0] = 0.0F;
    o[3 + i0] = 0.0F;
    o[6 + i0] = 0.0F;
    o[9 + i0] = 0.0F;
    for (i1 = 0; i1 < 3; i1++) {
      o[i0] += (float)b_a[i0 + 3 * i1] * params->m_uwb1[i1];
      o[3 + i0] += (float)b_a[i0 + 3 * i1] * params->m_uwb2[i1];
      o[6 + i0] += (float)b_a[i0 + 3 * i1] * params->m_uwb3[i1];
      o[9 + i0] += (float)b_a[i0 + 3 * i1] * params->m_uwb4[i1];
    }
  }

  a[3] = params->UWB4p0;
  b[3] = params->UWB4p1;

  //  prediction
  for (ia = 0; ia < 6; ia++) {
    n[ia] = x[ia];
  }

  for (i0 = 0; i0 < 6; i0++) {
    x[i0] = 0.0F;
    for (i1 = 0; i1 < 6; i1++) {
      x[i0] += d_a[i0 + 6 * i1] * n[i1];
      c_a[i0 + 6 * i1] = 0.0F;
      for (jp = 0; jp < 6; jp++) {
        c_a[i0 + 6 * i1] += d_a[i0 + 6 * jp] * P[jp + 6 * i1];
      }
    }
  }

  for (ia = 0; ia < 6; ia++) {
    for (i0 = 0; i0 < 6; i0++) {
      temp = 0.0F;
      for (i1 = 0; i1 < 6; i1++) {
        temp += c_a[ia + 6 * i1] * b_b[i1 + 6 * i0];
      }

      P[ia + 6 * i0] = temp + fv0[ia + 6 * i0];
    }

    n[ia] = x[ia];
  }

  for (ia = 0; ia < 3; ia++) {
    pos[ia] = x[ia];
  }

  InitialRangeArray[0] = rangeArray[0];
  InitialRangeArray[1] = rangeArray[1];
  InitialRangeArray[2] = rangeArray[2];
  InitialRangeArray[3] = rangeArray[3];
  K_size_idx_0 = 4;
  K_size_idx_1 = 6;
  memset(&K_data[0], 0, 24U * sizeof(float));
  for (iter = 0; iter < 4; iter++) {
    for (cntUWB = 0; cntUWB < 4; cntUWB++) {
      //          a=[0;0;0;0];
      //          b=[1;1;1;1];
      for (i0 = 0; i0 < 3; i0++) {
        b_pos[i0] = pos[i0] - o[i0 + 3 * cntUWB];
      }

      temp = norm(b_pos);

      // derivatives
      h_to_hn = 1.0F / b[cntUWB];
      hn_to_hs = 1.0F / (2.0F * (float)sqrt((double)temp));
      for (i0 = 0; i0 < 3; i0++) {
        H[cntUWB + (i0 << 2)] = h_to_hn * (hn_to_hs * (2.0F * pos[i0] - 2.0F *
          o[i0 + 3 * cntUWB]));
      }

      H[12 + cntUWB] = 0.0F;
      H[16 + cntUWB] = 0.0F;
      H[20 + cntUWB] = 0.0F;
      h[cntUWB] = (-a[cntUWB] + temp) / b[cntUWB];
      res[cntUWB] = InitialRangeArray[cntUWB] - h[cntUWB];
      for (i0 = 0; i0 < 6; i0++) {
        b_H[cntUWB + (i0 << 2)] = 0.0F;
        for (i1 = 0; i1 < 6; i1++) {
          b_H[cntUWB + (i0 << 2)] += H[cntUWB + (i1 << 2)] * P[i1 + 6 * i0];
        }
      }
    }

    for (i0 = 0; i0 < 4; i0++) {
      for (i1 = 0; i1 < 4; i1++) {
        temp = 0.0F;
        for (jp = 0; jp < 6; jp++) {
          temp += b_H[i0 + (jp << 2)] * H[i1 + (jp << 2)];
        }

        S[i0 + (i1 << 2)] = temp + fv1[i0 + (i1 << 2)];
      }
    }

    for (i0 = 0; i0 < 6; i0++) {
      for (i1 = 0; i1 < 4; i1++) {
        c[i0 + 6 * i1] = 0.0F;
        for (jp = 0; jp < 6; jp++) {
          c[i0 + 6 * i1] += P[i0 + 6 * jp] * H[i1 + (jp << 2)];
        }
      }
    }

    memcpy(&A[0], &S[0], sizeof(float) << 4);
    xgetrf(A, ipiv, &jp);
    for (br = 0; br < 4; br++) {
      jp = 6 * br;
      ar = br << 2;
      for (ib = 0; ib + 1 <= br; ib++) {
        kBcol = 6 * ib;
        if (A[ib + ar] != 0.0F) {
          for (ia = 0; ia < 6; ia++) {
            c[ia + jp] -= A[ib + ar] * c[ia + kBcol];
          }
        }
      }

      temp = 1.0F / A[br + ar];
      for (ia = 0; ia < 6; ia++) {
        c[ia + jp] *= temp;
      }
    }

    for (br = 3; br >= 0; br += -1) {
      jp = 6 * br;
      ar = br << 2;
      for (ib = br + 1; ib + 1 < 5; ib++) {
        kBcol = 6 * ib;
        if (A[ib + ar] != 0.0F) {
          for (ia = 0; ia < 6; ia++) {
            c[ia + jp] -= A[ib + ar] * c[ia + kBcol];
          }
        }
      }
    }

    for (br = 2; br >= 0; br += -1) {
      if (ipiv[br] != br + 1) {
        jp = ipiv[br] - 1;
        for (kBcol = 0; kBcol < 6; kBcol++) {
          temp = c[kBcol + 6 * br];
          c[kBcol + 6 * br] = c[kBcol + 6 * jp];
          c[kBcol + 6 * jp] = temp;
        }
      }
    }

    K_size_idx_0 = 6;
    K_size_idx_1 = 4;
    memcpy(&K_data[0], &c[0], 24U * sizeof(float));

    //  chi^2 outlier rejection
    xgetrf(S, ipiv, &jp);
    for (i0 = 0; i0 < 4; i0++) {
      b_c[i0] = res[i0];
    }

    for (br = 0; br < 4; br++) {
      ar = br << 2;
      for (ib = 0; ib + 1 <= br; ib++) {
        if (S[ib + ar] != 0.0F) {
          b_c[br] -= S[ib + ar] * b_c[ib];
        }
      }

      b_c[br] *= 1.0F / S[br + ar];
    }

    for (br = 3; br >= 0; br += -1) {
      ar = br << 2;
      for (ib = br + 1; ib + 1 < 5; ib++) {
        if (S[ib + ar] != 0.0F) {
          b_c[br] -= S[ib + ar] * b_c[ib];
        }
      }
    }

    for (br = 2; br >= 0; br += -1) {
      if (ipiv[br] != br + 1) {
        temp = b_c[br];
        b_c[br] = b_c[ipiv[br] - 1];
        b_c[ipiv[br] - 1] = temp;
      }
    }

    temp = 0.0F;
    for (i0 = 0; i0 < 4; i0++) {
      temp += b_c[i0] * res[i0];
    }

    if ((float)sqrt((double)temp) < 4.0F) {
      for (i0 = 0; i0 < 6; i0++) {
        c_data[i0] = x[i0] - n[i0];
      }

      for (i0 = 0; i0 < 4; i0++) {
        temp = 0.0F;
        for (i1 = 0; i1 < 6; i1++) {
          temp += H[i0 + (i1 << 2)] * c_data[i1];
        }

        b_c[i0] = res[i0] - temp;
      }

      for (i0 = 0; i0 < 6; i0++) {
        c_data[i0] = 0.0F;
      }

      for (ic = 1; ic < 7; ic++) {
        c_data[ic - 1] = 0.0F;
      }

      ar = 0;
      for (ib = 0; ib + 1 < 5; ib++) {
        if (b_c[ib] != 0.0F) {
          ia = ar;
          for (ic = 0; ic + 1 < 7; ic++) {
            ia++;
            c_data[ic] += b_c[ib] * c[ia - 1];
          }
        }

        ar += 6;
      }

      for (i0 = 0; i0 < 6; i0++) {
        n[i0] = x[i0] + c_data[i0];
      }

      outl = 0;
    } else {
      outl = 1;
    }
  }

  if (outl == 0) {
    //  only do update if there was no outlyer
    for (i0 = 0; i0 < 6; i0++) {
      for (i1 = 0; i1 < K_size_idx_0; i1++) {
        b_c_data[i1 + K_size_idx_0 * i0] = 0.0F;
      }
    }

    jp = K_size_idx_0 * 5;
    for (kBcol = 0; kBcol <= jp; kBcol += K_size_idx_0) {
      i0 = kBcol + K_size_idx_0;
      for (ic = kBcol; ic + 1 <= i0; ic++) {
        b_c_data[ic] = 0.0F;
      }
    }

    br = 0;
    for (kBcol = 0; kBcol <= jp; kBcol += K_size_idx_0) {
      ar = 0;
      i0 = br + K_size_idx_1;
      for (ib = br; ib + 1 <= i0; ib++) {
        if (H[ib] != 0.0F) {
          ia = ar;
          i1 = kBcol + K_size_idx_0;
          for (ic = kBcol; ic + 1 <= i1; ic++) {
            ia++;
            b_c_data[ic] += H[ib] * K_data[ia - 1];
          }
        }

        ar += K_size_idx_0;
      }

      br += K_size_idx_1;
    }

    memcpy(&B[0], &P[0], 36U * sizeof(float));
    for (i0 = 0; i0 < 6; i0++) {
      for (i1 = 0; i1 < 6; i1++) {
        c_a[i1 + 6 * i0] = (float)iv1[i1 + 6 * i0] - b_c_data[i1 + 6 * i0];
      }
    }

    for (i0 = 0; i0 < 6; i0++) {
      for (i1 = 0; i1 < 6; i1++) {
        P[i0 + 6 * i1] = 0.0F;
        for (jp = 0; jp < 6; jp++) {
          P[i0 + 6 * i1] += c_a[i0 + 6 * jp] * B[jp + 6 * i1];
        }
      }
    }
  }

  //  copy the new state n after the iterated update into the real state
  // % state augmentation
  //  find first oldest state
  //  copy the state to the output variables
  for (ia = 0; ia < 6; ia++) {
    x[ia] = n[ia];
    x_out[ia] = x[ia];
  }
}

//
// Arguments    : void
// Return Type  : void
//
void TargetEstimationNominalBatch_initialize()
{
  rt_InitInfAndNaN(8U);
  initialized_not_empty = false;
}

//
// Arguments    : void
// Return Type  : void
//
void TargetEstimationNominalBatch_terminate()
{
  // (no terminate code required)
}

//
// File trailer for TargetEstimationNominalBatch.cpp
//
// [EOF]
//
