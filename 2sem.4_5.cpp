#include <iostream>
#include <stack>
#include <string>
using std::stack;

template <class T>
struct node;

template <class T>
size_t getSize(node<T>* n)
{
	if (!n)
		return 0;
	else
		return n->size;
}

template <class T>
void fixSize(node<T>* n)
{
	if (!n)
		return;
	n->size = getSize(n->leftChild) + getSize(n->rightChild) + 1;
}

template <class T>
struct node
{
	node(T thisElement)
	{
		element = thisElement;
	}
	node<T>* leftChild = nullptr;
	node<T>* rightChild = nullptr;
	T element;
	size_t size = 1;
	int priority = rand();
};

template <class T>
class treap
{
	friend void split(size_t index, node<T>*& subTree, node<T>*& firstTree, node<T>*& secondTree);
public:
	~treap()
	{
		deleteSubTree(root);
	}
	void insertAt(size_t position, const T& element)
	{
		node<T>* left;
		node<T>* right;
		split(position, root, left, right);
		node<T>* newElement = new node<T>(element);
		node<T>* newLeft = merge(left, newElement);
		root = merge(newLeft, right);
	}
	void deleteAt(size_t position)
	{
		node<T>* left;
		node<T>* mid;
		split(position, root, left, mid);
		node<T>* del;
		node<T>* right;
		split(1, mid, del, right);
		delete del;
		root = merge(left, right);
	}
	T& getAt(size_t position)
	{
		return getAt(root, position);
	}
	void deleteSubTree(node<T>*& subTreeRoot)
	{
		if (subTreeRoot)
		{
			stack<node<T>*> subRootsForDelete;
			node<T>* rootForDelete = subTreeRoot;
			while (rootForDelete || subRootsForDelete.size() != 0)
			{
				if (rootForDelete)
				{
					subRootsForDelete.push(rootForDelete->rightChild);
					subRootsForDelete.push(rootForDelete->leftChild);
					delete rootForDelete;
				}
				rootForDelete = subRootsForDelete.top();
				subRootsForDelete.pop();
			}
		}
	}
private:
	T& getAt(node<T>* subTreeRoot, size_t position)
	{
		if (position == getSize(subTreeRoot->leftChild))
			return subTreeRoot->element;
		else if (position < getSize(subTreeRoot->leftChild))
			return getAt(subTreeRoot->leftChild, position);
		else
			return getAt(subTreeRoot->rightChild, position - getSize(subTreeRoot->leftChild) - 1);
	}
	node<T>* root = nullptr;
};

template <class T>
void split(size_t index, node<T>*& subTree, node<T>*& firstTree, node<T>*& secondTree)
{
	if (!subTree)
	{
		firstTree = secondTree = nullptr;
		return;
	}
	if (index > getSize(subTree->leftChild))
	{
		split(index - getSize(subTree->leftChild) - 1, subTree->rightChild, firstTree, secondTree);
		subTree->rightChild = firstTree;
		firstTree = subTree;
		fixSize(subTree);
	}
	else
	{
		split(index, subTree->leftChild, firstTree, secondTree);
		subTree->leftChild = secondTree;
		secondTree = subTree;
		fixSize(subTree);
	}
}

template <class T>
node<T>* merge(node<T>*& firstTree, node<T>*& secondTree)
{
	if (!firstTree || !secondTree)
	{
		return (firstTree ? firstTree : secondTree);
	}
	if (firstTree->priority > secondTree->priority)
	{
		firstTree->rightChild = merge(firstTree->rightChild, secondTree);
		fixSize(firstTree);
		return firstTree;
	}
	else
	{
		secondTree->leftChild = merge(firstTree, secondTree->leftChild);
		fixSize(secondTree);
		return secondTree;
	}
}

int main()
{
	treap<std::string> superStringArray;
	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++)
	{
		char command;
		std::cin >> command;
		size_t pos;
		std::cin >> pos;
		if (command == '+')
		{
			std::string str;
			std::cin >> str;
			superStringArray.insertAt(pos, str);
		}
		else if (command == '-')
		{
			size_t posEnd;
			std::cin >> posEnd;
			while (true)
			{
				superStringArray.deleteAt(pos);
				if (posEnd == pos)
					break;
				--posEnd;
			}
		}
		else if (command == '?')
		{
			std::cout << superStringArray.getAt(pos) << '\n';
		}
	}
}
