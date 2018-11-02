#include <cassert>
#include <cstdlib>
#include <iostream>
#include "lib.h"

int main() {
	try {
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
