const WebSocket = require("ws")
const PORT = 5000
const wsServer = new WebSocket.Server({
    port: PORT
})

const name = "[SERVER]"
const server_name = name + ': '
Client_list = {}

wsServer.on('connection', function (socket) {
    var sender = ''
    socket.send(server_name + "Connection established")
    for (const [key, value] of Object.entries(Client_list)) {
        socket.send(`${key}`)
    }

    num_clients()

    socket.on('message', function (data) {
        if (data[0] != '{') {
            sender = data.substring(0, data.indexOf(']') + 1)

            if (Client_list[sender] == undefined) {

                Client_list[sender] = socket

                wsServer.clients.forEach(function (client) {
                    client.send(server_name + sender + ': just connected')
                    client.send(sender)
                })

            } else {

                wsServer.clients.forEach(function (client) {
                    client.send(data)
                })
            }
        } else {
            try {
                Client_list['[ARDUINO]'].send(data)
            } catch (error) {
                wsServer.clients.forEach(function (client) {
                    client.send(server_name + 'Attenzione, Arduino sembra non essere connesso!')
                })
                console.log('ARDUINO non connesso')
            }
        }

    })

    socket.on('close', function () {
        Client_list[sender] = undefined
        wsServer.clients.forEach(function (client) {
            client.send(server_name + sender + ': just disconnected')
            client.send(sender)
        })
        num_clients()
    })

    socket.on('error', function () {
        wsServer.clients.forEach(function (client) {
            client.send(server_name + "Something went wrong, try to reconnect...")
        })
    })
})

console.log(server_name + "Online, listening at ws://127.0.0.1:" + PORT)

function num_clients() {
    console.log(server_name + "Clients connected: " + wsServer.clients.size + ", " + new Date())
}