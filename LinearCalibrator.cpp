#include "LinearCalibrator.h"
#include "Arduino.h"

void set_adc_params(ADC_PARAMS adc_params, float vcc, uint32_t resolution, int32_t factor){
    adc_params->vcc = vcc;
    adc_params->adc_resolution = resolution;
    adc_params->maximum = pow(2, adc_params->adc_resolution) - 1;
    adc_params->step = (float)adc_params->vcc / (float)adc_params->maximum;
    adc_params->factor = factor;
}

void get_coeffs(int32_t y2, int32_t y1, int32_t x2, int32_t x1, int32_t *gain, int32_t *offset, ADC_PARAMS adc_params) {
    float _gain = ((float)y2 - (float)y1) / ((float)x2 - (float)x1);
    float _offset = ((float)y2 - (_gain * (float)x2));
    *gain = _gain * adc_params->factor;
    *offset = (int32_t)_offset;
}

void compensate_params(int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params) {
    compensated_params[0] = gain_params[0] * temperature / adc_params->factor + gain_params[1];
    compensated_params[1] = offset_params[0] * temperature / adc_params->factor + offset_params[1];
}

int32_t compensate_sample(int32_t sample, int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params) {
    compensate_params(temperature, gain_params, offset_params, compensated_params, adc_params);
    return compensated_params[0] * sample / adc_params->factor + compensated_params[1]; 
}
