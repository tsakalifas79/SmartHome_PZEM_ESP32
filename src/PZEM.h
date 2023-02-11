// #ifndef PZEM_h
// #define PZEM_h

// #include <HardwareSerial.h>
// #include <PZEM004T.h>

// // HardwareSerial PzemSerial2(2);
// // #define RXD2 23 //16 is used for OLED_RST !
// // #define TXD2 17

// // PZEM004T pzem(&PzemSerial2);
// // IPAddress ip(192,168,1,1);

// HardwareSerial hwserial(UART0);     // Use hwserial UART0 at pins GPIO1 (TX) and GPIO3 (RX)
// PZEM004T pzem(&hwserial);           // Attach PZEM to hwserial
// IPAddress ip(192,168,1,1);
// bool pzemrdy = false;

// float v = 0;
// float i = 0;
// float p = 0;
// float e = 0;
// long time1 = millis();
// long time2 = time1;
// int timeout = 1000;

// void setupPZEM(){
//     //  PzemSerial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

//     // if(pzem.setAddress(ip))
//     //   Serial1.println("PZEM.. Connected");
//     ///////////////////////////////////////////////
//     // hwSerial1.swap();           // (optionally) swap hw_serial pins to gpio13(rx),15(tx)

//     while (!pzemrdy) {
//       Serial1.println("Connecting to PZEM...");
//       pzemrdy = pzem.setAddress(ip);
//       delay(1000);
//     }
// }

// void updatePZEM() {
//   time1 = millis();
//   if (time1 > time2) {
//     time2 = time1 + timeout;

//     // if(pzem.setAddress(ip)){
//       v = pzem.voltage(ip);
//       i = pzem.current(ip);
//       p = pzem.power(ip);
//       e = pzem.energy(ip)/1000;
//     // }
  
//     Serial1.println("V:" + String(v) + "\nA:" + String(i) + "\nW:" + String(p) + "\nH:" + String(e) + "\n");
//   }
// }

// #endif