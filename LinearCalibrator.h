#ifndef LINEAR_CALIBRATOR_H
#define LINEAR_CALIBRATOR_H

#define CALIB_FACTOR 16383
#define VCC 3.3
#define STEP VCC/CALIB_FACTOR

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct adc_params_t {
    float vcc;
    unsigned int adc_resolution;
    uint32_t maximum;
    float step;
    int32_t factor; // eg : Fixed gain = 16383/16383 = floating gain = 1.;
}adc_params_t;
typedef struct adc_params_t* ADC_PARAMS;

void set_adc_params(ADC_PARAMS adc_params, float vcc, uint32_t resolution, int32_t factor);

//output params gain and offset [a, b];
void resolve_params(int32_t *sensor_inputs, int32_t *read_values, int32_t *target_values, int32_t *output_params, ADC_PARAMS adc_params);

void resolve_shift(int32_t *temperatures, int32_t *params_low, int32_t *params_high, int32_t *gain_params, int32_t *offset_params, ADC_PARAMS adc_params);

void compensate_params(int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params);
int32_t compensate_sample(int32_t sample, int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params);


#ifdef __cplusplus
}
#endif


#endif