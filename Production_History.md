# CITYARRAY MVP - Production History

**Project**: CITYARRAY - Agentic AI Public Information Platform  
**Lead**: Eben (26 years LA radio experience, Founder)  
**Team**: Jonathan Pullen (CTO), Lucas Sojka (Tech Lead), Bartlomiej "Blacha" Blacha (Hardware)  
**Mission**: Solving society's most critical vulnerability—communications during crisis  
**Status**: MVP Development (Phase 1: Environment Setup)

---

## 📋 Executive Summary

CITYARRAY is an autonomous AI-powered information platform combining computer vision, LED displays, and voice interaction for deployment in smart cities, transportation hubs, and emergency response scenarios. This document tracks all development activities, hardware discoveries, firmware integration, and architectural decisions from inception through production deployment.

**Key Metrics**:
- Target Latency: <100ms decision latency
- Language Support: 15+ languages (MVP: English + Spanish)
- Object Detection: 5-10 object classes (YOLO-based)
- Hardware: Raspberry Pi 5 + AI Accelerator + OV5647 Camera + 8×8 RGB LED Matrix (×2)
- Display: 2" × 21" portable stand-mounted enclosure (16 pixels wide × 8 tall)

---

## 🚀 Development Timeline

### 2025-11-24 | 17:45 UTC | Phase 1: Environment Setup & Repository Migration

**Status**: ✅ COMPLETE (Pending Hardware Analysis)

#### Summary
Successfully migrated `sign-firmware` repository from Bitbucket to GitHub. Identified existing ESP32-based firmware architecture from Neio Systems AMBERARRAY-PCB REV. 2.0. Confirmed presence of display driver code (`display.c`, `display.h`) and GPIO mappings (`bcm.c`, `bcm.h`). Initiated hardware reverse-engineering to identify LED driver IC and Raspberry Pi 5 integration requirements.

#### Detailed Progress

**Git Migration**
- Cloned `sign-firmware` from Bitbucket to external drive D: (C: drive capacity exhausted)
- Migrated to GitHub: `https://github.com/onthedrops/CITYARRAY.git`
- Repository structure: Contains 8 Neio Systems HAILSIGN firmware variants (production, testing, config, font testing)
- Encountered: Case-sensitive filename collision in `/fonts` directory (HUNTER.TTF vs HUNTER.ttf) - Windows filesystem automatically resolved

**Hardware Files Uploaded**
- Added `CA_PCB.kicad_pcb` (AMBERARRAY PCB Layout - 4 layer board, 0.062" thick FR4)
- Pending: `CA_PCB.kicad_sch` (Schematic - needed to identify LED driver IC and GPIO pin assignments)

**Firmware Analysis Initiated**
- Discovered existing codebase is **Arduino-compatible ESP32 firmware** (not Raspberry Pi native)
- Key files identified:
  - `display.c/h` - LED matrix control logic (26,783 bytes - substantial implementation)
  - `bcm.c/h` - GPIO pin mappings and hardware abstraction layer
  - `bsp.c/h` - Board support package (hardware initialization)
  - Multiple font headers (Arial, Impact, Hunter, etc.) - suggests existing text rendering capability
  - `config.c/h` - Configuration management
  - `command.c/h` - Command/protocol handler

**Next Steps**
1. Extract LED driver IC identification from `display.h` and `bcm.h`
2. Map GPIO pin assignments (existing ESP32 → new Raspberry Pi 5)
3. Analyze control protocol (SPI, I2C, PWM, or parallel)
4. Validate power distribution feasibility (mini USB battery pack + secondary Pi power pack)

#### Blockers & Resolutions
| Issue | Status | Resolution |
|-------|--------|-----------|
| C: drive space exhausted during initial clone | ✅ RESOLVED | Relocated repository to external D: drive |
| KiCad schematic file missing from repo | ⏳ PENDING | Searching Neio Systems project files or reverse-engineering from firmware |

#### Files Modified/Added
- `Production_History.md` (NEW) - This file
- `CA_PCB.kicad_pcb` (NEW) - Added to repo

#### Commits This Session
```
Commit 1: Add CITYARRAY PCB layout
- Added CA PCB.kicad_pcb (4-layer AMBERARRAY board design)
- File size: 3.2 MB
- Date: 2025-11-24 17:45 UTC
```

---

## 📊 Phase Overview

### Phase 1: Environment Setup ✅
- [x] Repository migration (Bitbucket → GitHub)
- [x] Hardware files uploaded (PCB layout)
- [ ] Identify LED driver IC from firmware/schematic
- [ ] Map GPIO pins (ESP32 → Raspberry Pi 5)
- [ ] Validate power architecture

### Phase 2: Hardware Integration (⏳ NEXT)
- [ ] Reverse-engineer LED matrix driver circuit
- [ ] Design Raspberry Pi 5 GPIO control interface
- [ ] Develop LED matrix control library (Python)
- [ ] Validate camera module initialization
- **Estimated**: 4-6 hours

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

**Total Estimated Development Time**: 25-35 hours

---

## 🔧 Technical Architecture (In Progress)

### Current Understanding
- **Display**: 2× 8×8 RGB LED matrices, horizontally arranged (16 wide × 8 tall pixels)
- **Camera**: OV5647 (adjustable lens)
- **Compute**: Raspberry Pi 5 with M.2 HAT+ AI accelerator
- **Power**: Mini USB battery pack (LEDs), separate battery pack (Pi)
- **Control**: Existing firmware suggests dedicated LED driver IC (TBC)

### Known Dependencies
- PyTorch or ONNX inference runtime
- YOLOv8-Nano (pre-trained, quantized for edge)
- PocketSphinx (wake-word detection)
- pyttsx3 or CoquiTTS (text-to-speech, en/es)
- Flask (REST API orchestration)

---

## 📝 Notes & Decisions

### Decision Log

| Date | Decision | Rationale | Status |
|------|----------|-----------|--------|
| 2025-11-24 | Use external D: drive for repo | C: drive capacity insufficient for MVP development | ✅ IMPLEMENTED |
| 2025-11-24 | Migrate Bitbucket → GitHub | Accelerated development pipeline, better team collaboration | ✅ IMPLEMENTED |
| 2025-11-24 | Start with hardware reverse-engineering | Existing Neio Systems firmware holds blueprint for LED driver architecture | ⏳ IN PROGRESS |

### Lessons Learned
- Existing NEIOSYS firmware is substantial (26KB+ display.c) → significant prior art available
- ESP32-based architecture may require GPIO pin mapping exercise but core logic is reusable
- Font rendering already implemented → can focus on AI/detection integration

---

## 🎯 Next Actions (Priority Order)

1. **URGENT**: Extract `display.h` and `bcm.h` contents → identify LED driver IC and control protocol
2. Locate KiCad schematic (`.kicad_sch`) → validate power distribution for mini USB approach
3. Begin Phase 2: Hardware integration (GPIO library for Raspberry Pi 5)
4. Parallel: Set up YOLOv8-Nano quantization pipeline

---

## 📞 Contact & Resources

**Primary Contact**: Eben  
**Technical Leads**: Jonathan Pullen (CTO), Lucas Sojka (Tech Lead)  
**Hardware Specialist**: Bartlomiej "Blacha" Blacha  

**Repository**: https://github.com/onthedrops/CITYARRAY.git  
**Bitbucket Archive**: https://bitbucket.org/neiosys/sign-firmware.git  

---

*Last Updated: 2025-11-24 17:45 UTC*  
*Next Review: Upon Phase 2 completion*
