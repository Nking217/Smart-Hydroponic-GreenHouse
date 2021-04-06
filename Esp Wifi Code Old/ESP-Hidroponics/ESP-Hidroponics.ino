#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define CONNECTION_STATUS_WAITING 0
#define CONNECTION_STATUS_CONNECTING 1
#define CONNECTION_STATUS_CONNECTED 2
#define CONNECTION_STATUS_TIMEOUT 3
#define CONNECTION_STATUS_ERROR 4
#define CONNECTION_STATUS_MISSING_INFO 5

#define STATE_IDLE 0
#define STATE_CONNECT 1
#define STATE_SEND_GET_REQUEST 2
#define STATE_SEND_POST_REQUEST 3

#define ESP_ERROR_NOT_CONNECTED_TO_WIFI -20

#define COMMAND_PREFIX '~'
#define END_OF_COMMAND_STRING '$'
#define END_OF_RETURN_STRING '\0'

bool _DebugState = false;
//const char* ssid = "Sulam";
//const char* password = "2508572511";
//const char* ssid = "TP-LINK_AFE1";
//const char* password = "25970340";

byte _State = STATE_IDLE;
int _ResultCode;

String _ConnectParamsSSID;
String _ConnectParamsPassword;
String _ConnectParamsTimeoutStr;
int _ConnectParamsTimeout;
bool _ConnectParamsSSIDFull;
bool _ConnectParamsPasswordFull;
bool _ConnectParamsTimeoutStrFull;

String _RequestParamsUrl;
String _RequestParamsData;
String _RequestParamsReqid;
String _RequestParamsReqidFull;
bool _RequestParamsUrlFull;
bool _RequestParamsDataFull;

byte _ConnectionStatus = CONNECTION_STATUS_WAITING;
long _ConnectionStartTime;
long _ConnectionLastAtemptTime;


void setup () 
{
  Serial.begin(9600);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default, would try to act as both a client and an access-point and could cause network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  /* Configure module NOT to automatically connect on power on to the last used access point. */
  WiFi.setAutoConnect(false);
}

void loop() 
{ 
  //if trying to connect to wifi - continue trying
  if (_ConnectionStatus == CONNECTION_STATUS_CONNECTING || 
      _ConnectionStatus == CONNECTION_STATUS_ERROR) 
  {
    ReconnectToWifi();
  }
  
  if (Serial.available()) //check if the esp recive input from arduino
  {
    //read the next byte from the buffer
    byte incomingByte = Serial.read();

    // say what you got:
    /*
    Serial.print("I received: ");
    Serial.print(incomingByte, DEC);
    Serial.print("(");
    Serial.print(char(incomingByte));
    Serial.println(")");
    */
    
    if (incomingByte == COMMAND_PREFIX)
    {
      //no matter what happened until now...
      //we now will begin to read a new command 
      //(starting from the nect input char)
      SetState(STATE_IDLE);
    }
    else
    {
      switch (_State)
      {
        case STATE_IDLE:
          HandleCommandRequest(incomingByte);
          break;
          
        case STATE_CONNECT:
          //if waiting for connection arguments
          if (_ConnectionStatus == CONNECTION_STATUS_WAITING)
          {
            //read the arguments  
            if (ReadConnectArgs(incomingByte))
            {
              //if all argiments are OK then try to connect to WIFI using the arguments
              ConnectToWifi();
              SetState(STATE_IDLE); //finished connection
            }
          }
          else if (_ConnectionStatus == CONNECTION_STATUS_CONNECTING)
          {
            ReconnectToWifi();
          }
          else
          {
            DisconnectWifi();
          }
          break;

        case STATE_SEND_GET_REQUEST:
          if (ReadGetArgs(incomingByte))
          {
            SendGetRequest();
            SetState(STATE_IDLE);
          }
          break;

        case STATE_SEND_POST_REQUEST:
          if (ReadPostArgs(incomingByte))
          {
            SendPostRequest();
            SetState(STATE_IDLE);
          }
          break;
          
      }
    }

  }

 
}
///////////////////////////////////
///   Handle command requests   ///
///////////////////////////////////

