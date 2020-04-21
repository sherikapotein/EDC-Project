var express = require('express')
var ws = require('./ws')

var app = express()

app.use(express.static('public'));

app.get('/', function (req, res) {
    res.sendFile(__dirname + '/ws.html');
	res.sendFile(__dirname + '/alarm.mp3');

})

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})


    app.on('error', function(e){
        console.log(e);
    });
