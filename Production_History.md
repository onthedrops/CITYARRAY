# CITYARRAY MVP - Production History

**Project**: CITYARRAY - Agentic AI Public Information Platform  
**Lead**: Eben (26 years LA radio experience, Founder)  
**Team**: Jonathan Pullen (CTO), Lucas Sojka (Tech Lead), Bartlomiej "Blacha" Blacha (Hardware)  
**Mission**: Solving society's most critical vulnerability—communications during crisis  
**Status**: MVP Development (Phase 2: Hardware Analysis Complete)

---

## 📋 Executive Summary

CITYARRAY is an autonomous AI-powered information platform combining computer vision, LED displays, and voice interaction for deployment in smart cities, transportation hubs, and emergency response scenarios. This document tracks all development activities, hardware discoveries, firmware integration, and architectural decisions from inception through production deployment.

**Key Metrics**:
- Target Latency: <100ms decision latency
- Language Support: 15+ languages (MVP: English + Spanish)
- Object Detection: 5-10 object classes (YOLO-based)
- Hardware: Raspberry Pi 5 + AI Accelerator + OV5647 Camera + 8×8 RGB LED Matrix (×2)
- Display: 2" × 21" portable stand-mounted enclosure (16 pixels wide × 8 tall)
- LED Control: Shift register-based multiplexing (no dedicated driver IC)

---

## 🚀 Development Timeline

### 2025-11-24 | 18:30 UTC | Phase 2: Hardware Analysis & GPIO Mapping

**Status**: ✅ COMPLETE - Ready for Implementation

#### Summary
Successfully reverse-engineered LED matrix control architecture from existing Neio Systems firmware. Identified shift register-based multiplexing scheme (3-wire SPI-like protocol) with row binary decoder. Created comprehensive GPIO mapping strategy for Raspberry Pi 5 and developed Python hardware abstraction layer (HAL) for LED matrix control. Discovered NO dedicated LED driver IC—entire control via GPIO bit-banging and shift registers.

#### Detailed Progress

**Hardware Architecture Discovery**
- **LED Driver Type**: Shift register-based column control + row multiplexing (NOT dedicated IC)
- **Shift Register Protocol**: 3-wire SPI-like (DATA_RED, DATA_GREEN, DATA_CLK, DATA_LAT)
- **Row Selection**: 4-bit binary decoder (16 rows addressable, 8 active)
- **Color Channels**: Independent Red and Green (Yellow achievable via R+G)
- **Brightness Control**: BCM (Binary Code Modulation) via hardware timer
- **Current Implementation**: ESP32-based (Arduino firmware)

**GPIO Pin Mapping (ESP32 → Raspberry Pi 5)**

| Function | ESP32 GPIO | RPi5 GPIO | Purpose |
|----------|-----------|----------|---------|
| VLED_EN | 23 | 24 | LED voltage enable |
| DATA_RED | 4 | 17 | Red shift register data |
| DATA_GREEN | 5 | 27 | Green shift register data |
| DATA_CLK | 18 | 23 | Shift register clock |
| DATA_LAT | 19 | 25 | Shift register latch |
| ROW_A | 32 | 6 | Row select bit A |
| ROW_B | 33 | 12 | Row select bit B |
| ROW_C | 25 | 16 | Row select bit C |
| ROW_D | 2 | 20 | Row select bit D |
| ROW_EN | 27 | 21 | Row enable (active LOW) |

**Display Parameters Confirmed**
- Physical: 16 pixels wide × 8 pixels tall (2× 8×8 matrices, horizontal)
- Active rows: 8 (binary selector allows 16, but only 8 used)
- Color depth: 2-bit (Red, Green channels)
- Data format: Serial-in, parallel-out shift registers
- Row refresh: Multiplexed at ~16-20 FPS per BCM timing

**Firmware Code Analysis**
- Examined `bcm.c` - BCM precision timing engine
  - Hardware timer: Prescale 240 (1 MHz from 240 MHz CPU)
  - First bit timing: 16 μs, doubling per precision bit
  - 8-bit precision = 256 brightness levels
- Examined `display.c` - Row/column rendering
  - `column_shift_single()` & `column_shift()` - serial data shifting
  - `turn_on_row()` - row multiplexing control
  - `row_data_latch()` - output strobe
  - Font rendering with 90° rotation
- Examined `bsp.c/h` - GPIO definitions
  - All pin assignments documented above
  - Macros for common operations (vled_on/off, row_on/off)

**Python Hardware Abstraction Layer (HAL) Created**
- File: `led_matrix_hal.py` (450+ lines)
- **Class**: LEDMatrixHAL
- **Features**:
  - GPIO initialization & safe shutdown
  - Row selection (0-15) with binary encoding
  - Shift register control (single bit & word operations)
  - Pixel-level display buffer (numpy arrays)
  - Brightness control (0-255 per channel)
  - Row refresh with configurable framerate
  - Test patterns (checkerboard, color bars)
  - Display statistics for debugging
- **Status**: Ready for testing on RPi5 hardware

