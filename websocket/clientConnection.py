import paho.mqtt.client as mqtt
import time


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag = True
        print("connected OK")
    else:
        print("Bad connection Returned code=", rc)


mqtt.Client.connected_flag = False
broker = "192.168.10.172"
client = mqtt.Client("python1")
client.username_pw_set(username="laviz", password="laviz810")
client.on_connect = on_connect
client.loop_start()
print("Connecting to broker ", broker)
client.connect(broker)
while not client.connected_flag:
    print("In wait loop")
    time.sleep(1)
print("in Main Loop")
client.loop_stop()
client.disconnect()
