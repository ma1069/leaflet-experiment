#include "Arduino.h"

#ifndef wifiWrapper_h
  #define wifiWrapper_h

class wifiWrapper{
  public:
  void printWifiStatus();
  void connectESP(char ssid[20], char password[20] );
  void printSuccessConnection();
  String checkForIncomingMessage();
  void checkForConnectionEvents();
  void apiCallHTTP(char host[50], char* type, char endpoint[50], int port);
  void apiCallHTTPS(char host[50], char* type, char endpoint[50], int port);
  void apiCallHTTPpost(String payload, char server[50], char* type, char endpoint[50], int port);
  void apiCallHTTPSpost(String payload, char server[50], char* type, char endpoint[50], int port);
  int isAvailable();
  void httpPING(char server[50]);
  void httpsPING(char server[50]);
  void httpPUT(char server[50], unsigned long value);

};

  extern wifiWrapper wifi;
 
#endif

/* INDEX OF FUNCTIONS 
  SETUP
  expectSerial();
  blueUp(); // bang! bring up serial conection with bluefruit
  buttonUp();// initializes the buttons
  
  GENERAL
  bluePrint('char'); // prints passed argument
  pressEvent();//detects debounced press
  holdForButton;//holds up the whole loop for an expected event
  surePress();//Makes sure the button was release before registering another press
  
  TESTING
  printabletest(); 
  altkeystest();
  nonprinting();
  iphonekeyboard();
  rawkeytest();
  rawmousetest();
*/
