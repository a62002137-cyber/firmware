#include "core/powerSave.h"

#ifdef XPOWERS_CHIP_BQ25896
#include <Wire.h>
#include <XPowersLib.h>
XPowersPPM PPM;
#endif

void _setup_gpio() {

    // =========================
    // TFT BACKLIGHT FIX
    // =========================
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    delay(100);

    // =========================
    // BUTTONS
    // =========================
    pinMode(UP_BTN, INPUT_PULLUP);
    pinMode(SEL_BTN, INPUT_PULLUP);
    pinMode(DW_BTN, INPUT_PULLUP);
    pinMode(R_BTN, INPUT_PULLUP);
    pinMode(L_BTN, INPUT_PULLUP);

    // =========================
    // DISABLE UNUSED SPI DEVICES
    // =========================
    // pinMode(CC1101_SS_PIN, OUTPUT);
    // pinMode(NRF24_SS_PIN, OUTPUT);

    // digitalWrite(CC1101_SS_PIN, HIGH);
    // digitalWrite(NRF24_SS_PIN, HIGH);

    // =========================
    // CONFIG
    // =========================
    bruceConfigPins.rfModule = CC1101_SPI_MODULE;
    bruceConfigPins.irRx = RXLED;

    // =========================
    // I2C / PMU
    // =========================
    Wire.setPins(GROVE_SDA, GROVE_SCL);
    Wire.begin(GROVE_SDA, GROVE_SCL);

#ifdef XPOWERS_CHIP_BQ25896
    bool pmu_ret = false;

    pmu_ret = PPM.init(Wire, GROVE_SDA, GROVE_SCL, BQ25896_SLAVE_ADDRESS);

    if (pmu_ret) {

        PPM.setSysPowerDownVoltage(3300);
        PPM.setInputCurrentLimit(3250);

        Serial.printf("getInputCurrentLimit: %d mA\n",
                      PPM.getInputCurrentLimit());

        PPM.disableCurrentLimitPin();

        PPM.setChargeTargetVoltage(4208);
        PPM.setPrechargeCurr(64);

        PPM.setChargerConstantCurr(832);

        Serial.printf("getChargerConstantCurr: %d mA\n",
                      PPM.getChargerConstantCurr());

        PPM.enableMeasure(PowersBQ25896::CONTINUOUS);

        PPM.disableOTG();
        PPM.enableCharge();
    }
#endif
}

bool isCharging() {
    return PPM.isCharging();
}

int getBattery() {

    int voltage = PPM.getBattVoltage();

    int percent =
        (voltage - 3300) * 100 / (float)(4150 - 3350);

    if (percent < 0) return 1;

    if (percent > 100)
        percent = 100;

    if (PPM.isCharging() && percent >= 97) {

        PPM.disableBatLoad();
        percent = 95;
    }

    if (PPM.isChargeDone()) {
        percent = 100;
    }

    return percent;
}

// ===================================
// BRIGHTNESS FIX
// ===================================
void _setBrightness(uint8_t brightval) {

    pinMode(TFT_BL, OUTPUT);

    if (brightval == 0) {
        digitalWrite(TFT_BL, LOW);
    }
    else {
        digitalWrite(TFT_BL, HIGH);
    }
}
