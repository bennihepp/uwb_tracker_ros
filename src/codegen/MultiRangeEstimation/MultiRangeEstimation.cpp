//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: MultiRangeEstimation.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 12-Feb-2016 14:20:28
//

// Include Files
#include "rt_nonfinite.h"
#include "MultiRangeEstimation.h"

// Custom Source Code
#include <stdio.h>

// Variable Definitions
static boolean_T initialized_not_empty;
static double x[6];
static double P[36];

// Function Declarations
static void mrdivide(double A[24], const double B[16]);
static double norm(const double b_x[3]);
static void xgetrf(double A[16], int ipiv[4], int *info);

// Function Definitions

//
// Arguments    : double A[24]
//                const double B[16]
// Return Type  : void
//
static void mrdivide(double A[24], const double B[16])
{
  double b_A[16];
  int info;
  int ipiv[4];
  int jp;
  int jAcol;
  int k;
  int kBcol;
  int i;
  double temp;
  memcpy(&b_A[0], &B[0], sizeof(double) << 4);
  xgetrf(b_A, ipiv, &info);
  for (info = 0; info < 4; info++) {
    jp = 6 * info;
    jAcol = info << 2;
    for (k = 0; k + 1 <= info; k++) {
      kBcol = 6 * k;
      if (b_A[k + jAcol] != 0.0) {
        for (i = 0; i < 6; i++) {
          A[i + jp] -= b_A[k + jAcol] * A[i + kBcol];
        }
      }
    }

    temp = 1.0 / b_A[info + jAcol];
    for (i = 0; i < 6; i++) {
      A[i + jp] *= temp;
    }
  }

  for (info = 3; info >= 0; info += -1) {
    jp = 6 * info;
    jAcol = info << 2;
    for (k = info + 1; k + 1 < 5; k++) {
      kBcol = 6 * k;
      if (b_A[k + jAcol] != 0.0) {
        for (i = 0; i < 6; i++) {
          A[i + jp] -= b_A[k + jAcol] * A[i + kBcol];
        }
      }
    }
  }

  for (info = 2; info >= 0; info += -1) {
    if (ipiv[info] != info + 1) {
      jp = ipiv[info] - 1;
      for (jAcol = 0; jAcol < 6; jAcol++) {
        temp = A[jAcol + 6 * info];
        A[jAcol + 6 * info] = A[jAcol + 6 * jp];
        A[jAcol + 6 * jp] = temp;
      }
    }
  }
}

