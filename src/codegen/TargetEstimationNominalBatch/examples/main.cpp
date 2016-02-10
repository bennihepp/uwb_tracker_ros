//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 08-Feb-2016 14:02:33
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "TargetEstimationNominalBatch.h"
#include "main.h"

// Function Declarations
static void argInit_1x4_real32_T(float result[4]);
static void argInit_3x1_real32_T(float result[3]);
static void argInit_Params(Params *result);
static float argInit_real32_T();
static void c_main_TargetEstimationNominalB();

// Function Definitions

//
// Arguments    : float result[4]
// Return Type  : void
//
static void argInit_1x4_real32_T(float result[4])
{
  int idx1;

  // Loop over the array to initialize each element.
  for (idx1 = 0; idx1 < 4; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_real32_T();
  }
}

//
// Arguments    : float result[3]
// Return Type  : void
//
static void argInit_3x1_real32_T(float result[3])
{
  int idx0;

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < 3; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real32_T();
  }
}

//
// Arguments    : Params *result
// Return Type  : void
//
static void argInit_Params(Params *result)
{
  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  argInit_3x1_real32_T(result->tracker_mean);
  argInit_3x1_real32_T(result->m_uwb1);
  argInit_3x1_real32_T(result->m_uwb2);
  argInit_3x1_real32_T(result->m_uwb3);
  argInit_3x1_real32_T(result->m_uwb4);
  result->UWB1p0 = argInit_real32_T();
  result->UWB1p1 = argInit_real32_T();
  result->UWB2p0 = argInit_real32_T();
  result->UWB2p1 = argInit_real32_T();
  result->UWB3p0 = argInit_real32_T();
  result->UWB3p1 = argInit_real32_T();
  result->UWB4p0 = argInit_real32_T();
  result->UWB4p1 = argInit_real32_T();
}

//
// Arguments    : void
// Return Type  : float
//
static float argInit_real32_T()
{
  return 0.0F;
}

//
// Arguments    : void
// Return Type  : void
//
static void c_main_TargetEstimationNominalB()
{
  float fv2[4];
  Params r0;
  float fv3[3];
  float x_out[6];

  // Initialize function 'TargetEstimationNominalBatch' input arguments.
  // Initialize function input argument 'rangeArray'.
  // Initialize function input argument 'params'.
  // Initialize function input argument 'pos_init'.
  // Call the entry-point 'TargetEstimationNominalBatch'.
  argInit_1x4_real32_T(fv2);
  argInit_Params(&r0);
  argInit_3x1_real32_T(fv3);
  TargetEstimationNominalBatch(fv2, &r0, fv3, x_out);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  TargetEstimationNominalBatch_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  c_main_TargetEstimationNominalB();

  // Terminate the application.
  // You do not need to do this more than one time.
  TargetEstimationNominalBatch_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
