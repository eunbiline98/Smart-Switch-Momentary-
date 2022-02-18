# Smart Switch Momentary

DIY Smart Switch momentary 2 Gang and 2 Channel Relay SPDT

List Item

- Esp8266 (Wemos D1 Mini)
- Relay 5VDC SPDT Songle (Max Current 10A)
- Power Module Hi-Link 220VAC to 5VDC
- Resistor 1k, 10K (SMD 0805)
- Diode 1N4007 SMD
- Transistor 2n2222a
- Terminal Screw 6 Pin Hole
- Push Button (schneider momentary switch) Datasheet: https://download.schneider-electric.com/files?p_enDocType=Instruction+sheet&p_File_Name=MS18524.pdf&p_Doc_Ref=MS18524

# Schematic

![schematic momentary switch wifi](https://user-images.githubusercontent.com/50385294/137615797-40eabae7-91a9-4d51-8ee7-6c4925abf029.png)

# Layout Top View 
![Screenshot 2022-02-18 163614](https://user-images.githubusercontent.com/50385294/154657791-f572e95e-0cb7-4fda-842f-4ccfcbfc2a54.png)
# Layout Bottom View
![Screenshot 2022-02-18 163649](https://user-images.githubusercontent.com/50385294/154657780-461a0472-75ee-4fb4-809c-ebf900b1a6ee.png)





# Home Assistant

Under covers in HA, use this config

```
mqtt:
  broker: "Your IP Broker/server"
  port: Your port MQTT
  username: "Your username"
  password: Your password

switch:
  - platform: mqtt
    name: "General Lamp"
    state_topic: "switch/bedroom1/1/state"
    command_topic: "switch/1/command"
    payload_on: "1"
    payload_off: "0"

 switch:
  - platform: mqtt
    name: "Cove Lighting"
    state_topic: "switch/bedroom1/2/state"
    command_topic: "switch/2/command"
    payload_on: "1"
    payload_off: "0"
```

![IMG_20210930_115609](https://user-images.githubusercontent.com/50385294/136680298-2959f330-0586-41ed-94dd-7bfbbf4e573d.jpg)

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/DARuhkKLw8E/0.jpg)](https://www.youtube.com/watch?v=DARuhkKLw8E")

# Reference

- WiFi Manager (https://github.com/tzapu/WiFiManager)
- MQTT (https://github.com/knolleary/pubsubclient)

# Update

- DIY_switch_3_gang_Ver_1_0 (Test Board Circuit)
- DIY_switch_3_gang_Ver_1_0_WM (Update Feature WifI Manager)
- DIY_switch_3_gang_Ver_1_1_WM (Update Last State Condition Dashboard Home Assistant)
- DIY_switch_3_gang_Ver_1_2_WM (Update Feature WiFi and Mqtt Config Manager)

# Dimension

- L = 69,84 mm
- W = 70,87 mm
