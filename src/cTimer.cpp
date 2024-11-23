// src: QNX timer tutorial code
#include "cTimer.h"

// timer constructor
cTimer::cTimer(uint32_t sec, uint32_t msec) {

	channel_id = ChannelCreate(0);
	connection_id = ConnectAttach(0, 0, channel_id, 0, 0);
	if (connection_id == -1) {
		std::cerr << "Timer, Connect Attach error : " << errno << "\n";
	}

	SIGEV_PULSE_INIT(&sig_event, connection_id, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

	if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1) {
		std::cerr << "Timer, Init error : " << errno << "\n";
	}

	setTimerSpec(sec, 1000000 * msec);

	cycles_per_sec = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
}

cTimer::~cTimer() {

}

// starts the timer
void cTimer::startTimer() {
	// 0 --> relative timer (vs TIMER_ABSTIME for absolute)
	timer_settime(timer_id, 0, &timer_spec, NULL);
}

// configures the timer period
void cTimer::setTimerSpec(uint32_t sec, uint32_t nano) {
	timer_spec.it_value.tv_sec = sec;
	timer_spec.it_value.tv_nsec = nano;
	timer_spec.it_interval.tv_sec = sec;
	timer_spec.it_interval.tv_nsec = nano;
	timer_settime(timer_id, 0, &timer_spec, NULL);
}

// blocks until a message/event is received
void cTimer::waitTimer() {
	MsgReceive(channel_id, &msg_buffer, sizeof(msg_buffer), NULL);
}

// initialize start of counter
double cTimer::tick() {
	tick_cycles = ClockCycles();
	return ((double) tick_cycles / cycles_per_sec);
}

// stop counter and return CCs since last start
double cTimer::tock() {
	tock_cycles = ClockCycles();
	return ((double) tock_cycles - tick_cycles) / cycles_per_sec;
}
