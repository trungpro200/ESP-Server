
/*************************************************************
  Download latest ERa library here:
    https://github.com/eoh-jsc/era-lib/releases/latest
    https://www.arduino.cc/reference/en/libraries/era
    https://registry.platformio.org/libraries/eoh-ltd/ERa/installation

    ERa website:                https://e-ra.io
    ERa blog:                   https://iotasia.org
    ERa forum:                  https://forum.eoh.io
    Follow us:                  https://www.fb.com/EoHPlatform
 *************************************************************/

// Enable debug console
// Set CORE_DEBUG_LEVEL = 3 first
// #define ERA_DEBUG

#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"

// You should get Auth Token in the ERa App or ERa Dashboard
#define ERA_AUTH_TOKEN "5195f866-e7fe-40e0-9c42-adde595e5e16"

#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
#include <DHT.h>

#define RainSensor 34
#define FireSensor 33
#define Buzzer 17

const char ssid[] = "Mai Hong";
const char pass[] = "buihong2005";

ERaTimer timer;
DHT dht(5, DHT11);


/* This function print uptime every second */
void timerEvent() {
    ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);
}

enum rgbLed {red = 14, green = 12, blue = 27};

void Warning(){
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);
}

void setup() {
    /* Setup debug console */
    Serial.begin(9600);
    Serial.println("\nConnecting to Wifi...");

    try
    {    
        ERa.begin(ssid, pass);
        if (!ERa.connected()){
            throw ssid;
        }
    }
    catch(char* s[])
    {
        Serial.println("Unable to connect");
    }
    

    pinMode(35, INPUT);

    pinMode(Buzzer, OUTPUT);

    pinMode(14, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(27, OUTPUT);

    //Led
    analogWrite(14, 255);
    analogWrite(12, 255);
    analogWrite(27, 255);
    /* Setup timer called function every second */
    timer.setInterval(1000L, timerEvent);
    Serial.println("Connected");

    ERa.run();
    timer.run();

    digitalWrite(Buzzer, HIGH);
    delay(1000);
    digitalWrite(Buzzer, LOW);
    
}

void loop() {
    if (analogRead(FireSensor)<2000){
        Warning();
        return;
    }
    ERa.run();
    timer.run();
//Rain
    ERa.virtualWrite(V0, analogRead(RainSensor));
//Fire
    ERa.virtualWrite(V1, analogRead(FireSensor));
//DHT
    ERa.virtualWrite(V3, dht.readHumidity());
    ERa.virtualWrite(V4, dht.readTemperature());
}

