// ===== ESP32 Geiger Valley-to-Peak Detector (Δ >= 500) =====
// Counts a pulse when peak minus previous valley >= 500
// Prints "valley,peak" for each pulse detected

const int pin = 36; // ADC input (VP)
int noiseFloor = 100;
int thresholdDelta = 500;

int valley = 4095;    // track lowest seen value
int peak   = 0;       // track highest seen value
bool climbing = false;
unsigned long lastPulseUs = 0;
unsigned long refractoryUs = 1200; // ignore events too close together

void setup() {
  Serial.begin(115200);
}

void loop() {
  int v = analogRead(pin);
  unsigned long nowUs = micros();

  if (v < noiseFloor) { 
    // ignore tiny noise
    return;
  }

  // Update valley if we're not climbing
  if (!climbing && v < valley) {
    valley = v;
  }

  // Peak tracking if climbing
  if (climbing && v > peak) {
    peak = v;
  }

  // Detect start of climb
  if (!climbing && v > valley && (v - valley >= thresholdDelta)) {
    climbing = true;
    peak = v;
  }

  // Detect peak finished & pulse confirmed
  if (climbing && v < peak) {
    int delta = peak - valley;

    if (delta >= thresholdDelta && (nowUs - lastPulseUs) > refractoryUs) {
      Serial.print(valley);
      Serial.print(",");
      Serial.println(peak);

      lastPulseUs = nowUs;
    }

    // Reset for next cycle
    climbing = false;
    valley = v;
    peak = v;
  }
}
