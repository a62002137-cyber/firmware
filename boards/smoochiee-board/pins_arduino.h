#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

// =====================================
// UART (Serial / USB)
// =====================================
static const uint8_t TX = 43;
static const uint8_t RX = 44;

// =====================================
// I2C Bus (Shared: PMU, IO Expander, etc.)
// =====================================
static const uint8_t SDA = 8;
static const uint8_t SCL = 9;

// =====================================
// SPI Bus (Shared: TFT, Touch, CC1101, NRF24, SD)
// =====================================
static const uint8_t SS = 5;      // Default: TFT_CS
static const uint8_t MOSI = 21;   // Shared data out
static const uint8_t MISO = 19;   // Shared data in
static const uint8_t SCK = 18;    // Shared clock

// =====================================
// Serial Configuration
// =====================================
#define SERIAL_RX RX
#define SERIAL_TX TX
#define BAD_RX SERIAL_RX
#define BAD_TX SERIAL_TX
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX
#define USB_as_HID 1

// =====================================
// BUTTONS - DISABLED (Touch Only)
// =====================================
#define BTN_ALIAS "\"TOUCH\""
// #define HAS_5_BUTTONS  // Commented: using touch screen only

#define SEL_BTN -1
#define UP_BTN  -1
#define DW_BTN  -1
#define R_BTN   -1
#define L_BTN   -1
#define BTN_ACT LOW

// =====================================
// LED Indicators
// =====================================
#define RXLED -1
#define TXLED -1
#define LED_ON HIGH
#define LED_OFF LOW

// =====================================
// CC1101 (Sub-GHz RF Module) - ENABLED
// =====================================
#define USE_CC1101_VIA_SPI
#define CC1101_GDO0_PIN 6     // Interrupt 1
#define CC1101_GDO2_PIN 7     // Interrupt 2
#define CC1101_SS_PIN 3       // Unique CS
#define CC1101_MOSI_PIN MOSI  // Shared SPI
#define CC1101_SCK_PIN SCK
#define CC1101_MISO_PIN MISO

// =====================================
// NRF24 (2.4GHz RF Module) - ENABLED
// =====================================
#define USE_NRF24_VIA_SPI
#define NRF24_CE_PIN 11       // Chip Enable
#define NRF24_SS_PIN 10       // Unique CS
#define NRF24_MOSI_PIN MOSI   // Shared SPI
#define NRF24_SCK_PIN SCK
#define NRF24_MISO_PIN MISO

// =====================================
// Display & Screen Config
// =====================================
#define FP 1
#define FM 2
#define FG 3

#define HAS_SCREEN 1
#define ROTATION 1            // 0=0°, 1=90°, 2=180°, 3=270°
#define MINBRIGHT (uint8_t)1

// =====================================
//  TFT_eSPI Configuration - ILI9341 (PIN TFT TIDAK DIUBAH)
// =====================================
#define USER_SETUP_LOADED 1
#define ILI9341_DRIVER 1      // Driver chip sesuai modul loe

// Color order (jika warna kebalik nanti, tinggal uncomment TFT_BGR)
#define TFT_RGB_ORDER TFT_RGB
// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

#define TFT_WIDTH  240
#define TFT_HEIGHT 320        // Resolusi asli ILI9341 2.4"

#define TFT_BACKLIGHT_ON HIGH
#define TFT_BL 4              // GPIO 4 (Backlight)

// 🔒 PIN MAPPING TFT (SESUAI WIRING AWAL ISSUE)
#define TFT_RST  17           // 🔒 GPIO 17
#define TFT_DC   16           // 🔒 GPIO 16
#define TFT_CS    5           // 🔒 GPIO 5

// 🔒 SPI PINS (SHARED DENGAN TOUCH/RF/SD)
#define TFT_MISO 19           // 🔒 GPIO 19
#define TFT_MOSI 21           // 🔒 GPIO 21
#define TFT_SCLK 18           // 🔒 GPIO 18

// =====================================
// Touch Screen - ENABLED
// =====================================
#define TOUCH_CS 14           // 🔒 GPIO 14 (Terpisah dari TFT_CS)
#define HAS_TOUCH 1
// TOUCH_IRQ menggunakan GPIO 35 (input-only, otomatis terdeteksi library)

#define SMOOTH_FONT 1
#define SPI_FREQUENCY 40000000        // 40 MHz untuk display
#define SPI_READ_FREQUENCY 20000000   // 20 MHz untuk read
#define SPI_TOUCH_FREQUENCY 2500000   // 2.5 MHz untuk touch

// =====================================
// SD Card - ENABLED
// =====================================
#define SDCARD_CS 46          // Unique CS (strapping pin, aman setelah boot)
#define SDCARD_SCK SCK        // Shared SPI
#define SDCARD_MISO MISO
#define SDCARD_MOSI MOSI

// =====================================
// I2C Pins (Grove/Shared Bus)
// =====================================
#define GROVE_SDA 8
#define GROVE_SCL 9

// =====================================
// Alternative SPI Bus (untuk periferal lain)
// =====================================
#define SPI_SCK_PIN  13
#define SPI_MOSI_PIN 15
#define SPI_MISO_PIN 11
#define SPI_SS_PIN   2

// =====================================
// RGB LED (WS2812B) - ENABLED
// =====================================
#define HAS_RGB_LED 1
#define RGB_LED 45
#define LED_TYPE WS2812B
#define LED_ORDER GRB
#define LED_TYPE_IS_RGBW 0
#define LED_COUNT 16
#define LED_COLOR_STEP 15

// =====================================
// Power Management (PMU BQ25896) - ENABLED
// =====================================
#define XPOWERS_CHIP_BQ25896
// #define USE_BOOST              // ❌ DIHAPUS: udah ada di build_flags (.ini)
// I2C address: 0x6B (handled by library)

// =====================================
// Microphone (I2S) - ENABLED
// =====================================
#define PIN_CLK  13   // I2S Clock
#define PIN_DATA 33   // I2S Data (input-capable pin)
#define PIN_WS   34   // I2S Word Select (input-capable pin)

// =====================================
// IO Expander AW9523 - ENABLED
// Menggunakan I2C bus (SDA=8, SCL=9), address 0x58
// Pin di bawah adalah LOGICAL PIN di chip expander, BUKAN GPIO ESP32
// =====================================
// #define USE_IO_EXPANDER        // ❌ DIHAPUS: udah ada di build_flags (.ini)
// #define IO_EXPANDER_AW9523     // ❌ DIHAPUS: udah ada di build_flags (.ini)
#define IO_EXP_GPS  0
#define IO_EXP_MIC  1
#define IO_EXP_VIBRO 2
#define IO_EXP_CC_RX 3
#define IO_EXP_CC_TX 4

#endif /* Pins_Arduino_h */
