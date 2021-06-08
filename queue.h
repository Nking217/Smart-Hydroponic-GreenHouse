#ifndef QUEUE_H
#define QUEUE_H

#include <string.h>

class Queue 
{
  //https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
  
	public:
		Queue(int queueSize, int stringSize);
    ~Queue();
		bool IsFull();
		bool IsEmpty();

		//returns how many empty cells we have
		int GetEmptySpace();
		
    // Function to add to Circular queue
    bool enQueue(char* value);
    bool enQueue(char* value1, char* value2, char* value3, char* value4, char* value5, char* value6);
		
    // Function to return and delete element from Circular Queue 
    bool deQueue(char* data);

    void copy(char* dest, char* value1, char* value2, char* value3, char* value4, char* value5, char* value6);

	private:
		// Initialize front and rear 
		int _Rear;
		int _Front; 
	  
		// Circular Queue 
		int _QueueSize; 
    int _StringSize;
		char **_Arr; 
};


#endif 
