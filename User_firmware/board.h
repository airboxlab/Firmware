/*
 * board.h
 *
 * Created: 6/3/2014 9:59:08 AM
 *  Author: Tony
 */ 


#ifndef BOARD_H_
#define BOARD_H_

//#define ABL1
//#define ABL2
//#define ABL3	//1002
//#define ABL4
//#define ABL5	//1003
//#define ABL6	
//#define ABL7	//1001
//#define ABL8 
//#define ABL9
//#define ABL10
//#define ABLNAKED //45

#define OLD_BOARD
//#define BOARD_1_2
//#define BOARD_3_4
//#define BOARD_5_6
//#define BOARD_7_8
//#define BOARD_9_10


#ifdef OLD_BOARD
	#define IAQ				
	//#define MQ135			
#endif
#ifdef BOARD_1_2
	#define IAQ
	#define SENSOR_1		 
#endif
#ifdef BOARD_3_4
	#define IAQ
	#define SENSOR_1		
	#define SENSOR_2		 
#endif
#ifdef BOARD_5_6
	#define VZ87			
	#define IAQ			   
#endif
#ifdef BOARD_7_8
	#define SENSOR_1		
	#define	IAQ			  
#endif
#ifdef BOARD_9_10
	#define SENSOR_2		
	#define IAQ			    
#endif

#endif /* BOARD_H_ */