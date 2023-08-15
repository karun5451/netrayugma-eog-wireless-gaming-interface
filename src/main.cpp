#include <Arduino.h>

#define BIO_SIGNAL_HORIZONTAL 34
#define BIO_SIGNAL_VERTICAL 35

// Filter coefficients
const float hp_alpha = 0.8;  // High-pass filter coefficient
const float lp_alpha = 0.2;  // Low-pass filter coefficient

void setup() {
  Serial.begin(115200);
}

float highPassFilter(float input, float prev_output, float prev_input) {
  return hp_alpha * (prev_output + input - prev_input);
}

float lowPassFilter(float input, float prev_output) {
  return lp_alpha * input + (1 - lp_alpha) * prev_output;
}

void loop() {
  static float prev_horizontal = 0.0;
  static float prev_vertical = 0.0;
  static float prev_input_horizontal = 0.0;
  static float prev_input_vertical = 0.0;

  // Read raw EOG data from analog pins
  int raw_horizontal = analogRead(BIO_SIGNAL_HORIZONTAL);
  int raw_vertical = analogRead(BIO_SIGNAL_VERTICAL);

  // Apply high-pass and low-pass filters
  float filtered_horizontal = highPassFilter(raw_horizontal, prev_horizontal, prev_input_horizontal);
  float filtered_vertical = lowPassFilter(raw_vertical, prev_vertical);

  prev_horizontal = filtered_horizontal;
  prev_vertical = filtered_vertical;
  prev_input_horizontal = raw_horizontal;

  // Send filtered EOG data over serial
  Serial.print(filtered_horizontal);
  Serial.print(',');
  Serial.println(filtered_vertical);

  delay(10); // Adjust delay as per the ESp's cpu freq offset and as we add more functionality
}
