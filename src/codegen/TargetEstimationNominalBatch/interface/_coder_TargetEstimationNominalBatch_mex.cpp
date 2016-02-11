/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TargetEstimationNominalBatch_mex.cpp
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 11-Feb-2016 15:19:32
 */

/* Include Files */
#include "_coder_TargetEstimationNominalBatch_api.h"
#include "_coder_TargetEstimationNominalBatch_mex.h"

/* Variable Definitions */
static const char * emlrtEntryPoints[2] = { "TargetEstimationNominalBatch",
  "UWBMultiRangeEstimation" };

/* Function Declarations */
static void c_TargetEstimationNominalBatch_(int32_T nlhs, mxArray *plhs[1],
  int32_T nrhs, const mxArray *prhs[3]);
static void c_UWBMultiRangeEstimation_mexFu(int32_T nlhs, mxArray *plhs[2],
  int32_T nrhs, const mxArray *prhs[7]);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                const mxArray *plhs[1]
 *                int32_T nrhs
 *                const mxArray *prhs[3]
 * Return Type  : void
 */
static void c_TargetEstimationNominalBatch_(int32_T nlhs, mxArray *plhs[1],
  int32_T nrhs, const mxArray *prhs[3])
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
  TargetEstimationNominalBatch_terminate();
}

/*
 * Arguments    : int32_T nlhs
 *                const mxArray *plhs[2]
 *                int32_T nrhs
 *                const mxArray *prhs[7]
 * Return Type  : void
 */
static void c_UWBMultiRangeEstimation_mexFu(int32_T nlhs, mxArray *plhs[2],
  int32_T nrhs, const mxArray *prhs[7])
{
  int32_T n;
  const mxArray *inputs[7];
  const mxArray *outputs[2];
  int32_T b_nlhs;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 7) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 7, 4,
                        23, "UWBMultiRangeEstimation");
  }

  if (nlhs > 2) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 23,
                        "UWBMultiRangeEstimation");
  }

  /* Temporary copy for mex inputs. */
  for (n = 0; n < nrhs; n++) {
    inputs[n] = prhs[n];
  }

  /* Call the function. */
  UWBMultiRangeEstimation_api(inputs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);

  /* Module termination. */
  TargetEstimationNominalBatch_terminate();
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
  mexAtExit(TargetEstimationNominalBatch_atexit);

  /* Initialize the memory manager. */
  /* Module initialization. */
  TargetEstimationNominalBatch_initialize();

  /* Dispatch the entry-point. */
  switch (emlrtGetEntryPointIndex(nrhs, prhs, emlrtEntryPoints, 2)) {
   case 0:
    c_TargetEstimationNominalBatch_(nlhs, plhs, nrhs - 1, &prhs[1]);
    break;

   case 1:
    c_UWBMultiRangeEstimation_mexFu(nlhs, plhs, nrhs - 1, &prhs[1]);
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
 * File trailer for _coder_TargetEstimationNominalBatch_mex.cpp
 *
 * [EOF]
 */
