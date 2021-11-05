#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

// set at text box mqtt config manager
#define MQTT_SERVER "..........."
#define MQTT_PORT "........."
#define MQTT_USERNAME  "......"
#define MQTT_PASSWORD  "........."

const char *MQTT_TOPIC1 = "switch/bedroom1/1/state";
const char *MQTT_TOPIC2 = "switch/bedroom1/2/state";
const char *MQTT_STATUS = "switch/bedroom1/status";
const char *SWITCH1 = "switch/1/command";
const char *SWITCH2 = "switch/2/command";

const char *MQTT_CLIENT_ID = "swtich_ID_1";

// Pin Setup
#define relay_1 14
#define relay_2 12
#define btn_1 5
#define btn_2 4

int flag1 = 0;
int flag2 = 0;
int state = 0;
int wifi_trigger = 0;

int lastMilis = 0;
unsigned long nowMillis = millis();
bool shouldSaveConfig = false;

WiFiClient espClient;
PubSubClient client(espClient);
Ticker ticker;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

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

  //clean FS for testing
  //  SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(MQTT_SERVER, json["mqtt_server"]);
          strcpy(MQTT_PORT, json["mqtt_port"]);
          strcpy(MQTT_USERNAME, json["mqtt_username"]);
          strcpy(MQTT_PASSWORD, json["mqtt_password"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }

  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", MQTT_SERVER, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", MQTT_PORT, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", MQTT_USERNAME, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", MQTT_PASSWORD, 20);

  WiFiManager wm;
  wm.setAPCallback(configModeCallback);

  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_pass);

  if (!wm.autoConnect() || digitalRead(btn_2) == LOW)
  {
    Serial.println("failed to connect");
    wm.resetSettings();
    ESP.restart();
    delay(1000);
  }

  Serial.println("connected...");

  strcpy(MQTT_SERVER, custom_mqtt_server.getValue());
  strcpy(MQTT_PORT, custom_mqtt_port.getValue());
  strcpy(MQTT_USERNAME, custom_mqtt_user.getValue());
  strcpy(MQTT_PASSWORD, custom_mqtt_pass.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = MQTT_SERVER;
    json["mqtt_port"] = MQTT_PORT;
    json["mqtt_user"] = MQTT_USERNAME;
    json["mqtt_pass"] = MQTT_PASSWORD;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  ticker.detach();
  digitalWrite(2, LOW);
}

void button_func()
{
  // button1
  int stateBtn_1 = digitalRead(btn_1);
  if (!stateBtn_1 && !flag1)
  {
    if (state == 0)
    {
      client.publish(MQTT_TOPIC1, "1");
      digitalWrite(relay_1, HIGH);
      state = 1;
    }
    else
    {
      client.publish(MQTT_TOPIC1, "0");
      digitalWrite(relay_1, LOW);
      state = 0;
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
    if (state == 0)
    {
      client.publish(MQTT_TOPIC2, "1");
      digitalWrite(relay_2, HIGH);
      state = 1;
    }
    else
    {
      client.publish(MQTT_TOPIC2, "0");
      digitalWrite(relay_2, LOW);
      state = 0;
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

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    Serial.println();
    if ((char)message[i] != '"')
      messageTemp += (char)message[i];
  }

  if (String(topic) == SWITCH1)
  {
    if (messageTemp == "1") {
      digitalWrite(relay_1, HIGH);
      client.publish(MQTT_TOPIC1, "1");
    }
    else if (messageTemp == "0") {
      digitalWrite(relay_1, LOW);
      client.publish(MQTT_TOPIC1, "0");
    }
  }

  if (String(topic) == SWITCH2)
  {
    if (messageTemp == "1") {
      digitalWrite(relay_2, HIGH);
      client.publish(MQTT_TOPIC2, "1");
    }
    else if (messageTemp == "0") {
      digitalWrite(relay_2, LOW);
      client.publish(MQTT_TOPIC2, "0");
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, MQTT_STATUS, 1, 1, "Offline"))
    {
      Serial.println("connected");
      //subscribe
      client.subscribe(SWITCH1);
      client.subscribe(SWITCH2);
      client.publish(MQTT_STATUS, "Online", true);
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
