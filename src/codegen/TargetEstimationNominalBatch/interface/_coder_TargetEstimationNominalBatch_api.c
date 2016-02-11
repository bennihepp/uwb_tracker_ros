/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TargetEstimationNominalBatch_api.c
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 11-Feb-2016 15:19:32
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
  emlrtMsgIdentifier *parentId, real32_T y_data[], int32_T y_size[2]);
static const mxArray *b_emlrt_marshallOut(const real32_T u_data[], const int32_T
  u_size[2]);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, TargetEstimationParams *y);
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, TargetEstimationParams *y);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[3]);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *rangeArray,
  const char_T *identifier, real32_T y_data[], int32_T y_size[2]);
static const mxArray *emlrt_marshallOut(const real32_T u[6]);
static real32_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_init,
  const char_T *identifier, real32_T y[3]);
static uint8_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *num_of_units, const char_T *identifier);
static uint8_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static uint64_T j_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *timestamp_master_request_1_recv, const char_T *identifier);
static uint64_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *c_timestamp_master_request_1_ar, const char_T *identifier, uint64_T **y_data,
  int32_T y_size[2]);
static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, uint64_T **y_data, int32_T y_size[2]);
static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret_data[], int32_T ret_size[2]);
static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[3]);
static real32_T p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);
static uint8_T q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);
static uint64_T r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);
static void s_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, uint64_T **ret_data, int32_T ret_size[2]);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real32_T y_data[]
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y_data[], int32_T y_size[2])
{
  n_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y_data, y_size);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const real32_T u_data[]
 *                const int32_T u_size[2]
 * Return Type  : const mxArray *
 */
static const mxArray *b_emlrt_marshallOut(const real32_T u_data[], const int32_T
  u_size[2])
{
  const mxArray *y;
  const mxArray *m1;
  real32_T *pData;
  int32_T i0;
  int32_T i;
  y = NULL;
  m1 = emlrtCreateNumericArray(2, u_size, mxSINGLE_CLASS, mxREAL);
  pData = (real32_T *)mxGetData(m1);
  i0 = 0;
  for (i = 0; i < u_size[1]; i++) {
    pData[i0] = u_data[u_size[0] * i];
    i0++;
  }

  emlrtAssign(&y, m1);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *params
 *                const char_T *identifier
 *                TargetEstimationParams *y
 * Return Type  : void
 */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, TargetEstimationParams *y)
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
 *                TargetEstimationParams *y
 * Return Type  : void
 */
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, TargetEstimationParams *y)
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
  o_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *rangeArray
 *                const char_T *identifier
 *                real32_T y_data[]
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *rangeArray,
  const char_T *identifier, real32_T y_data[], int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(rangeArray), &thisId, y_data, y_size);
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
  y = p_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
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
 *                const mxArray *num_of_units
 *                const char_T *identifier
 * Return Type  : uint8_T
 */
static uint8_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *num_of_units, const char_T *identifier)
{
  uint8_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = i_emlrt_marshallIn(sp, emlrtAlias(num_of_units), &thisId);
  emlrtDestroyArray(&num_of_units);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint8_T
 */
static uint8_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  uint8_T y;
  y = q_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *timestamp_master_request_1_recv
 *                const char_T *identifier
 * Return Type  : uint64_T
 */
static uint64_T j_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *timestamp_master_request_1_recv, const char_T *identifier)
{
  uint64_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = k_emlrt_marshallIn(sp, emlrtAlias(timestamp_master_request_1_recv),
    &thisId);
  emlrtDestroyArray(&timestamp_master_request_1_recv);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint64_T
 */
static uint64_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  uint64_T y;
  y = r_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *c_timestamp_master_request_1_ar
 *                const char_T *identifier
 *                uint64_T **y_data
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *c_timestamp_master_request_1_ar, const char_T *identifier, uint64_T **y_data,
  int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  m_emlrt_marshallIn(sp, emlrtAlias(c_timestamp_master_request_1_ar), &thisId,
                     y_data, y_size);
  emlrtDestroyArray(&c_timestamp_master_request_1_ar);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                uint64_T **y_data
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, uint64_T **y_data, int32_T y_size[2])
{
  s_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y_data, y_size);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret_data[]
 *                int32_T ret_size[2]
 * Return Type  : void
 */
