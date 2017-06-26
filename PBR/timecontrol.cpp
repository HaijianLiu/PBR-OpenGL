// Include standard headers
#include <iostream>
#include <sys/time.h>

// Include header file
#include "timecontrol.hpp"

// < Program Start Time >
struct timeval startTime;

// < Initialize Program Start Time >
void initTime() {
	gettimeofday(&startTime,NULL);
}

// < Current Time to Program Start Time >
float currentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (float)(tv.tv_sec-startTime.tv_sec)+(tv.tv_usec-startTime.tv_usec)/1000000.0f;
}
