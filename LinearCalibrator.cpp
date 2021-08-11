#include "LinearCalibrator.h"
#include "Arduino.h"

void set_adc_params(ADC_PARAMS adc_params, float vcc, uint32_t resolution, int32_t factor){
    adc_params->vcc = vcc;
    adc_params->adc_resolution = resolution;
    adc_params->maximum = pow(2, adc_params->adc_resolution) - 1;
    adc_params->step = (float)adc_params->vcc / (float)adc_params->maximum;
    adc_params->factor = factor;
}

void resolve_params(int32_t *sensor_inputs, int32_t *read_values, int32_t *target_values, int32_t *output_params, ADC_PARAMS adc_params) {
    // equations are of form y = ax + b
    // compute a, then b for read values
    // inputs are of form [read_low, read_high];
    float a_read_values = ((float)read_values[1]-(float)read_values[0])/((float)sensor_inputs[1]-(float)sensor_inputs[0]);
    float b_read_values = (float)read_values[1] - (a_read_values * (float)sensor_inputs[1]);

    // compute a, then b for target values
    // inputs are of form [target_low, target_high];
    float a_target_values = ((float)target_values[1]-(float)target_values[0])/((float)sensor_inputs[1]-(float)sensor_inputs[0]);
    float b_target_values = (float)target_values[1] - a_target_values * (float)sensor_inputs[1];

    //output params [a, b];
    output_params[0] = (float)(a_target_values / a_read_values)*adc_params->factor;
    output_params[1] = b_target_values - b_read_values;
}

void resolve_shift(int32_t *temperatures, int32_t *params_low, int32_t *params_high, int32_t *gain_params, int32_t *offset_params, ADC_PARAMS adc_params) {
    float temp_gain_a = ((float)params_high[0]-(float)params_low[0])/((float)temperatures[1]-(float)temperatures[0]);
    float temp_gain_b = (float)params_high[0] - (temp_gain_a *(float)temperatures[1]);

    float temp_offset_a = ((float)params_high[1]-(float)params_low[1])/((float)temperatures[1]-(float)temperatures[0]);
    float temp_offset_b = (float)params_high[1] - (temp_offset_a *(float)temperatures[1]);

    gain_params[0] = temp_gain_a * adc_params->factor;
    gain_params[1] = temp_gain_b;

    offset_params[0] = temp_offset_a * adc_params->factor;
    offset_params[1] = temp_offset_b;
}

void compensate_params(int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params) {
    compensated_params[0] = gain_params[0] * temperature / adc_params->factor + gain_params[1];
    compensated_params[1] = offset_params[0] * temperature / adc_params->factor + offset_params[1];
}

int32_t compensate_sample(int32_t sample, int32_t temperature, int32_t *gain_params, int32_t *offset_params, int32_t *compensated_params, ADC_PARAMS adc_params) {
    compensate_params(temperature, gain_params, offset_params, compensated_params, adc_params);
    return compensated_params[0] * sample / adc_params->factor + compensated_params[1]; 
}
