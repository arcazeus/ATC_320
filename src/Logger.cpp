/*
 * Logger.cpp
 *
 *  Created on: Nov. 22, 2024
 *      Author: thoma
 */

#include "Logger.h"
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <ostream>

Logger::Logger() {
	// TODO Auto-generated constructor stub
	// Register with the name service here
	attach = name_attach(NULL, "LoggerServer", 0);
	if (attach == NULL) {
		std::cerr
				<< "Error: Failed to register Logger with name service in constructor!"
				<< std::endl;
		// Handle error appropriately (e.g., throw exception or set a flag)
	} else {
		std::cout << "Logger registered with name service in constructor."
				<< std::endl;
	}
}

void Logger::log_AirSpace(const char *filename, const char *change) {
	openlog("FileMonitor", LOG_PID | LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "File %s changed: %s", filename, change);
	closelog();
}

void Logger::log_OperatorCommand(const char *filename, const char *change) {
	openlog("FileMonitor", LOG_PID | LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "File %s changed: %s", filename, change);
	closelog();
}
Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

