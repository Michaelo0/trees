#include <iostream>
#include <fstream>
#include <memory>

template <typename key_t,typename value_t>
struct node {
	key_t key;
	value_t value;
	unsigned long long size;
	std::weak_ptr<node> parent;
	std::shared_ptr<node> left;
	std::shared_ptr<node> right;


	node(key_t key, value_t value)
	{
		this->key = key;
		this->value = value;
		this->size = 1;
		this->parent.reset();
		this->left = nullptr;
		this->right = nullptr;
		
		
	}

	};

template <typename key_t, typename value_t>

class random_tree
{
	std::shared_ptr<node<key_t, value_t> > root;

	void pre_order_traversal(std::ostream &outputstream, std::shared_ptr<node <key_t, value_t> > &x) {
		if (!x) return;
		outputstream << x->value << " ";
		pre_order_traversal(x->left);
		pre_order_traversal(x->right);
	}
	void in_order_traversal(std::ostream &outputstream, std::shared_ptr<node <key_t, value_t> > &x)
	{
		if (!x) return;
		in_order_traversal(outputstream, x->left);
		outputstream << x->value << " ";
		in_order_traversal(outputstream, x->right);
	}
	void post_order_traversal(std::ostream &outputstream, std::shared_ptr<node <key_t, value_t> > &x) {
		if (!x) return;
		post_order_traversal(x->left);
		post_order_traversal(x->right);
		outputstream << x->value << " ";
	}

	unsigned long long getsize(std::shared_ptr<node<key_t, value_t> > &x) 
	{
		if (!x) return 0;
		return x->size;
	}

	const std::shared_ptr<node <key_t, value_t> >  insert_root(std::shared_ptr<node <key_t, value_t> > &x, key_t key,value_t value)
	{
		if (!x) return  std::make_shared<node<key_t, value_t> >(key, value);
		if (key < x->key)
		{
			x->left = insert_root(x->left, key, value);
			rotate_right(x);
			return x;
		}
		if (key>x->key)
		{
			x->right = insert_root(x->right, key, value);
			rotate_left(x);
			return x;
		}
		
	}

	const std::shared_ptr<node<key_t, value_t> > insert_random(std::shared_ptr<node<key_t, value_t> > &x, key_t key,value_t value) {
		if (!x) return std::make_shared<node<key_t, value_t> >(key, value);
		if ( rand() % (x->size + 1) == 0) return insert_root(x, key, value);
		if (key < x->key) x->left = insert_random(x->left, key, value);
		else x->right = insert_random(x->right, key, value);
		x->size++;
		return x;
	}

	const std::shared_ptr<node<key_t, value_t> > join(std::shared_ptr<node<key_t, value_t> > &x, std::shared_ptr<node<key_t, value_t> > &y)
	{
		if (!x) return y;
		if (!y) return x;

		if (rand() % (x->size + y->size) < x->size)
		{
			x->right = join(x->right, y);
			fixsize(x);
			return x;
		}
		else
		{
			y->left = join(x, y->left);
			fixsize(y);
			return y;
		}

	}
	
	const std::shared_ptr<node<key_t, value_t> > remove(std::shared_ptr<node<key_t, value_t> > &x, key_t key)
	{
		if (!x) return x;
		if ((x->key == key))
		{
			auto y = join(x->left, x->right);
			x.reset();
			return y;
		}
		else if (key < x->key)
			x->left = remove(x->left, key);
		else
			x->right = remove(x->right, key);
			
	}

	const std::shared_ptr<node<key_t, value_t> > _check_node(std::shared_ptr<node<key_t, value_t> > &x, key_t key) {
		if (x == nullptr) {
			return nullptr;
		}

		if (x->key == key) {
			return x;
		}

		if (key < x->key) {
			return _check_node(x->left, key);
		}
		else {
			return _check_node(x->right, key);
		}
	}
public:
	random_tree() {
		root = nullptr;
	}

	void _pre_order_traversal(std::ostream &outputstream) {
		pre_order_traversal(outputstream, root);
	}

	void _in_order_traversal(std::ostream &outputstream) {
		in_order_traversal(outputstream, root);
	}

	void _post_order_traversal(std::ostream &outputstream) {
		post_order_traversal(outputstream, root);
	}

	bool insert(key_t key, value_t value) {
		insert_random(root, key, value);
		return true;
	}

	const std::shared_ptr<node<key_t, value_t> > search(key_t key) {
		auto x = root;
		while (x != nullptr) {
			if (key > x->key) {
				x = x->right;
			}
			else if (key < x->key) {
				x = x->left;
			}
			else { return x; }
		}
		return nullptr;
	}

	value_t minimum() {
		auto x = root;
		while (x->right != nullptr) x = x->right;
		return x->value;
	}

	value_t maximum() {
		auto x = root;
		while (x->left != nullptr) x = x->left;
		return x->value;
	}

	
	/*void fixsize() {
		size(root);
	}*/
	void fixsize(std::shared_ptr<node <key_t, value_t> > &x) 
	{
		x->size = getsize(x->left) + getsize(x->right) + 1;
	}

	void rotate_left(std::shared_ptr<node <key_t, value_t> > &x) {
		std::shared_ptr<node <key_t, value_t> > y = x->right;
		if (y != nullptr) {
			x->right = y->left;
			if (y->left != nullptr) y->left->parent = x;
			y->parent = x->parent;
		}
		if (x->parent.lock() == nullptr) { root = y; }
		else if (x == x->parent.lock()->left) { x->parent.lock()->left = y; }
		else { x->parent.lock()->right = y; }
		if (y != nullptr) { y->left = x; }
		x->parent = y;
		fixsize(x);
	}

	void rotate_right(std::shared_ptr<node <key_t, value_t> > &x) {
		std::shared_ptr<node <key_t, value_t> > y = x->left;
		if (y != nullptr) {
			x->left = y->right;
			if (y->right != nullptr) y->right->parent = x;
			y->parent = x->parent;
		}
		if (x->parent.lock() == nullptr) { root = y; }
		else if (x == x->parent.lock()->left) { x->parent.lock()->left = y; }
		else { x->parent.lock()->right = y; }
		if (y != nullptr) { y->right = x; }
		x->parent = y;
		fixsize(x);
	}
	
	/*void _insert_random(key_t key, value_t value) {		
		//insert_random(root,key, value);
	}*/

	bool _remove(key_t key)
	{
		if (_check_node(root,key)){
			remove(root, key);
			return true;
		}
		return false;
		
	}


};
