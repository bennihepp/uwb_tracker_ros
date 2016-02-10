//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TargetEstimationNominalBatch.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 08-Feb-2016 14:02:33
//
#ifndef __TARGETESTIMATIONNOMINALBATCH_H__
#define __TARGETESTIMATIONNOMINALBATCH_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "TargetEstimationNominalBatch_types.h"

// Function Declarations
extern void TargetEstimationNominalBatch(const float rangeArray[4], const Params
  *params, const float pos_init[3], float x_out[6]);
extern void TargetEstimationNominalBatch_initialize();
extern void TargetEstimationNominalBatch_terminate();

#endif

//
// File trailer for TargetEstimationNominalBatch.h
//
// [EOF]
//
