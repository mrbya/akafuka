#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LED 2

char ssid[] = "Crazzy";
char pass[] = "crazzynko";
int status = WL_IDLE_STATUS;

IPAddress broker(192, 167, 1, 20);
const int port = 1883;
const char* topic = "akafuka/blink";
const char* user = "";
const char* mpass = "";

WiFiClient espClient;
PubSubClient client(espClient);

void blink() {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Siz!");
    blink();
    blink();
}

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(ssid, pass);

    while(WiFi.status() != WL_CONNECTED) {
        blink();
        delay(5000);
    }

    Serial.println("Connected!");

    client.setServer(broker, port);
    client.setCallback(callback);
}

void loop() {
    while (true) {
        if (!client.connected()) {
            blink();
            while (!client.connected()) {
                Serial.println("mqtt reconnect...");
                Serial.print(client.state());
                if (client.connect("arduinoClient")) {
                    client.subscribe(topic);
                }
                delay(5000);
            }
        }
        client.loop();
    }
}

