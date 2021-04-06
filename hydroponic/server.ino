////////////////
///  SERVER  ///
////////////////

//used in the serverReconnect for the _ServerConnectionStatus - Cheek if the ESP is connected to the Server.
#define SERVER_CONNECTION_STATUS_CONNECTED 1
#define SERVER_CONNECTION_STATUS_DISCONNECTED 0

#define SERVER_TIME_TO_RECONNECT 30000

//used to define the type of request from the Server.
#define SERVER_REQUEST_NONE 0
#define SERVER_REQUEST_PING 1
#define SERVER_REQUEST_SENSOR_DATA 2
#define SERVER_REQUEST_LOG 3

void serverInit(){
  
}

void serverTest(){
  serverRandomTest();
}

byte _ServerConnectionStatus;
long _ServerConnectionStatusTime;
byte _ServerRequestType;


void serverSendLog(byte sensorId, long timeStamp, byte priority, String message){
  _ServerRequestType = SERVER_REQUEST_LOG;
  String url = "http://"+serverGetAdress()+"/board?";
  url += "cmd=log";
  String data = "";
  if(sensorId != 0){
    data += "sid=" + sensorId;
    data += "&";
  }
  data += "time=";
  data += timeStamp;
  data += "&";
  data += "priority=";
  data += priority;
  data += "&";
  data += "message=";
  data += message;
  //espSendPostRequest(url, data);
  /// Testing with get request ///
  espSendGetRequest(url);
  url = url + "?" + data;
}

void serverSendSensorData(byte sensorId, long timeStamp, int value){
  _ServerRequestType = SERVER_REQUEST_SENSOR_DATA;
  String url = "http://"+serverGetAdress()+"/board";
  String data = "cmd=measure";
  data += "&";
  data += "sid=" + String(sensorId);
  data += "&";
  data += "time=";
  data += String(timeStamp);
  data += "&";
  data += "value=";
  data += String(value);
  //espSendPostRequest(url, data);
  ///// Testing with get request /////
  url = url + "?" + data;
  espSendGetRequest(url);
}


void serverSendPing(){
  _ServerRequestType = SERVER_REQUEST_PING;
  String url = "http://"+serverGetAdress()+"/board?";
  url += "cmd=ping";
  _ServerConnectionStatusTime = millis();
}


void serverReconnect(){
  if(_WifiConnectionStatus!=CONNECTION_STATUS_CONNECTED){
    return;
  }
  if(_ServerConnectionStatus!=SERVER_CONNECTION_STATUS_CONNECTED){
    serverSendPing();
  }
  else if(millis() - _ServerConnectionStatusTime > SERVER_TIME_TO_RECONNECT){
    serverSendPing();
  }
}
String serverGetAdress(){
  //TODO: Add a server adress read function, add server eeprom read function
  return "hamama.ew.r.appspot.com";
}

String serverSetAdress(){
  //Idk if i need that but if i am, to do add a eeprom write function for the server address.
}


void serverSetAdress(String value){
  //TODO: save Adress to EEPROM memory
}

void serverNotifyError(int errorID, String error, int priorty){
  
}

void serverRandomTest(){
  Serial2.print("https://httpbin.org/status/200");
}
