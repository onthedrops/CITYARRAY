"""
CITYARRAY MVP - LED Matrix Hardware Abstraction Layer (HAL)
Raspberry Pi 5 GPIO Control for 16×8 RGB LED Matrix

Shift Register Architecture:
- Parallel data lines: DATA_RED, DATA_GREEN (serial-in)
- Clock: DATA_CLK (shift register clock)
- Latch: DATA_LAT (output enable strobe)
- Row Select: ROW_A, ROW_B, ROW_C, ROW_D (4-bit binary decoder)
- Row Enable: ROW_EN (active LOW)
- LED Power: VLED_EN (active HIGH)

Color Support: 2-bit (Red, Green) → Red, Green, Yellow (R+G)
"""

import RPi.GPIO as GPIO
import time
from typing import Tuple, List
import numpy as np

class LEDMatrixHAL:
    """
    Hardware Abstraction Layer for CITYARRAY LED Matrix Control
    
    Manages GPIO pins, shift register timing, row multiplexing,
    and BCM brightness control via timer-based row dwell.
    """
    
    # GPIO Pin Configuration (BCM numbering for Raspberry Pi 5)
    GPIO_CONFIG = {
        'VLED_EN': 24,      # LED voltage enable (HIGH = on)
        'DATA_RED': 17,     # Red channel shift register data
        'DATA_GREEN': 27,   # Green channel shift register data
        'DATA_CLK': 23,     # Shift register clock (rising edge)
        'DATA_LAT': 25,     # Shift register latch (HIGH pulse)
        'ROW_A': 6,         # Row select bit 0
        'ROW_B': 12,        # Row select bit 1
        'ROW_C': 16,        # Row select bit 2
        'ROW_D': 20,        # Row select bit 3
        'ROW_EN': 21,       # Row enable (LOW = active)
    }
    
    # Display Parameters
    DISPLAY_WIDTH = 16     # pixels (2× 8×8 matrices)
    DISPLAY_HEIGHT = 8    # pixels
    NUM_ROWS = 8          # active rows
    
    # BCM Brightness Timing (microseconds)
    FIRST_BCM_BIT_TIME = 16       # 16 μs for LSB
    BCM_PRECISION_BITS = 8         # 256 brightness levels
    
    def __init__(self, use_board_numbering: bool = False):
        """
        Initialize LED Matrix HAL
        
        Args:
            use_board_numbering: If True, use BOARD numbering. 
                                If False (default), use BCM numbering.
        """
        self.pin_mode = GPIO.BOARD if use_board_numbering else GPIO.BCM
        GPIO.setmode(self.pin_mode)
        
        self.pins = self.GPIO_CONFIG.copy()
        self._is_initialized = False
        self._current_row = 0
        self._current_bcm_bit = 0
        
        # Display buffers (16 wide × 8 tall, 2 channels)
        self.red_buffer = np.zeros((self.NUM_ROWS, self.DISPLAY_WIDTH), dtype=np.uint8)
        self.green_buffer = np.zeros((self.NUM_ROWS, self.DISPLAY_WIDTH), dtype=np.uint8)
        
        # Brightness control
        self.red_brightness = 255
        self.green_brightness = 255
        
    def initialize(self):
        """
        Initialize all GPIO pins and set to safe state
        """
        try:
            # Set all pins as outputs
            for pin_name, pin_num in self.pins.items():
                GPIO.setup(pin_num, GPIO.OUT)
            
            # Set safe initial states
            GPIO.output(self.pins['VLED_EN'], GPIO.LOW)    # LED power OFF
            GPIO.output(self.pins['ROW_EN'], GPIO.HIGH)    # Row disabled
            GPIO.output(self.pins['DATA_RED'], GPIO.LOW)
            GPIO.output(self.pins['DATA_GREEN'], GPIO.LOW)
            GPIO.output(self.pins['DATA_CLK'], GPIO.LOW)
            GPIO.output(self.pins['DATA_LAT'], GPIO.LOW)
            
            # Clear row selector
            for row_bit in ['ROW_A', 'ROW_B', 'ROW_C', 'ROW_D']:
                GPIO.output(self.pins[row_bit], GPIO.LOW)
            
            self._is_initialized = True
            print("[LED HAL] Initialized successfully")
            
        except Exception as e:
            print(f"[LED HAL ERROR] Initialization failed: {e}")
            raise
    
    def cleanup(self):
        """
        Safely shutdown GPIO and cleanup
        """
        try:
            GPIO.output(self.pins['VLED_EN'], GPIO.LOW)
            GPIO.output(self.pins['ROW_EN'], GPIO.HIGH)
            GPIO.cleanup()
            self._is_initialized = False
            print("[LED HAL] Cleanup complete")
        except Exception as e:
            print(f"[LED HAL WARNING] Cleanup error: {e}")
    
    def enable_led_power(self, enable: bool = True):
        """
        Control main LED power supply
        
        Args:
            enable: True = LED power ON, False = OFF
        """
        state = GPIO.HIGH if enable else GPIO.LOW
        GPIO.output(self.pins['VLED_EN'], state)
    
    def select_row(self, row: int):
        """
        Select which row to display (0-15, though only 0-7 active)
        
        Uses 4-bit binary decoder:
        row = (ROW_D << 3) | (ROW_C << 2) | (ROW_B << 1) | ROW_A
        
        Args:
            row: Row number (0-15)
        """
        if not (0 <= row <= 15):
            raise ValueError(f"Row must be 0-15, got {row}")
        
        # Set individual row select bits
        GPIO.output(self.pins['ROW_A'], GPIO.HIGH if (row & 0x01) else GPIO.LOW)
        GPIO.output(self.pins['ROW_B'], GPIO.HIGH if (row & 0x02) else GPIO.LOW)
        GPIO.output(self.pins['ROW_C'], GPIO.HIGH if (row & 0x04) else GPIO.LOW)
        GPIO.output(self.pins['ROW_D'], GPIO.HIGH if (row & 0x08) else GPIO.LOW)
        
        self._current_row = row
    
    def enable_row(self, enable: bool = True):
        """
        Enable or disable current row output
        
        Args:
            enable: True = enable row, False = disable
        """
        # ROW_EN is active LOW
        state = GPIO.LOW if enable else GPIO.HIGH
        GPIO.output(self.pins['ROW_EN'], state)
    
    def shift_out_bit(self, red_bit: bool, green_bit: bool):
        """
        Shift a single bit into the shift registers
        
        NOTE: LED turns ON when bit is LOW (inverted logic)
        
        Args:
            red_bit: 0 (LED on) or 1 (LED off) for red channel
            green_bit: 0 (LED on) or 1 (LED off) for green channel
        """
        # Set data lines
        GPIO.output(self.pins['DATA_RED'], GPIO.HIGH if red_bit else GPIO.LOW)
        GPIO.output(self.pins['DATA_GREEN'], GPIO.HIGH if green_bit else GPIO.LOW)
        
        # Pulse clock (rising edge)
        GPIO.output(self.pins['DATA_CLK'], GPIO.LOW)
        time.sleep(1e-6)  # 1 microsecond setup time
        GPIO.output(self.pins['DATA_CLK'], GPIO.HIGH)
        time.sleep(1e-6)  # 1 microsecond hold time
        GPIO.output(self.pins['DATA_CLK'], GPIO.LOW)
    
    def shift_out_word(self, red_word: int, green_word: int, num_bits: int = 8):
        """
        Shift multiple bits into the shift registers (LSB first)
        
        Args:
            red_word: Red channel data word
            green_word: Green channel data word
            num_bits: Number of bits to shift (max 32)
        """
        if num_bits > 32:
            raise ValueError("Maximum 32 bits per word")
        
        for i in range(num_bits):
            red_bit = (red_word >> i) & 0x01
            green_bit = (green_word >> i) & 0x01
            self.shift_out_bit(red_bit, green_bit)
    
    def latch_data(self):
        """
        Latch shifted data to shift register outputs
        
        Pulse DATA_LAT HIGH to enable parallel outputs
        """
        GPIO.output(self.pins['DATA_LAT'], GPIO.LOW)
        time.sleep(1e-6)
        GPIO.output(self.pins['DATA_LAT'], GPIO.HIGH)
        time.sleep(1e-6)
        GPIO.output(self.pins['DATA_LAT'], GPIO.LOW)
    
    def clear_shift_registers(self):
        """
        Clear all shift register data (all bits to 0)
        
        Shift out 128 zero bits (16 columns × 8 rows worth)
        """
        for _ in range(128):
            self.shift_out_bit(0, 0)
        self.latch_data()
    
    def set_pixel(self, row: int, col: int, red: bool, green: bool):
        """
        Set a single pixel in the display buffer
        
        Args:
            row: Row number (0-7)
            col: Column number (0-15)
            red: True to turn on red channel
            green: True to turn on green channel
        """
        if not (0 <= row < self.DISPLAY_HEIGHT and 0 <= col < self.DISPLAY_WIDTH):
            raise ValueError(f"Invalid pixel position: ({row}, {col})")
        
        # Inverted logic: 1 = LED off, 0 = LED on
        self.red_buffer[row, col] = 0 if red else 1
        self.green_buffer[row, col] = 0 if green else 1
    
    def clear_display(self):
        """
        Clear entire display buffer (all LEDs off)
        """
        self.red_buffer.fill(1)  # 1 = LED off
        self.green_buffer.fill(1)
    
    def set_brightness(self, red: int, green: int):
        """
        Set overall brightness levels (0-255)
        
        Args:
            red: Red brightness (0=off, 255=max)
            green: Green brightness (0=off, 255=max)
        """
        self.red_brightness = max(0, min(255, red))
        self.green_brightness = max(0, min(255, green))
    
    def display_row(self, row: int, dwell_time_ms: float = 1.0):
        """
        Display a single row and hold for dwell_time
        
        This is the core row refresh function called at ~60+ Hz
        for flicker-free display.
        
        Args:
            row: Row number (0-7)
            dwell_time_ms: Time to display row (milliseconds)
        """
        if not self._is_initialized:
            raise RuntimeError("HAL not initialized. Call initialize() first.")
        
        # Select and enable the row
        self.select_row(row)
        
        # Shift out all column data for this row
        for col in range(self.DISPLAY_WIDTH):
            red_bit = self.red_buffer[row, col]
            green_bit = self.green_buffer[row, col]
            self.shift_out_bit(red_bit, green_bit)
        
        # Latch data to outputs
        self.latch_data()
        
        # Enable the row
        self.enable_row(True)
        
        # Display for dwell time
        time.sleep(dwell_time_ms / 1000.0)
        
        # Disable row
        self.enable_row(False)
    
    def refresh_display(self, framerate_hz: int = 60):
        """
        Refresh entire display with specified frame rate
        
        Cycles through all 8 rows sequentially
        
        Args:
            framerate_hz: Target refresh rate (default 60 Hz)
        """
        dwell_ms = 1000.0 / (framerate_hz * self.NUM_ROWS)
        
        for row in range(self.NUM_ROWS):
            self.display_row(row, dwell_ms)
    
    def test_pattern_checkerboard(self):
        """
        Display alternating checkerboard pattern (red channel)
        """
        self.clear_display()
        for row in range(self.DISPLAY_HEIGHT):
            for col in range(self.DISPLAY_WIDTH):
                if (row + col) % 2 == 0:
                    self.set_pixel(row, col, red=True, green=False)
    
    def test_pattern_color_bars(self):
        """
        Display color bar pattern: Red | Yellow | Green
        """
        self.clear_display()
        for row in range(self.DISPLAY_HEIGHT):
            # Red (columns 0-5)
            for col in range(6):
                self.set_pixel(row, col, red=True, green=False)
            # Yellow (columns 6-10)
            for col in range(6, 11):
                self.set_pixel(row, col, red=True, green=True)
            # Green (columns 11-15)
            for col in range(11, 16):
                self.set_pixel(row, col, red=False, green=True)
    
    def get_display_stats(self) -> dict:
        """
        Get display statistics for debugging
        
        Returns:
            Dictionary with display parameters and current state
        """
        return {
            'width': self.DISPLAY_WIDTH,
            'height': self.DISPLAY_HEIGHT,
            'num_rows': self.NUM_ROWS,
            'current_row': self._current_row,
            'red_brightness': self.red_brightness,
            'green_brightness': self.green_brightness,
            'is_initialized': self._is_initialized,
            'pixels_on_red': int(np.sum(self.red_buffer == 0)),
            'pixels_on_green': int(np.sum(self.green_buffer == 0)),
        }


# ============================================================================
# Main Test/Demo
# ============================================================================

if __name__ == '__main__':
    """
    Test the LED Matrix HAL with basic patterns
    """
    
    hal = LEDMatrixHAL()
    
    try:
        print("[TEST] Initializing LED Matrix HAL...")
        hal.initialize()
        
        print("[TEST] Enabling LED power...")
        hal.enable_led_power(True)
        time.sleep(0.5)
        
        print("[TEST] Checkerboard pattern (5 seconds)...")
        hal.test_pattern_checkerboard()
        for _ in range(5):
            hal.refresh_display(framerate_hz=60)
        
        print("[TEST] Color bars pattern (5 seconds)...")
        hal.test_pattern_color_bars()
        for _ in range(5):
            hal.refresh_display(framerate_hz=60)
        
        print("[TEST] Clear display...")
        hal.clear_display()
        hal.refresh_display(framerate_hz=60)
        
        print("[TEST] Display stats:", hal.get_display_stats())
        
        print("[TEST] Complete!")
        
    except KeyboardInterrupt:
        print("\n[TEST] Interrupted by user")
    
    except Exception as e:
        print(f"[TEST ERROR] {e}")
    
    finally:
        print("[TEST] Cleaning up...")
        hal.cleanup()
