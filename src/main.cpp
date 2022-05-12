#include <Arduino.h>

void setup(void)
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("hello");
}

void loop(void)
{
    delay(1000);
    Serial.println(millis());
}
