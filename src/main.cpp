#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <esp_adc_cal.h>
#include <PubSubClient.h>

// Wifi and MQTT credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* mqtt_server = MQTT_SERVER;

// Wifi and MQTT instances
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
float battery_voltage = 0.0;

// TFT Pins has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
// #define TFT_MOSI            19
// #define TFT_SCLK            18
// #define TFT_CS              5
// #define TFT_DC              16
// #define TFT_RST             23
// #define TFT_BL              4   // Display backlight control pin

#define BUTTON_1            35
#define BUTTON_2            0
#define ADC_EN              14  //ADC_EN is the ADC detection enable port
#define ADC_PIN             34

int vref = 1100;
bool lightState = false;

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

void espDelay(int ms)
{
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_light_sleep_start();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void readVoltage()
{
  uint16_t v = analogRead(ADC_PIN);
  battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
}

void showVoltage()
{
  String voltage = "Voltage:" + String(battery_voltage) + "V";
  Serial.println(voltage);
  if(!lightState)
  {
    tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
    tft.fillScreen(TFT_DARKGREEN);
  }
  else
  {
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.fillScreen(TFT_GREEN);
  }
  tft.setTextDatum(MC_DATUM);
  tft.drawString(voltage,  tft.width() / 2, tft.height() / 2 );
}

void callback(char* topic, byte* payload, unsigned int length) {
  String payloadstr = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    payloadstr += (char)payload[i];
    // Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(payloadstr);
  if(strcmp(topic, "led") == 0)
  {
    if(payloadstr == "true")
    {
      lightState = true;
    }
    else
    {
      lightState = false;
    }
    showVoltage();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "Conectado");
      // ... and resubscribe
      client.subscribe("led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(ADC_EN, OUTPUT);
    digitalWrite(ADC_EN, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.fillScreen(TFT_RED);
    espDelay(1000);
    tft.fillScreen(TFT_BLUE);
    espDelay(1000);
    tft.fillScreen(TFT_GREEN);
    espDelay(1000);

    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(digitalRead(0) == LOW)
  {
    delay(250);
    client.publish("ledesp", "true");
  }
  else if(digitalRead(35) == LOW)
  {
    delay(250);
    client.publish("ledesp", "false");
  }
  if(millis() - lastMsg > 5000)
  {
    readVoltage();
    showVoltage();
    lastMsg = millis();
    client.publish("voltage", String(battery_voltage).c_str());
  }
}

