// Connect to MQTT broker
// Note. this example uses web-sockets (port 1884)
mqtt = new Paho.MQTT.Client(
  "mqtt-cloud-atmduino.herokuapp.com",
  Number(80),
  "LKI-1-client"
);
mqtt.connect({
  onSuccess: function () {
    console.log("connected!");
  },
});

function mainlamp() {
  console.log("");

  // Send 'toggle' message to command topic
  var x = document.getElementById("main_state");
  if (x.innerHTML == "On") {
    x.innerHTML = "Off";
    message = new Paho.MQTT.Message("off");
    message.destinationName = "tele/home/main";
    mqtt.send(message);
  } else {
    x.innerHTML = "On";
    message = new Paho.MQTT.Message("on");
    message.destinationName = "tele/home/main";
    mqtt.send(message);
  }
}

function livingroom() {
  console.log("");

  // Send 'toggle' message to command topic
  var x = document.getElementById("living_state");
  if (x.innerHTML == "On") {
    x.innerHTML = "Off";
    message = new Paho.MQTT.Message("off");
    message.destinationName = "tele/home/living";
    mqtt.send(message);
  } else {
    x.innerHTML = "On";
    message = new Paho.MQTT.Message("on");
    message.destinationName = "tele/home/living";
    mqtt.send(message);
  }
}

function garage() {
  console.log("");

  // Send 'toggle' message to command topic
  var x = document.getElementById("garage_state");
  if (x.innerHTML == "On") {
    x.innerHTML = "Off";
    message = new Paho.MQTT.Message("off");
    message.destinationName = "tele/home/garage";
    mqtt.send(message);
  } else {
    x.innerHTML = "On";
    message = new Paho.MQTT.Message("on");
    message.destinationName = "tele/home/garage";
    mqtt.send(message);
  }
}
