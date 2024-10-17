#include <iostream>
#include <csignal>
#include <ctime>
#include <cstring>
#include <unistd.h>

class Clock{

	Clock();

	void setTimer();
	void waitTimer();
	void startTimer();
	double Tik();
	double Tok();



private:
	timer_t timer_id;
	struct sigevent sev;
	struct itimerspec its;



};
