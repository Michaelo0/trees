#include<iostream>

template<typename T>
struct avl_node {
	T value;
	T key;
	int height;
	avl_node *left;
	avl_node *right;
};

template<typename T>
class AVL {
public:
	AVL();
	~AVL();

	bool insert(T , T );
	bool Delete(T , T );
	bool search(T , T );

	T max();
	T min();

	void PrintInOrderTraversal(std::ostream &);
	void PrintPreOrderTraversal(std::ostream &);
	void PrintPostOrderTraversal(std::ostream &);

private:
	avl_node<T> *root;
	avl_node<T> *NIL;

	void getHeight(avl_node<T> *);

	avl_node<T>* searchRecursive(avl_node<T> *, T , T );
	avl_node<T>* insertRecursive(avl_node<T> *, T , T );
	avl_node<T>* deleteRecursive(avl_node<T> *, T , T );
	avl_node<T>* rotateLeft(avl_node<T> *);
	avl_node<T>* rotateRight(avl_node<T> *);
	avl_node<T>* balance(avl_node<T> *);

	void PrintInOrderTraversalRecursive(std::ostream &, avl_node<T> *);
	void PrintPreOrderTraversalRecursive(std::ostream &, avl_node<T> *);
	void PrintPostOrderTraversalRecursive(std::ostream &, avl_node<T> *);

	void deallocate(avl_node<T> *);
};

template<typename T>
AVL<T>::AVL() {
	root = NIL = new avl_node<T>;
	NIL->height = 0;
	NIL->left = NIL->right = nullptr;
}

template<typename T>
AVL<T>::~AVL() {
	deallocate(root);
}

template<typename T>
avl_node<T>* AVL<T>::searchRecursive(avl_node<T> *temp, T key, T value) {
	if (temp == NIL) {
		return nullptr;
	}

	if (temp->value == value && temp->key == key) {
		return temp;
	}

	if (value <= temp->value && temp->key != key) {
		return searchRecursive(temp->left, key, value);
	}
	else {
		return searchRecursive(temp->right, key, value);
	}
}

template<typename T>
bool AVL<T>::search(T key, T value) {
	return searchRecursive(root, key, value) != nullptr;
}

template<typename T>
avl_node<T> *AVL<T>::insertRecursive(avl_node<T> *temp, T key, T value) {
	if (temp == NIL) {
		temp = new avl_node<T>;
		temp->key = key;
		temp->value = value;
		temp->left = temp->right = NIL;
		temp->height = 1;

		return temp;
	}

	if (value <= temp->value) {
		temp->left = insertRecursive(temp->left, key, value);
	}
	else {
		temp->right = insertRecursive(temp->right, key, value);
	}

	return balance(temp);
}

template<typename T>
bool AVL<T>::insert(T key, T value) {
	root = insertRecursive(root, key, value);
	return true;
}

template<typename T>
avl_node<T> *AVL<T>::deleteRecursive(avl_node<T> *temp, T key, T value) {
	avl_node<T> *t;
	if (temp == NIL) {
		return temp;
	}
	if (temp->value == value && temp->key == key) {
		if (temp->left == NIL || temp->right == NIL) {
			if (temp->left == NIL) {
				t = temp->right;
			}
			else {
				t = temp->left;
			}
			delete temp;
			return t;
		}
		else {
			for (t = temp->right; t->left != NIL; t = t->left);
			temp->value = t->value;
			temp->key = t->key;
			temp->right = deleteRecursive(temp->right, t->key, t->value);
			return balance(temp);
		}
	}

	if (value <= temp->value && key != temp->key) {
		temp->left = deleteRecursive(temp->left, key, value);
	}
	else {
		temp->right = deleteRecursive(temp->right, key, value);
	}

	return balance(temp);
}

template<typename T>
bool AVL<T>::Delete(T key, T value) {
	if (search(key, value)) {
		root = deleteRecursive(root, key, value);
		return true;
	}
	return false;
}

template<typename T>
T AVL<T>::max() {
	avl_node<T> *N = root;
	while (N->right != NIL) {
		N = N->right;
	}
	return N->value;
}

template<typename T>
T AVL<T>::min() {
	avl_node<T> *N = root;
	while (N->left != NIL) N = N->left;
	return N->value;
}

template<typename T>
void AVL<T>::PrintInOrderTraversal(std::ostream &ostream) {
	PrintInOrderTraversalRecursive(ostream, root);
}

template<typename T>
void AVL<T>::PrintPreOrderTraversal(std::ostream &ostream) {
	PrintPreOrderTraversalRecursive(ostream, root);
}

template<typename T>
void AVL<T>::PrintPostOrderTraversal(std::ostream &ostream) {
	PrintPostOrderTraversalRecursive(ostream, root);
}

template<typename T>
void AVL<T>::getHeight(avl_node<T> *N) {
	N->height = 1 + N->left->height > N->right->height ? N->left->height : N->right->height;
}

template<typename T>
avl_node<T> *AVL<T>::rotateLeft(avl_node<T> *temp) {
	avl_node<T> *t = temp->left;
	temp->left = t->right;
	t->right = temp;
	getHeight(temp);
	getHeight(t);

	return t;
}

template<typename T>
avl_node<T> *AVL<T>::rotateRight(avl_node<T> *temp) {
	avl_node<T> *N = temp->right;
	temp->right = N->left;
	N->left = temp;
	getHeight(temp);
	getHeight(N);

	return N;
}



template<typename T>
avl_node<T> *AVL<T>::balance(avl_node<T> *temp) {
	getHeight(temp);

	if (temp->left->height > temp->right->height + 1) {
		if (temp->left->right->height > temp->left->left->height) {
			temp->left = rotateRight(temp->left);
		}
		temp = rotateLeft(temp);
	}
	else if (temp->right->height > temp->left->height + 1) {
		if (temp->right->left->height > temp->right->right->height) {
			temp->right = rotateLeft(temp->right);
		}
		temp = rotateRight(temp);
	}

	return temp;
}

template<typename T>
void AVL<T>::PrintInOrderTraversalRecursive(std::ostream &ostream, avl_node<T> *temp) {
	if (temp == NIL) {
		return;
	}
	PrintInOrderTraversalRecursive(ostream, temp->left);
	ostream << temp->value << " ";
    ostream << "Key: " << temp->key << " Value:" << temp->value << std::endl;
	PrintInOrderTraversalRecursive(ostream, temp->right);
}

template<typename T>
void AVL<T>::PrintPreOrderTraversalRecursive(std::ostream &ostream, avl_node<T> *temp) {
	if (temp == NIL) {
		return;
	}
	ostream << temp->value << " ";
	PrintPreOrderTraversalRecursive(ostream, temp->left);
	PrintPreOrderTraversalRecursive(ostream, temp->right);
}

template<typename T>
void AVL<T>::PrintPostOrderTraversalRecursive(std::ostream &ostream, avl_node<T> *temp) {
	if (temp == NIL) {
		return;
	}
	PrintPostOrderTraversalRecursive(ostream, temp->left);
	PrintPostOrderTraversalRecursive(ostream, temp->right);
	ostream << temp->value << " ";
}

template<typename T>
void AVL<T>::deallocate(avl_node<T> *temp) {
	if (temp == NIL) {
		return;
	}
	deallocate(temp->left);
	deallocate(temp->right);
	delete temp;
}
