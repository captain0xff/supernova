#include <SDL3/SDL_main.h>

#include "../src/core.h"
#include "../src/constants.h"
#include "../src/networking.h"
#include "../src/graphics.h"
#include "../src/logging.h"
#include "../src/mixer.h"
#include "../src/font.h"
#include "../src/print.h"


using namespace std;



int main(int argc, char *argv[]) {
    Engine engine;

    for (string& str: NetUtils::get_local_addresses()) {
        SDL_Log("ok");
        SDL_Log("%s", str.c_str());
    }

    return 0;
}
