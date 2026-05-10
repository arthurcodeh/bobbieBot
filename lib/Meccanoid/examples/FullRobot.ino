#include <Meccanoid.h>

Meccanoid mecca(10);
MeccanoidServo servo1 = mecca.getServo(1);

void setup() {
    mecca.begin();
}

void loop() {
    // LED orange
    auto led = mecca.getLED();
    led.setColor(255, 165, 0);

    servo1.move(45);
    delay(500);
    servo1.move(135);
    delay(500);
}
