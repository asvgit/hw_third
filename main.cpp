#include <map>
#include "lib.h"


constexpr int magic_num = 10;

using factorial = util::fact<magic_num>;

int main() {
	try {
		{
			std::map<int, int> m{};
			for (size_t i = 0; i < magic_num; ++i) {
				m[i] = factorial::val(i);
			}
		}
		{
			std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, magic_num / 2>> m{};
			for (size_t i = 0; i < magic_num; ++i) {
				m[i] = factorial::val(i);
				std::cout << i << " " << m[i] << std::endl;
			}
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