//
// Arguments    : const double b_x[3]
// Return Type  : double
//
static double norm(const double b_x[3])
{
  double y;
  double scale;
  int k;
  double absxk;
  double t;
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 3; k++) {
    absxk = fabs(b_x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

//
// Arguments    : double A[16]
//                int ipiv[4]
//                int *info
// Return Type  : void
//
static void xgetrf(double A[16], int ipiv[4], int *info)
{
  int i3;
  int j;
  int c;
  int iy;
  int ix;
  double smax;
  int jy;
  double s;
  int b_j;
  int ijA;
  for (i3 = 0; i3 < 4; i3++) {
    ipiv[i3] = 1 + i3;
  }

  *info = 0;
  for (j = 0; j < 3; j++) {
    c = j * 5;
    iy = 0;
    ix = c;
    smax = fabs(A[c]);
    for (jy = 2; jy <= 4 - j; jy++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        iy = jy - 1;
        smax = s;
      }
    }

    if (A[c + iy] != 0.0) {
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

      i3 = (c - j) + 4;
      for (iy = c + 1; iy + 1 <= i3; iy++) {
        A[iy] /= A[c];
      }
    } else {
      *info = j + 1;
    }

    iy = c;
    jy = c + 4;
    for (b_j = 1; b_j <= 3 - j; b_j++) {
      smax = A[jy];
      if (A[jy] != 0.0) {
        ix = c + 1;
        i3 = (iy - j) + 8;
        for (ijA = 5 + iy; ijA + 1 <= i3; ijA++) {
          A[ijA] += A[ix] * -smax;
          ix++;
        }
      }

      jy += 4;
      iy += 4;
    }
  }

  if ((*info == 0) && (!(A[15] != 0.0))) {
    *info = 4;
  }
}

//
// Arguments    : double timestamp_master_request_1_recv
//                double timestamp_slave_reply_send
//                double timestamp_master_request_2_recv
//                const double timestamp_master_request_1_array_data[]
//                const int timestamp_master_request_1_array_size[2]
//                const double timestamp_slave_reply_array_data[]
//                const int timestamp_slave_reply_array_size[2]
//                const double timestamp_master_request_2_array_data[]
//                const int timestamp_master_request_2_array_size[2]
//                const MultiRangeEstimationParams *params
//                double tofs_data[]
//                int tofs_size[2]
//                double ranges_data[]
//                int ranges_size[2]
//                double clock_offsets_data[]
//                int clock_offsets_size[2]
//                double clock_skews_data[]
//                int clock_skews_size[2]
// Return Type  : void
//
void MultiRangeEstimation(double timestamp_master_request_1_recv, double
  timestamp_slave_reply_send, double timestamp_master_request_2_recv, const
  double timestamp_master_request_1_array_data[], const int
  timestamp_master_request_1_array_size[2], const double
  timestamp_slave_reply_array_data[], const int [2], const double
  timestamp_master_request_2_array_data[], const int [2], const
  MultiRangeEstimationParams *params, double tofs_data[], int tofs_size[2],
  double ranges_data[], int ranges_size[2], double clock_offsets_data[], int
  clock_offsets_size[2], double clock_skews_data[], int clock_skews_size[2])
{
  int loop_ub;
  int i0;
  double primary_tof;
  double primary_range;
  double clock_diff_1;
  double adjusted_processing_time_slave_us;
  int b_index;
  double clock_offset;
  double clock_skew;
  double tof_secondary;

  // % Compute time-of-flights, ranges, clock offsets and skews for multi-unit UWB tracker 
  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['num_of_units: %f\n' 10 0], num_of_units);
  //          coder.ceval('printf', ['timestamp_master_request_1_recv: %f\n' 10 0], timestamp_master_request_1_recv); 
  //          coder.ceval('printf', ['timestamp_master_request_1_array(1): %f\n' 10 0], timestamp_master_request_1_array(1)); 
  //      end
  tofs_size[0] = 1;
  tofs_size[1] = timestamp_master_request_1_array_size[1];
  loop_ub = timestamp_master_request_1_array_size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    tofs_data[i0] = 0.0;
  }

  ranges_size[0] = 1;
  ranges_size[1] = timestamp_master_request_1_array_size[1];
  loop_ub = timestamp_master_request_1_array_size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    ranges_data[i0] = 0.0;
  }

  clock_offsets_size[0] = 1;
  clock_offsets_size[1] = timestamp_master_request_1_array_size[1] + 1;
  loop_ub = timestamp_master_request_1_array_size[1] + 1;
  for (i0 = 0; i0 < loop_ub; i0++) {
    clock_offsets_data[i0] = 0.0;
  }

  clock_skews_size[0] = 1;
  clock_skews_size[1] = timestamp_master_request_1_array_size[1] + 1;
  loop_ub = timestamp_master_request_1_array_size[1] + 1;
  for (i0 = 0; i0 < loop_ub; i0++) {
    clock_skews_data[i0] = 0.0;
  }

  // % Compute time-of-flight, range and time-of-flight in DW timeunits of primary UWB unit 
  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['timediff_slave: %f\n' 10 0], timediff_slave); 
  //          coder.ceval('printf', ['timediff_master: %f\n' 10 0], timediff_master); 
  //      end
  // % Convert DW timeunit into microseconds
  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['dw_timeunits: %f\n' 10 0], dw_timeunits);
  //          coder.ceval('printf', ['microseconds: %f\n' 10 0], microseconds);
  //          coder.ceval('printf', ['DW_TIMEUNITS_TO_US: %f\n' 10 0], params.DW_TIMEUNITS_TO_US); 
  //      end
  primary_tof = (((2.0 * timestamp_slave_reply_array_data[0] -
                   timestamp_master_request_1_array_data[0]) -
                  timestamp_master_request_2_array_data[0]) +
                 ((timestamp_master_request_1_recv +
                   timestamp_master_request_2_recv) - 2.0 *
                  timestamp_slave_reply_send)) * params->DW_TIMEUNITS_TO_US /
    4.0;

  // % Convert microseconds time-of-flight into distance
  primary_range = primary_tof * params->SPEED_OF_LIGHT_IN_M_PER_US;

  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['primary_tof: %f' 10 0], primary_tof);
  //      end
  tofs_data[0] = primary_tof;
  ranges_data[0] = primary_range;

  //  clock offset
  // % Compute clock offset and skew of slave UWB unit
  //  clock skew
  // % Convert DW timeunit into microseconds
  clock_diff_1 = (timestamp_master_request_2_array_data[0] -
                  timestamp_master_request_1_array_data[0]) *
    params->DW_TIMEUNITS_TO_US;

  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['dw_timeunits: %f\n' 10 0], dw_timeunits);
  //          coder.ceval('printf', ['microseconds: %f\n' 10 0], microseconds);
  //          coder.ceval('printf', ['DW_TIMEUNITS_TO_US: %f\n' 10 0], params.DW_TIMEUNITS_TO_US); 
  //      end
  // % Convert DW timeunit into microseconds
  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['dw_timeunits: %f\n' 10 0], dw_timeunits);
  //          coder.ceval('printf', ['microseconds: %f\n' 10 0], microseconds);
  //          coder.ceval('printf', ['DW_TIMEUNITS_TO_US: %f\n' 10 0], params.DW_TIMEUNITS_TO_US); 
  //      end
  clock_offsets_data[timestamp_master_request_1_array_size[1]] =
    (timestamp_master_request_1_recv - timestamp_master_request_1_array_data[0])
    - primary_range / params->SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT;
  clock_skews_data[timestamp_master_request_1_array_size[1]] = (1.0 /
    ((timestamp_master_request_2_recv - timestamp_master_request_1_recv) *
     params->DW_TIMEUNITS_TO_US) - 1.0 / clock_diff_1) / (1.0 / clock_diff_1);

  //  Compute slave processing time
  // % Convert DW timeunit into microseconds
  //      if ~coder.target('MATLAB')
  //          coder.ceval('printf', ['dw_timeunits: %f\n' 10 0], dw_timeunits);
  //          coder.ceval('printf', ['microseconds: %f\n' 10 0], microseconds);
  //          coder.ceval('printf', ['DW_TIMEUNITS_TO_US: %f\n' 10 0], params.DW_TIMEUNITS_TO_US); 
  //      end
  adjusted_processing_time_slave_us = (timestamp_slave_reply_array_data[0] -
    timestamp_master_request_1_array_data[0]) * params->DW_TIMEUNITS_TO_US - 2.0
    * primary_tof;

  //  Alternative: Use slave measurements and clock_skew to compute slave
  //  processing time
  //      adjusted_processing_time_slave = double( ...
  //          timestamp_slave_reply_send - timestamp_master_request_1_recv ...
  //      ) * (1 + slave_clock_skew);
  for (b_index = 1; b_index - 1 <= timestamp_master_request_1_array_size[1] - 2;
       b_index++) {
    //  clock offset
    // % Compute clock offset and skew of secondary UWB unit
    clock_offset = (timestamp_master_request_1_array_data[b_index] -
                    timestamp_master_request_1_array_data[0]) -
      params->unit_distances.data[b_index] /
      params->SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT;

    //  clock skew
    clock_diff_1 = timestamp_master_request_2_array_data[0] -
      timestamp_master_request_1_array_data[0];
    clock_skew = (1.0 / (timestamp_master_request_2_array_data[b_index] -
                         timestamp_master_request_1_array_data[b_index]) - 1.0 /
                  clock_diff_1) / (1.0 / clock_diff_1);

    // % Convert DW timeunit into microseconds
    //      if ~coder.target('MATLAB')
    //          coder.ceval('printf', ['dw_timeunits: %f\n' 10 0], dw_timeunits); 
    //          coder.ceval('printf', ['microseconds: %f\n' 10 0], microseconds); 
    //          coder.ceval('printf', ['DW_TIMEUNITS_TO_US: %f\n' 10 0], params.DW_TIMEUNITS_TO_US); 
    //      end
    tof_secondary = (((timestamp_slave_reply_array_data[b_index] - clock_offset)
                      - timestamp_master_request_1_array_data[0]) *
                     params->DW_TIMEUNITS_TO_US * (1.0 + clock_skew) -
                     primary_tof) - adjusted_processing_time_slave_us;

    // % Convert microseconds time-of-flight into distance
    tofs_data[b_index] = tof_secondary;
    ranges_data[b_index] = tof_secondary * params->SPEED_OF_LIGHT_IN_M_PER_US;
    clock_offsets_data[b_index] = clock_offset;
    clock_skews_data[b_index] = clock_skew;
  }
}

