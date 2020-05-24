#include <iostream>
#include <vector>

template <class T>
class List
{
	template <bool isConst = false>
	class listIterator;
public:
	using const_iterator = listIterator<true>;
	using iterator = listIterator<false>;
	typedef T value_type;
	~List();
	List();
	List(size_t count, const T& value = T());
	List(List&& other) : first(other.first), last(other.last), len(other.len)
	{
		other.first = reinterpret_cast<Node*>(new fictitiousNode);
		other.last = reinterpret_cast<Node*>(new fictitiousNode);
		other.last->prev = other.first;
		other.first->next = other.last;
		other.len = 0;
	}
	List(const List& other);
	List& operator= (List&& other);
	List& operator= (const List& other);
	size_t size() const { return len; };
	T& front() { return *begin(); };
	T& back() { return *(--end()); };
	const T& front() const { return *cbegin(); };
	const T& back() const { return *(--cend()); };
	void clear();
	bool empty() const { return len == 0; }
	iterator begin() const { return ++iterator(first); }
	iterator end() const { return iterator(last); }
	const_iterator cbegin() const { return ++const_iterator(first); }
	const_iterator cend() const { return const_iterator(last); }
	std::reverse_iterator<iterator> rbegin() const
	{
		return std::make_reverse_iterator<iterator>(iterator(last));
	}
	std::reverse_iterator<iterator> rend() const
	{
		return std::make_reverse_iterator<iterator>(++iterator(first));
	}
	std::reverse_iterator<const_iterator> crbegin() const
	{
		return std::make_reverse_iterator<const_iterator>(const_iterator(last));
	}
	std::reverse_iterator<const_iterator> crend() const
	{
		return std::make_reverse_iterator<const_iterator>(++const_iterator(first));
	}
	iterator insert(iterator it, const T& element);
	iterator insert(iterator it, T&& element);
	template <class InputIter>
	iterator insert(iterator it, InputIter first, InputIter last);
	void erase(iterator it);
	void erase(iterator first, iterator second);
	void emplace(iterator it, T&& element) { insert(it, std::move(std::forward<T>(element))); }
	void emplace_front(T&& element) { push_front(std::move(std::forward<T>(element))); }
	void emplace_back(T&& element) { push_back(std::move(std::forward<T>(element))); }
	void push_back(const T& element) { insert(end(), element); }
	void push_back(T&& element) { insert(end(), std::move(element)); }
	void pop_back() { erase(--end()); }
	void push_front(const T& element) { insert(begin(), element); }
	void push_front(T&& element) { insert(begin(), std::move(element)); }
	void pop_front() { erase(begin()); }
	void reverse();
	void unique();
protected:
	struct Node
	{
		Node() = default;
		Node(const T& e, Node* p, Node* n) : element(e), next(n), prev(p) { ; }
		Node(T&& e, Node* p, Node* n) : element(std::move(e)), next(n), prev(p) { ; }
		T element;
		Node* next = nullptr;
		Node* prev = nullptr;
	};
	Node* first = nullptr;
	Node* last = nullptr;
	size_t len = 0;
private:
	struct fictitiousNode
	{
		char element[sizeof(T)];
		Node* next = nullptr;
		Node* prev = nullptr;
	};
};

//####################################################################################################

template <class T>
template <bool isConst>
class List<T>::listIterator
{
	friend class List;
public:
	typedef T value_type;
	typedef typename std::conditional<isConst, const T&, T&>::type reference;
	typedef typename std::conditional<isConst, const T*, T*>::type pointer;
	typedef typename std::bidirectional_iterator_tag iterator_category;
	typedef int difference_type;
	listIterator(List<T>::Node* n) : node(n) { ; }
	template <bool isOtherConst>
	listIterator(const List<T>::listIterator<isOtherConst>& it) : node(it.node) { ; }
	listIterator operator=(const listIterator& it) { node = it.node; return *this; }
	listIterator& operator++();
	listIterator operator++(int);
	listIterator& operator--();
	listIterator operator--(int);
	reference operator*() { return node->element; };
	pointer operator->() { return &(node->element); };
	template <bool isOtherConst>
	bool operator==(const List<T>::listIterator<isOtherConst>& it) const { return node == it.node; };
	template <bool isOtherConst>
	bool operator!=(const List<T>::listIterator<isOtherConst>& it) const { return !(*this == it); };
protected:
	typename List<T>::Node* node;
};