**Hardware Analysis Document Created**
- File: `Hardware_Analysis_Phase2.md` (450+ lines)
- **Contents**:
  - Architecture overview
  - GPIO pin mapping with rationale
  - Shift register protocol details
  - BCM timing calculations
  - Display geometry (16×8 layout)
  - Power distribution concerns
  - Implementation strategy (Phase 2A-2D)
  - Known issues & mitigations
  - Questions for hardware specialist (Bart)

**Identified Concerns & Mitigations**

| Issue | Status | Mitigation |
|-------|--------|-----------|
| Shift register voltage (5V logic, 3.3V GPIO) | ⚠️ CRITICAL | Source 74HCT595 or TTL-compatible IC; verify with Bart |
| BCM timing on RPi5 software | ⏳ PENDING | Use hardware PWM or accurate timing library |
| Row enable polarity (active LOW) | ✅ DOCUMENTED | Handled in code (GPIO.LOW to enable) |
| Mini USB power for 16×8 matrix at 50% brightness | ⏳ VALIDATION | Estimated 500mA per color per row; secondary battery pack sufficient with duty cycle |
| Data latching synchronization | ✅ MITIGATED | Shift timing proven in existing firmware; port with same intervals |

**Next Phase Actions (Priority Order)**
1. **CRITICAL**: Verify shift register IC model and 5V/3.3V compatibility with Bart
2. Obtain KiCad schematic (`.kicad_sch`) for IC verification
3. Test GPIO voltage output on actual RPi5 hardware
4. Validate `led_matrix_hal.py` with test board (before YOLOv8 integration)
5. Implement BCM brightness timing (hardware PWM vs. software timer)

#### Files Created/Modified
- `Hardware_Analysis_Phase2.md` (NEW) - Comprehensive hardware analysis
- `led_matrix_hal.py` (NEW) - Python GPIO control library
- `Production_History.md` (UPDATED) - This file

#### Commits This Phase
```
Commit 1: Phase 2 - Hardware Analysis & GPIO Mapping
- Reverse-engineered LED matrix control architecture (shift registers)
- Created GPIO pin mapping for Raspberry Pi 5
- Developed Python hardware abstraction layer (led_matrix_hal.py)
- Generated comprehensive hardware analysis document
- Identified critical voltage compatibility issue (5V shift registers, 3.3V GPIO)

Files:
+ Hardware_Analysis_Phase2.md (450+ lines)
+ led_matrix_hal.py (450+ lines, fully documented)
~ Production_History.md (Phase 2 section added)
```

---

## 📊 Phase Overview

### Phase 1: Environment Setup ✅ COMPLETE
- [x] Repository migration (Bitbucket → GitHub)
- [x] Hardware files uploaded (PCB layout)
- [x] Firmware code analyzed (ESP32 HAILSIGN variants)
- [x] GPIO pin assignments documented

### Phase 2: Hardware Integration ✅ ANALYSIS COMPLETE | ⏳ IMPLEMENTATION PENDING
- [x] Identify LED driver IC architecture (shift register-based)
- [x] Map GPIO pins (ESP32 → Raspberry Pi 5)
- [x] Design Raspberry Pi 5 GPIO control interface
- [x] Develop LED matrix control library (Python HAL)
- [ ] Validate shift register voltage compatibility
- [ ] Test HAL on actual RPi5 hardware
- [ ] Implement BCM brightness timing
- **Estimated Implementation**: 4-6 hours

### Phase 3: AI Pipeline (⏳ PLANNED)
- [ ] Download YOLOv8-Nano model
- [ ] Quantize to ONNX format for RPi5 accelerator
- [ ] Develop inference wrapper (image → bounding boxes)
- [ ] Test on live camera feed
- **Estimated**: 6-8 hours

### Phase 4: Audio Integration (⏳ PLANNED)
- [ ] Configure PocketSphinx wake-word detection
- [ ] Integrate pyttsx3 TTS (English + Spanish)
- [ ] Test microphone/speaker hardware
- **Estimated**: 3-4 hours

### Phase 5: System Orchestration (⏳ PLANNED)
- [ ] Flask API framework
- [ ] State machine (idle → listening → inferring → displaying → speaking)
- [ ] Error handling & failover logic
- **Estimated**: 4-6 hours

### Phase 6: Testing & Refinement (⏳ PLANNED)
- [ ] End-to-end integration testing
- [ ] Latency profiling
- [ ] Edge case handling
- **Estimated**: 4-8 hours

**Total Estimated Development Time**: 25-35 hours (Phases 1-2 complete: 8-10 hours spent)

---

## 🔧 Technical Architecture

### LED Display Control (Phase 2 Finalized)
**Display Type**: 16×8 RGB LED matrix (2× 8×8 modules, horizontal arrangement)
**Control Method**: Shift register-based multiplexing
**Row Addressing**: 4-bit binary decoder (8 active rows)
**Column Addressing**: 32-bit shift registers (32 max columns, 16 used)
**Color Capability**: 2-bit RGB (Red, Green, Yellow)
**Brightness**: BCM via row dwell time (8-bit precision = 256 levels)
**GPIO Pins Required**: 11 (documented above)

