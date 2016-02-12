//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: MultiRangeEstimation.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 12-Feb-2016 14:20:28
//
#ifndef __MULTIRANGEESTIMATION_H__
#define __MULTIRANGEESTIMATION_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "MultiRangeEstimation_types.h"

// Function Declarations
extern void MultiRangeEstimation(double timestamp_master_request_1_recv, double
  timestamp_slave_reply_send, double timestamp_master_request_2_recv, const
  double timestamp_master_request_1_array_data[], const int
  timestamp_master_request_1_array_size[2], const double
  timestamp_slave_reply_array_data[], const int
  timestamp_slave_reply_array_size[2], const double
  timestamp_master_request_2_array_data[], const int
  timestamp_master_request_2_array_size[2], const MultiRangeEstimationParams
  *params, double tofs_data[], int tofs_size[2], double ranges_data[], int
  ranges_size[2], double clock_offsets_data[], int clock_offsets_size[2], double
  clock_skews_data[], int clock_skews_size[2]);
extern void MultiRangeEstimation_initialize();
extern void MultiRangeEstimation_terminate();
extern void TargetEstimationNominalBatch(const double rangeArray_data[], const
  int rangeArray_size[2], const TargetEstimationParams *params, const double
  pos_init[3], double x_out[6]);

#endif

//
// File trailer for MultiRangeEstimation.h
//
// [EOF]
//
