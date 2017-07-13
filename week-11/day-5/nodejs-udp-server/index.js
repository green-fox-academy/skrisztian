// Socket.io
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var port = process.env.PORT || 3000;

// dgram
var udp_port = 54002;
var dgram = require('dgram');
var udp_server = dgram.createSocket('udp4');

// Start socket.io
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
  socket.on('udp message', function(msg){
    io.emit('udp message', msg);
    console.log(msg);
  });
});

http.listen(port, function(){
  console.log('listening on *:' + port);
});


// Create UDP listener port
udp_server.on('listening', function () {
    var address = udp_server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// Incoming UDP packages, log them
udp_server.on('message', function(message, remote) {
	var udp_packet = remote.address + ':' + remote.port +' - ' + message;
    console.log(udp_packet);
    //io.sockets.emit('udp message', udp_packet.toString());
    io.emit('udp message', udp_packet);
});

udp_server.bind(udp_port);