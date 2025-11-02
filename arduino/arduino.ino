int threshold = 600;   // default, can be changed via serial
bool inPulse = false;
int minVal = 1023;
unsigned long lastPulseTime = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Receive threshold from HTML
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd.startsWith("T")) {
      threshold = cmd.substring(1).toInt();
    }
  }

  // Read ADC as fast as possible
  int v = analogRead(A0);

  // Detect pulse
  if (!inPulse && v < threshold) {
    inPulse = true;
    minVal = v;
  }

  if (inPulse) {
    if (v < minVal) minVal = v;
    if (v > threshold) {         // Pulse finished
      inPulse = false;
      Serial.println(minVal);    // Send only the bottom of the pulse
    }
  }
}
