// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer

#include <stdint.h>
// --UUU-- Declare state variables for FiFo
//        size, buffer, put and get indexes
int put,get,size;
char buffer[16]; // change to 16

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init() {
// --UUU-- Complete this
	put=get =size= 15;
	//FIFOmail = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {
	if ( ((put-1)==get) || (put==0&&get==15) ){return(0);}
	if(put==0){put=16;}
   buffer[put-1] = data;
   put = (put-1);
	if(put ==0) {put=16;}
   return(1);

}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(char *datapt){
	if (get == put) {
          return(0);
   }
   *datapt = buffer[get];
   get = (get-1);
	 if(get==-1)get=15;
   return(1);

}
