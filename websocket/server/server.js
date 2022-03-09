const WebSocket = require("ws")
const ObservableSlim = require("observable-slim");

// var server = require('http').createServer().listen(8080, '192.168.1.161');
const PORT = 5000

const wsServer = new WebSocket.Server({
    port: PORT
})

const connection_list = ObservableSlim.create({}, true, function (changes) {
    broadcast(changes)
});

connection_list.Server = {
    name: 'Local_node_server',
    creation_time: timeStamp(),
    status: 1,
    sended_data: []
}

const socket_list = {}
const exception = []


function broadcast(data) {
    for (var i in socket_list) {
        if (!exception.includes(i)) {
            socket_list[i].send(JSON.stringify(data))
        }
    }
}

function isJson(str) {
    try {
        JSON.parse(str);
    }
    catch (e) {
        return false;
    }
    return true;
}

function timeStamp() {
    var current_time = new Date()
    var hour = current_time.getHours()
    var min = current_time.getMinutes()
    var seconds = current_time.getSeconds()
    hour = hour < 10 ? '0' + hour : hour
    min = min < 10 ? '0' + min : min
    seconds = seconds < 10 ? '0' + seconds : seconds
    return hour + ':' + min + ':' + seconds + ' - '
}

function disconnect(target) {
    try {
        if (connection_list.hasOwnProperty(target)) {
            socket_list[target].close()

            var index = exception.indexOf(target);
            if (index > -1) {
                exception.splice(index, 1);
            }
            WriteToConsole("Socket con " + target + " chiuso...")

            connection_list[target].status = 3
            WriteToConsole("Cambio lo stato della connessione...")

            delete connection_list[target]
            delete socket_list[target]
            WriteToConsole(target + " Eliminato dal database.")
        }
    } catch (error) {
        WriteToConsole("C'è stato un errore nello scollegare: " + target)
    }
}

function WriteToConsole(msg) {
    var timed_msg = timeStamp() + msg
    connection_list.Server.sended_data.push(timed_msg)
    console.log(timed_msg)
}

function checkStatus(target) {
    if (socket_list.hasOwnProperty(target)) {
        socket_list[target].readyState == 1 ? setTimeout(() => checkStatus(target), 5000) : connection_list[target].status = socket_list[target].readyState
    } else {
        return
    }
}


wsServer.on('connection', function (socket) {

    WriteToConsole('Nuova connessione in corso...')
    const device = {
        name: undefined,
        creation_time: undefined,
        status: socket.readyState,
        sended_data: []
    }

    socket.on('message', function (message) {
        if (isJson(message)) {
            var message_obj = JSON.parse(message)

            if (message_obj.type == 'HandShake' && device.name != undefined) {
                WriteToConsole(device.name + ' sta provando a ri-registrarsi sul server')
                return
            }
            if (message_obj.type != 'HandShake' && device.name == undefined) {
                WriteToConsole("Qualcuno sta provando ad inviare dati senza essersi identificato")
                WriteToConsole("Chiudo il socket")
                socket.close()
                return
            }


            switch (message_obj.type) {

                case 'HandShake':
                    var i = 0
                    var only_data = message_obj.option == 'only_data' ? true : false
                    var name = message_obj.value + '_' + i
                    while (connection_list.hasOwnProperty(name)) {
                        var name = message_obj.value + '_' + i++
                    }

                    device.name = name
                    device.creation_time = new Date()
                    device.status = socket.readyState

                    socket_list[name] = socket
                    connection_list[name] = device

                    if (!only_data) {
                        socket_list[name].send(JSON.stringify([{ type: 'Init_list', jsonPointer: '', newValue: connection_list }]))
                        socket_list[name].send(JSON.stringify([{ type: 'Set_name', newValue: device.name }]))
                    } else {
                        exception.push(device.name)
                    }
                    WriteToConsole('Connessione effettuata con: ' + device.name + ', opzione only_data: ' + only_data)
                    checkStatus(device.name)
                    break;

                case 'Disconnect':
                    message_obj.value.forEach(element => {
                        WriteToConsole("Forzo la disconnessione di: " + element)
                        connection_list[device.name].sended_data.push(timeStamp() + message)
                        disconnect(element)
                    });
                    break;
                default:
                    //Se un messaggio è senza type allora viene inviato anche agli 'only_data'
                    exception.forEach(element => {
                        socket_list[element].send(JSON.stringify(message_obj))
                    });
                    break;
            }
            try {
                connection_list[device.name].sended_data.push(timeStamp() + message)
            } catch (error) {
            }

        }
        else {
            WriteToConsole("Il formato del messaggio è incorretto!\nMittente:\t" + device.name + "\nMessaggio:\t" + message)
        }
    })

    socket.on('close', function () {
        disconnect(device.name)
    })
    socket.on('error', function () {
        disconnect(device.name)
    })

})

WriteToConsole("Server ONLINE, listening at ws://127.0.0.1:" + PORT)