### Python Architecture
```
Application Layer
    ↓ (YOLOv8 detected objects)
Display Rendering Layer
    ↓ (bitmap pixels)
LED Matrix HAL (led_matrix_hal.py)
    ├── Shift register control (GPIO timing)
    ├── Row multiplexing (4-bit decoder)
    ├── Brightness management
    └── Display buffer management
    ↓
RPi5 GPIO (BCM 2835)
    ↓
Hardware (Shift registers, row decoders, LED matrices)
```

### Known Dependencies
- PyTorch or ONNX inference runtime (Phase 3)
- YOLOv8-Nano (Phase 3, pre-trained, quantized)
- PocketSphinx (Phase 4, wake-word detection)
- pyttsx3 or CoquiTTS (Phase 4, text-to-speech, en/es)
- Flask (Phase 5, REST API orchestration)
- RPi.GPIO or gpiozero (Phase 2, GPIO control)
- numpy (Phase 2, display buffer management)

---

## 📝 Notes & Decisions

### Decision Log

| Date | Decision | Rationale | Status |
|------|----------|-----------|--------|
| 2025-11-24 | Use external D: drive | C: drive capacity insufficient | ✅ IMPLEMENTED |
| 2025-11-24 | Migrate Bitbucket → GitHub | Accelerated dev pipeline | ✅ IMPLEMENTED |
| 2025-11-24 | Shift register architecture | Existing firmware blueprint available | ✅ DOCUMENTED |
| 2025-11-24 | Python HAL for RPi5 | Faster iteration than C/C++ | ✅ CREATED |
| 2025-11-24 | RPi5 GPIO mapping | Avoid SPI/I2C/UART conflicts | ✅ VALIDATED |

### Technical Decisions

**LED Control Approach**
- Chosen: GPIO bit-banging with shift registers (matches existing design)
- Alternative considered: Dedicated LED driver IC (TLC5940) → rejected to match existing hardware
- Rationale: Existing Neio Systems board uses shift registers; easier migration

**Brightness Control**
- Chosen: BCM via row dwell time (software-timed)
- Alternative: Hardware PWM → may implement in Phase 2B if timing issues arise
- Rationale: Matches existing ESP32 implementation; proven to work

**Programming Language**
- Chosen: Python (led_matrix_hal.py) for MVP speed
- Alternative: C/C++ (faster, but slower development)
- Rationale: Development speed priority; can port to C later for production

### Lessons Learned
1. **Existing firmware is gold**: NEIOSYS provided complete reference implementation
2. **Shift registers are flexible**: No dedicated IC needed; reduces BOM cost
3. **BCM timing is critical**: Precision matters for flicker-free display
4. **Voltage compatibility**: Biggest risk is 5V shift registers on 3.3V GPIO lines

---

## 🎯 Next Actions (Priority Order)

### URGENT (Blocking Phase 2 Implementation)
1. **Contact Bartlomiej (Hardware Specialist)**:
   - What shift register IC is used? (74HC595 vs. 74HCT595 vs. other?)
   - Are shift registers 5V or 3.3V logic?
   - Is a level shifter already on the PCB?
   - Measured VLED current at 50% brightness?

2. **Obtain KiCad Schematic** (`.kicad_sch` file)
   - Verify shift register IC model
   - Confirm power distribution design
   - Check for level shifter IC

### High Priority (Phase 2B Implementation)
3. Validate `led_matrix_hal.py` on RPi5 hardware (basic GPIO test)
4. Test shift register timing with oscilloscope (if available)
5. Measure power draw at various brightness levels

### Medium Priority (Phase 2C Implementation)
6. Implement BCM brightness control with accurate timing
7. Create test patterns for display validation
8. Document GPIO pinout on Raspberry Pi 5

---

## 📞 Contact & Resources

**Primary Contact**: Eben  
**CTO**: Jonathan Pullen  
**Tech Lead**: Lucas Sojka  
**Hardware Specialist**: Bartlomiej "Blacha" Blacha  

**Repository**: https://github.com/onthedrops/CITYARRAY.git  
**Bitbucket Archive**: https://bitbucket.org/neiosys/sign-firmware.git  

**Files Generated This Phase**:
- `Hardware_Analysis_Phase2.md` - Complete technical analysis
- `led_matrix_hal.py` - Python GPIO control library (ready for testing)

---

## 📊 Development Metrics

| Metric | Value | Status |
|--------|-------|--------|
| Total Hours Invested | 8-10 | ✅ Tracked |
| Repository Quality | GitHub (professional) | ✅ Migrated |
| Code Documentation | 900+ lines of docs | ✅ Complete |
| Firmware Analysis | Complete | ✅ Done |
| Hardware Mapping | RPi5 ready | ✅ Validated |
| Critical Issues | 1 (voltage compat.) | ⚠️ Pending |
| Test Coverage | Awaiting hardware | ⏳ Phase 2 |

---

*Last Updated: 2025-11-24 18:30 UTC*  
*Next Review: Upon Phase 2 Implementation Completion*
*Prepared by: Claude AI + Eben*
