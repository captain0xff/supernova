#ifndef SUPERNOVA_ENUMS_H
#define SUPERNOVA_ENUMS_H



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



// Typedefs
typedef MOUSEBUTTON MB;
typedef FONTQUALITY FQ;

#endif /* SUPERNOVA_ENUMS_H */
