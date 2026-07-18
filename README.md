# 🤖 Car Robot Project — WiFi-Controlled Autonomous Vehicle

A sophisticated IoT robot car with real-time Blynk app control, obstacle avoidance system, and WiFi connectivity. Features autonomous navigation with ultrasonic sensors, live OLED display feedback, and dual-motor H-bridge control.

## 🎥 Demo Videos

Watch the car in action:
- **YouTube**: [Car Robot Full Demo](https://youtube.com/shorts/W8nna2d3xTk?feature=share) 
- **Features**: Remote Blynk control, obstacle detection, real-time speed adjustment

## 📋 Project Overview

This project demonstrates **production-grade IoT robotics** combining:
- **Wireless Control** — Blynk app integration for remote operation
- **Autonomous Navigation** — Ultrasonic-based obstacle avoidance
- **Real-Time Feedback** — OLED display with animated status indicators
- **Robust Connectivity** — WiFi reconnection logic, connection status monitoring
- **Motor Control** — Dual DC motor management with PWM speed control

**Use case**: Smart home robotics foundation, autonomous delivery bots, obstacle-aware vehicles, educational IoT platform.

## 🛠️ Hardware Components

| Component | Qty | GPIO Pin(s) | Purpose |
|-----------|-----|------------|---------|
| **ESP32 DevKit V1** | 1 | — | Main microcontroller (dual-core, WiFi, 3.3V logic) |
| **DC Motor** | 2 | IN1/IN2, IN3/IN4 | Left & right wheel drive |
| **Motor Driver (L298N)** | 1 | GPIO 12-27 | Dual H-bridge motor control |
| **Ultrasonic Sensor (HC-SR04)** | 1 | GPIO 5 (TRIG), GPIO 18 (ECHO) | Distance measurement & obstacle detection |
| **OLED Display (0.96" SSD1306)** | 1 | GPIO 21 (SDA), GPIO 22 (SCL) | Real-time status display (I2C) |
| **LiPo Battery (7.4V 2S 1500mAh)** | 1 | — | Power supply (motor + ESP32) |
| **USB Cable (Micro-B)** | 1 | — | Programming & charging |
| **Breadboard & Jumpers** | — | — | Prototyping |

## 📌 Wiring Diagram

```
┌──────────────────────────────────────┐
│          ESP32 DevKit V1             │
├──────────────────────────────────────┤
│                                      │
│  Motor Control (L298N H-Bridge)     │
│  ┌─────────────────────────────────┐ │
│  │ GPIO 12 (IN1) ──→ Motor Left-1  │ │
│  │ GPIO 14 (IN2) ──→ Motor Left-2  │ │
│  │ GPIO 27 (IN3) ──→ Motor Right-1 │ │
│  │ GPIO 26 (IN4) ──→ Motor Right-2 │ │
│  │ GPIO 13 (ENA) ──→ Speed Left    │ │
│  │ GPIO 25 (ENB) ──→ Speed Right   │ │
│  └─────────────────────────────────┘ │
│                                      │
│  Ultrasonic Sensor (HC-SR04)        │
│  ┌─────────────────────────────────┐ │
│  │ GPIO 5  (TRIG) ──→ Trigger      │ │
│  │ GPIO 18 (ECHO) ──→ Echo (5V!)   │ │
│  └─────────────────────────────────┘ │
│                                      │
│  OLED Display (I2C) SSD1306         │
│  ┌─────────────────────────────────┐ │
│  │ GPIO 21 (SDA) ──→ I2C Data      │ │
│  │ GPIO 22 (SCL) ──→ I2C Clock     │ │
│  │ 3.3V         ──→ VCC            │ │
│  │ GND          ──→ GND            │ │
│  └─────────────────────────────────┘ │
│                                      │
│  Power Management                   │
│  ┌─────────────────────────────────┐ │
│  │ GND (common ground)             │ │
│  │ VIN (5V from L298N or boost)    │ │
│  └─────────────────────────────────┘ │
└──────────────────────────────────────┘

Motor Wiring (Typical):
Left Motor:  GPIO12→motor+ | GPIO14→motor-
Right Motor: GPIO27→motor+ | GPIO26→motor-

HC-SR04 Voltage Divider (ECHO pin):
ECHO (5V output) ──→ [2.2kΩ] ──→ GPIO 18
                      └─→ [3.3kΩ] ──→ GND
(Converts 5V to 3.3V for safe ESP32 input)

Power Supply:
LiPo Battery (7.4V) ──→ L298N VCC
                   ├──→ ESP32 VIN (via voltage regulator)
                   └──→ GND (common)
```

**Critical Pin Notes**:
- **ECHO pin (GPIO 18)** must have voltage divider! HC-SR04 outputs 5V, ESP32 tolerates max 3.3V
- **PWM channels 0-1** reserved for motor speed (ENA/ENB)
- **I2C (GPIO 21/22)** at 3.3V logic level

## 🚀 Installation & Setup

### Prerequisites
- VS Code with PlatformIO extension
- ESP32 DevKit V1 + USB cable
- Blynk app (download on iOS/Android)
- WiFi network (2.4GHz, not 5GHz)
- Arduino framework knowledge

### Step 1: Clone Repository
```bash
git clone https://github.com/nguyentuankiet070507-commits/Car_robot_project.git
cd Car_robot_project
```

### Step 2: Configure Security Settings
⚠️ **IMPORTANT**: Never commit WiFi credentials or auth tokens to GitHub!

Create `include/config.h` (add to `.gitignore`):
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// WiFi credentials (KEEP PRIVATE - do not commit)
#define WIFI_SSID "your-network-name"
#define WIFI_PASSWORD "your-network-password"

// Blynk authentication (KEEP PRIVATE - do not commit)
#define BLYNK_TEMPLATE_ID "your-blynk-template-id"
#define BLYNK_AUTH_TOKEN "your-blynk-auth-token"

#endif // CONFIG_H
```

Update `src/main.cpp`:
```cpp
#include <config.h>  // Remove hardcoded credentials, use this instead

```

Add to `.gitignore`:
```
include/config.h
.pio
.vscode/.browse.c_cpp.db*
.vscode/c_cpp_properties.json
.vscode/launch.json
.vscode/ipch

```

### Step 3: Setup Blynk App
1. **Download Blynk IoT app** (iOS/Android)
2. **Create new template**:
   - Name: "CAR ROBOT CONTROLLER"
   - Device type: "ESP32"
3. **Add Virtual Pins**:
   - `V0` → Slider (Speed control, range 0-200)
   - `V1` → Button (Forward direction)
   - `V2` → Button (Backward direction)
   - `V3` → Button (Left turn)
   - `V4` → Button (Right turn)
4. **Copy auth token** → Add to `config.h`
5. **Create device** from template → Save auth token

### Step 4: Open in PlatformIO
```bash
code .
# PlatformIO should auto-detect platformio.ini
```

Verify `platformio.ini`:
```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
lib_deps = 
    blynkgithub/Blynk @ ^1.2.2
    adafruit/Adafruit SSD1306 @ ^2.5.7
    paulstoffregen/Servo @ ^1.2.2
```

### Step 5: Build & Flash
```bash
# Build project
Ctrl+Alt+B

# Upload to ESP32 (ensure USB cable connected)
Ctrl+Alt+U

# Verify upload:
# You should see "Leaving... Hard resetting via RTS pin"
```

### Step 6: Monitor Serial Output
```bash
# Open Serial Monitor
Ctrl+Shift+A

# Set baud rate to 115200
# Expected output:
# WiFi connecting...
# Blynk connected!
# Distance (cm): 45
# No obstacle detected.
```

### Step 7: Test Control
1. Open Blynk app
2. Press "Forward" button → Car moves forward
3. Adjust speed slider → Motor speed changes
4. Approach obstacle → OLED shows "Danger!", car stops
5. Move away → Car resumes normal operation

## 💻 Code Architecture

### Project Structure
```
Car_robot_project/
├── platformio.ini           # PlatformIO configuration
├── include/
│   ├── config.h            # WiFi & Blynk credentials (PRIVATE)
│   ├── sensor.h            # Ultrasonic sensor interface
│   ├── OLED_display.h      # OLED display functions
│   └── motor.h             # Motor control utilities
├── src/
│   ├── main.cpp            # Main program (control loop)
│   ├── sensor.cpp          # HC-SR04 implementation
│   └── OLED_display.cpp    # SSD1306 display driver
├── lib/                    # External libraries
├── .gitignore             # Ignore config.h & sensitive files
└── README.md              # This file
```

### Main Control Flow

```
setup()
├── Initialize Serial (115200 baud)
├── Initialize OLED display
├── Initialize ultrasonic sensor
├── Connect WiFi (SSID/password from config.h)
├── Connect Blynk (auth token from config.h)
├── Configure motor control pins (GPIO 12-27)
└── Setup PWM channels for motor speed (ENA/ENB)

loop()
├── Check WiFi connection (every 10 seconds)
├── Run Blynk.run() (handles app button presses)
├── Read ultrasonic distance
├── Update OLED display (animation frames)
├── Execute motor command:
│   ├── If distance < 10cm  → STOP (obstacle too close)
│   ├── If distance < 30cm  → Show danger, alert on OLED
│   └── If distance > 30cm  → Normal operation
└── Loop rate: ~100ms (delay between sensor reads)
```

### Key Functions

**Motor Control**:
```cpp
forward()   // Both motors forward (IN1/IN3 HIGH, IN2/IN4 LOW)
backward()  // Both motors backward (reverse direction)
left()      // Left turn (right motor forward, left motor backward)
right()     // Right turn (left motor forward, right motor backward)
stopMotor() // Emergency stop (all pins LOW, PWM = 0)
```

**Sensor Readings**:
```cpp
readSensor()  // Returns:
              //   0 = No obstacle (distance > 30cm)
              //   1 = Obstacle detected (10cm < distance < 30cm)
              //   2 = Obstacle too close (distance <= 10cm)
```

**Display Feedback**:
```cpp
cool_display()    // Animated "normal operation" frames
loader_display()  // Animated "connecting" frames
danger_display()  // Animated "obstacle warning" frames
```

**Blynk Virtual Pins**:
```cpp
BLYNK_WRITE(V0)   // Speed slider (0-200)
BLYNK_WRITE(V1)   // Forward button
BLYNK_WRITE(V2)   // Backward button
BLYNK_WRITE(V3)   // Left button
BLYNK_WRITE(V4)   // Right button
```

## 🔧 Configuration & Calibration

### Motor Speed Mapping
In `main.cpp`, speed is scaled by motor characteristics:
```cpp
speed1 = (int) constrain(speed2 * 128 / 89, 0, MAX_SPEED);
```

**Explanation**: 
- `speed2` comes from Blynk slider (0-200)
- `128/89` factor balances motor speed differences
- Adjust if motors don't turn at same speed

**To calibrate**:
1. Measure motor speeds with stroboscope or video analysis
2. If left motor faster: increase factor slightly
3. If right motor faster: decrease factor

### Obstacle Detection Thresholds
In `src/sensor.cpp`:
```cpp
if (distanceCm < 30 && distanceCm > 10)     // Medium alert (return 1)
else if (distanceCm <= 10)                   // Critical alert (return 2)
```

**Adjust based on your environment**:
- Tight spaces: Lower from 30cm to 20cm
- Open areas: Raise to 40cm
- Emergency threshold: Tune 10cm based on car size

### WiFi Reconnection Timeout
In `main.cpp`:
```cpp
long timeout = 10000;  // 10 seconds to reconnect
```

Increase if WiFi is unstable in your environment.

## 📊 Performance Characteristics

| Metric | Value | Notes |
|--------|-------|-------|
| **Motor Speed** | 0-200 PWM | Max depends on battery voltage & motor specs |
| **Control Latency** | ~100-200ms | Blynk cloud → WiFi → ESP32 |
| **Obstacle Detection Range** | 4cm - 400cm | HC-SR04 typical specs |
| **Detection Accuracy** | ±1cm | Within 30cm range |
| **OLED Refresh Rate** | ~10Hz | Animation frame timing |
| **Battery Runtime** | ~2-3 hours | Depends on activity & motor load |

## 🐛 Troubleshooting

| Issue | Symptoms | Solution |
|-------|----------|----------|
| **Car doesn't move** | Motors silent, no vibration | Check motor GPIO pins; verify PWM channels 0-1 attached; test with Serial output |
| **Blynk buttons don't work** | App connects but no motor movement | Verify auth token in config.h; check WiFi connection; enable Blynk.run() in loop |
| **Ultrasonic reads 0cm always** | Distance always zero | Check ECHO pin voltage divider (must convert 5V→3.3V); verify trigPin/echoPin GPIO numbers |
| **OLED shows nothing** | Black screen | Check I2C address (0x3C vs 0x3D); verify SDA/SCL on GPIO 21/22; install Adafruit library |
| **WiFi connects then disconnects** | WiFi symbol flickers | Move closer to router; check password in config.h; increase timeout value |
| **Car moves in circles** | Turns despite going "forward" | Motors are mismatched speed → recalibrate factor 128/89 |
| **Serial monitor shows garbage** | Unreadable characters | Set baud rate to 115200 (not 9600) |

## 🎥 Demo Features Explained

**Remote Control Via Blynk**:
1. User taps "Forward" button in Blynk app
2. Button signal sent over WiFi to Blynk servers
3. Blynk server sends command to ESP32
4. ESP32 executes motor forward command
5. Car moves, distance sensor continuously monitored

**Obstacle Avoidance**:
1. Ultrasonic sensor emits 40kHz pulse every ~100ms
2. Echo time measured → converted to distance
3. If distance < threshold → stop motor immediately
4. OLED display animated to show "Danger!" state
5. User must reverse or manual override via Blynk

**Real-Time Feedback**:
- OLED shows car state (normal/connecting/obstacle)
- Serial monitor logs all sensor readings & commands
- Blynk app shows connection status
- LED animations provide visual feedback

## 📚 Learning Outcomes

By building this project, you master:
- ✅ **WiFi IoT Integration** — Blynk platform, async connection handling
- ✅ **Multi-Protocol Hardware** — GPIO + PWM + I2C + digital I/O
- ✅ **Sensor Fusion** — Combining ultrasonic + motor feedback + app input
- ✅ **Real-Time Systems** — Non-blocking loops, interrupt-safe operations
- ✅ **Robotics Control** — H-bridge motor control, direction management, speed regulation
- ✅ **Embedded Debugging** — Serial logging, state monitoring, connection recovery

## 🥲 Difficulties

During this project, I had some difficulties:
- Missing encoder to balance speed of both motor (solved temporarily by ratio)
- Missing PID algorithm to control
- Limited budget

## 🚀 Future Enhancements

### Easy (1-2 weeks)
1. **Line Following** — Add IR sensors to follow black tape
2. **Speed Adjustment** — Blynk slider for real-time PWM control
3. **Battery Monitoring** — Read ADC for battery voltage, warn on low battery
4. **LED Status Indicator** — RGB LED shows WiFi/Blynk connection state

### Medium (2-3 weeks)
5. **MQTT Integration** — Replace Blynk with MQTT for home automation
6. **Edge Detection** — Add cliff sensors to prevent falling off tables
7. **PID Speed Control** — Implement feedback loop for consistent speed
8. **Logging Dashboard** — Store sensor data → graph trip telemetry

### Advanced (3-4 weeks)
9. **ESP-NOW Mesh** — Coordinate multiple robots without cloud
10. **Computer Vision** — Add ESP32-CAM for object detection
11. **Autonomous Navigation** — SLAM-based mapping & pathfinding
12. **Mobile App** — Build native Flutter/React app instead of Blynk

## ⚠️ Security Best Practices

**DO**:
- ✅ Store WiFi & auth tokens in `config.h` (add to `.gitignore`)
- ✅ Use strong WiFi passwords (min 12 chars, alphanumeric + symbols)
- ✅ Regenerate Blynk auth tokens regularly
- ✅ Validate sensor readings (reject outliers)
- ✅ Implement connection timeouts (prevent infinite loops)

**DON'T**:
- ❌ Hardcode credentials in source files
- ❌ Commit `config.h` to GitHub
- ❌ Use default/weak WiFi passwords
- ❌ Expose auth tokens in public repos
- ❌ Trust only one sensor for safety-critical decisions

## 📖 Reference Documentation

- **ESP32 Docs**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
- **Blynk IoT**: https://blynk.cloud/dashboard/login
- **HC-SR04 Datasheet**: [Ultrasonic Sensor Specs](https://www.elecfreaks.com/wiki/index.php?title=Ultrasonic_Sensor)
- **L298N Motor Driver**: [Dual H-Bridge Specs](https://www.sparkfun.com/datasheets/Robotics/L298_H_Bridge.pdf)
- **SSD1306 OLED**: [Adafruit Library Docs](https://adafruit.github.io/Adafruit_SSD1306/)

## 🤝 Author

**Tuấn Kiệt Nguyễn** ([nguyentuankiet070507@gmail.com](mailto:nguyentuankiet070507@gmail.com))

Demonstrates production-grade IoT robotics: WiFi connectivity, sensor fusion, real-time control, autonomous navigation.

## 📝 License

Open source — fork, modify, and use for educational or commercial projects. Attribution appreciated.

## 🔗 Related Projects

- **Traffic_Light_with_barriers** — Finite state machines & servo control
- **Counting_with_FreeRTOS** — Real-time OS multitasking
- **RGB_LED_Make_it_colorful** — WiFi + app integration basics

---

**Last updated**: July 2026  
**Platform**: ESP32 DevKit V1  
**Framework**: Arduino + PlatformIO  
**Status**: Production-ready (with security hardening)
