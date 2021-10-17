#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = ".............";
const char* password = "............";
const char* MQTT_SERVER = "192.168.x.xxx";
const char* MQTT_TOPIC = "switch/bedroom/1";

const char* MQTT_CLIENT_ID = "switch_bedroom1";
const char* MQTT_USERNAME = "..........";
const char* MQTT_PASSWORD = "..........";

WiFiClient espClient;
PubSubClient client(espClient);

// Pin Setup
#define relay_1 14
#define relay_2 12
#define btn_1 5
#define btn_2 4
int flag1 = 0;
int flag2 = 0;
int state = 0;

void setup_wifi() {
  // Connect to Wifi Network
  pinMode(2, OUTPUT); //WiFi Indicator
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2, HIGH);
    delay(80);
    digitalWrite(2, LOW);
    delay(80);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void button_func() {
  // button1
  int stateBtn_1 = digitalRead(btn_1);
  if (!stateBtn_1 && !flag1) {
    if (state == 0) {
      client.publish(MQTT_TOPIC,"1");
      state = 1;
    } else {
      client.publish(MQTT_TOPIC,"2");
      state = 0;
    }
    flag1 = 1;
  } else if (stateBtn_1 && flag1) {
    flag1 = 0;
  }
  // button2
  int stateBtn_2 = digitalRead(btn_2);
  if (!stateBtn_2 && !flag2) {
    if (state == 0) {
      client.publish(MQTT_TOPIC,"3");
      state = 1;
    } else {
      client.publish(MQTT_TOPIC,"4");
      state = 0;
    }
    flag2 = 1;
  } else if (stateBtn_2 && flag2) {
    flag2 = 0;
  }
  delay(100);
}

void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messageTemp;

  if (String(topic) != MQTT_TOPIC) {
    return;
  }

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    if ((char)message[i] != '"')
      messageTemp += (char)message[i];
  }
  // Changes the output state according to the message
  Serial.println("");
  Serial.print("Changing output to ");
  //  Serial.println(messageTemp);
  if (messageTemp == "1") {
    Serial.println("Relay_1On");
    digitalWrite(relay_1, HIGH);
  }
  if (messageTemp == "2") {
    Serial.println("Relay_1Off");
    digitalWrite(relay_1, LOW);
  }
  if (messageTemp == "3") {
    Serial.println("Relay_2On");
    digitalWrite(relay_2, HIGH);
  }
  if (messageTemp == "4") {
    Serial.println("Relay_2Off");
    digitalWrite(relay_2, LOW);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD )) {
      Serial.println("connected");
      //subscribe
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  setup_wifi();

  // MQTT Server Setup
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  pinMode(2, OUTPUT);//WiFi Indicator
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);

  pinMode(btn_1, INPUT);
  pinMode(btn_2, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  button_func();
  client.loop();
}
