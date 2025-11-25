# CITYARRAY MVP - Session Handoff Brief

**Current Date**: 2025-11-24  
**Session Status**: Phase 2 Complete, Ready for Phase 3  
**Last Updated**: 18:30 UTC  

---

## 🎯 Quick Context (30-second summary)

**Project**: CITYARRAY - AI-powered LED information platform for crisis communication  
**Founder/Lead**: Eben (26 years LA radio)  
**Team**: Jonathan (CTO, 40y system integration), Lucas (Tech Lead, 15y), Bartlomiej "Blacha" (Hardware, 11y PCB design)  
**Goal**: MVP combining Raspberry Pi 5, OV5647 camera, 16×8 RGB LED matrix, wake-word voice, and YOLOv8 object detection

---

## ✅ Completed Work (Sessions 1-2)

### Phase 1: Environment Setup (✅ COMPLETE)
- Migrated `sign-firmware` repo from Bitbucket to GitHub
- Repository: `https://github.com/onthedrops/CITYARRAY.git`
- Analyzed 8 existing Neio Systems HAILSIGN firmware variants
- Uploaded PCB layout (`CA_PCB.kicad_pcb`)

### Phase 2: Hardware Analysis (✅ COMPLETE)
- **Discovered**: LED control uses shift registers (NOT dedicated LED driver IC)
- **Shift Protocol**: 3-wire SPI-like (DATA_RED, DATA_GREEN, DATA_CLK, DATA_LAT)
- **Row Multiplexing**: 4-bit binary decoder (16 rows addressable, 8 active)
- **Color**: 2-bit RGB (Red, Green) → Yellow capable
- **Display**: 16×8 pixels (2× 8×8 matrices, horizontal arrangement)

#### GPIO Mapping (ESP32 → Raspberry Pi 5)
```
VLED_EN=24    DATA_RED=17   DATA_GREEN=27  DATA_CLK=23   DATA_LAT=25
ROW_A=6       ROW_B=12      ROW_C=16       ROW_D=20      ROW_EN=21
(11 GPIO pins total)
```

#### Key Deliverables
1. **Hardware_Analysis_Phase2.md** - Complete technical specification
2. **led_matrix_hal.py** - Production-ready Python library (450+ lines)
   - Shift register control
   - Row multiplexing
   - Pixel-level display buffer (numpy)
   - Brightness management
   - Test patterns included
3. **Production_History.md** - Comprehensive development log

---

## ⚠️ Critical Blockers

### VOLTAGE COMPATIBILITY (BLOCKS Phase 2 Implementation)
- **Issue**: Shift registers likely 5V logic, RPi5 GPIO is 3.3V
- **Risk**: GPIO pin damage if no level shifter
- **Blocker**: Need to verify with Bartlomiej:
  1. What shift register IC? (74HC595 vs 74HCT595 vs other?)
  2. Is it 5V or 3.3V logic?
  3. Is level shifter already on PCB?
  4. Measured VLED current at 50% brightness?

**Action**: Contact Bartlomiej before proceeding with Phase 2 implementation

---

## 📋 Remaining Phases (25-35 hours total)

### Phase 3: AI Pipeline (⏳ NEXT)
**Duration**: 6-8 hours  
**Scope**:
- Download YOLOv8-Nano model
- Quantize to ONNX for RPi5 accelerator
- Camera integration (OV5647)
- Inference wrapper (image → bounding boxes)
- Latency profiling (<100ms target)

### Phase 4: Audio Integration
**Duration**: 3-4 hours  
**Scope**:
- PocketSphinx wake-word detection
- pyttsx3 TTS (English + Spanish)
- Microphone/speaker testing

### Phase 5: System Orchestration
**Duration**: 4-6 hours  
**Scope**:
- Flask REST API
- State machine (idle → listening → inferring → displaying → speaking)
- Error handling

### Phase 6: Testing & Refinement
**Duration**: 4-8 hours  
**Scope**:
- End-to-end integration
- Latency measurement
- Edge case handling

---

## 🛠️ Tech Stack

