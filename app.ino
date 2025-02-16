#include <Adafruit_NeoPixel.h>

// Define the pin connections
#define PIR_SENSOR_PIN D8
#define LED_STRIP_PIN D10
#define LIGHT_SENSOR_PIN A0
#define NUM_LEDS 28

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

// Variables to keep track of motion and timing
bool motionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long motionTimeout = 10 * 60 * 1000; // 10 minutes in milliseconds
const int darkThreshold = 500; // Adjust based on your light sensor and environment

void setup() {
  // Initialize the PIR sensor pin as input
  pinMode(PIR_SENSOR_PIN, INPUT);

  // Initialize the light sensor pin as input
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Start Serial Monitor for debugging
  Serial.begin(115200);
  Serial.println("System started. Waiting for motion and darkness...");
}

void loop() {
  // Read the PIR sensor and light sensor
  int motionState = digitalRead(PIR_SENSOR_PIN);
  int lightLevel = analogRead(LIGHT_SENSOR_PIN);

  // Print sensor values to Serial Monitor for debugging
  Serial.print("Motion: ");
  Serial.print(motionState);
  Serial.print(" | Light Level: ");
  Serial.println(lightLevel);

  // Check if motion is detected and it's dark
  if (motionState == HIGH && lightLevel < darkThreshold) {
    motionDetected = true;
    lastMotionTime = millis(); // Update the last motion time
    Serial.println("Motion detected in darkness! LED strip ON.");
  }

  // If motion is detected, turn on the LED strip to white
  if (motionDetected) {
    setStripColor(strip.Color(255, 255, 255)); // White color
  }

  // Check if the motion timeout has been reached
  if (motionDetected && (millis() - lastMotionTime > motionTimeout)) {
    motionDetected = false;
    setStripColor(strip.Color(0, 0, 0)); // Turn off the LED strip
    Serial.println("No motion for 10 minutes. LED strip OFF.");
  }

  delay(500); // Small delay to avoid flooding the Serial Monitor
}

// Function to set the color of the entire LED strip
void setStripColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