//
// Arguments    : void
// Return Type  : void
//
void MultiRangeEstimation_initialize()
{
  rt_InitInfAndNaN(8U);
  initialized_not_empty = false;
}

//
// Arguments    : void
// Return Type  : void
//
void MultiRangeEstimation_terminate()
{
  // (no terminate code required)
}

//
// UNTITLED Summary of this function goes here
//    Detailed explanation goes here
//  model
// Arguments    : const double rangeArray_data[]
//                const int rangeArray_size[2]
//                const TargetEstimationParams *params
//                const double pos_init[3]
//                double x_out[6]
// Return Type  : void
//
void TargetEstimationNominalBatch(const double rangeArray_data[], const int [2],
  const TargetEstimationParams *params, const double pos_init[3], double x_out[6])
{
  int i1;
  static const signed char iv0[36] = { 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0,
    0, 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 10 };

  int k;
  double o[12];
  double a[12];
  double b[12];
  int i2;
  static const signed char b_a[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  double n[6];
  double c_a[36];
  static const double d_a[36] = { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.016666666666666666, 0.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 0.016666666666666666, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
    0.016666666666666666, 0.0, 0.0, 1.0 };

  double temp;
  static const double b_b[36] = { 1.0, 0.0, 0.0, 0.016666666666666666, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.016666666666666666, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
    0.016666666666666666, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

  static const double varargin_1[36] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2 };

  double pos[3];
  double InitialRangeArray[4];
  int K_size_idx_0;
  int K_size_idx_1;
  double K_data[36];
  double H[24];
  int iter;
  double h[4];
  double b_H[24];
  double res[4];
  int cntUWB;
  double b_pos[3];
  double h_n;
  double h_to_hn;
  double hn_to_hs;
  double S[16];
  static const double R[16] = { 0.03, 0.0, 0.0, 0.0, 0.0, 0.03, 0.0, 0.0, 0.0,
    0.0, 0.03, 0.0, 0.0, 0.0, 0.0, 0.03 };

  double dv0[24];
  int ipiv[4];
  double y[4];
  int cr;
  int br;
  double C_data[6];
  int ic;
  int ar;
  int ib;
  int ia;
  int outl;
  double b_C_data[36];
  signed char I[36];
  double B[36];
  if (!initialized_not_empty) {
    initialized_not_empty = true;
    for (i1 = 0; i1 < 36; i1++) {
      P[i1] = iv0[i1];
    }

    P[14] = 0.0;
    for (k = 0; k < 3; k++) {
      for (i1 = 0; i1 < 3; i1++) {
        P[(i1 + 6 * (3 + k)) + 3] = 0.0;
      }

      x[k] = pos_init[k];
    }

    x[3] = 0.0;
    x[4] = 0.0;
    x[5] = 0.0;
  }

  for (i1 = 0; i1 < 12; i1++) {
    o[i1] = 0.0;
    a[i1] = 0.0;
    b[i1] = 0.0;
  }

  a[0] = params->UWB1p0;
  b[0] = params->UWB1p1;
  a[1] = params->UWB2p0;
  b[1] = params->UWB2p1;
  a[2] = params->UWB3p0;
  b[2] = params->UWB3p1;
  for (i1 = 0; i1 < 3; i1++) {
    o[i1] = 0.0;
    o[3 + i1] = 0.0;
    o[6 + i1] = 0.0;
    o[9 + i1] = 0.0;
    for (i2 = 0; i2 < 3; i2++) {
      o[i1] += (double)b_a[i1 + 3 * i2] * params->m_uwb1[i2];
      o[3 + i1] += (double)b_a[i1 + 3 * i2] * params->m_uwb2[i2];
      o[6 + i1] += (double)b_a[i1 + 3 * i2] * params->m_uwb3[i2];
      o[9 + i1] += (double)b_a[i1 + 3 * i2] * params->m_uwb4[i2];
    }
  }

  a[3] = params->UWB4p0;
  b[3] = params->UWB4p1;

  //  prediction
  for (k = 0; k < 6; k++) {
    n[k] = x[k];
  }

  for (i1 = 0; i1 < 6; i1++) {
    x[i1] = 0.0;
    for (i2 = 0; i2 < 6; i2++) {
      x[i1] += d_a[i1 + 6 * i2] * n[i2];
      c_a[i1 + 6 * i2] = 0.0;
      for (k = 0; k < 6; k++) {
        c_a[i1 + 6 * i2] += d_a[i1 + 6 * k] * P[k + 6 * i2];
      }
    }
  }

  for (k = 0; k < 6; k++) {
    for (i1 = 0; i1 < 6; i1++) {
      temp = 0.0;
      for (i2 = 0; i2 < 6; i2++) {
        temp += c_a[k + 6 * i2] * b_b[i2 + 6 * i1];
      }

      P[k + 6 * i1] = temp + varargin_1[k + 6 * i1];
    }

    n[k] = x[k];
  }

  for (k = 0; k < 3; k++) {
    pos[k] = x[k];
  }

  InitialRangeArray[0] = rangeArray_data[0];
  InitialRangeArray[1] = rangeArray_data[1];
  InitialRangeArray[2] = rangeArray_data[2];
  InitialRangeArray[3] = rangeArray_data[3];
  K_size_idx_0 = 4;
  K_size_idx_1 = 6;
  memset(&K_data[0], 0, 24U * sizeof(double));
  for (iter = 0; iter < 4; iter++) {
    for (cntUWB = 0; cntUWB < 4; cntUWB++) {
      //          a=[0;0;0;0];
      //          b=[1;1;1;1];
      for (i1 = 0; i1 < 3; i1++) {
        b_pos[i1] = pos[i1] - o[i1 + 3 * cntUWB];
      }

      h_n = norm(b_pos);

      // derivatives
      h_to_hn = 1.0 / b[cntUWB];
      hn_to_hs = 1.0 / (2.0 * sqrt(h_n));
      for (i1 = 0; i1 < 3; i1++) {
        H[cntUWB + (i1 << 2)] = h_to_hn * (hn_to_hs * (2.0 * pos[i1] - 2.0 *
          o[i1 + 3 * cntUWB]));
      }

      H[12 + cntUWB] = 0.0;
      H[16 + cntUWB] = 0.0;
      H[20 + cntUWB] = 0.0;
      h[cntUWB] = (-a[cntUWB] + h_n) / b[cntUWB];
      res[cntUWB] = InitialRangeArray[cntUWB] - h[cntUWB];
      for (i1 = 0; i1 < 6; i1++) {
        b_H[cntUWB + (i1 << 2)] = 0.0;
        for (i2 = 0; i2 < 6; i2++) {
          b_H[cntUWB + (i1 << 2)] += H[cntUWB + (i2 << 2)] * P[i2 + 6 * i1];
        }
      }
    }

    for (i1 = 0; i1 < 4; i1++) {
      for (i2 = 0; i2 < 4; i2++) {
        temp = 0.0;
        for (k = 0; k < 6; k++) {
          temp += b_H[i1 + (k << 2)] * H[i2 + (k << 2)];
        }

        S[i1 + (i2 << 2)] = temp + R[i1 + (i2 << 2)];
      }
    }

    for (i1 = 0; i1 < 6; i1++) {
      for (i2 = 0; i2 < 4; i2++) {
        dv0[i1 + 6 * i2] = 0.0;
        for (k = 0; k < 6; k++) {
          dv0[i1 + 6 * i2] += P[i1 + 6 * k] * H[i2 + (k << 2)];
        }
      }
    }

    mrdivide(dv0, S);
    K_size_idx_0 = 6;
    K_size_idx_1 = 4;
    memcpy(&K_data[0], &dv0[0], 24U * sizeof(double));

    //  chi^2 outlier rejection
    xgetrf(S, ipiv, &k);
    for (i1 = 0; i1 < 4; i1++) {
      y[i1] = res[i1];
    }

    for (cr = 0; cr < 4; cr++) {
      br = cr << 2;
      for (k = 0; k + 1 <= cr; k++) {
        if (S[k + br] != 0.0) {
          y[cr] -= S[k + br] * y[k];
        }
      }

      y[cr] *= 1.0 / S[cr + br];
    }

    for (cr = 3; cr >= 0; cr += -1) {
      br = cr << 2;
      for (k = cr + 1; k + 1 < 5; k++) {
        if (S[k + br] != 0.0) {
          y[cr] -= S[k + br] * y[k];
        }
      }
    }

    for (k = 2; k >= 0; k += -1) {
      if (ipiv[k] != k + 1) {
        temp = y[k];
        y[k] = y[ipiv[k] - 1];
        y[ipiv[k] - 1] = temp;
      }
    }

    temp = 0.0;
    for (i1 = 0; i1 < 4; i1++) {
      temp += y[i1] * res[i1];
    }

    if (sqrt(temp) < 4.0) {
      for (i1 = 0; i1 < 6; i1++) {
        C_data[i1] = x[i1] - n[i1];
      }

      for (i1 = 0; i1 < 4; i1++) {
        temp = 0.0;
        for (i2 = 0; i2 < 6; i2++) {
          temp += H[i1 + (i2 << 2)] * C_data[i2];
        }

        y[i1] = res[i1] - temp;
      }

      for (i1 = 0; i1 < 6; i1++) {
        C_data[i1] = 0.0;
      }

      for (ic = 1; ic < 7; ic++) {
        C_data[ic - 1] = 0.0;
      }

      ar = 0;
      for (ib = 0; ib + 1 < 5; ib++) {
        if (y[ib] != 0.0) {
          ia = ar;
          for (ic = 0; ic + 1 < 7; ic++) {
            ia++;
            C_data[ic] += y[ib] * K_data[ia - 1];
          }
        }

        ar += 6;
      }

      for (i1 = 0; i1 < 6; i1++) {
        n[i1] = x[i1] + C_data[i1];
      }

      outl = 0;
    } else {
      outl = 1;
    }
  }

  if (outl == 0) {
    //  only do update if there was no outlyer
    for (i1 = 0; i1 < 6; i1++) {
      for (i2 = 0; i2 < K_size_idx_0; i2++) {
        b_C_data[i2 + K_size_idx_0 * i1] = 0.0;
      }
    }

    k = K_size_idx_0 * 5;
    for (cr = 0; cr <= k; cr += K_size_idx_0) {
      i1 = cr + K_size_idx_0;
      for (ic = cr; ic + 1 <= i1; ic++) {
        b_C_data[ic] = 0.0;
      }
    }

    br = 0;
    for (cr = 0; cr <= k; cr += K_size_idx_0) {
      ar = 0;
      i1 = br + K_size_idx_1;
      for (ib = br; ib + 1 <= i1; ib++) {
        if (H[ib] != 0.0) {
          ia = ar;
          i2 = cr + K_size_idx_0;
          for (ic = cr; ic + 1 <= i2; ic++) {
            ia++;
            b_C_data[ic] += H[ib] * K_data[ia - 1];
          }
        }

        ar += K_size_idx_0;
      }

      br += K_size_idx_1;
    }

    for (i1 = 0; i1 < 36; i1++) {
      I[i1] = 0;
    }

    for (k = 0; k < 6; k++) {
      I[k + 6 * k] = 1;
    }

    memcpy(&B[0], &P[0], 36U * sizeof(double));
    for (i1 = 0; i1 < 6; i1++) {
      for (i2 = 0; i2 < 6; i2++) {
        c_a[i2 + 6 * i1] = (double)I[i2 + 6 * i1] - b_C_data[i2 + 6 * i1];
      }
    }

    for (i1 = 0; i1 < 6; i1++) {
      for (i2 = 0; i2 < 6; i2++) {
        P[i1 + 6 * i2] = 0.0;
        for (k = 0; k < 6; k++) {
          P[i1 + 6 * i2] += c_a[i1 + 6 * k] * B[k + 6 * i2];
        }
      }
    }
  }

  //  copy the new state n after the iterated update into the real state
  // % state augmentation
  //  find first oldest state
  //  copy the state to the output variables
  for (k = 0; k < 6; k++) {
    x[k] = n[k];
    x_out[k] = x[k];
  }
}

//
// File trailer for MultiRangeEstimation.cpp
//
// [EOF]
//
