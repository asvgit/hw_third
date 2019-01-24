#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace util {
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
	static int val(const int) {
		return value;
	}
};
} // end of namespace util

template<typename T, int S = 5>
struct logging_allocator {
	const int m_max_part_size = S;
	std::size_t m_container_size = 0;
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
			// std::cout << __PRETTY_FUNCTION__ << "[n = " << m_max_part_size * n << "]" << std::endl;
			auto p = std::malloc(m_max_part_size * n * sizeof(T));
			if (!p)
				throw std::bad_alloc();
			m_mem.push_back(p);
			++m_container_size;
			return reinterpret_cast<T *>(p);
		} else {
			// std::cout << __PRETTY_FUNCTION__ << "[n = " << m_max_part_size * n << "]" <<
			//     "(allocated)" << std::endl;
			const size_t step = (m_container_size % m_max_part_size) * sizeof(T);
			auto p = (size_t)m_mem.back() + step;
			++m_container_size;
			return reinterpret_cast<T *>(p);
		}
	}

	void deallocate(T *p, std::size_t) {
		--m_container_size;
		if (!m_container_size) {
			// std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "](clean all)" << std::endl;
			std::free(p);
			m_mem.clear();
		} else {
			// std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" <<
			//     "(fictitious)" << "(" << m_container_size << "," << m_mem.size() << ")";
			if (!(m_container_size % m_max_part_size)) {
				std::free(reinterpret_cast<T *>(m_mem.back()));
				// std::cout << "(free last)";
				m_mem.pop_back();
			}
			// std::cout << std::endl;
		}
	}

	template<typename U, typename ...Args>
	void construct(U *p, Args &&...args) {
		// std::cout << __PRETTY_FUNCTION__ << "(c_size: " << m_container_size << ")" << std::endl;
		new(p) U(std::forward<Args>(args)...);
	}

	template<typename U>
	void destroy(U *p) {
		// std::cout << __PRETTY_FUNCTION__ << std::endl;
		p->~U();
	}
};
