'use-strict'
let mqtt = require('mqtt');

// Web sockets hosts
//const host = "ws://localhost:8882"
const host = "wss://test-mqtt-server-kdtisjwi5a-ew.a.run.app:443"

// MQTT hosts
//const host = "mqtt://localhost:8882"
//const host = "mqtts://test-mqtt-server2-kdtisjwi5a-ew.a.run.app:443"


let clientId = 'test web client ' + Date.now
let client = mqtt.connect(host, {    
    clientId: clientId
  });

client.on('connect', function () {
  console.log("Connected to server")
  client.subscribe('responses');
  sendMessage()
  
  setInterval(() => {
      sendMessage()
  }, 15000);
 

})

client.on('message', function (topic, message) {
  console.log('Received a message in topic: ' + topic + ": " + message.toString());
});

client.on('error', function(e) {
  console.log('Received an error: ' + e.message)
})

client.on('disconnect', function() {
  console.log('Client was disconnected ' )
})
client.on('reconnect', function() {
  console.log('Client was reconnected ')
})

client.on('close', function() {
  console.log('Client was closed ' )
})

client.on('offline', function() {
  console.log('Client was offline ')
})

function sendMessage() {
  let now = new Date().toISOString()
  let msg = "Current time = " + now
  console.log("Publishing data", msg)
  client.publish('messages', msg);
}