void HandleCommandRequest(byte incomingByte)
{
  switch(incomingByte)
  {
    //connect
    //~c<SSID>$<password>$<timeout>$
    case 'c':
    case 'C':
      {
        DebugPrintln("handling connect request");
        InitReadConnectArgs();
        DisconnectWifi();
        SetState(STATE_CONNECT);
      }
      break;

    //disconnect
    //~d
    case 'd':
    case 'D':
      {
        DebugPrintln("handling disconect request");
        DisconnectWifi();
        SetState(STATE_IDLE);
      }
      break;
            
    //GET request
    //~g<request url>$
    case 'g':
    case 'G':
      {
        DebugPrintln("handling Get request");
        InitReadGetArgs();
        SetState(STATE_SEND_GET_REQUEST);
      }
      break;
      
    //POST request
    //~p<request url>$<data>$     
    case 'p':
    case 'P':
      {
        DebugPrintln("handling Post request");
        InitReadPostArgs();
        SetState(STATE_SEND_POST_REQUEST);
        break;
      }

    //get IP address
    //~a
    case 'a':
    case 'A':
      {
        Serial.println("IP:");
        IPAddress ip = WiFi.localIP();
        Serial.print(ip);
        Serial.print(END_OF_RETURN_STRING);
        SetState(STATE_IDLE);
      }
      break;

    //get status
    //~s
    case 's':
    case 'S':
      {
      /*  Serial.println("Status:");
        byte connectionStatus = GetConnectionStatus();
        if (connectionStatus == CONNECTION_STATUS_CONNECTED)
        {
          Serial.print("SSID = ");
          Serial.println(WiFi.SSID());
        } */
        printConnectionStatus();
        SetState(STATE_IDLE);
      }
      break;

    //set debug mode on
    //~b
    case 'b':
    case 'B':
      {
        _DebugState = true;
        DebugPrintln("Debug mode on");
      }
      break;

    //set debug mode off
    //~e
    case 'e':
    case 'E':
      {
        DebugPrintln("Debug mode off");
        _DebugState = false;
      }
      break;  
  }
}

//////////////////////////////////////
///   read GET request arguments   ///
//////////////////////////////////////
void InitReadGetArgs()
{
  _RequestParamsUrl = "";
  _RequestParamsUrlFull = false;
  _RequestParamsReqid = "";
  _RequestParamsReqidFull = false;
}

bool ReadGetArgs(byte incomingByte)
{
    //convert data from byte to char type
    char c = char(incomingByte);
    
    if(!_RequestParamsReqidFull) //check if the reqid isn't full
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _RequestParamsReqidFull = true;
            DebugBegin();
            DebugPrint("POST reqid = ");
            DebugPrint(_RequestParamsReqid);
            DebugEnd();
        }
        else
        {
            //add the char to the string of the reqid
            _RequestParamsReqid += c;
        }
    }
    else if(!_RequestParamsUrlFull) //check if the url isn't full
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _RequestParamsUrlFull = true;
            DebugBegin();
            DebugPrint("GET url = ");
            DebugPrint(_RequestParamsUrl);
            DebugEnd();
        }
        else
        {
            //add the char to the string of the url
            _RequestParamsUrl += c;
        }
    }

    //if we have all the data then return true
    if (_RequestParamsUrlFull)
        return true;
    else
        return false;
}

void SendGetRequest()
{
  //Check WiFi connection status
  if (!IsConnected())
  {
    printConnectionStatus();
    return;
  }
  
  HTTPClient http;  //Declare an object of class HTTPClient

  String url = _RequestParamsUrl;
  String reqid = _RequestParamsReqid;
  http.begin(url);  //Specify request destination
  http.addHeader("REQID", reqid);
  int httpCode = http.GET();      //Send the request

  //print status code
  _ResultCode = httpCode;
  Serial.print("RESAULT_STATUS:");
  PrintStatusCode(httpCode);
  
  if (httpCode > 0) //Check the returning code
  { 
    String retrunReqid = http.header("REQID");
    Serial.print("RESAULT_REQID:");
    Serial.println(retrunReqid);
    
    String payload = http.getString();   //Get the request response payload
    
    if (httpCode == t_http_codes::HTTP_CODE_OK)
    {
      Serial.print("PAYLOAD:");
      Serial.println(payload);             //Print the response payload
    }
    else
    {
      Serial.println("HEADERS:");
      for(int i = 0; i < http.headers(); i++)
      {
        Serial.println(http.header(i));
      }
      Serial.println("PAYLOAD:");
      Serial.println(payload);
    }
  }
  
  Serial.print(END_OF_RETURN_STRING);
  http.end();   //Close connection
}

