#include "../src/core.h"
#include "../src/print.h"

using namespace std;


int main(int argc, char **argv) {
	Engine engine;
	
	{
		IO file("../CMakeLists.txt");
		file.close();
	}

	print("cool");

	return 0;
}
