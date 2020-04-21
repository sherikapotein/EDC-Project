var WebSocketServer = require('ws').Server
 wss = new WebSocketServer({port: 40510})

  wss.broadcast = function (data) {
    wss.clients.forEach(function (client) {
      client.send(data)
    })
  }

var clients = new Array();
var cliData=new Array(14);
cliData[1]="q";
console.log(cliData[1]);






function heartbeat() {
  this.isAlive = true;
  
}

function noop() {}


wss.on('connection', function (ws, req) {

	ws.isAlive = true;
  	ws.on('pong', heartbeat);

	var ip = req.connection.remoteAddress;
	console.log('ip: %s', ip)
	var pos = ip.lastIndexOf(".");
  var subst=ip.substr(pos+1,ip.length)
  

//for localhost addresss browser for client information
console.log('subst %s',subst);
pos = subst.lastIndexOf(":");
subst=subst.substr(pos+1,subst.length)



	console.log("last numeral: %s",subst);
	var index=parseInt(subst);	
	console.log("last numeral INT: %d",index);
  index=index-90;
  
  if(index<0) index*=-1;

        //ws.id = wss.getUniqueID();
	ws.id=index;        

	wss.clients.forEach(function each(client) {
        	console.log('Client.ID: ' + client.id);
		});





	ws.on('message', function incoming(data) {
      
      
      var rectStatus, normStatus,alterStatus,tripStatus; 
      var recvStatus=data.toString();
      var b;
      //console.log('recvStatus is %s',recvStatus);
     /* b=recvStatus.split("#");
      console.log(b[0]);
      console.log(b[1]);
      console.log(b[2]);
      console.log(b[3]);
      console.log(b[4]);
      console.log(b[5]);
*/

/*
      var indexLast=recvStatus.lastIndexOf('#');

      var tripStatus=recvStatus.substr(indexLast+1,recvStatus.length);
      indexFirst=recvStatus.indexOf('#');

      var tempString=recvStatus.substr(indexFirst+1,indexLast-indexFirst-1);
      console.log("tempString 1 %s",tempString);

      tempIndex=tempString.indexOf('#');
      var tempString=tempString.substr(tempIndex+1,1);
      console.log("tempString 2 %s",tempString);

      tempIndex=tempString.indexOf('#');
      var tempString=tempString.substr(tempIndex+1,1);
      console.log("tempString 3 %s",tempString);

      tempIndex=tempString.indexOf('#');
      var tempString=tempString.substr(tempIndex+1,1);
      console.log("tempString 4 %s",tempString);

      tempIndex=tempString.indexOf('#');
      var tempString=tempString.substr(tempIndex+1,1);
      console.log("tempString 5 %s",tempString);
      */

      // console.log('index Last is %s LENGTH IS %s EDC IS %s and HEALTH IS %s',indexLast,recvStatus.length,edc,health);


      if(data.toString().indexOf('EDC#')) 
        console.log("message ON YES NEW: %s",data);
      //else if(data.toString()=="EDC#1#NO")
        {
          wss.clients.forEach(function each(client) {
            if(client.id==89)            
             client.send(data.toString());
            console.log('SENT ALARM MESSAGE');
          });
      }
      
      console.log("message: %s",data);
    		});

//ws.on('error', () => console.log('errored'));
/*var ip = req.connection.remoteAddress;
console.log('ip: %s', ip)
var pos = ip.lastIndexOf(".");
var subst=ip.substr(pos+1,ip.length)
console.log("last numeral: %s",subst);
var index=parseInt(subst);	
console.log("last numeral INT: %d",index);
index=index-90;
clients[index]=ws;
*/
	ws.send(`Welcome young Gum!!`);


ws.on('close', function () {
      var idx=ws.id;

      // wss.clients.forEach(function each(client) {
      //   if(client.id==89)            
      //   {var status='EDC#'+idx+'TERM'; 
      //   console.log('sending %s',status);
      //   client.send(status);
      // }
      // });

      var s='EDC#'+idx+'#TERM'+'#TERM'+'#TERM'+'#TERM';
      wss.broadcast(s);
      console.log(s);
      });

//var myVar = setInterval(myTimer, 3000);

//ws.on('message', function incoming(data) {







//wss.on('error', () => console.log('!!!!!SERVER errored!!!!!!!'));


//ws.on('error', () => console.log('!!!!!!!!!SOCKET errored!!!'));




/*
function myTimer() {
  var d = new Date();
  var h=d.toString();
  ws.send(`yes`);
}*/
})


function sendAlarm(){
wss.clients.forEach(function each(client) {
  client.send(`EDC#1#YES`);
  console.log('SENT ALARM MESSAGE');
});
}



const interval = setInterval(function ping() {
  wss.clients.forEach(function each(ws) {
    if (ws.isAlive === false) {console.log('terminated SOMEONE!!!!!');return ws.terminate();}
    ws.isAlive = false;
    ws.ping(noop);

	//console.log('CLIENT LENGTH %d',clients.length);
	////for (var i=0; i<clients.length; i++) {
	////var sendStr="YO ARE CLIENT %d "+i;
       //// clients[i].send(sendStr);
    ////}


  });
}, 1000);


wss.getUniqueID = function () {
    function s4() {
        return Math.floor((1 + Math.random()) * 0x10000).toString(16).substring(1);
    }
    return s4() + s4() + '-' + s4();
};




