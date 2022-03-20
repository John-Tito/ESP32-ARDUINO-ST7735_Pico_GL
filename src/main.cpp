
/* gears.c */

/*
 * 3-D gear wheels.  This program is in the public domain.
 *
 * Brian Paul
 */
#include <Arduino.h>
#include <gear.hpp>
hw_timer_t *tim1 = NULL;
uint32_t tim1_IRQ_count = 0, cnt_cpy = 0;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR tim1Interrupt()
{
    portENTER_CRITICAL_ISR(&timerMux);
    tim1_IRQ_count++;
    portEXIT_CRITICAL_ISR(&timerMux);
}
void setup()
{
    delay(5000);
    Serial.begin(115200);
    Serial.println("Start");
    int ret = ui_init(&scene_init, &scene_update);
    if (ret != 0)
    {
        Serial.printf("error: ui_init unexpected end with code %d!\n", ret);
    }
    pinMode(SENSOR_VP, INPUT);
    pinMode(SENSOR_VN, INPUT);
    tim1 = timerBegin(0, 80, true); //80MHZ, ESP32主频80MHz --> 1M
    timerAttachInterrupt(tim1, tim1Interrupt, true);
    timerAlarmWrite(tim1, 10000, true); //100
    timerAlarmEnable(tim1);
    Serial.println("Start looping!");
}

void loop()
{
    if (cnt_cpy != tim1_IRQ_count)
    {
        cnt_cpy = tim1_IRQ_count;
        if (0 == cnt_cpy % 100)
            Serial.printf("\nVp:%d,Vn%d\n", analogRead(SENSOR_VP), analogRead(SENSOR_VN));
        if (0 == cnt_cpy % 3)
        {
            ui_loop();
        }
    }
}