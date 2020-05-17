template <class T>
struct unwrap_reference_wrapper
{
	using type = T;
};

template <class T>
struct unwrap_reference_wrapper<std::reference_wrapper<T>>
{
	using type = T&;
};

template <class T>
using decay_t = typename unwrap_reference_wrapper<typename std::decay<T>::type>::type;

template <class... Types>
class Tuple;

template <size_t i, class T, class... Types>
struct indexType
{
	using type = typename indexType<i - 1, Types...>::type;
	static const type& get(const Tuple<T, Types...>& t) { return indexType<i - 1, Types...>::get(t.otherElements); }
	static type& get(Tuple<T, Types...>& t) { return indexType<i - 1, Types...>::get(t.otherElements); }
	static type&& get(Tuple<T, Types...>&& t) { return indexType<i - 1, Types...>::get(t.otherElements); }
};

template <class T, class... Types>
struct indexType<0, T, Types...>
{
	using type = T;
	static const T& get(const Tuple<T, Types...>& t) { return t.element; }
	static T& get(Tuple<T, Types...>& t) { return t.element; }
	static T get(Tuple<T, Types...>&& t) { return t.element; }
};

template <class C, class T, class... Types>
struct typeElement
{
	static const C& get(const Tuple<T, Types...>& t) { return typeElement<C, Types...>::get(t.otherElements); }
	static C& get(Tuple<T, Types...>& t) { return typeElement<C, Types...>::get(t.otherElements); }
	static C get(Tuple<T, Types...>&& t) { return typeElement<C, Types...>::get(t.otherElements); }
};

template <class T, class... Types>
struct typeElement<T, T, Types...>
{
	static const T& get(const Tuple<T, Types...>& t) { return t.element; }
	static T& get(Tuple<T, Types...>& t) { return t.element; }
	static T get(Tuple<T, Types...>&& t) { return t.element; }
};

template <>
class Tuple<>
{
public:
	Tuple() = default;
	~Tuple() = default;
	Tuple(const Tuple& other) = default;
	Tuple(Tuple&& other) = default;
	Tuple& operator=(const Tuple& other) = default;
	Tuple& operator=(Tuple&& other) = default;
	void swap(Tuple& other) { ; }
	bool operator< (const Tuple& other) const { return false; }
	bool operator<= (const Tuple& other) const { return true; }
	bool operator> (const Tuple& other) const { return false; }
	bool operator>= (const Tuple& other) const { return true; }
	bool operator== (const Tuple& other) const { return true; }
	bool operator!= (const Tuple& other) const { return false; }
};

template <class T, class... Types>
class Tuple<T, Types...>
{
public:
	Tuple() : element(T()), otherElements(Tuple<Types...>()) { ; }
	template <class TInput, class... TypesInput>
	explicit Tuple(TInput&& e, TypesInput&& ... other) : element(std::forward<TInput>(e)), otherElements(Tuple<Types...>(std::forward<TypesInput>(other)...)) { ; }
	~Tuple() = default;
	Tuple(const Tuple& other) : element(other.element), otherElements(other.otherElements) { ; }
	Tuple(Tuple&& other) : element(std::move(other.element)), otherElements(std::move(other.otherElements)) { ; }
	Tuple& operator=(const Tuple& other)
	{
		element = other.element;
		otherElements = other.otherElements;
		return *this;
	}
	Tuple& operator=(Tuple&& other)
	{
		element = std::move(other.element);
		otherElements = std::move(other.otherElements);
		return *this;
	}
	void swap(Tuple& other)
	{
		std::swap(element, other.element);
		std::swap(otherElements, other.otherElements);
	}
	bool operator< (const Tuple& other) const { return element != other.element ? element < other.element : otherElements < other.otherElements; }
	bool operator> (const Tuple& other) const { return element != other.element ? element > other.element : otherElements > other.otherElements; }
	bool operator<= (const Tuple& other) const { return !(*this > other); }
	bool operator>= (const Tuple& other) const { return !(*this < other); }
	bool operator== (const Tuple& other) const { return element != other.element ? false : otherElements == other.otherElements; }
	bool operator!= (const Tuple& other) const { return !(*this == other); }
	Tuple(const T& a, const Tuple<Types...>& tuple) : element(a), otherElements(tuple) { ; }
	T element;
	Tuple<Types...> otherElements;
};

template <class... Types>
auto makeTuple(Types&&... elements)
{
	return Tuple<decay_t<Types>...>(std::forward<Types>(elements)...);
}

template <class T, class... Types>
Tuple<T, Types...> addElement(const T& element, const Tuple<Types...>& other)
{
	return Tuple<T, Types...>(element, other);
}

Tuple<> tupleCat()
{
	return Tuple<>();
}

template <class... TTuples>
auto tupleCat(const Tuple<>& first, const TTuples&... other)
{
	Tuple<> t = first;
	return tupleCat(other...);
}

template <class T, class... Types, class... TTuples>
auto tupleCat(const Tuple<T, Types...>& first, const TTuples&... other)
{
	return addElement(first.element, std::move(tupleCat(first.otherElements, other...)));
}

template <size_t i, class T, class... Types>
const typename indexType<i, T, Types...>::type& get(const Tuple<T, Types...>& t)
{
	return indexType<i, T, Types...>::get(t);
}

template <size_t i, class T, class... Types>
typename indexType<i, T, Types...>::type& get(Tuple<T, Types...>& t)
{
	return indexType<i, T, Types...>::get(t);
}

template <size_t i, class T, class... Types>
typename indexType<i, T, Types...>::type get(Tuple<T, Types...>&& t)
{
	return indexType<i, T, Types...>::get(t);
}

template <class C, class T, class... Types>
const C& get(const Tuple<T, Types...>& t)
{
	return typeElement<C, T, Types...>::get(t);
}

template <class C, class T, class... Types>
C& get(Tuple<T, Types...>& t)
{
	return typeElement<C, T, Types...>::get(t);
}

template <class C, class T, class... Types>
C get(Tuple<T, Types...>&& t)
{
	return typeElement<C, T, Types...>::get(t);
}
