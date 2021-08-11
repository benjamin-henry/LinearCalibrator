#include <LinearCalibrator.h>


// temperatures are x10 to get them in integer format
// [25°C, 75°C]
int32_t temperatures[2] = {
  250, 750
};

// binary converted voltage inputs (.5V, 4.5V) with a 16 bit adc and a 5V vcc
int32_t inputs[2] = {
  6554,
  58982
};

// binary read inputs @ 25°C
int32_t read_temp_low[2] = {
  6671,
  59270
};

// binary read inputs @ 75°C
int32_t read_temp_high[2] = {
  6829,
  59807
};

// target binary values : this is what we want as results
int32_t targets[2] = {
  6554,
  58982
};

// gain and offset @ 25°C
int32_t params_low[2] = {
  0,0
};

// gain and offset @ 75°C
int32_t params_high[2] = {
  0,0
};

// gain and offset parameters are temperature dependent,
// so we have to compute them each time we compensate a sample.
// they are also seen as linear functions ax+b where x is the temperature.
int32_t comp_gain_params[2] = {0,0};
int32_t comp_offset_params[2] = {0,0};

// compensated gain and offset
int32_t compensated_params[2] = {0,0};

// to store info like VCC, adc resolution, conversion factor...
adc_params_t adc_params;

void setup() {
  Serial.begin(115200);

  set_adc_params(&adc_params, 5.f, 16, 16383);

  resolve_params(inputs, read_temp_low, targets, params_low, &adc_params);
  resolve_params(inputs, read_temp_high, targets, params_high, &adc_params);

  resolve_shift(temperatures, params_low, params_high, comp_gain_params, comp_offset_params, &adc_params);


  // simulation sample value
  int32_t sample = 6671; // 0.5V @ 25°C
  
  // simulation current temperature
  int32_t current_temperature = 250;

  // compensated sample
  int32_t output = compensate_sample(sample, current_temperature, comp_gain_params, comp_offset_params, compensated_params, &adc_params);
  
  Serial.println(output); // in binary
  Serial.println((float)output * adc_params.step, 6); // in volts
}

void loop() {
  

}
