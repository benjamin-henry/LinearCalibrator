#ifndef LINEAR_CALIBRATOR_H
#define LINEAR_CALIBRATOR_H

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

void get_coeffs(int32_t y2, int32_t y1, int32_t x2, int32_t x1, int32_t *gain, int32_t *offset, ADC_PARAMS adc_params);

// xxx_params = gain and offset [a, b];
void compensate_params(int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params);

int32_t compensate_sample(int32_t sample, int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params);

#ifdef __cplusplus
}
#endif

#endif
