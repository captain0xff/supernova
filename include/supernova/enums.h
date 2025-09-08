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
enum LOGLEVEL {
	ERROR = 1,
	WARN = 2,
	INFO = 4,
};



// Typedefs
typedef MOUSEBUTTON MB;
typedef FONTQUALITY FQ;
typedef LOGLEVEL LC;

#endif /* SUPERNOVA_ENUMS_H */
