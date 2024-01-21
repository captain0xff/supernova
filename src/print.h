#ifndef SUPERNOVA_PRINT_H
#define SUPERNOVA_PRINT_H


#include <iostream>



// Structs
struct PrintArgs {
	std::string sep = " ", end = "\n";
};



// Functions
template <typename Arg, typename... Args>
inline void print(const PrintArgs &print_args, Arg&& arg, Args&&... args) {
	std::cout << std::forward<Arg>(arg);
	((std::cout << print_args.sep << std::forward<Args>(args)), ...);
	std::cout << print_args.end;
}

template <typename Arg, typename... Args>
inline void print(Arg&& arg, Args&&... args) {
	std::cout << std::forward<Arg>(arg);
	((std::cout << ' ' << std::forward<Args>(args)), ...);
	std::cout << '\n';
}

inline void print() {
	std::cout << std::endl;
}

#endif /* SUPERNOVA_PRINT_H */