///////////////////////////////////////
///   read POST request arguments   ///
///////////////////////////////////////
void InitReadPostArgs()
{
  _RequestParamsUrl = "";
  _RequestParamsData = "";
  _RequestParamsReqid = "";
  _RequestParamsUrlFull = false;
  _RequestParamsDataFull = false;
  _RequestParamsReqidFull = false;
}

bool ReadPostArgs(byte incomingByte)
{
    //convert data from byte to char type
    char c = char(incomingByte);
    if(!_RequestParamsReqidFull) //check if the reqid isn't full
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _RequestParamsReqidFull = true;
            DebugBegin();
            DebugPrint("POST reqid = ");
            DebugPrint(_RequestParamsReqid);
            DebugEnd();
        }
        else
        {
            //add the char to the string of the url
            _RequestParamsReqid += c;
        }
    }
    else if(!_RequestParamsUrlFull) //check if the url isn't full
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _RequestParamsUrlFull = true;
            DebugBegin();
            DebugPrint("POST url = ");
            DebugPrint(_RequestParamsUrl);
            DebugEnd();
        }
        else
        {
            //add the char to the string of the url
            _RequestParamsUrl += c;
        }
    }
    else if(!_RequestParamsDataFull)
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _RequestParamsDataFull = true;
            DebugBegin();
            DebugPrint("POST data = ");
            DebugPrint(_RequestParamsData);
            DebugEnd();
        }
        else
        {
            //add the char to the string of the data
            _RequestParamsData += c;
        }
    }

    //if we have all the data then return true
    if (_RequestParamsUrlFull && 
        _RequestParamsDataFull)
        return true;
    else
        return false;
}

void SendPostRequest()
{
  //Check WiFi connection status
  if (!IsConnected())
  {
    printConnectionStatus();
    return;
  }
  
  HTTPClient http;  //Declare an object of class HTTPClient

  String url = _RequestParamsUrl;
  String data = _RequestParamsData;
  String reqid = _RequestParamsReqid;
  http.begin(url);  //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
  http.addHeader("REQID", reqid);
  int httpCode = http.POST(data);   //Send the request

  //print status code
  _ResultCode = httpCode;
  Serial.print("RESAULT_STATUS:");
  PrintStatusCode(httpCode);
  
  if (httpCode > 0) //Check the returning code
  { 
    String retrunReqid = http.header("REQID");
    Serial.print("RESULT_REQID:");
    Serial.println(retrunReqid);
    
    String payload = http.getString();   //Get the request response payload
    
    if (httpCode == t_http_codes::HTTP_CODE_OK)
    {
      Serial.print("PAYLOAD:");
      Serial.println(payload);             //Print the response payload
    }
    else
    {
      Serial.println("HEADERS:");
      for(int i = 0; i < http.headers(); i++)
      {
        Serial.println(http.header(i));
      }
      Serial.println("PAYLOAD:");
      Serial.println(payload);
      
    } 
  }
  
  Serial.print(END_OF_RETURN_STRING);
  http.end();   //Close connection
}

//////////////////////////////////////////
///   read wifi connection arguments   ///
//////////////////////////////////////////
void InitReadConnectArgs()
{
  _ConnectParamsSSID = "";
  _ConnectParamsPassword = "";
  _ConnectParamsTimeoutStr = "";
  _ConnectParamsTimeout = 0;
  _ConnectParamsSSIDFull = false;
  _ConnectParamsPasswordFull = false;
  _ConnectParamsTimeoutStrFull = false;
}

