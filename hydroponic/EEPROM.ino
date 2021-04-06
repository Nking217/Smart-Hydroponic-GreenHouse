////////////////////////////////////
/// EEPROM Memory write and read ///
////////////////////////////////////
#include <EEPROM.h>

#define EEPROM_ESP_SSID 0
#define EEPROM_ESP_PASSWORD 1
#define EEPROM_ESP_CONNECTION_TIMEOUT 2
#define EEPROM_ESP_SSID_LENGTH 32
#define EEPROM_ESP_PASSWORD_LENGHT 30
#define EEPROM_ESP_CONNECTION_TIMEOUT_LENGHT 6

class AddressInfo
{
  public:
  int Address;
  int Length;
};

AddressInfo _Addresses[3];

void eepromInit(){
  int address = 0;
  _Addresses[EEPROM_ESP_SSID] = {address, EEPROM_ESP_SSID_LENGTH};  //SSID: address 0 length 32
  address += EEPROM_ESP_SSID_LENGTH;
  _Addresses[EEPROM_ESP_PASSWORD] = {address, EEPROM_ESP_PASSWORD_LENGHT}; //Password: address 32, length 30
  address += EEPROM_ESP_PASSWORD_LENGHT;
  _Addresses[EEPROM_ESP_CONNECTION_TIMEOUT] = {address, EEPROM_ESP_CONNECTION_TIMEOUT_LENGHT};  //timeout: address 62, length 6
}

void eepromWrite(byte address, String value){
  AddressInfo a = _Addresses[address];
  int currentAddress = a.Address;
  for (int i = 0; i < a.Length; i++)
  {
    EEPROM.write(currentAddress++, value[i]);
    if (value[i] == '\0') break;
  }
}

void eepromWrite(byte address, int value){
  byte byte1 = value >> 8;
  byte byte2 = value & 0xFF;
  
  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);
}


int eepromReadInt(byte address){
  AddressInfo a = _Addresses[address];
  int currentAddress = a.Address;

  byte byte1 = EEPROM.read(currentAddress);
  byte byte2 = EEPROM.read(currentAddress + 1);
  return (byte1 << 8) + byte2;
}

void eepromReadString(byte address, String buffer){
  AddressInfo a = _Addresses[address];
  int currentAddress = a.Address;
  for (int i = 0; i < a.Length; i++)
  {
    byte data = EEPROM.read(currentAddress++);
    buffer[i] = (char)data;
    if(data == 0) break;
  }
}
