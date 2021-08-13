example : 

```cpp
#include <LinearCalibrator.h>

// temperatures are x10 to get them in integer format
// [25°C, 75°C]
int32_t temperatures[2] = {250, 750};

// binary read inputs @ 25°C
int32_t read_temp_low[2] = {6671,59401};

// binary read inputs @ 75°C
int32_t read_temp_high[2] = {6829,59807};

// target binary values : this is what we want as results
int32_t targets[2] = {6554,58982};

// gain and offset @ 25°C
int32_t gain_low = 0;
int32_t offset_low = 0;

// gain and offset @ 75°C
int32_t gain_high = 0;
int32_t offset_high = 0;

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

  // 5.0Vcc, 16bit adc resolution, 16383 as conversion factor
  set_adc_params(&adc_params, 5.f, 16, 16383);

  // compute parameters @ 25 and 75°C
  get_coeffs(targets[1], targets[0], read_temp_low[1], read_temp_low[0], &gain_low, &offset_low, &adc_params);
  get_coeffs(targets[1], targets[0], read_temp_high[1], read_temp_high[0], &gain_high, &offset_high, &adc_params);

  // compute parameters shift due to temperature
  get_coeffs(gain_high, gain_low, temperatures[1], temperatures[0], &comp_gain_params[0], &comp_gain_params[1], &adc_params);
  get_coeffs(offset_high, offset_low, temperatures[1], temperatures[0], &comp_offset_params[0], &comp_offset_params[1], &adc_params);

  // simulation sample value
  int32_t sample = 6671; // 0.5V @ 25°C
  
  // simulation current temperature
  int32_t current_temperature = 250;

  // compensate sample
  int32_t output = compensate_sample(sample, current_temperature, comp_gain_params, comp_offset_params, compensated_params, &adc_params);
  
  Serial.println(output); // in binary
  Serial.println((float)output * adc_params.step, 6); // in volts
}

void loop() {}
```

Outputs : 
```
6555
0.500114
```
