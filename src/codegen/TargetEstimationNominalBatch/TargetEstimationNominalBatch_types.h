//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TargetEstimationNominalBatch_types.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 11-Feb-2016 15:19:32
//
#ifndef __TARGETESTIMATIONNOMINALBATCH_TYPES_H__
#define __TARGETESTIMATIONNOMINALBATCH_TYPES_H__

// Include Files
#include "rtwtypes.h"

// Type Definitions
typedef struct {
  float tracker_mean[3];
  float m_uwb1[3];
  float m_uwb2[3];
  float m_uwb3[3];
  float m_uwb4[3];
  float UWB1p0;
  float UWB1p1;
  float UWB2p0;
  float UWB2p1;
  float UWB3p0;
  float UWB3p1;
  float UWB4p0;
  float UWB4p1;
} TargetEstimationParams;

#endif

//
// File trailer for TargetEstimationNominalBatch_types.h
//
// [EOF]
//
