#include <Meccanoid.h>

Meccanoid mecca(10);
MeccanoidServo servo1 = mecca.getServo(1);

void setup() {
    mecca.begin();
}

void loop() {
    servo1.move(0);
    delay(1000);
    servo1.move(90);
    delay(1000);
    servo1.move(180);
    delay(1000);
}