bool ReadConnectArgs(byte incomingByte)
{
    //convert data from byte to char type
    char c = char(incomingByte);

    if(!_ConnectParamsSSIDFull) //check if the name of the wifi isn't full
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _ConnectParamsSSIDFull = true;
            DebugBegin();
            DebugPrint("SSID = ");
            DebugPrint(_ConnectParamsSSID);
            DebugEnd();
            if (_ConnectParamsSSID.length() == 0)
              _ConnectionStatus = CONNECTION_STATUS_MISSING_INFO;
        }
        else
        {
            //add the char to the string of the SSID
            _ConnectParamsSSID += c;
        }
    }
    else if(!_ConnectParamsPasswordFull)
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _ConnectParamsPasswordFull = true;
            DebugBegin(); 
            DebugPrint("password = ");
            DebugPrint(_ConnectParamsPassword);
            DebugEnd();
            if (_ConnectParamsPassword.length() == 0)
              _ConnectionStatus = CONNECTION_STATUS_MISSING_INFO;
        }
        else
        {
            //add the char to the string of the password
            _ConnectParamsPassword += c;
        }
    }
    else if(!_ConnectParamsTimeoutStrFull)
    {
        if(c == END_OF_COMMAND_STRING) //End of string
        {
            _ConnectParamsTimeoutStrFull = true;
            DebugBegin();
            DebugPrint("timeout = ");
            DebugPrint(_ConnectParamsTimeoutStr);
            DebugEnd();
            if (_ConnectParamsTimeoutStr.length() == 0 || !IsValidNumber(_ConnectParamsTimeoutStr))
              _ConnectionStatus = CONNECTION_STATUS_MISSING_INFO;
            else
              _ConnectParamsTimeout = _ConnectParamsTimeoutStr.toInt(); //timeout in seconds
        }
        else
        {
            //add the char to the string of the timeout
            _ConnectParamsTimeoutStr += c;
        }
    }

    //if we have all the data then return true
    if (_ConnectParamsSSIDFull && 
        _ConnectParamsPasswordFull && 
        _ConnectParamsTimeoutStrFull &&
        _ConnectionStatus != CONNECTION_STATUS_MISSING_INFO)
        return true;
    else
        return false;
}



///////////////////////////
///   Connect to wifi   ///
///////////////////////////

bool DisconnectWifi()
{
  _ConnectionStatus = CONNECTION_STATUS_WAITING;
  WiFi.disconnect();
  printConnectionStatus();
}

//log in to wifi network
//use the _ConnectParamsSSID, _ConnectParamsPassword and _ConnectParamsTimeout
//returns true if succeeded
bool ConnectToWifi()
{
  _ConnectionStartTime = millis();
  _ConnectionLastAtemptTime = millis();
  _ConnectionStatus = CONNECTION_STATUS_CONNECTING;

  //connect to WIFI
  PrintConnecting();
  WiFi.begin(_ConnectParamsSSID, _ConnectParamsPassword); //start connecting to wifi

  //check if wifi is connected
  if (IsConnected()) 
  {
    _ConnectionStatus = CONNECTION_STATUS_CONNECTED;
    printConnectionStatus();
    return true;
  }
  else
  {
    return false; 
  }
}

//continue trying to reconnect to wifi
//use the _ConnectSSID, _ConnectPassword and _ConnectTimeout
//returns true if succeeded
bool ReconnectToWifi()
{
  long now = millis();

  //if not tried to connect yet
  //and enough time has passed since the last atempt to connect
  //then try to reconnect
  if (
        (_ConnectionStatus == CONNECTION_STATUS_WAITING ||  //waiting to connect
         _ConnectionStatus == CONNECTION_STATUS_TIMEOUT ||  //last time was not successful 
         _ConnectionStatus == CONNECTION_STATUS_ERROR) &&   //there was an error 
        (now - _ConnectionLastAtemptTime >= 500)
     )
  {
    if (_ConnectParamsSSIDFull && 
        _ConnectParamsPasswordFull && 
        _ConnectParamsTimeoutStrFull &&
        _ConnectionStatus != CONNECTION_STATUS_MISSING_INFO)
    {
      _ConnectionStatus = CONNECTION_STATUS_MISSING_INFO;
      printConnectionStatus();
      return false;
    }
        
    //connect to WIFI
    PrintConnecting();
    WiFi.begin(_ConnectParamsSSID, _ConnectParamsPassword); //start connecting to wifi
    _ConnectionLastAtemptTime = millis();
    _ConnectionStatus = CONNECTION_STATUS_CONNECTING;
  }

  if (WiFi.status() == WL_CONNECTED) 
  {
    if(CheckIP())
    {
      _ConnectionStatus = CONNECTION_STATUS_CONNECTED;
      printConnectionStatus();      
      return true;
    }
    else
    {
      _ConnectionStatus = CONNECTION_STATUS_ERROR;
      printConnectionStatus();
      return false;
    }
  }

  //check timeout
  if (_ConnectParamsTimeout > 0) //if there is timeout defined
  {
    if(now - _ConnectionStartTime >= _ConnectParamsTimeout*1000)
    {
      _ConnectionStatus = CONNECTION_STATUS_TIMEOUT;
      WiFi.disconnect();
      printConnectionStatus();
    }
  }

  return false;
}





