//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: MultiRangeEstimation_types.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 12-Feb-2016 14:20:28
//
#ifndef __MULTIRANGEESTIMATION_TYPES_H__
#define __MULTIRANGEESTIMATION_TYPES_H__

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray_real_T_1x255
#define struct_emxArray_real_T_1x255

struct emxArray_real_T_1x255
{
  double data[255];
  int size[2];
};

#endif                                 //struct_emxArray_real_T_1x255

typedef struct {
  double US_TO_DW_TIMEUNITS;
  double DW_TIMEUNITS_TO_US;
  double SPEED_OF_LIGHT_IN_M_PER_US;
  double SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT;
  emxArray_real_T_1x255 unit_distances;
} MultiRangeEstimationParams;

typedef struct {
  double tracker_mean[3];
  double m_uwb1[3];
  double m_uwb2[3];
  double m_uwb3[3];
  double m_uwb4[3];
  double UWB1p0;
  double UWB1p1;
  double UWB2p0;
  double UWB2p1;
  double UWB3p0;
  double UWB3p1;
  double UWB4p0;
  double UWB4p1;
} TargetEstimationParams;

#endif

//
// File trailer for MultiRangeEstimation_types.h
//
// [EOF]
//
