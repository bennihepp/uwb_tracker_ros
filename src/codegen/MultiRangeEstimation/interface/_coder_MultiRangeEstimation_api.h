/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_MultiRangeEstimation_api.h
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 12-Feb-2016 14:20:28
 */

#ifndef ___CODER_MULTIRANGEESTIMATION_API_H__
#define ___CODER_MULTIRANGEESTIMATION_API_H__

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_MultiRangeEstimation_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T_1x255
#define struct_emxArray_real_T_1x255

struct emxArray_real_T_1x255
{
  real_T data[255];
  int32_T size[2];
};

#endif                                 /*struct_emxArray_real_T_1x255*/

#ifndef typedef_emxArray_real_T_1x255
#define typedef_emxArray_real_T_1x255

typedef struct emxArray_real_T_1x255 emxArray_real_T_1x255;

#endif                                 /*typedef_emxArray_real_T_1x255*/

#ifndef typedef_MultiRangeEstimationParams
#define typedef_MultiRangeEstimationParams

typedef struct {
  real_T US_TO_DW_TIMEUNITS;
  real_T DW_TIMEUNITS_TO_US;
  real_T SPEED_OF_LIGHT_IN_M_PER_US;
  real_T SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT;
  emxArray_real_T_1x255 unit_distances;
} MultiRangeEstimationParams;

#endif                                 /*typedef_MultiRangeEstimationParams*/

#ifndef typedef_TargetEstimationParams
#define typedef_TargetEstimationParams

typedef struct {
  real_T tracker_mean[3];
  real_T m_uwb1[3];
  real_T m_uwb2[3];
  real_T m_uwb3[3];
  real_T m_uwb4[3];
  real_T UWB1p0;
  real_T UWB1p1;
  real_T UWB2p0;
  real_T UWB2p1;
  real_T UWB3p0;
  real_T UWB3p1;
  real_T UWB4p0;
  real_T UWB4p1;
} TargetEstimationParams;

#endif                                 /*typedef_TargetEstimationParams*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void MultiRangeEstimation(real_T timestamp_master_request_1_recv, real_T
  timestamp_slave_reply_send, real_T timestamp_master_request_2_recv, real_T
  timestamp_master_request_1_array_data[], int32_T
  timestamp_master_request_1_array_size[2], real_T
  timestamp_slave_reply_array_data[], int32_T timestamp_slave_reply_array_size[2],
  real_T timestamp_master_request_2_array_data[], int32_T
  timestamp_master_request_2_array_size[2], MultiRangeEstimationParams *params,
  real_T tofs_data[], int32_T tofs_size[2], real_T ranges_data[], int32_T
  ranges_size[2], real_T clock_offsets_data[], int32_T clock_offsets_size[2],
  real_T clock_skews_data[], int32_T clock_skews_size[2]);
extern void MultiRangeEstimation_api(const mxArray *prhs[7], const mxArray *
  plhs[4]);
extern void MultiRangeEstimation_atexit(void);
extern void MultiRangeEstimation_initialize(void);
extern void MultiRangeEstimation_terminate(void);
extern void MultiRangeEstimation_xil_terminate(void);
extern void TargetEstimationNominalBatch(real_T rangeArray_data[], int32_T
  rangeArray_size[2], TargetEstimationParams *params, real_T pos_init[3], real_T
  x_out[6]);
extern void TargetEstimationNominalBatch_api(const mxArray *prhs[3], const
  mxArray *plhs[1]);

#endif

/*
 * File trailer for _coder_MultiRangeEstimation_api.h
 *
 * [EOF]
 */