IPAddress GetLocalIp()
{
    return WiFi.localIP();
}

bool IsConnected()
{
  return GetConnectionStatus() == CONNECTION_STATUS_CONNECTED;
}

byte GetConnectionStatus()
{
    if (_ConnectionStatus == CONNECTION_STATUS_WAITING)
        return CONNECTION_STATUS_WAITING;

    //check if connected
    if (WiFi.status() == WL_CONNECTED && CheckIP())
    { 
        //if we didn't know it was connected - now we know...
        _ConnectionStatus = CONNECTION_STATUS_CONNECTED;
    }
    else
    {
        //if we thought it was connected, then now we know it is not connected
        if (_ConnectionStatus == CONNECTION_STATUS_CONNECTED)
            _ConnectionStatus = CONNECTION_STATUS_ERROR;
    }
    return _ConnectionStatus;
}

String ConnectionStatusToString(byte connectionStatus)
{
  switch (connectionStatus)
  {
    case CONNECTION_STATUS_WAITING: return "WAITING";
    case CONNECTION_STATUS_CONNECTING: return "CONNECTING";
    case CONNECTION_STATUS_CONNECTED: return "CONNECTED";
    case CONNECTION_STATUS_TIMEOUT: return "TIMEOUT";
    case CONNECTION_STATUS_ERROR: return "ERROR";
  }
}

void printConnectionStatus(){
  byte connectionStatus = GetConnectionStatus();
  String str = ConnectionStatusToString(connectionStatus);
  Serial.print("CONNECTION_STATUS:");
  Serial.print(str);
  Serial.print(END_OF_RETURN_STRING);
}

void PrintConnecting()
{
  DebugBegin();
  DebugPrint("Connecting to ");
  DebugPrint(_ConnectParamsSSID);
  DebugPrint("...");
  DebugEnd();
}

void PrintStatusCode(int code)
{
  Serial.print(RsultCodeToString(code));
  Serial.println(END_OF_RETURN_STRING);
}

