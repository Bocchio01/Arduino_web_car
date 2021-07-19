const WebSocket = require("ws");

const URL = "ws://curious-wealthy-pigment.glitch.me/";

const ws = new WebSocket(URL, {
    headers: {
        "user-agent": "user_bocchio"
}
});

ws.on('open', function () {
    ws.send("Client connected");
})

ws.on('message', function (data) {
    console.log("Received message: " + data);
})