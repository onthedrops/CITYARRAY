# CITYARRAY MVP - Phase 2: Hardware Analysis & GPIO Mapping

**Document Date**: 2025-11-24  
**Phase**: 2 - Hardware Integration  
**Status**: Analysis Complete, Ready for Implementation  

---

## 🔍 Hardware Architecture Discovery

### LED Display Control Architecture

**Type**: Shift Register + Row Multiplexing (NOT dedicated LED driver IC)
**Color Support**: 2-bit (Red, Green) → Red + Green + Yellow capability
**Row Multiplexing**: 4-bit binary selection (16 rows max)
**Column Control**: Serial shift registers (parallel output to LED matrix)

---

## 📍 GPIO Pin Mapping (ESP32 Current Implementation)

### Current ESP32 Pinout
```
VLED_EN_PIN       = GPIO 23   (LED voltage enable, active HIGH)
DATA_RED_PIN      = GPIO 4    (Red shift register data input, serial)
DATA_GREEN_PIN    = GPIO 5    (Green shift register data input, serial)
DATA_CLK_PIN      = GPIO 18   (Shift register clock)
DATA_LAT_PIN      = GPIO 19   (Shift register latch/strobe, active HIGH)
ROW_A_PIN         = GPIO 32   (Row select bit A)
ROW_B_PIN         = GPIO 33   (Row select bit B)
ROW_C_PIN         = GPIO 25   (Row select bit C)
ROW_D_PIN         = GPIO 2    (Row select bit D, hardware revision dependent)
ROW_EN_PIN        = GPIO 27   (Row enable, active LOW)
DATA_RETURN_PIN   = GPIO 35   (Data feedback/return, unused in current version)
```

**Total GPIO Pins Required**: 11

### Row Selection Logic (4-bit binary decoder)
```
Row Index = (ROW_D << 3) | (ROW_C << 2) | (ROW_B << 1) | ROW_A

Examples:
Row 0:  A=0, B=0, C=0, D=0 → 0000
Row 7:  A=1, B=1, C=1, D=0 → 0111
Row 15: A=1, B=1, C=1, D=1 → 1111
```

---

## 🎯 Raspberry Pi 5 GPIO Mapping Strategy

### Proposed RPi5 GPIO Assignment

| Function | ESP32 GPIO | RPi5 GPIO (BCM) | Purpose |
|----------|-----------|-----------------|---------|
| VLED_EN | 23 | 24 | Enable LED power |
| DATA_RED | 4 | 17 | Red shift register data |
| DATA_GREEN | 5 | 27 | Green shift register data |
| DATA_CLK | 18 | 23 | Shift register clock |
| DATA_LAT | 19 | 25 | Shift register latch |
| ROW_A | 32 | 6 | Row select bit A |
| ROW_B | 33 | 12 | Row select bit B |
| ROW_C | 25 | 16 | Row select bit C |
| ROW_D | 2 | 20 | Row select bit D |
| ROW_EN | 27 | 21 | Row enable (active LOW) |
| DATA_RETURN | 35 | 26 | Data return (optional) |

**Rationale**: 
- Selected RPi5 GPIO pins to avoid SPI, I2C, and UART conflicts
- GPIO 27 on RPi5 is not available, so ROW_EN → 21
- GPIO 2 and GPIO 3 reserved for I2C (camera, audio modules)

---

## 🔧 Shift Register Architecture Analysis

### Control Protocol: SPI-like Serial Interface (3-wire)

**Shift Method**: LSB-first, serial-in parallel-out (SIPO)

**Timing Sequence** (for each column pixel):
```
1. Set DATA_RED and DATA_GREEN lines (1 bit each)
2. Pulse DATA_CLK (rising edge latches data into register)
3. Repeat for next pixel
4. After all column pixels shifted, pulse DATA_LAT to enable output
```

**Shift Register Count**: Estimated 2-4 cascaded 74HC595 (or similar)
- Each 74HC595 = 8-bit shift register
- 2× for Red channel, 2× for Green channel (16 columns per row)
- OR 1× per color with parallel load capability

**Current Implementation Uses**:
- `column_shift_single(rBit, gBit)` - shifts 1 bit
- `column_shift(rData, gData, nBits)` - shifts up to 32 bits
- `row_data_latch()` - latches to output
- `shift_registers_clear()` - resets registers

---

## ⚡ Power Distribution & Timing

### Power Considerations

**VLED Power Rail**: Powers LED matrices directly
- 5V typical for RGB LED matrix
- Peak current: 8×8 matrix @ full brightness ≈ 500mA per color per row
- Mini USB battery pack (secondary): 2A capacity sufficient with duty cycle

**GPIO Logic Power**: 3.3V (RPi5 native)
- Shift register logic: 5V typically (compatibility check required)
- Possible level shifter needed (3.3V → 5V for shift registers)

**Critical Check**: 
- Verify shift register IC voltage requirements
- Ensure DATA_RED, DATA_GREEN, DATA_CLK can drive 5V shift registers from 3.3V GPIO
- May require 74HCT595 (5V tolerant input) instead of 74HC595 (strict 5V)

### BCM Brightness Timing (ESP32 Implementation)

**Current Approach**: Hardware timer interrupt-driven
```
Timer Prescale: 240 (1 MHz clock from 240 MHz CPU)
First BCM bit: 16 ticks = 16 μs
Subsequent bits: Double timing each step (16, 32, 64, 128, ...)
BRIGHTNESS_PRECISION_BITS: Likely 8 bits (256 levels)
Row refresh cycle: 16 rows × (16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048) μs ≈ 61.4 ms
Frame rate: ~16 FPS (acceptable for LED displays)
```

