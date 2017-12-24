#include <iostream>
#include <fstream>

template <class T>
struct aa_node {
	aa_node *right;
	aa_node *left;
	aa_node *parent;
	int count, level;
	T key;
	T value;
	
};

template <class T>
class AATree {
public:
	AATree();
	~AATree();

	T insert(T, T);
	bool Delete(T, T);

	T min();
	T max();
	bool search(T , T );
	void PrintInOrderTraversal(std::ostream &);
private:
	aa_node<T> *root;
	
	aa_node<T> *insertRecursive(aa_node<T> *, aa_node<T> *);
	aa_node<T> *searchRecursive(aa_node<T> *, T, T);

	void deallocate(aa_node<T> *);
	void skew(aa_node<T> *);
	void rebal(aa_node<T> *);
	void print(std::ostream &, aa_node<T> *);
	
	bool split(aa_node<T> *);
	bool DeleteRecursive(aa_node<T> *, aa_node<T> *, T, T);
};

template <class T>
AATree<T>::AATree() {
	root = new aa_node<T>;
	root = nullptr;
}

template <class T>
void AATree<T>::deallocate(aa_node<T> *n) {
	if (n == nullptr) {
		return;
	}
	deallocate(n->left);
	deallocate(n->right);
	delete n;
}

template <class T>
AATree<T>::~AATree() {
	deallocate(root);
}

template <class T>
aa_node<T> *AATree<T>::insertRecursive(aa_node<T> *temp, aa_node<T> *ins) {
	if (root == nullptr) {
		root = ins;
		ins->count = 1;
		ins->parent = nullptr;
		ins->left = nullptr;
		ins->right = nullptr;
		return root;
	}
	if (ins->value <= temp->value && ins->key != temp->key) {
		if (temp->left) {
			return insertRecursive(temp->left, ins);
		}
		temp->left = ins;
		ins->parent = temp;
		ins->count = 1;
		rebal(ins);

		return ins;
	}
	if (ins->value > temp->value || ins->key != temp->key) {
		if (temp->right) {
			return insertRecursive(temp->right, ins);
		}
		temp->right = ins;
		ins->parent = temp;
		ins->count = 1;
		rebal(ins);

		return ins;
	}
	temp->count++;

	delete ins;
	return temp;
}

template <class T>
T AATree<T>::insert(T key, T value) {
	aa_node<T> *temp = new aa_node<T>;
	temp->key = key;
	temp->value = value;
	temp->level = 1;
	temp->count = 0;
	temp->left = nullptr;
	temp->right = nullptr;
	temp->parent = nullptr;
	temp = insertRecursive(root, temp);
	return temp->count;
}

template <class T>
bool AATree<T>::DeleteRecursive(aa_node<T> *parent, aa_node<T> *current, T key, T value) {
	if (!current) {
		return false;
	}
	if (current->value == value && current->key == key) {
		if (current->left == nullptr || current->right == nullptr) {
			aa_node<T>* temp = current->left;
			if (current->right) {
				temp = current->right;
			}
			if (parent) {
				if (parent->left == current) {
					parent->left = temp;
				}
				else {
					parent->right = temp;
				}
			}
			else {
				root = temp;
			}
		}
		else {
			aa_node<T>* validSubs = current->right;
			while (validSubs->left) {
				validSubs = validSubs->left;
			}
			T tempKey = current->key;
			T tempValue = current->value;

			current->key = validSubs->key;
			current->value = validSubs->value;

			validSubs->key = tempKey;
			validSubs->value = tempValue;

			return DeleteRecursive(current, current->right, tempKey, tempValue);
		}
		delete current;
		return true;
	}
	return DeleteRecursive(current, current->left, key, value) ||
		DeleteRecursive(current, current->right, key, value);
}

template <class T>
bool AATree<T>::Delete(T key, T value) {
	return this->DeleteRecursive(nullptr, root, key, value);
}

template <class T>
void AATree<T>::skew(aa_node<T> *temp)
{
	aa_node<T> *ptr = temp->left;
	if (temp->parent->left == temp)
		temp->parent->left = ptr;
	else
		temp->parent->right = ptr;
	ptr->parent = temp->parent;
	temp->parent = ptr;
	temp->left = ptr->right;
	if (temp->left != nullptr)
		temp->left->parent = temp;
	ptr->right = temp;
	temp->level = (temp->left ? temp->left->level + 1 : 1);
}

template <class T>
bool AATree<T>::split(aa_node<T> *temp)
{
	aa_node<T>* ptr = temp->right;
	if (ptr && ptr->right && (ptr->right->level == temp->level)) {
		if (temp->parent->left == temp) {
			temp->parent->left = ptr;
		}
		else {
			temp->parent->right = ptr;
		}
		ptr->parent = temp->parent;
		temp->parent = ptr;
		temp->right = ptr->left;
		if (temp->right != nullptr) {
			temp->right->parent = temp;
		}
		ptr->left = temp;
		ptr->level = temp->level + 1;
		return true;
	}
	return false;
}

template <class T>
void AATree<T>::rebal(aa_node<T> *temp) {
	temp->left = nullptr;
	temp->right = nullptr;
	temp->level = 1;
	for (temp = temp->parent; temp != root; temp = temp->parent) {
		if (temp->level != (temp->left ? temp->left->level + 1 : 1)) {
			skew(temp);
			if (temp->right == nullptr) {
				temp = temp->parent;
			}
			else if (temp->level != temp->right->level) {
				temp = temp->parent;
			}
		}
		if (temp->parent != root) {
			if (!split(temp->parent)) {
				break;
			}
		}
	}
}

template <class T>
void AATree<T>::print(std::ostream &ostream, aa_node<T> *temp) {
	if (!temp) {
		return;
	}
	print(ostream, temp->left);
	ostream << temp->value << " ";
    ostream << "Key: " << temp->key << " Value: " << temp->value << std::endl;
	print(ostream, temp->right);
}

template <class T>
void AATree<T>::PrintInOrderTraversal(std::ostream &ostream) {
	print(ostream, root);
}

template <class T>
T AATree<T>::min() {
	aa_node<T> *N = root;
	while (N->left != nullptr) {
		N = N->left;
	}
	return N->value;
}

template <class T>
T AATree<T>::max() {
	aa_node<T> *N = root;
	while (N->right != nullptr) {
		N = N->right;
	}
	return N->value;
}

template <class T>
aa_node<T> *AATree<T>::searchRecursive(aa_node<T> *temp, T key, T value) {
	if (temp == nullptr) {
		return nullptr;
	}

	if (value == temp->value && key == temp->key) {
		return temp;
	}
	else if (value < temp->value && key != temp->key) {
		return searchRecursive(temp->left, key, value);
	}
	else if (value > temp->value) {
		return searchRecursive(temp->right, key, value);
	}
	else {
		return nullptr;
	}
}

template <class T>
bool AATree<T>::search(T key, T value) {
	return searchRecursive(root, key, value) != nullptr;
}
