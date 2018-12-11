#include <map>
#include "lib.h"


constexpr int magic_num = 10;

using factorial = util::fact<magic_num>;

template<typename T, typename _Alloc = std::allocator<T>>
class List {

	_Alloc m_alloc;
	int m_size;
	T *m_data;
	List *next;

public:
	List(): m_size(0), m_data(nullptr), m_alloc(), next(nullptr) {}
	List(_Alloc alloc): m_size(0), m_data(nullptr), m_alloc(alloc), next(nullptr) {}

	~List() {
		if (next != nullptr)
			delete next;
		m_alloc.destroy(m_data);
		m_alloc.deallocate(m_data, 1);
	}

	void push_back(const T val) {
		++m_size;
		if (m_data == nullptr) {
			m_data = m_alloc.allocate(m_size + 1);
			m_alloc.construct(m_data, val);
			return;
		}
		if (next == nullptr)
			next = new List(m_alloc);
		next->push_back(val);
	}

	int& operator[](const int index) {
		assert(index >= 0 && index < m_size);
		if (!index)
			return *m_data;
		next->operator[](index - 1);
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
			List<int> a;
			for (size_t i = 0; i < magic_num; ++i) {
				a.push_back(factorial::val(i));
			}
			for (size_t i = 0; i < magic_num; ++i) {
				std::cout << i << " " << a[i] << std::endl;
			}
		}

		// {
		//     List<int, logging_allocator<int, magic_num>> a;
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