**For Raspberry Pi 5**: Options
1. **Hardware Timer**: RPi5 has PWM hardware, use `/dev/mem` for GPIO level triggering
2. **Software Timing**: Use Python `time.sleep()` with tight loops (less reliable)
3. **Hybrid**: Use PWM for row enable, software for column updates

---

## 📊 Display Geometry (16×8 Configuration)

### Physical Layout
```
Horizontal arrangement: 2× 8×8 matrices = 16 pixels wide × 8 pixels tall
Row multiplexing: 8 physical rows (3-bit selector would suffice)
BUT: Hardware uses 4-bit selector (16-row capable) → only 8 rows active

Logical arrangement (per firmware comments):
A1 A2 A3 A4  A5 A6 A7 A8   (Row 0 - leftmost 8 pixels)
B1 B2 B3 B4  B5 B6 B7 B8   (Row 1)
C1 C2 C3 C4  C5 C6 C7 C8   (Row 2)
D1 D2 D3 D4  D5 D6 D7 D8   (Row 3)
E1 E2 E3 E4  E5 E6 E7 E8   (Row 4)
F1 F2 F3 F4  F5 F6 F7 F8   (Row 5)
G1 G2 G3 G4  G5 G6 G7 G8   (Row 6)
H1 H2 H3 H4  H5 H6 H7 H8   (Row 7 - rightmost 8 pixels)

Total: 16 columns × 8 rows = 128 pixels
Color capability: 2-bit RGB (R+G channels, no Blue independent)
```

---

## 🚀 Implementation Strategy for RPi5

### Phase 2A: Hardware Abstraction Layer (HAL)

**Python GPIO Library**: `RPi.GPIO` or `gpiozero` (higher-level)

```python
# Hardware control class
class LEDMatrixHAL:
    def __init__(self):
        # Initialize GPIO pins per mapping above
        self.pins = {
            'VLED_EN': 24,
            'DATA_RED': 17,
            'DATA_GREEN': 27,
            'DATA_CLK': 23,
            'DATA_LAT': 25,
            'ROW_A': 6,
            'ROW_B': 12,
            'ROW_C': 16,
            'ROW_D': 20,
            'ROW_EN': 21,
        }
    
    def shift_out(self, red_bit, green_bit):
        """Shift single bit into shift registers"""
        # Set data lines
        # Pulse clock
        # Latch if needed
    
    def select_row(self, row_num):
        """Select active row (0-15)"""
        # Set ROW_A, ROW_B, ROW_C, ROW_D per binary encoding
    
    def enable_row(self):
        """Enable row output (ROW_EN LOW)"""
    
    def disable_row(self):
        """Disable row output (ROW_EN HIGH)"""
    
    def clear_shift_registers(self):
        """Reset shift registers"""
```

### Phase 2B: BCM Brightness Control

**Strategy**: Use RPi5 PWM or software timing
- Implement row dwell time based on brightness bit
- For wake-word-triggered inference: only update on demand (no continuous refresh)

### Phase 2C: Integration with YOLOv8 Output

```python
# Example flow
1. Detect object with YOLOv8 (e.g., "person" at position x=100, y=50)
2. Render object label/icon to 16×8 bitmap
3. Push bitmap to LED matrix via HAL
4. Set brightness levels per color
5. Trigger TTS: "Person detected"
```

---

## ⚠️ Known Issues & Mitigations

| Issue | Impact | Mitigation |
|-------|--------|-----------|
| Shift register voltage (5V vs 3.3V) | GPIO damage risk | Use level shifter IC or 74HCT595 |
| BCM timing on software (RPi5) | Brightness flickering | Use hardware PWM or accurate sleep |
| Row enable polarity (active LOW) | Inverted logic | Invert ROW_EN pin in code |
| Data latching synchronization | Display glitches | Ensure latch pulse timing correct |
| Power draw on mini USB battery | Brownout risk | Monitor VLED voltage, add capacitor |

---

## 📋 Next Steps (Phase 2 Implementation)

**2A. Hardware Validation** (2 hours)
- [ ] Verify shift register IC model from PCB schematic
- [ ] Check 5V/3.3V compatibility, source level shifter if needed
- [ ] Test GPIO voltage output on test RPi5
- [ ] Validate row selector logic with multimeter

**2B. Python GPIO Library Development** (3 hours)
- [ ] Create `led_matrix_hal.py` with shift register control
- [ ] Implement row selection and enable/disable
- [ ] Test each GPIO individually
- [ ] Create unit tests for HAL functions

**2C. BCM Brightness Implementation** (2 hours)
- [ ] Implement timer-based row refresh (15-20 FPS target)
- [ ] Test brightness levels (0-255 per color)
- [ ] Validate color mixing (Red + Green = Yellow)

**2D. Integration Testing** (1 hour)
- [ ] Display test patterns (checkerboard, color bars)
- [ ] Verify 16×8 pixel mapping
- [ ] Test text rendering (simple ASCII)

**Total Phase 2 Estimate**: 8-10 hours

---

## 🔗 Related Files

- `bcm.c` - ESP32 timer/BCM implementation (reference)
- `bsp.c` - ESP32 GPIO control (reference)
- `display.c` - Row/column rendering logic (to port)
- `CA_PCB.kicad_pcb` - PCB layout (for IC identification)

---

## 📞 Questions for Hardware Specialist (Bart)

1. What shift register IC is used? (74HC595, 74HCT595, or other?)
2. Are shift registers 5V or 3.3V logic?
3. Is a level shifter already on the PCB, or do we need to add one?
4. What is the measured VLED current at 50% brightness?
5. Do we need the DATA_RETURN_PIN for anything (currently unused)?

---

*Document prepared by: Claude AI + Eben*  
*Ready for Phase 2 Hardware Integration Implementation*
