/*
 * Logger.h
 *
 *  Created on: Nov. 22, 2024
 *      Author: thoma
 */

#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>


#ifndef ATC_320_SRC_LOGGER_H_
#define ATC_320_SRC_LOGGER_H_

class Logger {
public:
	Logger();
	virtual ~Logger();

	void log_AirSpace(const char *filename, const char *change);
	void log_OperatorCommand(const char *filename, const char *change);

private:

	name_attach_t* attach;
};

#endif /* ATC_320_SRC_LOGGER_H_ */
