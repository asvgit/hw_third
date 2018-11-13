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
	static int val(const int n) {
		return value;
	}
};
} // end of namespace util

template<typename T, int S = 5>
struct logging_allocator {
	const int m_max_part_size = S;
	int m_container_size = 0;
	std::vector<void*> m_mem;
	std::vector<int> m_mem_sizes;

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

		std::cout << "(part = " << m_max_part_size << ")"<< std::endl;
		if (!m_container_size) {
			if (n > m_max_part_size) {
				std::cout << "(tapa)"<< std::endl;
				const int alloc_n = n + m_max_part_size - (n % m_max_part_size);
				if (m_mem.size()) {
					if (m_mem_sizes.front() > n) {
						std::free(m_mem.front());
						auto p = std::malloc(alloc_n * sizeof(T));
						if (!p) throw std::bad_alloc();
						m_mem.front() = p;
						std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "(allocated)" << "(c_size: " << m_container_size << ")"<< std::endl;
					}
					return reinterpret_cast<T *>(m_mem.front());
				}
				auto p = std::malloc(alloc_n * sizeof(T));
				if (!p)
					throw std::bad_alloc();
				m_mem.push_back(p);
				m_mem_sizes.push_back(alloc_n);
				std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "(allocated)" << "(c_size: " << m_container_size << ")"<< std::endl;
				return reinterpret_cast<T *>(m_mem.front());
			}

			if (m_mem.size()) {
				std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "(allocated)" << "(c_size: " << m_container_size << ")"<< std::endl;
				return reinterpret_cast<T *>(m_mem.front());
			}
			auto p = std::malloc(m_max_part_size * sizeof(T));
			if (!p) throw std::bad_alloc();
			m_mem.push_back(p);
			m_mem_sizes.push_back(m_max_part_size);
			std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "(allocated)" << "(c_size: " << m_container_size << ")"<< std::endl;
			return reinterpret_cast<T *>(p);

		}

		if (m_mem.size() * m_max_part_size < (m_container_size + n)) {
			auto p = std::malloc(m_max_part_size * sizeof(T));
			if (!p) throw std::bad_alloc();
			m_mem.push_back(p);
			m_mem_sizes.push_back(m_max_part_size);
			std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "(allocated)" << "(c_size: " << m_container_size << ")"<< std::endl;
			return reinterpret_cast<T *>(p);
		}

		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "(fictitious)" << "(c_size: " << m_container_size << ")"<< std::endl;
		const size_t step = (m_container_size % m_max_part_size) * sizeof(T);
		auto p = (size_t)m_mem.back() + step;
		return reinterpret_cast<T *>(p);
	}

	void deallocate(T *p, std::size_t n) {
		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" <<
			"(fictitious)" << "(" << m_container_size << "," << m_mem.size() << ")" << std::endl;
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

	~logging_allocator() {
		while (m_mem.size()) {
			std::free(m_mem.back());
			m_mem.pop_back();
			std::cout << "(free last)";
		}
		m_mem_sizes.clear();
	}
};
