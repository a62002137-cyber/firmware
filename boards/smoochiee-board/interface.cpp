#include "core/powerSave.h"

/***************************************************************************************
** Function name: _setup_gpio()
** Description: initial setup for the device
***************************************************************************************/

#ifdef XPOWERS_CHIP_BQ25896
#include <Wire.h>
#include <XPowersLib.h>
XPowersPPM PPM;
#endif

void _setup_gpio() {

    // =====================================
    // TFT BACKLIGHT FIX
    // =====================================
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    delay(100);

    // =====================================
    // BUTTONS
    // =====================================
    pinMode(UP_BTN, INPUT_PULLUP);
    pinMode(SEL_BTN, INPUT_PULLUP);
    pinMode(DW_BTN, INPUT_PULLUP);
    pinMode(R_BTN, INPUT_PULLUP);
    pinMode(L_BTN, INPUT_PULLUP);

    // =====================================
    // OPTIONAL SPI DEVICES
    // Disabled to avoid SPI conflicts
    // =====================================
#ifdef CC1101_SS_PIN
    pinMode(CC1101_SS_PIN, OUTPUT);
    digitalWrite(CC1101_SS_PIN, HIGH);
#endif

#ifdef NRF24_SS_PIN
    pinMode(NRF24_SS_PIN, OUTPUT);
    digitalWrite(NRF24_SS_PIN, HIGH);
#endif

    // =====================================
    // CONFIG
    // =====================================
    bruceConfigPins.rfModule = CC1101_SPI_MODULE;
    bruceConfigPins.irRx = RXLED;

    // =====================================
    // I2C / PMU
    // =====================================
    Wire.setPins(GROVE_SDA, GROVE_SCL);
    Wire.begin(GROVE_SDA, GROVE_SCL);

#ifdef XPOWERS_CHIP_BQ25896

    bool pmu_ret = false;

    pmu_ret = PPM.init(
        Wire,
        GROVE_SDA,
        GROVE_SCL,
        BQ25896_SLAVE_ADDRESS
    );

    if (pmu_ret) {

        PPM.setSysPowerDownVoltage(3300);

        PPM.setInputCurrentLimit(3250);

        Serial.printf(
            "getInputCurrentLimit: %d mA\n",
            PPM.getInputCurrentLimit()
        );

        PPM.disableCurrentLimitPin();

        PPM.setChargeTargetVoltage(4208);

        PPM.setPrechargeCurr(64);

        PPM.setChargerConstantCurr(832);

        Serial.printf(
            "getChargerConstantCurr: %d mA\n",
            PPM.getChargerConstantCurr()
        );

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

    if (percent < 0)
        return 1;

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

/*********************************************************************
** Function: setBrightness
**********************************************************************/
void _setBrightness(uint8_t brightval) {

    pinMode(TFT_BL, OUTPUT);

    if (brightval == 0) {

        digitalWrite(TFT_BL, LOW);

    } else {

        digitalWrite(TFT_BL, HIGH);
    }
}

/*********************************************************************
** Function: InputHandler
**********************************************************************/
void InputHandler(void) {

    static unsigned long tm = 0;

    if (millis() - tm < 200 && !LongPress)
        return;

    bool _u = digitalRead(UP_BTN);
    bool _d = digitalRead(DW_BTN);
    bool _l = digitalRead(L_BTN);
    bool _r = digitalRead(R_BTN);
    bool _s = digitalRead(SEL_BTN);

    if (!_s || !_u || !_d || !_r || !_l) {

        tm = millis();

        if (!wakeUpScreen())
            AnyKeyPress = true;
        else
            return;
    }

    if (!_l) {
        PrevPress = true;
    }

    if (!_r) {
        NextPress = true;
    }

    if (!_u) {

        UpPress = true;

        PrevPagePress = true;
    }

    if (!_d) {

        DownPress = true;

        NextPagePress = true;
    }

    if (!_s) {
        SelPress = true;
    }

    if (!_l && !_r) {

        EscPress = true;

        NextPress = false;

        PrevPress = false;
    }
}

/*********************************************************************
** Function: powerOff
**********************************************************************/
void powerOff() {

    esp_sleep_enable_ext0_wakeup(
        (gpio_num_t)SEL_BTN,
        BTN_ACT
    );

    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
**********************************************************************/
void checkReboot() {

    int countDown = 0;

    if (
        digitalRead(L_BTN) == BTN_ACT &&
        digitalRead(R_BTN) == BTN_ACT
    ) {

        uint32_t time_count = millis();

        while (
            digitalRead(L_BTN) == BTN_ACT &&
            digitalRead(R_BTN) == BTN_ACT
        ) {

            if (millis() - time_count > 500) {

                if (countDown == 0) {

                    int textWidth =
                        tft.textWidth("PWR OFF IN 3/3", 1);

                    tft.fillRect(
                        tftWidth / 2 - textWidth / 2,
                        7,
                        textWidth,
                        18,
                        bruceConfig.bgColor
                    );
                }

                tft.setTextSize(1);

                tft.setTextColor(
                    bruceConfig.priColor,
                    bruceConfig.bgColor
                );

                countDown =
                    (millis() - time_count) / 1000 + 1;

                if (countDown < 4) {

                    tft.drawCentreString(
                        "PWR OFF IN " +
                        String(countDown) +
                        "/3",
                        tftWidth / 2,
                        12,
                        1
                    );

                } else {

                    tft.fillScreen(
                        bruceConfig.bgColor
                    );

                    while (
                        digitalRead(L_BTN) == BTN_ACT ||
                        digitalRead(R_BTN) == BTN_ACT
                    );

                    delay(200);

                    powerOff();
                }

                delay(10);
            }
        }

        delay(30);

        if (millis() - time_count > 500) {

            tft.fillRect(
                60,
                12,
                tftWidth - 60,
                tft.fontHeight(1),
                bruceConfig.bgColor
            );

            drawStatusBar();
        }
    }
}
