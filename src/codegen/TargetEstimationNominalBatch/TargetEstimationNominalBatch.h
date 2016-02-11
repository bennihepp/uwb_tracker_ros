//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TargetEstimationNominalBatch.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 11-Feb-2016 15:19:32
//
#ifndef __TARGETESTIMATIONNOMINALBATCH_H__
#define __TARGETESTIMATIONNOMINALBATCH_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "TargetEstimationNominalBatch_types.h"

// Function Declarations
extern void TargetEstimationNominalBatch(const float rangeArray_data[], const
  int rangeArray_size[2], const TargetEstimationParams *params, const float
  pos_init[3], float x_out[6]);
extern void TargetEstimationNominalBatch_initialize();
extern void TargetEstimationNominalBatch_terminate();
extern void UWBMultiRangeEstimation(unsigned char num_of_units, unsigned long
  timestamp_master_request_1_recv, unsigned long timestamp_slave_reply_send,
  unsigned long timestamp_master_request_2_recv, const unsigned long
  c_timestamp_master_request_1_ar[], const int d_timestamp_master_request_1_ar[2],
  const unsigned long c_timestamp_slave_reply_array_d[], const int
  c_timestamp_slave_reply_array_s[2], const unsigned long
  c_timestamp_master_request_2_ar[], const int d_timestamp_master_request_2_ar[2],
  float tofs_data[], int tofs_size[2], float ranges_data[], int ranges_size[2]);

#endif

//
// File trailer for TargetEstimationNominalBatch.h
//
// [EOF]
//