template <class T>
template <bool isConst>
List<T>::listIterator<isConst>& List<T>::listIterator<isConst>::operator++()
{
	node = node->next;
	return *this;
}

template <class T>
template <bool isConst>
List<T>::listIterator<isConst> List<T>::listIterator<isConst>::operator++(int)
{
	listIterator result = *this;
	node = node->next;
	return result;
}

template <class T>
template <bool isConst>
List<T>::listIterator<isConst>& List<T>::listIterator<isConst>::operator--()
{
	node = node->prev;
	return *this;
}

template <class T>
template <bool isConst>
List<T>::listIterator<isConst> List<T>::listIterator<isConst>::operator--(int)
{
	listIterator result = *this;
	node = node->prev;
	return result;
}

//####################################################################################################

template <class T>
List<T>::List()
{
	first = reinterpret_cast<Node*>(new fictitiousNode);
	last = reinterpret_cast<Node*>(new fictitiousNode);
	last->prev = first;
	first->next = last;
}

template <class T>
List<T>::~List()
{
	if (first == nullptr)
		return;
	clear();
	delete first;
	delete last;
}

template <class T>
List<T>::List(size_t count, const T& value)
{
	*this = List();
	for (size_t i = 0; i < count; i++)
	{
		insert(end(), value);
	}
}

template <class T>
List<T>::List(const List& other)
{
	*this = List();
	for (auto element : other)
		push_back(element);
}

template <class T>
void List<T>::clear()
{
	if (first == nullptr)
		* this = List();
	else
		erase(begin(), end());
}

template <class T>
List<T>& List<T>::operator= (List&& other)
{
	if (&other == this)
		return *this;
	first = other.first;
	last = other.last;
	len = other.len;
	other.first = reinterpret_cast<Node*>(new fictitiousNode);
	other.last = reinterpret_cast<Node*>(new fictitiousNode);
	other.last->prev = other.first;
	other.first->next = other.last;
	other.len = 0;
	return *this;
}

template <class T>
List<T>& List<T>::operator= (const List& other)
{
	if (&other == this)
		return *this;
	clear();
	for (auto element : other)
		push_back(element);
	return *this;
}

template <class T>
typename List<T>::iterator List<T>::insert(iterator it, const T& element)
{
	Node* prev = it.node->prev;
	it.node->prev = new Node(element, prev, it.node);
	prev->next = it.node->prev;
	++len;
	return --it;
}

template <class T>
typename List<T>::iterator List<T>::insert(iterator it, T&& element)
{
	Node* prev = it.node->prev;
	it.node->prev = new Node(std::move(element), prev, it.node);
	prev->next = it.node->prev;
	++len;
	return --it;
}

template <class T>
template <class InputIter>
typename List<T>::iterator List<T>::insert(iterator it, InputIter first, InputIter last)
{
	while (first != last)
	{
		insert(it, *first);
		++first;
	}
	return --it;
}

template <class T>
void List<T>::erase(iterator it)
{
	it.node->prev->next = it.node->next;
	it.node->next->prev = it.node->prev;
	delete it.node;
	--len;
}

template <class T>
void List<T>::erase(iterator first, iterator second)
{
	while (first != second)
	{
		auto next = first;
		++next;
		erase(first);
		first = next;
	}
}

template <class T>
void List<T>::reverse()
{
	for (auto s = begin(), e = end(); e != s && --e != s; ++s)
	{
		std::swap(*s, *e);
	}
}

template <class T>
void List<T>::unique()
{
	auto first = begin();
	auto last = end();
	iterator result = first;
	while (++first != last)
	{
		if (!(*result == *first))
			* (++result) = *first;
	}
	erase(++result, end());
}
