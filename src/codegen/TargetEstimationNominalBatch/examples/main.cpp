//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 11-Feb-2016 15:19:32
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
static void argInit_1xd255_real32_T(float result_data[], int result_size[2]);
static void argInit_1xd255_uint64_T(unsigned long result_data[], int
  result_size[2]);
static void argInit_3x1_real32_T(float result[3]);
static void argInit_TargetEstimationParams(TargetEstimationParams *result);
static float argInit_real32_T();
static unsigned long argInit_uint64_T();
static unsigned char argInit_uint8_T();
static void c_main_TargetEstimationNominalB();
static void main_UWBMultiRangeEstimation();

// Function Definitions

//
// Arguments    : float result_data[]
//                int result_size[2]
// Return Type  : void
//
static void argInit_1xd255_real32_T(float result_data[], int result_size[2])
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
    result_data[idx1] = argInit_real32_T();
  }
}

//
// Arguments    : unsigned long result_data[]
//                int result_size[2]
// Return Type  : void
//
static void argInit_1xd255_uint64_T(unsigned long result_data[], int
  result_size[2])
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
    result_data[idx1] = argInit_uint64_T();
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
// Arguments    : TargetEstimationParams *result
// Return Type  : void
//
static void argInit_TargetEstimationParams(TargetEstimationParams *result)
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
// Return Type  : unsigned long
//
static unsigned long argInit_uint64_T()
{
  return 0UL;
}

//
// Arguments    : void
// Return Type  : unsigned char
//
static unsigned char argInit_uint8_T()
{
  return 0;
}

//
// Arguments    : void
// Return Type  : void
//
static void c_main_TargetEstimationNominalB()
{
  int rangeArray_size[2];
  float rangeArray_data[255];
  TargetEstimationParams r0;
  float fv2[3];
  float x_out[6];

  // Initialize function 'TargetEstimationNominalBatch' input arguments.
  // Initialize function input argument 'rangeArray'.
  argInit_1xd255_real32_T(rangeArray_data, rangeArray_size);

  // Initialize function input argument 'params'.
  // Initialize function input argument 'pos_init'.
  // Call the entry-point 'TargetEstimationNominalBatch'.
  argInit_TargetEstimationParams(&r0);
  argInit_3x1_real32_T(fv2);
  TargetEstimationNominalBatch(rangeArray_data, rangeArray_size, &r0, fv2, x_out);
}

//
// Arguments    : void
// Return Type  : void
//
static void main_UWBMultiRangeEstimation()
{
  unsigned char num_of_units;
  unsigned long timestamp_master_request_1_recv;
  unsigned long timestamp_slave_reply_send;
  unsigned long timestamp_master_request_2_recv;
  int c_timestamp_master_request_1_ar[2];
  unsigned long d_timestamp_master_request_1_ar[255];
  int c_timestamp_slave_reply_array_s[2];
  unsigned long c_timestamp_slave_reply_array_d[255];
  int c_timestamp_master_request_2_ar[2];
  unsigned long d_timestamp_master_request_2_ar[255];
  int ranges_size[2];
  float ranges_data[255];
  int tofs_size[2];
  float tofs_data[255];

  // Initialize function 'UWBMultiRangeEstimation' input arguments.
  num_of_units = argInit_uint8_T();
  timestamp_master_request_1_recv = argInit_uint64_T();
  timestamp_slave_reply_send = argInit_uint64_T();
  timestamp_master_request_2_recv = argInit_uint64_T();

  // Initialize function input argument 'timestamp_master_request_1_array'.
  argInit_1xd255_uint64_T(d_timestamp_master_request_1_ar,
    c_timestamp_master_request_1_ar);

  // Initialize function input argument 'timestamp_slave_reply_array'.
  argInit_1xd255_uint64_T(c_timestamp_slave_reply_array_d,
    c_timestamp_slave_reply_array_s);

  // Initialize function input argument 'timestamp_master_request_2_array'.
  argInit_1xd255_uint64_T(d_timestamp_master_request_2_ar,
    c_timestamp_master_request_2_ar);

  // Call the entry-point 'UWBMultiRangeEstimation'.
  UWBMultiRangeEstimation(num_of_units, timestamp_master_request_1_recv,
    timestamp_slave_reply_send, timestamp_master_request_2_recv,
    d_timestamp_master_request_1_ar, c_timestamp_master_request_1_ar,
    c_timestamp_slave_reply_array_d, c_timestamp_slave_reply_array_s,
    d_timestamp_master_request_2_ar, c_timestamp_master_request_2_ar, tofs_data,
    tofs_size, ranges_data, ranges_size);
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
  main_UWBMultiRangeEstimation();

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
