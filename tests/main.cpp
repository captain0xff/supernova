#include "../src/core.h"
#include "../src/constants.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;



int main(int argc, char* argv[]) {

	IO file("test.txt");
	string text = file.read();
	cout << text << endl;
	file.close();

	ifstream file2("test.txt");
	if (file2) {
		ostringstream ss;
		ss << file2.rdbuf();
		text = ss.str();
	}
	cout << "No. of chars: " << text.length() << endl;
	cout << text << endl;
	file2.close();

	return 0;
}
