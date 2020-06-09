#ifndef __UDDT_H__
#define __UDDT_H__

typedef struct PS2_DATA_T{
	unsigned char Select		 			:1;
	unsigned char Joy_L 					:1;
	unsigned char Joy_R 					:1;
	unsigned char Start 					:1;
	unsigned char UpArrow			 		:1;
	unsigned char RightArrow 			:1;
	unsigned char DownArrow		 		:1;
	unsigned char LeftArrow 			:1;
	unsigned char L2 							:1;
	unsigned char R2 							:1;
	unsigned char L1 							:1;
	unsigned char R1 							:1;
	unsigned char Triangle	 			:1;
	unsigned char Circle		 			:1;
	unsigned char Cross			 			:1;
	unsigned char Square		 			:1;
	unsigned char RJoy_X		 				;
	unsigned char RJoy_Y		 				;
	unsigned char LJoy_X		 				;
	unsigned char LJoy_Y		 				; }
PS2_data_t;
 
#endif