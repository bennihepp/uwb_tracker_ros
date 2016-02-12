//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 12-Feb-2016 14:20:28
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
#include "MultiRangeEstimation.h"
#include "main.h"

// Custom Source Code
#include <stdio.h>

// Function Declarations
static void argInit_1xd255_real_T(double result_data[], int result_size[2]);
static void argInit_3x1_real_T(double result[3]);
static void argInit_MultiRangeEstimationParams(MultiRangeEstimationParams
  *result);
static void argInit_TargetEstimationParams(TargetEstimationParams *result);
static double argInit_real_T();
static void main_MultiRangeEstimation();
static void main_TargetEstimationNominalBatch();

// Function Definitions

//
// Arguments    : double result_data[]
//                int result_size[2]
// Return Type  : void
//
static void argInit_1xd255_real_T(double result_data[], int result_size[2])
{
  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result_size[0] = 1;
  result_size[1] = 2;

  // Loop over the array to initialize each element.
  for (idx1 = 0; idx1 < 2; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result_data[idx1] = argInit_real_T();
  }
}

//
// Arguments    : double result[3]
// Return Type  : void
//
static void argInit_3x1_real_T(double result[3])
{
  int idx0;

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < 3; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
}

//
// Arguments    : MultiRangeEstimationParams *result
// Return Type  : void
//
static void argInit_MultiRangeEstimationParams(MultiRangeEstimationParams
  *result)
{
  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  result->US_TO_DW_TIMEUNITS = argInit_real_T();
  result->DW_TIMEUNITS_TO_US = argInit_real_T();
  result->SPEED_OF_LIGHT_IN_M_PER_US = argInit_real_T();
  result->SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT = argInit_real_T();
  argInit_1xd255_real_T(result->unit_distances.data, result->unit_distances.size);
}

//
// Arguments    : TargetEstimationParams *result
// Return Type  : void
//
static void argInit_TargetEstimationParams(TargetEstimationParams *result)
{
  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  argInit_3x1_real_T(result->tracker_mean);
  argInit_3x1_real_T(result->m_uwb1);
  argInit_3x1_real_T(result->m_uwb2);
  argInit_3x1_real_T(result->m_uwb3);
  argInit_3x1_real_T(result->m_uwb4);
  result->UWB1p0 = argInit_real_T();
  result->UWB1p1 = argInit_real_T();
  result->UWB2p0 = argInit_real_T();
  result->UWB2p1 = argInit_real_T();
  result->UWB3p0 = argInit_real_T();
  result->UWB3p1 = argInit_real_T();
  result->UWB4p0 = argInit_real_T();
  result->UWB4p1 = argInit_real_T();
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_MultiRangeEstimation()
{
  double timestamp_master_request_1_recv;
  double timestamp_slave_reply_send;
  double timestamp_master_request_2_recv;
  int timestamp_master_request_1_array_size[2];
  double timestamp_master_request_1_array_data[255];
  int timestamp_slave_reply_array_size[2];
  double timestamp_slave_reply_array_data[255];
  int timestamp_master_request_2_array_size[2];
  double timestamp_master_request_2_array_data[255];
  MultiRangeEstimationParams params;
  int clock_skews_size[2];
  double clock_skews_data[256];
  int clock_offsets_size[2];
  double clock_offsets_data[256];
  int ranges_size[2];
  double ranges_data[255];
  int tofs_size[2];
  double tofs_data[255];

  // Initialize function 'MultiRangeEstimation' input arguments.
  timestamp_master_request_1_recv = argInit_real_T();
  timestamp_slave_reply_send = argInit_real_T();
  timestamp_master_request_2_recv = argInit_real_T();

  // Initialize function input argument 'timestamp_master_request_1_array'.
  argInit_1xd255_real_T(timestamp_master_request_1_array_data,
                        timestamp_master_request_1_array_size);

  // Initialize function input argument 'timestamp_slave_reply_array'.
  argInit_1xd255_real_T(timestamp_slave_reply_array_data,
                        timestamp_slave_reply_array_size);

  // Initialize function input argument 'timestamp_master_request_2_array'.
  argInit_1xd255_real_T(timestamp_master_request_2_array_data,
                        timestamp_master_request_2_array_size);

  // Initialize function input argument 'params'.
  argInit_MultiRangeEstimationParams(&params);

  // Call the entry-point 'MultiRangeEstimation'.
  MultiRangeEstimation(timestamp_master_request_1_recv,
                       timestamp_slave_reply_send,
                       timestamp_master_request_2_recv,
                       timestamp_master_request_1_array_data,
                       timestamp_master_request_1_array_size,
                       timestamp_slave_reply_array_data,
                       timestamp_slave_reply_array_size,
                       timestamp_master_request_2_array_data,
                       timestamp_master_request_2_array_size, &params, tofs_data,
                       tofs_size, ranges_data, ranges_size, clock_offsets_data,
                       clock_offsets_size, clock_skews_data, clock_skews_size);
}

//
// Arguments    : void
// Return Type  : void
//
static void main_TargetEstimationNominalBatch()
{
  int rangeArray_size[2];
  double rangeArray_data[255];
  TargetEstimationParams r0;
  double dv1[3];
  double x_out[6];

  // Initialize function 'TargetEstimationNominalBatch' input arguments.
  // Initialize function input argument 'rangeArray'.
  argInit_1xd255_real_T(rangeArray_data, rangeArray_size);

  // Initialize function input argument 'params'.
  // Initialize function input argument 'pos_init'.
  // Call the entry-point 'TargetEstimationNominalBatch'.
  argInit_TargetEstimationParams(&r0);
  argInit_3x1_real_T(dv1);
  TargetEstimationNominalBatch(rangeArray_data, rangeArray_size, &r0, dv1, x_out);
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
  MultiRangeEstimation_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_MultiRangeEstimation();
  main_TargetEstimationNominalBatch();

  // Terminate the application.
  // You do not need to do this more than one time.
  MultiRangeEstimation_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
