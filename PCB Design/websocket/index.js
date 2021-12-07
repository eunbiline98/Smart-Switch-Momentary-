const aedes = require('aedes')()
const ws = require('websocket-stream')
const port = 8882
const WEB_SOCKETS = true
let httpServer 

if (WEB_SOCKETS) {
    httpServer = require('http').createServer()
    ws.createServer({ server: httpServer }, aedes.handle)
} else {
    httpServer = require('net').createServer(aedes.handle)
}

httpServer.listen(port, function () {
    let type = (WEB_SOCKETS) ? "WebSockets" : "HTTP"
  console.log('STARTED: ' + type + ' server listening on port ', port)
})


aedes.subscribe('messages', function (packet, callback) {
    let msg = packet.payload.toString()
    console.log('Received message', msg);
    aedes.publish(
        {            
            topic: 'responses',
            payload: msg,
            retain: false
        }
    );

});