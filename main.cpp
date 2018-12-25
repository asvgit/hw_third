#include <map>
#include "lib.h"
#include <memory>


constexpr int magic_num = 10;

using factorial = util::fact<magic_num>;

template<typename T, typename _Alloc = std::allocator<T>>
class List {
	struct Node {
		T m_data = 0;
		Node *next = nullptr;

		Node(const T &val) : m_data(val), next(nullptr) {}

		T& at(const int i) {
			if (!i)
				return m_data;
			return next->at(i - 1);
		}

		void append(Node *node) {
			if (next == nullptr)
				next = node;
			else
				next->append(node);
		}
	}* head;
	using RebindedAlloc = typename _Alloc::template rebind<Node>::other;
	RebindedAlloc m_alloc;
	int m_size;

	void DeleteList(Node *head) {
		if (head == nullptr)
			return;
		if (head->next != nullptr) {
			DeleteList(head->next);
		}
		m_alloc.destroy(head);
		m_alloc.deallocate(head, 1);
	}

public:
	List(): head(nullptr), m_alloc(), m_size(0) {}

	~List() {
		DeleteList(head);
	}

	void push_back(const T &val) {
		++m_size;
		Node* node = m_alloc.allocate(1);
		m_alloc.construct(node, val);
		if (head == nullptr) {
			head = node;
			return;
		}
		head->append(node);
	}

	T& operator[](const int index) {
		assert(index >= 0 && index < m_size);
		return head->at(index);
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
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
