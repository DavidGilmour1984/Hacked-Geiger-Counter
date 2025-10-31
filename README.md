# ESP32 Radiation Pulse Detector + Web GUI

This project turns an ESP32 into a fast digital Geiger-style pulse counter using the **ADC** and a custom rising-edge detector. The ESP32 sends event packets to a browser-based HTML interface, which displays:

- Live radiation pulse count
- Counts-per-minute (CPM)
- Calculated microsieverts/hour (µSv/h)
- Live table of detected low/high ADC ranges for each pulse
- Start/Stop + Auto-reset logging controls

### Purpose

This firmware + GUI pair detects radiation events from a Geiger tube amplifier or scintillation detector. Rather than relying on simple thresholding, the ESP32 detects events by identifying **large positive ADC jumps** between consecutive readings. This makes it robust against drift and noise, and capable of detecting closely-spaced pulses.

### How Detection Works

The ESP32 ADC continuously streams samples. A radiation event is registered when:

- ADC value rises by **> 500** between adjacent samples (adjustable)
- Only *rising-edge* slopes are used to trigger counts
- Values below a minimum gate are ignored
- No timing delay is used (100% continuous sampling)

Each pulse sends:

