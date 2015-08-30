var keypress = require('keypress');
var noble = require('noble');

var FORWARD = 127;
var BACKWARD = -127;

console.log('noble - kickass');

keypress(process.stdin);

process.stdin.setRawMode(true);
process.stdin.resume();

process.stdin.on('keypress', function (ch, key) {
  console.log(key.name);

  if (key.name === 'q') {
    setMotors([0, 0, 0, 0]);
  } else if (key.name === 'up') {
    setMotors([FORWARD, 0, BACKWARD, 0]);
  } else if (key.name === 'down') {
    setMotors([BACKWARD, 0, FORWARD, 0]);
  } else if (key.name === 'left') {
    setMotors([0, FORWARD, 0, BACKWARD]);
  } else if (key.name === 'right') {
    setMotors([0, BACKWARD, 0, FORWARD]);
  } else if (key.name === 'l') {
    setMotors([FORWARD, FORWARD, FORWARD, FORWARD]);
  } else if (key.name === 'r') {
    setMotors([BACKWARD, BACKWARD, BACKWARD, BACKWARD]);
  }


  if (key && key.ctrl && key.name === 'c') {
    process.stdin.pause();
    process.exit();
  }
});


noble.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === 'poweredOn') {
    noble.startScanning(['51d0']);
  } else {
    noble.stopScanning();
  }
});

var motorCharacteristic = null;

function setMotors(speeds) {
  if (motorCharacteristic) {
    var data = new Buffer(4);

    for (var i = 0; i < speeds.length; i++) {
      data.writeInt8(speeds[i], i);
    }

    motorCharacteristic.write(data);
  }
}

noble.on('discover', function(peripheral) {
  console.log('on -> discover: ' + peripheral);

  noble.stopScanning();

  peripheral.connect(function() {
    peripheral.discoverSomeServicesAndCharacteristics(['51d0'], [], function(error, services, characteristics) {

      motorCharacteristic = characteristics[0];
    });
  });
});