### Hardware
- Raspberry Pi 5 + M.2 HAT+ AI accelerator
- OV5647 camera (adjustable lens)
- 2× 8×8 RGB LED matrices (mini USB powered)
- Speaker/microphone module
- Portable 2" × 21" stand-mounted enclosure

### Software
- **ML**: YOLOv8-Nano + ONNX Runtime
- **Audio**: PocketSphinx (wake-word) + pyttsx3 (TTS)
- **GPIO**: led_matrix_hal.py (custom Python library)
- **API**: Flask REST
- **Language**: Python (MVP) + C/C++ (later production)

### Repository Structure
```
CITYARRAY/
├── Production_History.md              ← Development log (newest first)
├── Hardware_Analysis_Phase2.md        ← Complete hardware spec
├── led_matrix_hal.py                  ← GPIO control library (ready to test)
├── NEIOSYS_HAILSIGN-ProdFirmware-SP/  ← Reference implementation (ESP32)
├── CA_PCB.kicad_pcb                   ← PCB layout
└── [future phases...]
```

---

## 📊 Current Status Dashboard

| Phase | Status | Effort | Blocker |
|-------|--------|--------|---------|
| 1: Setup | ✅ 100% | 2-3h | None |
| 2: Hardware | ✅ Analysis 100% | 4-6h analysis | Voltage compat (Bart) |
| 3: AI | ⏳ Planned | 6-8h | Phase 2 resolution |
| 4: Audio | ⏳ Planned | 3-4h | Phase 3 complete |
| 5: Orchestration | ⏳ Planned | 4-6h | Phase 4 complete |
| 6: Testing | ⏳ Planned | 4-8h | Phase 5 complete |

---

## 🎯 Immediate Next Actions (Priority Order)

### URGENT (Before Phase 2 Implementation)
1. **Contact Bartlomiej** → Voltage compatibility questions
2. **Obtain KiCad schematic** (`.kicad_sch`) → Verify shift register IC

### HIGH (Phase 3 Start)
3. Once voltage resolved → Validate `led_matrix_hal.py` on RPi5 hardware
4. Begin Phase 3: YOLOv8-Nano setup
5. Test camera module initialization

### MEDIUM (Parallel)
6. Research PocketSphinx + pyttsx3 setup
7. Plan Flask API architecture
8. Document test procedures

---

## 💾 Repository & Files

**GitHub**: `https://github.com/onthedrops/CITYARRAY.git`

**Key Branches**: `main` (all production work)

**Files to Know About**:
- `Production_History.md` - **Start here for context**
- `Hardware_Analysis_Phase2.md` - **Hardware specifications**
- `led_matrix_hal.py` - **Production-ready GPIO library**
- `NEIOSYS_HAILSIGN-ProdFirmware-SP/` - **Reference firmware** (ESP32)

---

## 🔐 Context Preservation for Opus 4.5

**How to maximize continuity**:
1. Load this document first (gives quick context)
2. Reference Production_History.md for detailed progress
3. Examine Hardware_Analysis_Phase2.md for technical details
4. Review led_matrix_hal.py to understand GPIO implementation
5. Check userMemories section (Claude's stored knowledge about you & project)

**Key Context to Maintain**:
- Display is 16×8 (horizontal, NOT vertical)
- 11 GPIO pins required (clearly mapped)
- CRITICAL: Voltage compatibility issue pending with Bartlomiej
- No dedicated LED driver IC (shift registers only)
- Python HAL ready for testing (don't rewrite!)
- YOLOv8-Nano for object detection (5-10 classes)
- Wake-word triggered inference only (battery life priority)
- <100ms latency target

---

## 🚀 Ready to Proceed?

**Next Opus 4.5 Session Should**:
1. Load this handoff document
2. Check Production_History.md for detailed context
3. Wait for Bartlomiej's voltage compatibility answers
4. Once resolved → Proceed with Phase 2 implementation validation
5. Then start Phase 3: AI/Vision pipeline

**Timeline**: MVP complete in 25-35 hours total (8-10 hours invested so far)

---

*Handoff prepared: 2025-11-24 18:35 UTC*  
*For: Claude Opus 4.5 (next session)*  
*By: Claude Haiku + Eben*
