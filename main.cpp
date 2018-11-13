#include <map>
#include "lib.h"


constexpr int magic_num = 10;

using factorial = util::fact<magic_num>;

template<typename T, typename _Alloc = std::allocator<T>>
class Array {

	_Alloc m_alloc1;
	_Alloc m_alloc2;
	bool use_first_alloc;
	int m_size;
	T *m_data;

public:
	Array(): m_size(0)
			, m_data(nullptr)
			, m_alloc1()
			, m_alloc2()
			, use_first_alloc(true) {}

	~Array() { delete[] m_data; }

	void push_back(const T val) {
		auto &alloc = use_first_alloc ? m_alloc1 : m_alloc2;
		auto &realloc = use_first_alloc ? m_alloc2 : m_alloc1;

		auto new_data = realloc.allocate(m_size + 1);
		for (int i = 0; i < m_size; ++i) {
			realloc.construct(&new_data[i], m_data[i]);
			alloc.destroy(&m_data[i]);
		}
		realloc.construct(&new_data[m_size], val);
		if (m_size)
			alloc.deallocate(m_data, m_size);
		m_data = new_data;
		++m_size;
		use_first_alloc = !use_first_alloc;
	}

	int& operator[](int index) {
		assert(index >= 0 && index < m_size);
		return m_data[index];
	}

	int size() { return m_size; }
};

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
			for (size_t i = 0; i < magic_num; ++i)
				m[i] = factorial::val(i);
			for (size_t i = 0; i < magic_num; ++i)
				std::cout << i << " " << m[i] << std::endl;
		}

		{
			Array<int> a;
			for (size_t i = 0; i < magic_num; ++i) {
				a.push_back(factorial::val(i));
			}
			for (size_t i = 0; i < magic_num; ++i) {
				std::cout << i << " " << a[i] << std::endl;
			}
		}

		{
			Array<int, logging_allocator<int, 1>> a;
			for (size_t i = 0; i < magic_num; ++i) {
				a.push_back(factorial::val(i));
			}
			for (size_t i = 0; i < magic_num; ++i) {
				std::cout << i << " " << a[i] << std::endl;
			}
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
