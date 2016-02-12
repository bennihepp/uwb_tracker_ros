/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_MultiRangeEstimation_api.c
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 12-Feb-2016 14:20:28
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_MultiRangeEstimation_api.h"
#include "_coder_MultiRangeEstimation_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true, false, 131419U, NULL,
  "MultiRangeEstimation", NULL, false, { 2045744189U, 2170104910U, 2743257031U,
    4284093946U }, NULL };

/* Function Declarations */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *b_emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2]);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *timestamp_master_request_1_array, const char_T *identifier, real_T **y_data,
  int32_T y_size[2]);
static const mxArray *c_emlrt_marshallOut(const real_T u[6]);
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T **y_data, int32_T y_size[2]);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, MultiRangeEstimationParams *y);
static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *timestamp_master_request_1_recv, const char_T *identifier);
static const mxArray *emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2]);
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, MultiRangeEstimationParams *y);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T y_data[], int32_T y_size[2]);
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, TargetEstimationParams *y);
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, TargetEstimationParams *y);
static void j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T y[3]);
static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_init,
  const char_T *identifier))[3];
static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[3];
static real_T m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);
static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T **ret_data, int32_T ret_size[2]);
static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T ret_data[], int32_T ret_size[2]);
static void p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T ret[3]);
static real_T (*q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[3];

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = m_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const real_T u_data[]
 *                const int32_T u_size[2]
 * Return Type  : const mxArray *
 */
static const mxArray *b_emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2])
{
  const mxArray *y;
  static const int32_T iv1[2] = { 0, 0 };

  const mxArray *m1;
  y = NULL;
  m1 = emlrtCreateNumericArray(2, iv1, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m1, (void *)u_data);
  emlrtSetDimensions((mxArray *)m1, *(int32_T (*)[2])&u_size[0], 2);
  emlrtAssign(&y, m1);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *timestamp_master_request_1_array
 *                const char_T *identifier
 *                real_T **y_data
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *timestamp_master_request_1_array, const char_T *identifier, real_T **y_data,
  int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  d_emlrt_marshallIn(sp, emlrtAlias(timestamp_master_request_1_array), &thisId,
                     y_data, y_size);
  emlrtDestroyArray(&timestamp_master_request_1_array);
}

/*
 * Arguments    : const real_T u[6]
 * Return Type  : const mxArray *
 */
static const mxArray *c_emlrt_marshallOut(const real_T u[6])
{
  const mxArray *y;
  static const int32_T iv2[1] = { 0 };

  const mxArray *m2;
  static const int32_T iv3[1] = { 6 };

  y = NULL;
  m2 = emlrtCreateNumericArray(1, iv2, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m2, (void *)u);
  emlrtSetDimensions((mxArray *)m2, *(int32_T (*)[1])&iv3[0], 1);
  emlrtAssign(&y, m2);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real_T **y_data
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T **y_data, int32_T y_size[2])
{
  n_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y_data, y_size);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *params
 *                const char_T *identifier
 *                MultiRangeEstimationParams *y
 * Return Type  : void
 */
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, MultiRangeEstimationParams *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(params), &thisId, y);
  emlrtDestroyArray(&params);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *timestamp_master_request_1_recv
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *timestamp_master_request_1_recv, const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(timestamp_master_request_1_recv),
    &thisId);
  emlrtDestroyArray(&timestamp_master_request_1_recv);
  return y;
}

