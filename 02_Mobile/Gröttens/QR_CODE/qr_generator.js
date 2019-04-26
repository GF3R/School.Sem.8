var QRCode = require('qrcode');
QRCode.toString("Test").then(function(qrcode) {
  console.log(qrcode);
});

