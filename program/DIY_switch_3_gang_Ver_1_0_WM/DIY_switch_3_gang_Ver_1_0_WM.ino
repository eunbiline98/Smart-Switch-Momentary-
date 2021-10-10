#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <Ticker.h>

const char *ssid = "LKIBIZ";
const char *password = "lkibiz2019";
const char *MQTT_SERVER = "192.168.10.164";
const char *MQTT_TOPIC = "switch/bedroom/1";

const char *MQTT_CLIENT_ID = "switch_bedroom1";
const char *MQTT_USERNAME = "eunbiline";
const char *MQTT_PASSWORD = "hwangeunbi98";

WiFiClient espClient;
PubSubClient client(espClient);
Ticker ticker;

// Pin Setup
#define relay_1 14
#define relay_2 12
#define btn_1 5
#define btn_2 4

int flag1 = 0;
int flag2 = 0;
int state1 = 0;
int state2 = 0;

int lastMilis = 0;
unsigned long nowMillis = millis();

void tick()
{
  //toggle state
  digitalWrite(2, !digitalRead(2)); // set pin GPIO2 Led Wemos On-board
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup_wifi()
{
  WiFi.mode(WIFI_STA);
  pinMode(2, OUTPUT);
  ticker.attach(0.6, tick);

  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  //
  //  bool res;
  //  res = wm.autoConnect("LKI Smart Button WiFi", "lki2021");

  if (!wm.autoConnect())
  { // SSID from IDChip
    Serial.println("failed to connect");
    //    wm.resetSettings();
    ESP.restart();
    delay(1000);
  }

  Serial.println("connected...");
  ticker.detach();
  digitalWrite(2, LOW);
}

void button_func()
{
  // button1
  int stateBtn_1 = digitalRead(btn_1);
  if (!stateBtn_1 && !flag1)
  {
    if (state1 == 0)
    {
      client.publish(MQTT_TOPIC, "1");
      state1 = 1;
    }
    else
    {
      client.publish(MQTT_TOPIC, "2");
      state1 = 0;
    }
    flag1 = 1;
  }
  else if (stateBtn_1 && flag1)
  {
    flag1 = 0;
  }
  // button2
  int stateBtn_2 = digitalRead(btn_2);
  if (!stateBtn_2 && !flag2)
  {
    if (state2 == 0)
    {
      client.publish(MQTT_TOPIC, "3");
      state2 = 1;
    }
    else
    {
      client.publish(MQTT_TOPIC, "4");
      state2 = 0;
    }
    flag2 = 1;
  }
  else if (stateBtn_2 && flag2)
  {
    flag2 = 0;
  }
  delay(100);
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messageTemp;

  if (String(topic) != MQTT_TOPIC)
  {
    return;
  }

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    if ((char)message[i] != '"')
      messageTemp += (char)message[i];
  }
  // Changes the output state according to the message
  Serial.println("");
  Serial.print("Changing output to ");
  //  Serial.println(messageTemp);
  if (messageTemp == "1")
  {
    Serial.println("Relay_1On");
    digitalWrite(relay_1, HIGH);
  }
  if (messageTemp == "2")
  {
    Serial.println("Relay_1Off");
    digitalWrite(relay_1, LOW);
  }
  if (messageTemp == "3")
  {
    Serial.println("Relay_2On");
    digitalWrite(relay_2, HIGH);
  }
  if (messageTemp == "4")
  {
    Serial.println("Relay_2Off");
    digitalWrite(relay_2, LOW);
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("connected");
      //subscribe
      client.subscribe(MQTT_TOPIC);
    }
    else
    {
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
  // Serial port for debugging purposes
  Serial.begin(115200);
  setup_wifi();

  // MQTT Server Setup
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  pinMode(2, OUTPUT); //WiFi Indicator
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);

  pinMode(btn_1, INPUT);
  pinMode(btn_2, INPUT);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  button_func();
  client.loop();
}
