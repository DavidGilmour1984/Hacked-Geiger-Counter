// ========= ESP32 HIGH-RATE GEIGER PEAK DETECTOR =========
// Detects valley -> peak even if pulses overlap.
// Prints: low,high
// No delay, ultra-fast loop, works at high CPM
// Baud: 115200

const int PIN = 36;

// ==== Tuning ====
int MIN_DELTA = 700;        // pulse = rise of this magnitude
int BASE_DECAY = 3;         // how fast baseline creeps up
int RESET_DROP = 30;        // how much ADC must fall to re-arm
int NOISE_FLOOR = 80;       // ignore below this
int REFRACTORY_US = 400;    // very short block to avoid double count
// =================

int baseline = 4095;
int peak = 0;
bool armed = true;
unsigned long lastPulseUs = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  Serial.println("READY");
}

void loop() {
  int v = analogRead(PIN);
  if (v < 0) v = 0;
  if (v < NOISE_FLOOR) v = NOISE_FLOOR;

  unsigned long now = micros();
  bool refractory = (now - lastPulseUs) < REFRACTORY_US;

  // Track baseline = rolling valley
  if (v < baseline) baseline = v;
  else baseline += BASE_DECAY; // slow float upward

  int rise = v - baseline;

  // Pulse detected
  if (armed && !refractory && rise >= MIN_DELTA) {
    peak = v;
    Serial.print(baseline);
    Serial.print(",");
    Serial.println(peak);
    lastPulseUs = now;
    armed = false;
  }

  // Re-arm logic when pulse falls a bit
  if (!armed && (peak - v) >= RESET_DROP) {
    baseline = v;
    peak = 0;
    armed = true;
  }

  // No delay, free-run
}
