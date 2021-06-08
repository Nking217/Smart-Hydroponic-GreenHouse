////////////////////////
/// ESP-WIFI NETWORK ///
////////////////////////

#define NO_DATA "!@@#@@!"   //used for ReadFromESP when there is no data yet

//used in the espReconnect for the _WifiConnectionStatus - Updating the connection status of the ESP.
#define CONNECTION_STATUS_WAITING 0 
#define CONNECTION_STATUS_CONNECTING 1 
#define CONNECTION_STATUS_CONNECTED 2 
#define CONNECTION_STATUS_TIMEOUT 3 
#define CONNECTION_STATUS_ERROR 4
#define CONNECTION_STATUS_MISSING_INFO 5

#define ESP_TIME_TO_RECONNECT 30000

#define END_OF_RETRUN_STRING '\0'
#define END_OF_COMMAND_STRING '$'


long _LastTimeConnectRequest;

void espInit(){
  Serial3.begin(9600);
}

void espTest(){
  Serial.print("This is a test");
  espSetDebugMode(true);
  ConnectToWifi();
  String content;
  do {
    content = readFromEsp();
    if (content != NO_DATA)
      Serial.println(content);
  } 
  while(content.indexOf("CONNECTION_STATUS:CONNECTED") == -1);
  byte sensorId = 4;
  long timeStamp = 0;
  int value = 6900;
//  serverSendSensorData(sensorId, timeStamp, value);
  espSendGetRequest("http://hamama.ew.r.appspot.com/board?cmd=measure&sid=4&time=0&value=6900");  //TODO Make a get request for the server on espSendGetRequest...
  do{
    content = readFromEsp();
    if (content != NO_DATA)
      Serial.println(content);
  }
  while(content.indexOf("RESAULT_STATUS:") == -1);
}

byte _WifiConnectionStatus;
long _WifiConnectionStatusTime;

void espSetDebugMode(bool mode){
  if(mode == true){
    Serial3.print("~b");
  }
  else{
    Serial3.print("~e");
  }
}

void espReconnect(){
  switch(_WifiConnectionStatus){
    case CONNECTION_STATUS_CONNECTED:
      return;
    case CONNECTION_STATUS_WAITING:
      ConnectToWifi();
    case CONNECTION_STATUS_CONNECTING:
      return;
    case CONNECTION_STATUS_TIMEOUT:
    case CONNECTION_STATUS_ERROR:
      if(millis() - _LastTimeConnectRequest > ESP_TIME_TO_RECONNECT){
        ConnectToWifi();
      }
      else{
        return;
      }
    case CONNECTION_STATUS_MISSING_INFO:
      return;
  }
}


String _SerialReadBuffer;

String readFromEsp(){
  if(Serial3.available()){
    String a1 = Serial3.readString();
    Serial.print(a1);
    int index = a1.indexOf(END_OF_RETRUN_STRING);
    if(index >= 0 ){
      String temp = a1.substring(0, index) + _SerialReadBuffer;
      _SerialReadBuffer = a1.substring(index,a1.length());
      return temp;
    }
    else{
      _SerialReadBuffer = _SerialReadBuffer + a1;
      return NO_DATA;
    }
  }
  return NO_DATA;
}

void ConnectToWifi(){
  String ssid;//[EEPROM_ESP_SSID_LENGTH];
  String password;//[EEPROM_ESP_PASSWORD_LENGHT];
  ssid = espGetWifiSSID();
  password = espGetWifiPassword();
  int timeout = espGetConnectionTimeout();
  espConnectToWifi(ssid, password, timeout);
}



void espConnectToWifi(String ssid, String password, int connectionTimeout){
  //~c<SSID>$<password>$<timeout>$
 /* Serial.print("~c");
  Serial.print(ssid); 
  Serial.print(END_OF_COMMAND_STRING);
  Serial.print(password);
  Serial.print(END_OF_COMMAND_STRING);
  Serial.print(connectionTimeout);
  Serial.print(END_OF_COMMAND_STRING); 
  */
  
  Serial3.print("~c");
  Serial3.print(ssid); 
  Serial3.print(END_OF_COMMAND_STRING);
  Serial3.print(password);
  Serial3.print(END_OF_COMMAND_STRING);
  Serial3.print(connectionTimeout);
  Serial3.print(END_OF_COMMAND_STRING); 
  _LastTimeConnectRequest = millis();
}


void espDisconnectFromWifi(){
  Serial3.print("~d");
}

void espGetIp(){
  Serial3.print("~a");
}

void espSendGetRequest(String url){
  //~g<request url>$
  Serial3.print("~g");
  Serial3.print(url);
  Serial3.print(END_OF_COMMAND_STRING);
}

void espSendPostRequest(long reqid, String url, String data){
  //~p<request url>$<data>$   
  Serial3.print("~p");
  Serial3.print(reqid);
  Serial3.print(END_OF_COMMAND_STRING);
  Serial3.print(url);
  Serial3.print(END_OF_COMMAND_STRING);
  Serial3.print(data);
  Serial3.print(END_OF_COMMAND_STRING);
}

void espSetWifiSSID(String value){
  eepromWrite(EEPROM_ESP_SSID,value);
}

void espSetWifiPassword(String value){
  eepromWrite(EEPROM_ESP_PASSWORD,value);
}

void espSetConnectionTimeout(int value){
  eepromWrite(EEPROM_ESP_CONNECTION_TIMEOUT,value);
}

String espGetWifiSSID(){
  return "Sulam";
  //eepromReadString(EEPROM_ESP_SSID,buffer);
  //strcpy(buffer, "NAVE");   //TODO: remove this
}

String espGetWifiPassword(){
  return "2508572511";
  //eepromReadString(EEPROM_ESP_PASSWORD,buffer);
  //strcpy(buffer, "0524424546");   //TODO: remove this
}

int espGetConnectionTimeout(){
  return "45000";
  //return eepromReadInt(EEPROM_ESP_CONNECTION_TIMEOUT);
}
