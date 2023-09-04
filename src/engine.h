#ifndef SUPERNOVA_ENGINE_H
#define SUPERNOVA_ENGINE_H


#if __has_include("graphics.h")
#ifndef IMAGE_ENABLED
#define IMAGE_ENABLED
#endif /* IMAGE_ENABLED */
#endif /* __has_include("graphics.h") */

#include "core.h"

#if __has_include("graphics.h")
#include "graphics.h"
#endif /* __has_include("graphics.h") */

#if __has_include("mixer.h")
#include "mixer.h"
#endif /* __has_include("mixer.h") */

#if __has_include("font.h")
#include "font.h"
#endif /* __has_include("font.h") */

#if __has_include("networking.h")
#include "networking.h"
#endif /* __has_include("networking.h") */

#endif /* SUPERNOVA_ENGINE_H */
