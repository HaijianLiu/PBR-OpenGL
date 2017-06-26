#ifndef timecontrol_hpp
#define timecontrol_hpp


/*----------------------------------------------------------------------------*/
void initTime();
/*------------------------------------------------------------------------------
< Initialize Program Start Time >

Initialize program start time by current time (not time zone)
Store to struct timeval startTime
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
float currentTime();
/*------------------------------------------------------------------------------
< Current Time to Program Start Time >

................................................................................
Return:

float : count from startTime to now ( eg. 7890.098765s )
------------------------------------------------------------------------------*/


#endif /* timecontrol_hpp */
