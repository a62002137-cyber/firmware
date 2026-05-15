#include "core/powerSave.h"

/***************************************************************************************
** Function name: _setup_gpio()
** Description: initial setup for the device (ESP32-S3 + ILI9341 Touch Only)
***************************************************************************************/

void _setup_gpio() {

    // =====================================
    // TFT BACKLIGHT
    // =====================================
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Backlight ON
    delay(100);

    // =====================================
    // BUTTONS - DISABLED (Touch Only)
    // Set pins to INPUT only if they're valid (>= 0)
    // =====================================
    if (UP_BTN >= 0) pinMode(UP_BTN, INPUT_PULLUP);
    if (SEL_BTN >= 0) pinMode(SEL_BTN, INPUT_PULLUP);
    if (DW_BTN >= 0) pinMode(DW_BTN, INPUT_PULLUP);
    if (R_BTN >= 0) pinMode(R_BTN, INPUT_PULLUP);
    if (L_BTN >= 0) pinMode(L_BTN, INPUT_PULLUP);

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
    // I2C / PMU (Optional - disable if not used)
    // =====================================
    #ifdef GROVE_SDA
    Wire.setPins(GROVE_SDA, GROVE_SCL);
    Wire.begin(GROVE_SDA, GROVE_SCL);
    #endif

#ifdef XPOWERS_CHIP_BQ25896
    // PMU code tetap ada tapi conditional
    bool pmu_ret = false;
    pmu_ret = PPM.init(Wire, GROVE_SDA, GROVE_SCL, BQ25896_SLAVE_ADDRESS);
    
    if (pmu_ret) {
        PPM.setSysPowerDownVoltage(3300);
        PPM.setInputCurrentLimit(3250);
        PPM.disableCurrentLimitPin();
        PPM.setChargeTargetVoltage(4208);
        PPM.setPrechargeCurr(64);
        PPM.setChargerConstantCurr(832);
        PPM.enableMeasure(PowersBQ25896::CONTINUOUS);
        PPM.disableOTG();
        PPM.enableCharge();
    }
#endif
}

bool isCharging() {
#ifdef XPOWERS_CHIP_BQ25896
    return PPM.isCharging();
#else
    return false;  // No PMU
#endif
}

int getBattery() {
#ifdef XPOWERS_CHIP_BQ25896
    int voltage = PPM.getBattVoltage();
    int percent = (voltage - 3300) * 100 / (float)(4150 - 3350);
    
    if (percent < 0) return 1;
    if (percent > 100) percent = 100;
    
    if (PPM.isCharging() && percent >= 97) {
        PPM.disableBatLoad();
        percent = 95;
    }
    
    if (PPM.isChargeDone()) {
        percent = 100;
    }
    
    return percent;
#else
    return 100;  // No battery monitoring, return 100%
#endif
}

/*********************************************************************
** Function: setBrightness
**********************************************************************/
void _setBrightness(uint8_t brightval) {

    pinMode(TFT_BL, OUTPUT);

    if (brightval == 0) {
        digitalWrite(TFT_BL, LOW);  // OFF
    } else {
        digitalWrite(TFT_BL, HIGH); // ON (full brightness)
        // Kalau mau PWM untuk brightness control, perlu setup LEDC
    }
}

/*********************************************************************
** Function: InputHandler
** Description: Handle input from buttons (if enabled) or touch
**********************************************************************/
void InputHandler(void) {

    static unsigned long tm = 0;

    if (millis() - tm < 200 && !LongPress)
        return;

    // Read buttons ONLY if they're enabled (>= 0)
    bool _u = (UP_BTN >= 0) ? digitalRead(UP_BTN) : BTN_ACT;
    bool _d = (DW_BTN >= 0) ? digitalRead(DW_BTN) : BTN_ACT;
    bool _l = (L_BTN >= 0) ? digitalRead(L_BTN) : BTN_ACT;
    bool _r = (R_BTN >= 0) ? digitalRead(R_BTN) : BTN_ACT;
    bool _s = (SEL_BTN >= 0) ? digitalRead(SEL_BTN) : BTN_ACT;

    // Check if any button is pressed (LOW = pressed with INPUT_PULLUP)
    bool anyPressed = false;
    if (UP_BTN >= 0 && !_u) anyPressed = true;
    if (DW_BTN >= 0 && !_d) anyPressed = true;
    if (L_BTN >= 0 && !_l) anyPressed = true;
    if (R_BTN >= 0 && !_r) anyPressed = true;
    if (SEL_BTN >= 0 && !_s) anyPressed = true;

    if (anyPressed) {
        tm = millis();

        if (!wakeUpScreen())
            AnyKeyPress = true;
        else
            return;
    }

    // Process individual buttons (only if enabled)
    if (L_BTN >= 0 && !_l) {
        PrevPress = true;
    }

    if (R_BTN >= 0 && !_r) {
        NextPress = true;
    }

    if (UP_BTN >= 0 && !_u) {
        UpPress = true;
        PrevPagePress = true;
    }

    if (DW_BTN >= 0 && !_d) {
        DownPress = true;
        NextPagePress = true;
    }

    if (SEL_BTN >= 0 && !_s) {
        SelPress = true;
    }

    // ESC: L + R together
    if ((L_BTN >= 0 && !_l) && (R_BTN >= 0 && !_r)) {
        EscPress = true;
        NextPress = false;
        PrevPress = false;
    }
}

/*********************************************************************
** Function: powerOff
**********************************************************************/
void powerOff() {

    // Wake up from SEL_BTN if available
    if (SEL_BTN >= 0) {
        esp_sleep_enable_ext0_wakeup((gpio_num_t)SEL_BTN, BTN_ACT);
    } else {
        // Fallback: wake from touch IRQ if available
        #ifdef TOUCH_IRQ
        esp_sleep_enable_ext0_wakeup((gpio_num_t)TOUCH_IRQ, LOW);
        #endif
    }

    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** Description: Check for L+R button hold to power off
**********************************************************************/
void checkReboot() {

    // Only check if both buttons exist
    if (L_BTN < 0 || R_BTN < 0) return;

    int countDown = 0;

    if (digitalRead(L_BTN) == BTN_ACT && digitalRead(R_BTN) == BTN_ACT) {

        uint32_t time_count = millis();

        while (digitalRead(L_BTN) == BTN_ACT && digitalRead(R_BTN) == BTN_ACT) {

            if (millis() - time_count > 500) {

                if (countDown == 0) {
                    int textWidth = tft.textWidth("PWR OFF IN 3/3", 1);
                    tft.fillRect(
                        tftWidth / 2 - textWidth / 2,
                        7,
                        textWidth,
                        18,
                        bruceConfig.bgColor
                    );
                }

                tft.setTextSize(1);
                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);

                countDown = (millis() - time_count) / 1000 + 1;

                if (countDown < 4) {
                    tft.drawCentreString(
                        "PWR OFF IN " + String(countDown) + "/3",
                        tftWidth / 2,
                        12,
                        1
                    );
                } else {
                    tft.fillScreen(bruceConfig.bgColor);

                    while (digitalRead(L_BTN) == BTN_ACT || digitalRead(R_BTN) == BTN_ACT);
                    
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
