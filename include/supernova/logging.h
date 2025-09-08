#ifndef SUPERNOVA_LOGGING_H
#define SUPERNOVA_LOGGING_H


#include <sstream>
#include <string>
#include <format>

#include <SDL3/SDL_log.h>

#include "enums.h"



// Structs
struct LogArgs {
	std::string sep = " ", pre = "";
};



// Functions
int get_log_state();
void set_log_state(const int log_level, const bool enable=true);
void set_log_level(const int log_level);


template <typename Arg, typename... Args>
std::string log_to_string(Arg&& arg, Args&&... args) {
	std::stringstream buffer;
	buffer << std::forward<Arg>(arg);
	((buffer << ' ' << std::forward<Args>(args)), ...);

	return buffer.str();
}

template <typename Arg, typename... Args>
std::string log_to_string(const LogArgs &log_args, Arg&& arg, Args&&... args) {
	std::stringstream buffer;
	buffer << log_args.pre;
	buffer << std::forward<Arg>(arg);
	((buffer << log_args.sep << std::forward<Args>(args)), ...);

	return buffer.str();
}


template <typename Arg, typename... Args>
void log_info(Arg&& arg, Args&&... args) {
	if (get_log_state() & INFO)
		SDL_Log("%s", log_to_string({}, arg, args...).c_str());
}

template <typename Arg, typename... Args>
void log_info(const LogArgs &log_args, Arg&& arg, Args&&... args) {
	if (get_log_state() & INFO)
		SDL_Log("%s", log_to_string(log_args, arg, args...).c_str());
}

template<typename ...A>
void flog_info(std::format_string<A...> fmt, A&&...args){
	if (get_log_state() & INFO)
			SDL_Log("%s", format(fmt, std::forward<A>(args)...).c_str());
}


template <typename Arg, typename... Args>
void log_error(Arg&& arg, Args&&... args) {
	if (get_log_state() & ERROR) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"%s",log_to_string({}, arg, args...).c_str()
		);
	}
}

template <typename Arg, typename... Args>
void log_error(const LogArgs &log_args, Arg&& arg, Args&&... args) {
	if (get_log_state() & ERROR) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION,
			"%s",log_to_string(log_args, arg, args...).c_str()
		);
	}
}

template<typename ...A>
void flog_error(std::format_string<A...> fmt, A&&...args){
	if (get_log_state() & ERROR) {
		SDL_LogError(
			SDL_LOG_CATEGORY_APPLICATION, 
			"%s", format(fmt, std::forward<A>(args)...).c_str()
		);
	}
}


template <typename Arg, typename... Args>
void log_warn(Arg&& arg, Args&&... args) {
	if (get_log_state() & WARN) {
		SDL_LogWarn(
			SDL_LOG_CATEGORY_APPLICATION,
			log_to_string({}, arg, args...).c_str()
		);
	}
}

template <typename Arg, typename... Args>
void log_warn(const LogArgs &log_args, Arg&& arg, Args&&... args) {
	if (get_log_state() & WARN) {
		SDL_LogWarn(
			SDL_LOG_CATEGORY_APPLICATION,
			log_to_string(log_args, arg, args...).c_str()
		);
	}
}

template<typename ...A>
void flog_warn(std::format_string<A...> fmt, A&&...args){
	if (get_log_state() & WARN) {
		SDL_LogWarn(
			SDL_LOG_CATEGORY_APPLICATION, 
			format(fmt, std::forward<A>(args)...).c_str()
		);
	}
}

#endif /* SUPERNOVA_LOGGING_H */
