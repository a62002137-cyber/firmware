#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

// ===== ESP32 DevKit V4 (Classic) Pinout =====
static const uint8_t TX = 1;
static const uint8_t RX = 3;
static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

static const uint8_t SS = 5;
static const uint8_t MOSI = 23;
static const uint8_t MISO = 19;
static const uint8_t SCK = 18;

// ===== GPIO Aliases (ESP32 classic max GPIO39) =====
static const uint8_t G0 = 0;   static const uint8_t G1 = 1;
static const uint8_t G2 = 2;   static const uint8_t G3 = 3;
static const uint8_t G4 = 4;   static const uint8_t G5 = 5;
static const uint8_t G6 = 6;   static const uint8_t G7 = 7;
static const uint8_t G8 = 8;   static const uint8_t G9 = 9;
static const uint8_t G10 = 10; static const uint8_t G11 = 11;
static const uint8_t G12 = 12; static const uint8_t G13 = 13;
static const uint8_t G14 = 14; static const uint8_t G15 = 15;
static const uint8_t G16 = 16; static const uint8_t G17 = 17;
static const uint8_t G18 = 18; static const uint8_t G19 = 19;
static const uint8_t G21 = 21; static const uint8_t G22 = 22;
static const uint8_t G23 = 23; static const uint8_t G25 = 25;
static const uint8_t G26 = 26; static const uint8_t G27 = 27;
static const uint8_t G32 = 32; static const uint8_t G33 = 33;
static const uint8_t G34 = 34; static const uint8_t G35 = 35;
static const uint8_t G36 = 36; static const uint8_t G39 = 39;

// ===== Bruce Firmware Specific =====
#define HAS_SCREEN 1
#define ROTATION 1
#define MINBRIGHT 100

// ===== ST7735 128x160 =====
#define TFT_CS    17
#define TFT_DC    16
#define TFT_RST   -1
#define TFT_BL    32
#define TFT_MOSI  23
#define TFT_SCLK  18
#define TFT_MISO  -1          // ⚠️ Write-only!

// ===== SD Card (Shared SPI) =====
#define SDCARD_CS    4
#define SDCARD_SCK   18
#define SDCARD_MISO  19
#define SDCARD_MOSI  23

// ===== CC1101 (Shared SPI) =====
#define CC1101_GDO0_PIN 14
#define CC1101_SS_PIN   5
#define CC1101_MOSI_PIN 23
#define CC1101_SCK_PIN  18
#define CC1101_MISO_PIN 19
#define CC1101_GDO2_PIN -1

// ===== NRF24L01 (Shared SPI) =====
#define NRF24_CE_PIN    2
#define NRF24_SS_PIN    15
#define NRF24_MOSI_PIN  23
#define NRF24_SCK_PIN   18
#define NRF24_MISO_PIN  19

// ===== RFID RC522 (Shared SPI) =====
#define RFID_SS_PIN     27
#define RFID_RST_PIN    13
#define RFID_IRQ_PIN    12
#define RFID_MOSI_PIN   23
#define RFID_SCK_PIN    18
#define RFID_MISO_PIN   19

// ===== BadUSB CH9329 (Optional - Disable if not used) =====
#define BAD_TX -1    // ⚠️ Set -1 kalau nggak pakai modul CH9329
#define BAD_RX -1    // ⚠️ Set -1 kalau nggak pakai modul CH9329

// ===== GPS Serial (UART1) =====
//#define GPS_SERIAL_TX 22
//#define GPS_SERIAL_RX 21

// ===== Buttons (5-Way Navigation) =====

#define HAS_BTN 1
#define HAS_5_BUTTONS
#define BTN_ALIAS "\"Ok\""

// Semua pin stabil + ada internal pullup
#define SEL_BTN 21
#define UP_BTN 22
#define DW_BTN 25
#define R_BTN 26
#define L_BTN 33

#define BTN_ACT LOW

#define BTN_ACT LOW             // Active-low (button ke GND saat ditekan)

// ===== I2C / Serial / GPS =====
#define GROVE_SDA 33
#define GROVE_SCL 26
#define GPS_SERIAL_TX 22
#define GPS_SERIAL_RX 21

// ===== SPI Global =====
#define SPI_SCK_PIN  18
#define SPI_MISO_PIN 19
#define SPI_MOSI_PIN 23
#define SPI_SS_PIN   5

// ===== Font Size untuk ST7735 128x160 =====
#define FM 1        // Font Medium: 1=kecil, 2=default, 3=besar
#define FP 1        // Font Small: 1=paling kecil
#define LW 6        // Letter Width multiplier (opsional)
#define LH 8        // Letter Height multiplier (opsional)

#endif
