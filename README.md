# Smart-Switch-Momentary-
DIY Smart Switch momentary 2 Gang and 2 Channel Relay SPDT

List Item

- Esp8266 (Wemos D1 Mini)
- Relay 5VDC SPDT Songle (Max Current 10A)
- Power Module Hi-Link 220VAC to 5VDC
- Resistor 1k, 10K (SMD 0805)
- Diode 1N4007 SMD
- Transistor 2n2222a
- Terminal Screw 6 Pin Hole
- Push Button (schneider momentary switch)

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
    name: "PIR Sensor"
    state_topic: "switch/bedroom1/1/state"
    command_topic: "switch/1/command"
    payload_on: "1"
    payload_off: "0"
    
 switch:
  - platform: mqtt
    name: "PIR Sensor"
    state_topic: "switch/bedroom1/2/state"
    command_topic: "switch/2/command"
    payload_on: "1"
    payload_off: "0"
```

Datasheet: 
https://download.schneider-electric.com/files?p_enDocType=Instruction+sheet&p_File_Name=MS18524.pdf&p_Doc_Ref=MS18524

![IMG_20210930_115609](https://user-images.githubusercontent.com/50385294/136680298-2959f330-0586-41ed-94dd-7bfbbf4e573d.jpg)

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/DARuhkKLw8E/0.jpg)](https://www.youtube.com/watch?v=DARuhkKLw8E")

Dimension

L = 69,84 mm
W = 70,87 mm
