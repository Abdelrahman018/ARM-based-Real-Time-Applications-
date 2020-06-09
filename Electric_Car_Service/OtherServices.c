#include "OtherServices.h"



uint8_t* itoaTwoNum(uint32_t value) 
{ 
  static int8_t BufferMsg[10] = {0};
	if(value > 9)
		sprintf(BufferMsg, "%d",value);
	else
		sprintf(BufferMsg, "0%d",value);
        return BufferMsg; 
}	