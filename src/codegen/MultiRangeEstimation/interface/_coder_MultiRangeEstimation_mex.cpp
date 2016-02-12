/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_MultiRangeEstimation_mex.cpp
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 12-Feb-2016 14:20:28
 */

/* Include Files */
#include "_coder_MultiRangeEstimation_api.h"
#include "_coder_MultiRangeEstimation_mex.h"

/* Custom Source Code */
#include <stdio.h>

/* Variable Definitions */
static const char * emlrtEntryPoints[2] = { "MultiRangeEstimation",
  "TargetEstimationNominalBatch" };

/* Function Declarations */
static void MultiRangeEstimation_mexFunction(int32_T nlhs, mxArray *plhs[4],
  int32_T nrhs, const mxArray *prhs[7]);
static void TargetEstimationNominalBatch_mexFunction(int32_T nlhs, mxArray *
  plhs[1], int32_T nrhs, const mxArray *prhs[3]);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                const mxArray *plhs[4]
 *                int32_T nrhs
 *                const mxArray *prhs[7]
 * Return Type  : void
 */
static void MultiRangeEstimation_mexFunction(int32_T nlhs, mxArray *plhs[4],
  int32_T nrhs, const mxArray *prhs[7])
{
  int32_T n;
  const mxArray *inputs[7];
  const mxArray *outputs[4];
  int32_T b_nlhs;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 7) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 7, 4,
                        20, "MultiRangeEstimation");
  }

  if (nlhs > 4) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 20,
                        "MultiRangeEstimation");
  }

  /* Temporary copy for mex inputs. */
  for (n = 0; n < nrhs; n++) {
    inputs[n] = prhs[n];
  }

  /* Call the function. */
  MultiRangeEstimation_api(inputs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);

  /* Module termination. */
  MultiRangeEstimation_terminate();
}

/*
 * Arguments    : int32_T nlhs
 *                const mxArray *plhs[1]
 *                int32_T nrhs
 *                const mxArray *prhs[3]
 * Return Type  : void
 */
static void TargetEstimationNominalBatch_mexFunction(int32_T nlhs, mxArray *
  plhs[1], int32_T nrhs, const mxArray *prhs[3])
{
  int32_T n;
  const mxArray *inputs[3];
  const mxArray *outputs[1];
  int32_T b_nlhs;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 3, 4,
                        28, "TargetEstimationNominalBatch");
  }

  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 28,
                        "TargetEstimationNominalBatch");
  }

  /* Temporary copy for mex inputs. */
  for (n = 0; n < nrhs; n++) {
    inputs[n] = prhs[n];
  }

  /* Call the function. */
  TargetEstimationNominalBatch_api(inputs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);

  /* Module termination. */
  MultiRangeEstimation_terminate();
}

/*
 * Arguments    : int32_T nlhs
 *                const mxArray * const plhs[]
 *                int32_T nrhs
 *                const mxArray * const prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  mexAtExit(MultiRangeEstimation_atexit);

  /* Initialize the memory manager. */
  /* Module initialization. */
  MultiRangeEstimation_initialize();

  /* Dispatch the entry-point. */
  switch (emlrtGetEntryPointIndex(nrhs, prhs, emlrtEntryPoints, 2)) {
   case 0:
    MultiRangeEstimation_mexFunction(nlhs, plhs, nrhs - 1, &prhs[1]);
    break;

   case 1:
    TargetEstimationNominalBatch_mexFunction(nlhs, plhs, nrhs - 1, &prhs[1]);
    break;
  }
}

/*
 * Arguments    : void
 * Return Type  : emlrtCTX
 */
emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/*
 * File trailer for _coder_MultiRangeEstimation_mex.cpp
 *
 * [EOF]
 */
