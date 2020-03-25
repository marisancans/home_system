
const WebSocket = require('ws');

function connect() {
  var ws = new WebSocket('ws://127.0.0.1:9001/plants');
  ws.onopen = function() {
  	  setInterval(function(){
          data = {
              "command": "event",
              "event_name": "new_plant_data",
              "data": {
                  "ts": Date.now(),
                  "measurement": 123
              }
          }

        ws.send(JSON.stringify(data))
      }, 1000);
  };

  ws.onmessage = function(e) {
    console.log('Message:', e.data);
  };

  ws.onclose = function(e) {
    console.log('Socket is closed. Reconnect will be attempted in 1 second.', e.reason);
    setTimeout(function() {
      connect();
    }, 1000);
  };

  ws.onerror = function(err) {
    console.error('Socket encountered error: ', err.message, 'Closing socket');
    ws.close();
  };
}

connect();
