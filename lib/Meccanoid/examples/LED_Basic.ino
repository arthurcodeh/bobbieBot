#include <Meccanoid.h>

Meccanoid mecca(10);   // pin data = 10

void setup() {
    mecca.begin();
}

void loop() {
    auto led = mecca.getLED();
    led.setColor(255, 0, 0);
    delay(1000);
    led.setColor(0, 255, 0);
    delay(1000);
    led.setColor(0, 0, 255);
    delay(1000);
}
