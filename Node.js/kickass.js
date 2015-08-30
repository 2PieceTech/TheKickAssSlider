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
    setMotor([0, 0, 0, 0]);
  } else if (key.name === 'up') {
    setMotor([FORWARD, 0, BACKWARD, 0]);
  } else if (key.name === 'down') {
    setMotor([BACKWARD, 0, FORWARD, 0]);
  } else if (key.name === 'left') {
    setMotor([0, FORWARD, 0, BACKWARD]);
  } else if (key.name === 'right') {
    setMotor([0, BACKWARD, 0, FORWARD]);
  } else if (key.name === 'r') {
    setMotor([FORWARD, FORWARD, FORWARD, FORWARD]);
  } else if (key.name === 'l') {
    setMotor([BACKWARD, BACKWARD, BACKWARD, BACKWARD]);
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

var motorCharacteristics = null;

function setMotor(speeds) {
  if (motorCharacteristics) {
    for (var i = 0; i < speeds.length; i++) {
      var buffer = new Buffer(1);

      buffer.writeInt8(speeds[i], 0);

      motorCharacteristics[i].write(buffer);
    }
  }
}

noble.on('discover', function(peripheral) {
  console.log('on -> discover: ' + peripheral);

  noble.stopScanning();

  peripheral.connect(function() {
    peripheral.discoverSomeServicesAndCharacteristics(['51d0'], [], function(error, services, characteristics) {

      motorCharacteristics = characteristics;


      // peripheral.disconnect(function() {
      //   process.exit(0);
      // });
    });
  });
});



