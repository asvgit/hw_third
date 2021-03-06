#include <map>
#include "lib.h"
#include <memory>


constexpr int magic_num = 10;

using factorial = util::fact<magic_num>;

template<typename T, typename _Alloc = std::allocator<T>>
class List {
	struct Node {
		T m_data;
		Node *next = nullptr;
		List *m_parent;

		Node(const T &val, List *parent) : m_data(val), next(nullptr), m_parent(parent) {}
		~Node() {
			if (next != nullptr) {
				m_parent->m_alloc.destroy(next);
				m_parent->m_alloc.deallocate(next, 1);
			}
		}

		T& at(const int i) {
			if (!i)
				return m_data;
			return next->at(i - 1);
		}

		static void append(List *parent, Node *&node, const T &val) {
			if (node == nullptr) {
				node = parent->m_alloc.allocate(1);
				parent->m_alloc.construct(node, std::forward<const T>(val), parent);
				return;
			}
			Node::append(parent, node->next, std::forward<const T>(val));
		}
	}* head;
	friend struct Node;
	using RebindedAlloc = typename _Alloc::template rebind<Node>::other;
	RebindedAlloc m_alloc;
	int m_size;

public:
	List() : head(nullptr), m_alloc(), m_size(0) {}

	List(const List &list) : head(nullptr), m_alloc(), m_size(0) {
		for (size_t i = 0; i < list.size(); ++i)
			push_back(list[i]);
	}
	List(List &&list) : head(nullptr), m_alloc(), m_size(list.m_size) {
		std::cout << "List" << std::endl;
		std::swap(head, list.head);
	}
	template<typename __Alloc>
	List(const List<T, __Alloc> &list) : head(nullptr), m_alloc(), m_size(0) {
		for (size_t i = 0; i < list.size(); ++i)
			push_back(list[i]);
	}

	~List() {
		if (head == nullptr)
			return;
		m_alloc.destroy(head);
		m_alloc.deallocate(head, 1);
	}

	void push_back(const T &val) {
		++m_size;
		Node::append(this, head, std::forward<const T>(val));
	}

	T& operator[](const int index) const {
		assert(index >= 0 && index < m_size);
		return head->at(index);
	}

	int size() const { return m_size; }
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
			std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, magic_num / 2>> m {};
			for (size_t i = 0; i < magic_num; ++i) {
				m[i] = factorial::val(i);
			}
			for (size_t i = 0; i < magic_num; ++i) {
				std::cout << i << " " << m[i] << std::endl;
			}
		}

		{
			List<int> a;
			for (size_t i = 0; i < magic_num; ++i) {
				a.push_back(factorial::val(i));
			}
		}

		{
			List<int, logging_allocator<int, magic_num>> a;
			for (size_t i = 0; i < magic_num; ++i) {
				a.push_back(factorial::val(i));
			}
			for (size_t i = 0; i < magic_num; ++i) {
				std::cout << i << " " << a[i] << std::endl;
			}
			List<int, logging_allocator<int, magic_num>> b(std::move(a));
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
