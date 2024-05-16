#ifndef SUPERNOVA_ENUMS_H
#define SUPERNOVA_ENUMS_H

#include <SDL3/SDL.h>



// Mouse buttons
enum MOUSEBUTTON: int {
	NONE = 0,
	LEFT = 1,
	MIDDLE = 2,
	RIGHT = 4,
	X1 = 8,
	X2 = 16
};


// Font qualities
enum FONTQUALITY: int {
	SOLID,
	SHADED,
	BLENDED,
	LCD
};


// Log categories
enum LOGCATEGORY {
	CRITICAL = SDL_LOG_CATEGORY_CUSTOM,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	VERBOSE,
	TOTAL_NUM
};



// Typedefs
typedef MOUSEBUTTON MB;
typedef FONTQUALITY FQ;
typedef LOGCATEGORY LC;

#endif /* SUPERNOVA_ENUMS_H */
