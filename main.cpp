#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include "lib.h"

template<typename T, int S = 5>
struct logging_allocator {
	const int m_max_part_size = S;
	int m_container_size = 0;
	std::vector<void*> m_mem;

	using value_type = T;

	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	template<typename U>
	struct rebind {
		using other = logging_allocator<U, S>;
	};

	T *allocate(std::size_t n) {
		if (m_mem.size() * m_max_part_size <= m_container_size) {
			std::cout << __PRETTY_FUNCTION__ << "[n = " << m_max_part_size * n << "]" << std::endl;
			auto p = std::malloc(m_max_part_size * n * sizeof(T));
			if (!p)
				throw std::bad_alloc();
			m_mem.push_back(p);
			return reinterpret_cast<T *>(p);
		} else {
			std::cout << __PRETTY_FUNCTION__ << "[n = " << m_max_part_size * n << "]" <<
				"(allocated)" << std::endl;
			auto p = m_mem.back() + (m_container_size % m_max_part_size) * sizeof(T);
			return reinterpret_cast<T *>(p);
		}
	}

	void deallocate(T *p, std::size_t n) {
		if (!m_container_size) {
			std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "](clean all)" << std::endl;
			std::free(p);
			m_mem.clear();
		} else {
			std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" <<
				"(fictitious)" << "(" << m_container_size << "," << m_mem.size() << ")";
			if (!(m_container_size % m_max_part_size)) {
				std::free(reinterpret_cast<T *>(m_mem.back()));
				std::cout << "(free last)";
				m_mem.pop_back();
			}
			std::cout << std::endl;
		}
	}

	template<typename U, typename ...Args>
	void construct(U *p, Args &&...args) {
		std::cout << __PRETTY_FUNCTION__ << "(c_size: " << m_container_size << ")" << std::endl;
		new(p) U(std::forward<Args>(args)...);
		++m_container_size;
	}

	template<typename U>
	void destroy(U *p) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		p->~U();
		--m_container_size;
	}
};

template<int I>
struct fact {
	static const int value = I * fact<I - 1>::value;
	static int val(const int n) {
		if (I == n)
			return value;
		return fact<I - 1>::val(n);
	}
};

template<>
struct fact<0> {
	static const int value = 1;
	static int val(const int n) {
		return value;
	}
};

constexpr int magic_num = 10;

int main() {
	try {
		for (size_t i = 0; i < magic_num; ++i) {
			std::cout << fact<magic_num>::val(i) << std::endl;
		}

		auto m = std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, magic_num>>{};
		for (size_t i = 0; i < 20; ++i) {
			m[i] = i;
			std::cout << std::endl;
		}
		for (size_t i = 0; i < 20; ++i) {
			std::cout << m[i] << std::endl;
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
