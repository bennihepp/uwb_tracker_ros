/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TargetEstimationNominalBatch_api.h
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 08-Feb-2016 14:02:33
 */

#ifndef ___CODER_TARGETESTIMATIONNOMINALBATCH_API_H__
#define ___CODER_TARGETESTIMATIONNOMINALBATCH_API_H__

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_TargetEstimationNominalBatch_api.h"

/* Type Definitions */
#ifndef typedef_Params
#define typedef_Params

typedef struct {
  real32_T tracker_mean[3];
  real32_T m_uwb1[3];
  real32_T m_uwb2[3];
  real32_T m_uwb3[3];
  real32_T m_uwb4[3];
  real32_T UWB1p0;
  real32_T UWB1p1;
  real32_T UWB2p0;
  real32_T UWB2p1;
  real32_T UWB3p0;
  real32_T UWB3p1;
  real32_T UWB4p0;
  real32_T UWB4p1;
} Params;

#endif                                 /*typedef_Params*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void TargetEstimationNominalBatch(real32_T rangeArray[4], Params *params,
  real32_T pos_init[3], real32_T x_out[6]);
extern void TargetEstimationNominalBatch_api(const mxArray * const prhs[3],
  const mxArray *plhs[1]);
extern void TargetEstimationNominalBatch_atexit(void);
extern void TargetEstimationNominalBatch_initialize(void);
extern void TargetEstimationNominalBatch_terminate(void);
extern void TargetEstimationNominalBatch_xil_terminate(void);

#endif

/*
 * File trailer for _coder_TargetEstimationNominalBatch_api.h
 *
 * [EOF]
 */
