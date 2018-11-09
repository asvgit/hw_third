#include <map>
#include "lib.h"


constexpr int magic_num = 10;

using factorial = util::fact<magic_num>;

template<typename T, typename _Alloc = std::allocator<T>>
class Array {

	_Alloc m_alloc;
	int m_size;
	T *m_data;

public:
	Array(): m_size(0), m_data(nullptr), m_alloc() {}

	~Array() { delete[] m_data; }

	void push_back(const T val) {
		auto new_data = m_alloc.allocate(m_size + 1);
		for (int i = 0; i < m_size; ++i) {
			m_alloc.construct(&new_data[i], m_data[i]);
			m_alloc.destroy(&m_data[i]);
		}
		m_alloc.construct(&new_data[m_size], val);
		if (m_size)
			m_alloc.deallocate(m_data, m_size);
		m_data = new_data;
		++m_size;
	}

	int& operator[](int index) {
		assert(index >= 0 && index < m_size);
		return m_data[index];
	}

	int size() { return m_size; }
};

int main() {
	try {
		// {
		//     std::map<int, int> m{};
		//     for (size_t i = 0; i < magic_num; ++i) {
		//         m[i] = factorial::val(i);
		//     }
		// }
		// {
		//     std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, magic_num / 2>> m{};
		//     for (size_t i = 0; i < magic_num; ++i) {
		//         m[i] = factorial::val(i);
		//         std::cout << i << " " << m[i] << std::endl;
		//     }
		// }

		{
			Array<int> a;
			for (size_t i = 0; i < magic_num; ++i) {
				a.push_back(factorial::val(i));
			}
			for (size_t i = 0; i < magic_num; ++i) {
				std::cout << i << " " << a[i] << std::endl;
			}
		}

		// {
		//     Array<int, logging_allocator<int, magic_num>> a;
		//     for (size_t i = 0; i < magic_num; ++i) {
		//         a.push_back(factorial::val(i));
		//     }
		//     for (size_t i = 0; i < magic_num; ++i) {
		//         std::cout << i << " " << a[i] << std::endl;
		//     }
		// }
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