/*
 * Arguments    : const real_T u_data[]
 *                const int32_T u_size[2]
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const real_T u_data[], const int32_T
  u_size[2])
{
  const mxArray *y;
  static const int32_T iv0[2] = { 0, 0 };

  const mxArray *m0;
  y = NULL;
  m0 = emlrtCreateNumericArray(2, iv0, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m0, (void *)u_data);
  emlrtSetDimensions((mxArray *)m0, *(int32_T (*)[2])&u_size[0], 2);
  emlrtAssign(&y, m0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                MultiRangeEstimationParams *y
 * Return Type  : void
 */
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, MultiRangeEstimationParams *y)
{
  emlrtMsgIdentifier thisId;
  static const int32_T dims = 0;
  static const char * fieldNames[5] = { "US_TO_DW_TIMEUNITS",
    "DW_TIMEUNITS_TO_US", "SPEED_OF_LIGHT_IN_M_PER_US",
    "SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT", "unit_distances" };

  thisId.fParent = parentId;
  thisId.bParentIsCell = false;
  emlrtCheckStructR2012b(sp, parentId, u, 5, fieldNames, 0U, &dims);
  thisId.fIdentifier = "US_TO_DW_TIMEUNITS";
  y->US_TO_DW_TIMEUNITS = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a
    (sp, u, 0, "US_TO_DW_TIMEUNITS")), &thisId);
  thisId.fIdentifier = "DW_TIMEUNITS_TO_US";
  y->DW_TIMEUNITS_TO_US = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a
    (sp, u, 0, "DW_TIMEUNITS_TO_US")), &thisId);
  thisId.fIdentifier = "SPEED_OF_LIGHT_IN_M_PER_US";
  y->SPEED_OF_LIGHT_IN_M_PER_US = b_emlrt_marshallIn(sp, emlrtAlias
    (emlrtGetFieldR2013a(sp, u, 0, "SPEED_OF_LIGHT_IN_M_PER_US")), &thisId);
  thisId.fIdentifier = "SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT";
  y->SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT = b_emlrt_marshallIn(sp, emlrtAlias
    (emlrtGetFieldR2013a(sp, u, 0, "SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT")),
    &thisId);
  thisId.fIdentifier = "unit_distances";
  g_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "unit_distances")), &thisId, y->unit_distances.data, y->unit_distances.size);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real_T y_data[]
 *                int32_T y_size[2]
 * Return Type  : void
 */
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T y_data[], int32_T y_size[2])
{
  o_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y_data, y_size);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *params
 *                const char_T *identifier
 *                TargetEstimationParams *y
 * Return Type  : void
 */
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *params,
  const char_T *identifier, TargetEstimationParams *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  i_emlrt_marshallIn(sp, emlrtAlias(params), &thisId, y);
  emlrtDestroyArray(&params);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                TargetEstimationParams *y
 * Return Type  : void
 */
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
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
  j_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "tracker_mean")),
                     &thisId, y->tracker_mean);
  thisId.fIdentifier = "m_uwb1";
  j_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb1")),
                     &thisId, y->m_uwb1);
  thisId.fIdentifier = "m_uwb2";
  j_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb2")),
                     &thisId, y->m_uwb2);
  thisId.fIdentifier = "m_uwb3";
  j_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb3")),
                     &thisId, y->m_uwb3);
  thisId.fIdentifier = "m_uwb4";
  j_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "m_uwb4")),
                     &thisId, y->m_uwb4);
  thisId.fIdentifier = "UWB1p0";
  y->UWB1p0 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB1p0")), &thisId);
  thisId.fIdentifier = "UWB1p1";
  y->UWB1p1 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB1p1")), &thisId);
  thisId.fIdentifier = "UWB2p0";
  y->UWB2p0 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB2p0")), &thisId);
  thisId.fIdentifier = "UWB2p1";
  y->UWB2p1 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB2p1")), &thisId);
  thisId.fIdentifier = "UWB3p0";
  y->UWB3p0 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB3p0")), &thisId);
  thisId.fIdentifier = "UWB3p1";
  y->UWB3p1 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB3p1")), &thisId);
  thisId.fIdentifier = "UWB4p0";
  y->UWB4p0 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB4p0")), &thisId);
  thisId.fIdentifier = "UWB4p1";
  y->UWB4p1 = b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "UWB4p1")), &thisId);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real_T y[3]
 * Return Type  : void
 */
static void j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real_T y[3])
{
  p_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *pos_init
 *                const char_T *identifier
 * Return Type  : real_T (*)[3]
 */
static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *pos_init,
  const char_T *identifier))[3]
{
  real_T (*y)[3];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = l_emlrt_marshallIn(sp, emlrtAlias(pos_init), &thisId);
  emlrtDestroyArray(&pos_init);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[3]
 */
  static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[3]
{
  real_T (*y)[3];
  y = q_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId)
{
  real_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, &dims);
  ret = *(real_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real_T **ret_data
 *                int32_T ret_size[2]
 * Return Type  : void
 */
static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T **ret_data, int32_T ret_size[2])
{
  int32_T iv4[2];
  boolean_T bv0[2] = { false, true };

  static const int32_T dims[2] = { 1, 255 };

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims, &bv0[0],
    iv4);
  ret_size[0] = iv4[0];
  ret_size[1] = iv4[1];
  *ret_data = (real_T *)mxGetData(src);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real_T ret_data[]
 *                int32_T ret_size[2]
 * Return Type  : void
 */
static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T ret_data[], int32_T ret_size[2])
{
  int32_T iv5[2];
  boolean_T bv1[2] = { false, true };

  static const int32_T dims[2] = { 1, 255 };

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims, &bv1[0],
    iv5);
  ret_size[0] = iv5[0];
  ret_size[1] = iv5[1];
  emlrtImportArrayR2015b(sp, src, (void *)ret_data, 8, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real_T ret[3]
 * Return Type  : void
 */
static void p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real_T ret[3])
{
  static const int32_T dims[1] = { 3 };

  int32_T i0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 1U, dims);
  for (i0 = 0; i0 < 3; i0++) {
    ret[i0] = (*(real_T (*)[3])mxGetData(src))[i0];
  }

  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[3]
 */
