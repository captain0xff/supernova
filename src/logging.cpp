#include "logging.h"



// Globals
static int log_state = INFO|WARN|ERROR;

int get_log_state() {
	return log_state;
}

void set_log_state(const int log_level, const bool enable) {
	if (enable)
		log_state |= log_level;
	else
		log_state &= ~log_level;
}

void set_log_level(const int log_level) {
	for (int i = 1; i <= INFO; i *= 2) {
		if (i <= log_level)
			set_log_state(i);
		else
			set_log_state(i, false);
	}
}