String RsultCodeToString(int resultCode)
{
  switch (resultCode)
  {
    case ESP_ERROR_NOT_CONNECTED_TO_WIFI: return "ESP_ERROR_NOT_CONNECTED_TO_WIFI";
    case HTTPC_ERROR_CONNECTION_REFUSED: return "HTTPC_ERROR_CONNECTION_REFUSED";
    case HTTPC_ERROR_SEND_HEADER_FAILED: return "HTTPC_ERROR_SEND_HEADER_FAILED";
    case HTTPC_ERROR_SEND_PAYLOAD_FAILED: return "HTTPC_ERROR_SEND_PAYLOAD_FAILED";
    case HTTPC_ERROR_NOT_CONNECTED: return "HTTPC_ERROR_NOT_CONNECTED";
    case HTTPC_ERROR_CONNECTION_LOST: return "HTTPC_ERROR_CONNECTION_LOST";
    case HTTPC_ERROR_NO_STREAM: return "HTTPC_ERROR_NO_STREAM";
    case HTTPC_ERROR_NO_HTTP_SERVER: return "HTTPC_ERROR_NO_HTTP_SERVER";
    case HTTPC_ERROR_TOO_LESS_RAM: return "HTTPC_ERROR_TOO_LESS_RAM";
    case HTTPC_ERROR_ENCODING: return "HTTPC_ERROR_ENCODING";
    case HTTPC_ERROR_STREAM_WRITE: return "HTTPC_ERROR_STREAM_WRITE";
    case HTTPC_ERROR_READ_TIMEOUT: return "HTTPC_ERROR_READ_TIMEOUT";
    case HTTPCLIENT_DEFAULT_TCP_TIMEOUT: return "HTTPCLIENT_DEFAULT_TCP_TIMEOUT";
    case t_http_codes::HTTP_CODE_CONTINUE: return "HTTP_CODE_CONTINUE"; //100
    case t_http_codes::HTTP_CODE_SWITCHING_PROTOCOLS: return "HTTP_CODE_SWITCHING_PROTOCOLS"; //101
    case t_http_codes::HTTP_CODE_PROCESSING: return "HTTP_CODE_PROCESSING"; //102
    case t_http_codes::HTTP_CODE_OK: return "HTTP_CODE_OK"; //200
    case t_http_codes::HTTP_CODE_CREATED: return "HTTP_CODE_CREATED"; //201
    case t_http_codes::HTTP_CODE_ACCEPTED: return "HTTP_CODE_ACCEPTED"; //202
    case t_http_codes::HTTP_CODE_NON_AUTHORITATIVE_INFORMATION: return "HTTP_CODE_NON_AUTHORITATIVE_INFORMATION"; //203
    case t_http_codes::HTTP_CODE_NO_CONTENT: return "HTTP_CODE_NO_CONTENT"; //204
    case t_http_codes::HTTP_CODE_RESET_CONTENT: return "HTTP_CODE_RESET_CONTENT"; //205
    case t_http_codes::HTTP_CODE_PARTIAL_CONTENT: return "HTTP_CODE_PARTIAL_CONTENT"; //206
    case t_http_codes::HTTP_CODE_MULTI_STATUS: return "HTTP_CODE_MULTI_STATUS"; //207
    case t_http_codes::HTTP_CODE_ALREADY_REPORTED: return "HTTP_CODE_ALREADY_REPORTED"; //208
    case t_http_codes::HTTP_CODE_IM_USED: return "HTTP_CODE_IM_USED"; //226
    case t_http_codes::HTTP_CODE_MULTIPLE_CHOICES: return "HTTP_CODE_MULTIPLE_CHOICES"; //300
    case t_http_codes::HTTP_CODE_MOVED_PERMANENTLY: return "HTTP_CODE_MOVED_PERMANENTLY"; //301
    case t_http_codes::HTTP_CODE_FOUND: return "HTTP_CODE_FOUND"; //302
    case t_http_codes::HTTP_CODE_SEE_OTHER: return "HTTP_CODE_SEE_OTHER"; //303
    case t_http_codes::HTTP_CODE_NOT_MODIFIED: return "HTTP_CODE_NOT_MODIFIED"; //304
    case t_http_codes::HTTP_CODE_USE_PROXY: return "HTTP_CODE_USE_PROXY"; //305
    case t_http_codes::HTTP_CODE_TEMPORARY_REDIRECT: return "HTTP_CODE_TEMPORARY_REDIRECT"; //307
    case t_http_codes::HTTP_CODE_PERMANENT_REDIRECT: return "HTTP_CODE_PERMANENT_REDIRECT"; //308
    case t_http_codes::HTTP_CODE_BAD_REQUEST: return "HTTP_CODE_BAD_REQUEST"; //400
    case t_http_codes::HTTP_CODE_UNAUTHORIZED: return "HTTP_CODE_UNAUTHORIZED"; //401
    case t_http_codes::HTTP_CODE_PAYMENT_REQUIRED: return "HTTP_CODE_PAYMENT_REQUIRED"; //402
    case t_http_codes::HTTP_CODE_FORBIDDEN: return "HTTP_CODE_FORBIDDEN"; //403
    case t_http_codes::HTTP_CODE_NOT_FOUND: return "HTTP_CODE_NOT_FOUND"; //404
    case t_http_codes::HTTP_CODE_METHOD_NOT_ALLOWED: return "HTTP_CODE_METHOD_NOT_ALLOWED "; //405
    case t_http_codes::HTTP_CODE_NOT_ACCEPTABLE: return "HTTP_CODE_NOT_ACCEPTABLE"; //406
    case t_http_codes::HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED: return "HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED"; //407
    case t_http_codes::HTTP_CODE_REQUEST_TIMEOUT: return "HTTP_CODE_REQUEST_TIMEOUT"; //408
    case t_http_codes::HTTP_CODE_CONFLICT: return "HTTP_CODE_CONFLICT"; //409
    case t_http_codes::HTTP_CODE_GONE: return "HTTP_CODE_GONE"; //410
    case t_http_codes::HTTP_CODE_LENGTH_REQUIRED: return "HTTP_CODE_LENGTH_REQUIRED"; //411
    case t_http_codes::HTTP_CODE_PRECONDITION_FAILED: return "HTTP_CODE_PRECONDITION_FAILED"; //412
    case t_http_codes::HTTP_CODE_PAYLOAD_TOO_LARGE: return "HTTP_CODE_PAYLOAD_TOO_LARGE"; //413
    case t_http_codes::HTTP_CODE_URI_TOO_LONG: return "HTTP_CODE_URI_TOO_LONG"; //414
    case t_http_codes::HTTP_CODE_UNSUPPORTED_MEDIA_TYPE: return "HTTP_CODE_UNSUPPORTED_MEDIA_TYPE"; //415
    case t_http_codes::HTTP_CODE_RANGE_NOT_SATISFIABLE: return "HTTP_CODE_RANGE_NOT_SATISFIABLE"; //416
    case t_http_codes::HTTP_CODE_EXPECTATION_FAILED: return "HTTP_CODE_EXPECTATION_FAILED"; //417
    case t_http_codes::HTTP_CODE_MISDIRECTED_REQUEST: return "HTTP_CODE_MISDIRECTED_REQUEST"; //421
    case t_http_codes::HTTP_CODE_UNPROCESSABLE_ENTITY: return "HTTP_CODE_UNPROCESSABLE_ENTITY"; //422
    case t_http_codes::HTTP_CODE_LOCKED: return "HTTP_CODE_LOCKED"; //423
    case t_http_codes::HTTP_CODE_FAILED_DEPENDENCY: return "HTTP_CODE_FAILED_DEPENDENCY"; //424
    case t_http_codes::HTTP_CODE_UPGRADE_REQUIRED: return "HTTP_CODE_UPGRADE_REQUIRED"; //426
    case t_http_codes::HTTP_CODE_PRECONDITION_REQUIRED: return "HTTP_CODE_PRECONDITION_REQUIRED"; //428
    case t_http_codes::HTTP_CODE_TOO_MANY_REQUESTS: return "HTTP_CODE_TOO_MANY_REQUESTS"; //429
    case t_http_codes::HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE: return "HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE"; //431
    case t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR: return "HTTP_CODE_INTERNAL_SERVER_ERROR"; //500
    case t_http_codes::HTTP_CODE_NOT_IMPLEMENTED: return "HTTP_CODE_NOT_IMPLEMENTED"; //501
    case t_http_codes::HTTP_CODE_BAD_GATEWAY: return "HTTP_CODE_BAD_GATEWAY"; //502
    case t_http_codes::HTTP_CODE_SERVICE_UNAVAILABLE: return "HTTP_CODE_SERVICE_UNAVAILABLE"; //503
    case t_http_codes::HTTP_CODE_GATEWAY_TIMEOUT: return "HTTP_CODE_GATEWAY_TIMEOUT"; //504
    case t_http_codes::HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED: return "HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED"; //505
    case t_http_codes::HTTP_CODE_VARIANT_ALSO_NEGOTIATES: return "HTTP_CODE_VARIANT_ALSO_NEGOTIATES"; //506
    case t_http_codes::HTTP_CODE_INSUFFICIENT_STORAGE: return "HTTP_CODE_INSUFFICIENT_STORAGE"; //507
    case t_http_codes::HTTP_CODE_LOOP_DETECTED: return "HTTP_CODE_LOOP_DETECTED"; //508
    case t_http_codes::HTTP_CODE_NOT_EXTENDED: return "HTTP_CODE_NOT_EXTENDED"; //510
    case t_http_codes::HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED: return "HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED"; //511
    default: return "UNKNOWN_ERROR";
  }
}