static real_T (*q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[3]
{
  real_T (*ret)[3];
  static const int32_T dims[1] = { 3 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 1U, dims);
  ret = (real_T (*)[3])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * Arguments    : const mxArray *prhs[7]
 *                const mxArray *plhs[4]
 * Return Type  : void
 */
  void MultiRangeEstimation_api(const mxArray *prhs[7], const mxArray *plhs[4])
{
  real_T (*tofs_data)[255];
  real_T (*ranges_data)[255];
  real_T (*clock_offsets_data)[256];
  real_T (*clock_skews_data)[256];
  real_T timestamp_master_request_1_recv;
  real_T timestamp_slave_reply_send;
  real_T timestamp_master_request_2_recv;
  int32_T timestamp_master_request_1_array_size[2];
  real_T (*timestamp_master_request_1_array_data)[255];
  int32_T timestamp_slave_reply_array_size[2];
  real_T (*timestamp_slave_reply_array_data)[255];
  int32_T timestamp_master_request_2_array_size[2];
  real_T (*timestamp_master_request_2_array_data)[255];
  MultiRangeEstimationParams params;
  int32_T clock_skews_size[2];
  int32_T clock_offsets_size[2];
  int32_T ranges_size[2];
  int32_T tofs_size[2];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  tofs_data = (real_T (*)[255])mxMalloc(sizeof(real_T [255]));
  ranges_data = (real_T (*)[255])mxMalloc(sizeof(real_T [255]));
  clock_offsets_data = (real_T (*)[256])mxMalloc(sizeof(real_T [256]));
  clock_skews_data = (real_T (*)[256])mxMalloc(sizeof(real_T [256]));
  prhs[3] = emlrtProtectR2012b(prhs[3], 3, false, 255);
  prhs[4] = emlrtProtectR2012b(prhs[4], 4, false, 255);
  prhs[5] = emlrtProtectR2012b(prhs[5], 5, false, 255);

  /* Marshall function inputs */
  timestamp_master_request_1_recv = emlrt_marshallIn(&st, emlrtAliasP(prhs[0]),
    "timestamp_master_request_1_recv");
  timestamp_slave_reply_send = emlrt_marshallIn(&st, emlrtAliasP(prhs[1]),
    "timestamp_slave_reply_send");
  timestamp_master_request_2_recv = emlrt_marshallIn(&st, emlrtAliasP(prhs[2]),
    "timestamp_master_request_2_recv");
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[3]),
                     "timestamp_master_request_1_array", (real_T **)
                     &timestamp_master_request_1_array_data,
                     timestamp_master_request_1_array_size);
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[4]), "timestamp_slave_reply_array",
                     (real_T **)&timestamp_slave_reply_array_data,
                     timestamp_slave_reply_array_size);
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[5]),
                     "timestamp_master_request_2_array", (real_T **)
                     &timestamp_master_request_2_array_data,
                     timestamp_master_request_2_array_size);
  e_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "params", &params);

  /* Invoke the target function */
  MultiRangeEstimation(timestamp_master_request_1_recv,
                       timestamp_slave_reply_send,
                       timestamp_master_request_2_recv,
                       *timestamp_master_request_1_array_data,
                       timestamp_master_request_1_array_size,
                       *timestamp_slave_reply_array_data,
                       timestamp_slave_reply_array_size,
                       *timestamp_master_request_2_array_data,
                       timestamp_master_request_2_array_size, &params,
                       *tofs_data, tofs_size, *ranges_data, ranges_size,
                       *clock_offsets_data, clock_offsets_size,
                       *clock_skews_data, clock_skews_size);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(*tofs_data, tofs_size);
  plhs[1] = emlrt_marshallOut(*ranges_data, ranges_size);
  plhs[2] = b_emlrt_marshallOut(*clock_offsets_data, clock_offsets_size);
  plhs[3] = b_emlrt_marshallOut(*clock_skews_data, clock_skews_size);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void MultiRangeEstimation_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  MultiRangeEstimation_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void MultiRangeEstimation_initialize(void)
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
void MultiRangeEstimation_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * Arguments    : const mxArray *prhs[3]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void TargetEstimationNominalBatch_api(const mxArray *prhs[3], const mxArray
  *plhs[1])
{
  real_T (*x_out)[6];
  int32_T rangeArray_size[2];
  real_T (*rangeArray_data)[255];
  TargetEstimationParams params;
  real_T (*pos_init)[3];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  x_out = (real_T (*)[6])mxMalloc(sizeof(real_T [6]));
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, 255);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, false, -1);

  /* Marshall function inputs */
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "rangeArray", (real_T **)
                     &rangeArray_data, rangeArray_size);
  h_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "params", &params);
  pos_init = k_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "pos_init");

  /* Invoke the target function */
  TargetEstimationNominalBatch(*rangeArray_data, rangeArray_size, &params,
    *pos_init, *x_out);

  /* Marshall function outputs */
  plhs[0] = c_emlrt_marshallOut(*x_out);
}

/*
 * File trailer for _coder_MultiRangeEstimation_api.c
 *
 * [EOF]
 */
