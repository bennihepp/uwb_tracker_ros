/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TargetEstimationNominalBatch_api.c
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 08-Feb-2016 14:02:33
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_TargetEstimationNominalBatch_api.h"
#include "_coder_TargetEstimationNominalBatch_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true, false, 131419U, NULL,
  "TargetEstimationNominalBatch", NULL, false, { 2045744189U, 2170104910U,
    2743257031U, 4284093946U }, NULL };

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[4]);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, Params *y);
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, Params *y);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[3]);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *rangeArray,
  const char_T *identifier, real32_T y[4]);
static const mxArray *emlrt_marshallOut(const real32_T u[6]);
static real32_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_init,
  const char_T *identifier, real32_T y[3]);
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[4]);
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[3]);
static real32_T j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real32_T y[4]
 * Return Type  : void
 */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[4])
{
  h_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *params
 *                const char_T *identifier
 *                Params *y
 * Return Type  : void
 */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, Params *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  d_emlrt_marshallIn(sp, emlrtAlias(params), &thisId, y);
  emlrtDestroyArray(&params);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                Params *y
 * Return Type  : void
 */
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, Params *y)
{
  emlrtMsgIdentifier thisId;
  static const int32_T dims = 0;
  static const char * fieldNames[13] = { "tracker_mean", "m_uwb1", "m_uwb2",
    "m_uwb3", "m_uwb4", "UWB1p0", "UWB1p1", "UWB2p0", "UWB2p1", "UWB3p0",
    "UWB3p1", "UWB4p0", "UWB4p1" };

  thisId.fParent = parentId;
  thisId.bParentIsCell = false;
  emlrtCheckStructR2012b(sp, parentId, u, 13, fieldNames, 0U, &dims);
  thisId.fIdentifier = "tracker_mean";
  e_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "tracker_mean")),
                     &thisId, y->tracker_mean);
  thisId.fIdentifier = "m_uwb1";
  e_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb1")),
                     &thisId, y->m_uwb1);
  thisId.fIdentifier = "m_uwb2";
  e_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb2")),
                     &thisId, y->m_uwb2);
  thisId.fIdentifier = "m_uwb3";
  e_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb3")),
                     &thisId, y->m_uwb3);
  thisId.fIdentifier = "m_uwb4";
  e_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb4")),
                     &thisId, y->m_uwb4);
  thisId.fIdentifier = "UWB1p0";
  y->UWB1p0 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB1p0")), &thisId);
  thisId.fIdentifier = "UWB1p1";
  y->UWB1p1 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB1p1")), &thisId);
  thisId.fIdentifier = "UWB2p0";
  y->UWB2p0 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB2p0")), &thisId);
  thisId.fIdentifier = "UWB2p1";
  y->UWB2p1 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB2p1")), &thisId);
  thisId.fIdentifier = "UWB3p0";
  y->UWB3p0 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB3p0")), &thisId);
  thisId.fIdentifier = "UWB3p1";
  y->UWB3p1 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB3p1")), &thisId);
  thisId.fIdentifier = "UWB4p0";
  y->UWB4p0 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB4p0")), &thisId);
  thisId.fIdentifier = "UWB4p1";
  y->UWB4p1 = f_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB4p1")), &thisId);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real32_T y[3]
 * Return Type  : void
 */
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[3])
{
  i_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *rangeArray
 *                const char_T *identifier
 *                real32_T y[4]
 * Return Type  : void
 */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *rangeArray,
  const char_T *identifier, real32_T y[4])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(rangeArray), &thisId, y);
  emlrtDestroyArray(&rangeArray);
}

/*
 * Arguments    : const real32_T u[6]
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const real32_T u[6])
{
  const mxArray *y;
  static const int32_T iv0[1] = { 6 };

  const mxArray *m0;
  real32_T *pData;
  int32_T i;
  y = NULL;
  m0 = emlrtCreateNumericArray(1, iv0, mxSINGLE_CLASS, mxREAL);
  pData = (real32_T *)mxGetData(m0);
  for (i = 0; i < 6; i++) {
    pData[i] = u[i];
  }

  emlrtAssign(&y, m0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real32_T
 */
static real32_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real32_T y;
  y = j_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *pos_init
 *                const char_T *identifier
 *                real32_T y[3]
 * Return Type  : void
 */
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_init,
  const char_T *identifier, real32_T y[3])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  e_emlrt_marshallIn(sp, emlrtAlias(pos_init), &thisId, y);
  emlrtDestroyArray(&pos_init);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret[4]
 * Return Type  : void
 */
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[4])
{
  static const int32_T dims[2] = { 1, 4 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 2U, dims);
  emlrtImportArrayR2015b(sp, src, ret, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret[3]
 * Return Type  : void
 */
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[3])
{
  static const int32_T dims[1] = { 3 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 1U, dims);
  emlrtImportArrayR2015b(sp, src, ret, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real32_T
 */
static real32_T j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  real32_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 0U, &dims);
  emlrtImportArrayR2015b(sp, src, &ret, 4, false);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray * const prhs[3]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void TargetEstimationNominalBatch_api(const mxArray * const prhs[3], const
  mxArray *plhs[1])
{
  real32_T rangeArray[4];
  Params params;
  real32_T pos_init[3];
  real32_T x_out[6];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAliasP((const mxArray *)prhs[0]), "rangeArray",
                   rangeArray);
  c_emlrt_marshallIn(&st, emlrtAliasP((const mxArray *)prhs[1]), "params",
                     &params);
  g_emlrt_marshallIn(&st, emlrtAliasP((const mxArray *)prhs[2]), "pos_init",
                     pos_init);

  /* Invoke the target function */
  TargetEstimationNominalBatch(rangeArray, &params, pos_init, x_out);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(x_out);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void TargetEstimationNominalBatch_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  TargetEstimationNominalBatch_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void TargetEstimationNominalBatch_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void TargetEstimationNominalBatch_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_TargetEstimationNominalBatch_api.c
 *
 * [EOF]
 */
