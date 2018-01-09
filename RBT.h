
#include <iostream>
#include <fstream>
#include <memory>


	enum color_t { red, black }; 
								 
	template <typename key_t, typename value_t>
	struct red_black_tree_node {
		key_t key;    
		value_t value; 
		color_t color; 
		std::weak_ptr<red_black_tree_node> parent;    
		std::shared_ptr<red_black_tree_node> left;   
		std::shared_ptr<red_black_tree_node> right;  
													  
		red_black_tree_node(key_t key, value_t value, color_t color) {
			this->key = key;
			this->value = value;
			this->color = color;
			this->parent.reset();
			this->left = nullptr;
			this->right = nullptr;
		}
		
		void info() const {
			std::cout << this->key << "\t";
			if (this->color == red) {
				std::cout << "red" << "\t";
			}
			else if (this->color == black) {
				std::cout << "black" << "\t";
			}
			if (this->left != nullptr) {
				std::cout << this->left->key << "\t";
			}
			else {
				std::cout << "null" << "\t";
			}
			if (this->right != nullptr) {
				std::cout << this->right->key << "\t";
			}
			else {
				std::cout << "null" << "\t";
			}
			if (this->parent.lock() != nullptr) {
				std::cout << this->parent.lock()->key << std::endl;
			}
			else {
				std::cout << "null" << std::endl;
			}
		}
	};
	
	template <typename key_t, typename value_t>
	class red_black_tree {
	private:
		std::shared_ptr<red_black_tree_node <key_t, value_t> > root;
		void pre_order_traversal(std::shared_ptr<red_black_tree_node <key_t, value_t> > &x) {
			if (x == nullptr) return;
			x->info();
			pre_order_traversal(x->left);
			pre_order_traversal(x->right);
		}
		void in_order_traversal(std::ostream &outputstream, std::shared_ptr<red_black_tree_node <key_t, value_t> > &x)
		{
			if (x == nullptr) return;
			in_order_traversal(outputstream, x->left);
			outputstream << x->info();
			in_order_traversal(outputstream, x->right);
		}
		void post_order_traversal(std::shared_ptr<red_black_tree_node <key_t, value_t> > &x) {
			if (x == nullptr) return;
			post_order_traversal(x->left);
			post_order_traversal(x->right);
			x->info();
		}
		unsigned long long size(std::shared_ptr<red_black_tree_node <key_t, value_t> > &x) {
			if (x == nullptr) return 0;
			return size(x->left)  size(x->right)  1;
		}
		void left_rotate(std::shared_ptr<red_black_tree_node <key_t, value_t> > &x) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > y = x->right;
			if (y != nullptr) {
				x->right = y->left;
				if (y->left != nullptr) y->left->parent = x;
				y->parent = x->parent;
			}
			if (x->parent.lock() == nullptr) {
				root = y;
			}
			else if (x == x->parent.lock()->left) {
				x->parent.lock()->left = y;
			}
			else {
				x->parent.lock()->right = y;
			}
			if (y != nullptr) {
				y->left = x;
			}
			x->parent = y;
		}
		void right_rotate(std::shared_ptr<red_black_tree_node <key_t, value_t> > &x) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > y = x->left;
			if (y != nullptr) {
				x->left = y->right;
				if (y->right != nullptr) y->right->parent = x;
				y->parent = x->parent;
			}
			if (x->parent.lock() == nullptr) {
				root = y;
			}
			else if (x == x->parent.lock()->left) {
				x->parent.lock()->left = y;
			}
			else {
				x->parent.lock()->right = y;
			}
			if (y != nullptr) {
				y->right = x;
			}
			x->parent = y;
		}
		void fix(std::shared_ptr<red_black_tree_node <key_t, value_t> > &x) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > parent = nullptr;
			std::shared_ptr<red_black_tree_node <key_t, value_t> > grand_parent = nullptr;
			while ((x != root) && (x->color != black) && (x->parent.lock()->color == red)) {
				parent = x->parent.lock();
				grand_parent = x->parent.lock()->parent.lock();
				/**
				* @brief Case A - Parent of x is left child of the grand parent of x
				*/
				if (parent == grand_parent->left) {
					std::shared_ptr<red_black_tree_node <key_t, value_t> > uncle = grand_parent->right;
					/**
					* @brief Case 1 - The uncle of x is also red. Only recoloring is required
					*/
					if (uncle != nullptr && uncle->color == red) {
						grand_parent->color = red;
						parent->color = black;
						uncle->color = black;
						x = grand_parent;
					}
					else {
						/**
						* @brief Case 2 - x is right child of its parent. Left rotation is required
						*/
						if (x == parent->right) {
							left_rotate(parent);
							x = parent;
							parent = x->parent.lock();
						}
						/**
						* @brief Case 3 - x is left child of its parent. Right rotation is required
						*/
						right_rotate(grand_parent);
						std::swap(parent->color, grand_parent->color);
						x = parent;
					}
				}
				else {
					/**
					* @brief Case B - Parent of x is right child of the grand parent of x
					*/
					std::shared_ptr<red_black_tree_node <key_t, value_t> > uncle = grand_parent->left;
					/**
					* @brief Case 1 - The uncle of x is also red. Only recoloring required
					*/
					if ((uncle != nullptr) && (uncle->color == red)) {
						grand_parent->color = red;
						parent->color = black;
						uncle->color = black;
						x = grand_parent;
					}
					else {
						/**
						* @brief Case 2 - x is left child of its parent. Right rotation is required
						*/
						if (x == parent->left) {
							right_rotate(parent);
							x = parent;
							parent = x->parent.lock();
						}
						/**
						* @brief Case 3 - x is right child of its parent. Left rotation is required
						*/
						left_rotate(grand_parent);
						std::swap(parent->color, grand_parent->color);
						x = parent;
					}
				}
			}
			root->color = black;
		}

	public:
		red_black_tree() {
			root = nullptr;
		}
		~red_black_tree() {}
	
		void pre_order_traversal() {
			pre_order_traversal(root);
		}
		
		void _in_order_traversal(std::ostream &outputstream) {
			in_order_traversal(outputstream, root);
		}
		
		void post_order_traversal() {
			post_order_traversal(root);
		}
		
		void breadth_first_traversal() {
			breadth_first_traversal(root);
		}
		
		 const std::shared_ptr<red_black_tree_node <key_t, value_t> > insert(key_t key, value_t value) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > current = root;
			std::shared_ptr<red_black_tree_node <key_t, value_t> > parent = nullptr;
			while (current != nullptr) {
				parent = current;
				if (key > current->key) {
					current = current->right;
				}
				else if (key < current->key) {
					current = current->left;
				}
				else {
					return nullptr;
				}
			}
			current = std::make_shared<red_black_tree_node <key_t, value_t> >(key, value, red);
			current->parent = parent;
			if (parent == nullptr) {
				root = current;
			}
			else if (current->key > parent->key) {
				parent->right = current;
			}
			else if (current->key < parent->key) {
				parent->left = current;
			}
			fix(current);
			return current;
		}
		
		 bool _remove(key_t key) {
			 red_black_tree_node <key_t, value_t> x = root;

			 if (search(key) == nullptr) return false;

			 while (x != nullptr) {
				 if (key < x->key) {
					 x = x->left; 
				 }
				 else if (key > x->key) {
					 x = x->right; 
				 }
				 else {
					 if (x->left == nullptr && x->right == nullptr) {
						 if (x->parent == nullptr)
							  root = nullptr;
						 else if (x->parent->right != nullptr) {
							 if (x->parent->right->key == x->key)
								  x->parent->right = nullptr;
							 else
								  x->parent->left = nullptr;
							 
						 }
						 else
							  x->parent->left = nullptr;
						 x.reset();
						 return true;
						 
					 }
					 else if (x->left == nullptr) {
						 if (x->parent != nullptr) {
							 x->right->parent = x->parent;
							 if (x->parent->right != nullptr) {
								 if (x->parent->right->key == x->key)
									  x->parent->right = x->right;
								 else
									  x->parent->left = x->right;
								 
							 }
							 else {
								 x->parent->left = x->right;
								 
							 }
							 
						 }
						 else {
							 x->right->parent = nullptr;
							 root = x->right;
							 
						 }
						 x.reset();
						 return true;
						 
					 }
					 else if (x->right == nullptr) {
						 if (x->parent != nullptr) {
							 x->left->parent = x->parent;
							 if (x->parent->right != nullptr) {
								 if (x->parent->right->key == x->key)
									  x->parent->right = x->left;
								 else
									  x->parent->left = x->left;
								 
							 }
							 else {
								 x->parent->left = x->left;
								 
							 }
							 
						 }
						 else {
							 x->left->parent = nullptr;
							 root = x->left;
							 
						 }
						 x.reset();
						 return true;
						 
					 }
					 else {
						 red_black_tree_node <key_t, value_t> child = x->left;
						 while (child->right != nullptr)
							  child = child->right;
						 x->value = child->value;
						 x->key = child->key;
						 if (child->left != nullptr) {
							 if (child->parent->left->key == child->key) {
								 child->parent->left = child->left;
								 child->left->parent = child->parent;
								 
							 }
							 else {
								 child->parent->right = child->left;
								 child->left->parent = child->parent;
								 
							 }
							 
						 }
						 else {
							 if (child->parent->left != nullptr) {
								 if (child->parent->left->key == child->key)
									  child->parent->left = nullptr;
								 else
									  child->parent->right = nullptr;
								 
							 }
							 else {
								 child->parent->right = nullptr;
								 
							 }
							 
						 }
						 child.reset();
						 return true;
						 
					 }
					 
				 }
				 
			 }
			 
		 }

		const std::shared_ptr<red_black_tree_node <key_t, value_t> > search(key_t key) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > x = root;
			while (x != nullptr) {
				if (key > x->key) {
					x = x->right;
				}
				else if (key < x->key) {
					x = x->left;
				}
				else {
					return x;
				}
			}
			return nullptr;
		}
		
		value_t minimum() {
			auto x = root;
			if (x == nullptr) return 0;
			while (x->left != nullptr) x = x->left;
			return x->value;
		}
		
		value_t maximum() {
			auto x = root;
			if (x == nullptr) return 0;
			while (x->right != nullptr) x = x->right;
			return x->value;
		}
		
		const std::shared_ptr<red_black_tree_node <key_t, value_t> > successor(key_t key) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > x = root;
			while (x != nullptr) {
				if (key > x->key) {
					x = x->right;
				}
				else if (key < x->key) {
					x = x->left;
				}
				else {
					if (x->right != nullptr) {
						x = x->right;
						while (x->left != nullptr) x = x->left;
						return x;
					}
					std::shared_ptr<red_black_tree_node <key_t, value_t> > parent = x->parent.lock();
					while (parent != nullptr && x == parent->right) {
						x = parent;
						parent = parent->parent.lock();
					}
					return parent;
				}
			}
			return nullptr;
		}
		
		const std::shared_ptr<red_black_tree_node <key_t, value_t> > predecessor(key_t key) {
			std::shared_ptr<red_black_tree_node <key_t, value_t> > x = root;
			while (x != nullptr) {
				if (key > x->key) {
					x = x->right;
				}
				else if (key < x->key) {
					x = x->left;
				}
				else {
					if (x->left != nullptr) {
						x = x->left;
						while (x->right != nullptr) x = x->right;
						return x;
					}
					std::shared_ptr<red_black_tree_node <key_t, value_t> > parent = x->parent.lock();
					while (parent != nullptr && x == parent->left) {
						x = parent;
						parent = parent->parent.lock();
					}
					return parent;
				}
			}
			return nullptr;
		}
		
		
		unsigned long long size() {
			return size(root);
		}
		
		bool empty() {
			if (root == nullptr) {
				return true;
			}
			else {
				return false;
			}
		}
	};