static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret_data[], int32_T ret_size[2])
{
  int32_T iv1[2];
  boolean_T bv0[2] = { false, true };

  static const int32_T dims[2] = { 1, 255 };

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "single|double", false, 2U, dims,
    &bv0[0], iv1);
  ret_size[0] = iv1[0];
  ret_size[1] = iv1[1];
  emlrtImportArrayR2015b(sp, src, (void *)ret_data, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret[3]
 * Return Type  : void
 */
static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
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
static real32_T p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : uint8_T
 */
static uint8_T q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  uint8_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "uint8", false, 0U, &dims);
  ret = *(uint8_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : uint64_T
 */
static uint64_T r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  uint64_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "uint64", false, 0U, &dims);
  ret = *(uint64_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                uint64_T **ret_data
 *                int32_T ret_size[2]
 * Return Type  : void
 */
static void s_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, uint64_T **ret_data, int32_T ret_size[2])
{
  int32_T iv2[2];
  boolean_T bv1[2] = { false, true };

  static const int32_T dims[2] = { 1, 255 };

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "uint64", false, 2U, dims, &bv1[0],
    iv2);
  ret_size[0] = iv2[0];
  ret_size[1] = iv2[1];
  *ret_data = (uint64_T *)mxGetData(src);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const mxArray * const prhs[3]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void TargetEstimationNominalBatch_api(const mxArray * const prhs[3], const
  mxArray *plhs[1])
{
  int32_T rangeArray_size[2];
  real32_T rangeArray_data[255];
  TargetEstimationParams params;
  real32_T pos_init[3];
  real32_T x_out[6];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAliasP((const mxArray *)prhs[0]), "rangeArray",
                   rangeArray_data, rangeArray_size);
  c_emlrt_marshallIn(&st, emlrtAliasP((const mxArray *)prhs[1]), "params",
                     &params);
  g_emlrt_marshallIn(&st, emlrtAliasP((const mxArray *)prhs[2]), "pos_init",
                     pos_init);

  /* Invoke the target function */
  TargetEstimationNominalBatch(rangeArray_data, rangeArray_size, &params,
    pos_init, x_out);

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
 * Arguments    : const mxArray *prhs[7]
 *                const mxArray *plhs[2]
 * Return Type  : void
 */
void UWBMultiRangeEstimation_api(const mxArray *prhs[7], const mxArray *plhs[2])
{
  uint8_T num_of_units;
  uint64_T timestamp_master_request_1_recv;
  uint64_T timestamp_slave_reply_send;
  uint64_T timestamp_master_request_2_recv;
  int32_T c_timestamp_master_request_1_ar[2];
  uint64_T (*d_timestamp_master_request_1_ar)[255];
  int32_T c_timestamp_slave_reply_array_s[2];
  uint64_T (*c_timestamp_slave_reply_array_d)[255];
  int32_T c_timestamp_master_request_2_ar[2];
  uint64_T (*d_timestamp_master_request_2_ar)[255];
  int32_T ranges_size[2];
  real32_T ranges_data[255];
  int32_T tofs_size[2];
  real32_T tofs_data[255];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  prhs[4] = emlrtProtectR2012b(prhs[4], 4, false, 255);
  prhs[5] = emlrtProtectR2012b(prhs[5], 5, false, 255);
  prhs[6] = emlrtProtectR2012b(prhs[6], 6, false, 255);

  /* Marshall function inputs */
  num_of_units = h_emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "num_of_units");
  timestamp_master_request_1_recv = j_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]),
    "timestamp_master_request_1_recv");
  timestamp_slave_reply_send = j_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]),
    "timestamp_slave_reply_send");
  timestamp_master_request_2_recv = j_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]),
    "timestamp_master_request_2_recv");
  l_emlrt_marshallIn(&st, emlrtAlias(prhs[4]),
                     "timestamp_master_request_1_array", (uint64_T **)
                     &d_timestamp_master_request_1_ar,
                     c_timestamp_master_request_1_ar);
  l_emlrt_marshallIn(&st, emlrtAlias(prhs[5]), "timestamp_slave_reply_array",
                     (uint64_T **)&c_timestamp_slave_reply_array_d,
                     c_timestamp_slave_reply_array_s);
  l_emlrt_marshallIn(&st, emlrtAlias(prhs[6]),
                     "timestamp_master_request_2_array", (uint64_T **)
                     &d_timestamp_master_request_2_ar,
                     c_timestamp_master_request_2_ar);

  /* Invoke the target function */
  UWBMultiRangeEstimation(num_of_units, timestamp_master_request_1_recv,
    timestamp_slave_reply_send, timestamp_master_request_2_recv,
    *d_timestamp_master_request_1_ar, c_timestamp_master_request_1_ar,
    *c_timestamp_slave_reply_array_d, c_timestamp_slave_reply_array_s,
    *d_timestamp_master_request_2_ar, c_timestamp_master_request_2_ar, tofs_data,
    tofs_size, ranges_data, ranges_size);

  /* Marshall function outputs */
  plhs[0] = b_emlrt_marshallOut(tofs_data, tofs_size);
  plhs[1] = b_emlrt_marshallOut(ranges_data, ranges_size);
}

/*
 * File trailer for _coder_TargetEstimationNominalBatch_api.c
 *
 * [EOF]
 */