void DebugPrintln(const char *text){
  if (!_DebugState) return;
  Serial.println();
  Serial.print("DEBUG:");
  Serial.print(text);
  Serial.println(END_OF_RETURN_STRING);
}

void DebugBegin()
{
  if (!_DebugState) return;
  Serial.println();
  Serial.print("DEBUG:");
}
void DebugPrint(String text){
  if (!_DebugState) return;
  Serial.print(text.c_str());
}
void DebugPrint(const char *text){
  if (!_DebugState) return;
  Serial.print(text);
}
void DebugPrint(int value){
  if (!_DebugState) return;
  Serial.print(value);
}
void DebugEnd()
{
  if (!_DebugState) return;
  Serial.println(END_OF_RETURN_STRING);
}


void SetState(byte state)
{
  _State = state;

  DebugBegin();
  DebugPrint("Set state to:");
  DebugPrint(_State);
  DebugEnd();
}



bool CheckIP()
{
    IPAddress ip = WiFi.localIP();
    for(int i=0;i<4;i++)
    {
        if(ip[i] != 0)
            return true;
    }
    return false;
}

bool IsValidNumber(String str)
{
   for(byte i=0;i<str.length();i++)
   {
      if(isDigit(str.charAt(i))) return true;
   }
   return false;
} 
