// ===== ESP32 Geiger — Adaptive Pulse Detector =====
// Auto-tracks baseline and detects pulses by rising edge above noise

const int pin = 36;

// adaptive filter coefficients
float baseline = 0;
float alpha = 0.02;        // ↓ alpha = more sensitive   ↑ alpha = less sensitive
int dynamicOffset = 800;   // ↓ dynamicOffset = more sensitive   ↑ = less sensitive
int minRise = 20;          // ↓ minRise = more sensitive        ↑ = less sensitive
int endSlope = -8;         // (not sensitivity — leave alone unless pulse merging)
int refractoryUs = 1800;   // ↓ refractoryUs = more sensitive to close pulses   ↑ = less sensitive / avoids doubles



bool pulseActive = false;
int peak = 0;
int lastV = 0;
unsigned long lastPulseUs = 0;

void setup() {
  Serial.begin(9600);
  delay(200);
  Serial.println("READY");
}

void loop() {
  int v = analogRead(pin);
  unsigned long now = micros();
  int slope = v - lastV;

  // ===== Adaptive baseline update =====
  if (!pulseActive) {
    baseline = baseline * (1.0 - alpha) + v * alpha;
  }

  int delta = v - baseline;  // pulse height above noise floor

  // ===== Pulse start =====
  if (!pulseActive &&
      delta > dynamicOffset &&
      slope > minRise &&
      (now - lastPulseUs) > refractoryUs)
  {
    pulseActive = true;
    peak = v;
    lastPulseUs = now;
  }

  // ===== Track max =====
  if (pulseActive && v > peak) {
    peak = v;
  }

  // ===== Pulse end =====
  if (pulseActive && slope < endSlope) {
    pulseActive = false;
    Serial.println(peak);
  }

  lastV = v;
}
