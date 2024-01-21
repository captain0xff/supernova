#ifndef SUPERNOVA_LOGGING_H
#define SUPERNOVA_LOGGING_H


#include <sstream>
#include <string>

#include <SDL2/SDL_log.h>



// Functions
template <typename Arg, typename... Args>
std::string log_to_string(Arg&& arg, Args&&... args) {
	std::stringstream buffer;
	buffer << std::forward<Arg>(arg);
	((buffer << ' ' << std::forward<Args>(args)), ...);

	return buffer.str();
}

template <typename Arg, typename... Args>
void log(Arg&& arg, Args&&... args) {
	SDL_Log("%s", log_to_string(arg, args...).c_str());
}

template <typename Arg, typename... Args>
void log_error(Arg&& arg, Args&&... args) {
	SDL_LogError(0, "%s", log_to_string(arg, args...).c_str());
}

template <typename Arg, typename... Args>
void log_warn(Arg&& arg, Args&&... args) {
	SDL_LogWarn(0, "%s", log_to_string(arg, args...).c_str());
}

#endif /* SUPERNOVA_LOGGING_H */
