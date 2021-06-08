#include "queue.h"

//https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/

//TODO:Queue should be of static sized char arrays.
//TODO: define queue size and string static size
Queue::Queue(int queueSize, int stringSize) 
{ 
  _Front = _Rear = -1; 
  _QueueSize = queueSize;
  _StringSize = stringSize;
      
  _Arr = new char*[queueSize];
  for(int i = 0; i < queueSize; i++)
  {
    _Arr[i] = new char[stringSize];
  }
} 

Queue::~Queue()
{
  for(int i = 0; i < _QueueSize; i++) 
  {
    delete[] _Arr[i]; // delete the 2nd dimension array
  }
  delete[] _Arr; // delete array itself
}


bool Queue::IsFull()
{
  return ((_Front == 0 && _Rear == _QueueSize-1) || 
          (_Rear == (_Front-1)%(_QueueSize-1)));
}

bool Queue::IsEmpty()
{
  return _Front == -1;
}

//returns how many empty cells we have
int Queue::GetEmptySpace()
{
  if (_Front == -1 && _Rear == -1)
    return _QueueSize;
  else if (_Front == _Rear)
    return _QueueSize-1;
  else if (_Front < _Rear) 
    return _QueueSize - (_Rear - _Front + 1);
  else 
    return _Front - _Rear - 1; 
}

// Function to add to Circular queue
bool Queue::enQueue(char* value)
{
  return Queue::enQueue((char*)NULL, (char*)value, (char*)NULL, (char*)NULL, (char*)NULL, (char*)NULL);
}

// Function to add to Circular queue
bool Queue::enQueue(char* value1, char* value2, char* value3, char* value4, char* value5, char* value6)
{ 
  if (IsFull())   /* queue is full */
  {
    return false;
  }
  else if (_Front == -1) /* Insert First Element */
  { 
    _Front = _Rear = 0;
  } 
  else if (_Rear == _QueueSize-1 && _Front != 0) 
  { 
    _Rear = 0; 
  } 
  else
  { 
    _Rear++; 
  } 
  Queue::copy(_Arr[_Rear], value1, value2, value3, value4, value5, value6);
}

void Queue::copy(char* dest, char* value1, char* value2, char* value3, char* value4, char* value5, char* value6)
{
  if (value1 != NULL || value1[0] != '/0')
  {
    strcpy(dest, value1);
    dest = dest + strlen(value1);  
  }
  if (value2 != NULL || value2[0] != '/0')
  {
    strcpy(dest, value2);
    dest = dest + strlen(value2);  
  }
  if (value3 != NULL || value3[0] != '/0')
  {
    strcpy(dest, value3);
    dest = dest + strlen(value3);  
  }
  if (value4 != NULL || value4[0] != '/0')
  {
    strcpy(dest, value4);
    dest = dest + strlen(value4);  
  }
  if (value5 != NULL || value5[0] != '/0')
  {
    strcpy(dest, value5);
    dest = dest + strlen(value5);  
  }
  if (value6 != NULL || value6[0] != '/0')
  {
    strcpy(dest, value6);
    dest = dest + strlen(value6);  
  }  
  dest[0] = '\0';
}

// Function to return and delete element from Circular Queue 
bool Queue::deQueue(char* data) 
{ 
  if (IsEmpty())  /* queue is empty */
  { 
    return false;
  } 
  
  char* temp = _Arr[_Front]; 
  _Arr[_Front][0] = '\0';
  if (_Front == _Rear) 
  { 
    _Front = -1; 
    _Rear = -1; 
  } 
  else if (_Front == _QueueSize-1) 
  {
    _Front = 0; 
  }
  else
  {
    _Front++; 
  }
  
  strcpy(data, temp);
  return true;
}

/*
void Queue::print()
{
  for (int i = 0; i < SERIAL_SEND_QUEUE_SIZE; i++)
  {
    Serial.print(i);
    Serial.print(":");
    for (int j = 0; j < SERIAL_SEND_PACKET_SIZE; j++)
    {
      Serial.print(_Arr[i][j]);
    }
    if (i == _Front && i == _Rear)
      Serial.print("   <- front+rear");
    else if (i == _Front)
      Serial.print("   <- front");
    else if (i == _Rear)
      Serial.print("   <- rear");
    Serial.println();
  }
  Serial.print("empty space = ");
  Serial.println(GetEmptySpace());
  Serial.print("is empty = ");
  Serial.println(IsEmpty());
  Serial.print("is full = ");
  Serial.println(IsFull());
  Serial.println("--------------");
}
*/
