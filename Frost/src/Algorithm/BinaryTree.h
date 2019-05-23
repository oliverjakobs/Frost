#pragma once

#include <vector>
#include <stdio.h>
#include <algorithm>

template<typename Type>
struct TreeNode
{
	Type key;
	TreeNode* left;
	TreeNode* right;
	int height;

	TreeNode(Type k) 
		: key(k), left(nullptr), right(nullptr), height(1) { }

	TreeNode<Type>* rightRotate();
	TreeNode<Type>* leftRotate();

	void updateHeight();
	int getBalance() const;
};

template<typename Type>
inline TreeNode<Type>* TreeNode<Type>::rightRotate()
{
	TreeNode<Type>* newRoot = this->left;
	TreeNode<Type>* node = newRoot->right;

	// Perform rotation
	newRoot->right = this;
	this->left = node;

	// Update heights
	this->updateHeight();
	newRoot->updateHeight();

	// Return new root
	return newRoot;
}

template<typename Type>
inline TreeNode<Type>* TreeNode<Type>::leftRotate()
{
	TreeNode<Type>* newRoot = this->right;
	TreeNode<Type>* node = newRoot->left;

	// Perform rotation
	newRoot->left = this;
	this->right = node;

	//  Update heights
	this->updateHeight();
	newRoot->updateHeight();

	// Return new root
	return newRoot;
}

template<typename Type>
inline void TreeNode<Type>::updateHeight()
{
	height = std::max(left ? left->height : 0, right ? right->height : 0) + 1;
}

template<typename Type>
inline int TreeNode<Type>::getBalance() const
{
	return (left ? left->height : 0) - (right ? right->height : 0);
}

template<typename Type>
TreeNode<Type>* minValueNode(TreeNode<Type>* node)
{
	auto min = node;

	while (min->left != nullptr)
	{
		min = min->left;
	}

	return min;
}

template<typename Type>
class BinaryTree
{
private:
	TreeNode<Type>* m_root;
	bool balanced;

	void destroy(TreeNode<Type>* node);

	TreeNode<Type>* insert(TreeNode<Type>* node, Type key);
	TreeNode<Type>* erase(TreeNode<Type>* node, Type key);
	TreeNode<Type>* search(TreeNode<Type>* node, Type key);

	std::vector<TreeNode<Type>*> preorder(TreeNode<Type>* node);
	std::vector<TreeNode<Type>*> inorder(TreeNode<Type>* node);
	std::vector<TreeNode<Type>*> postorder(TreeNode<Type>* node);

	void print(TreeNode<Type>* node, int space);
public:
	BinaryTree(bool balanced = false);
	~BinaryTree();

	void destroy();

	void insert(Type key);
	void erase(Type key);
	TreeNode<Type>* search(Type key);

	std::vector<Type> preorder();
	std::vector<Type> inorder();
	std::vector<Type> postorder();

	void print();
};

template<typename Type>
void BinaryTree<Type>::destroy(TreeNode<Type>* node)
{
	if (node != nullptr)
	{
		destroy(node->left);
		destroy(node->right);

		delete node;
		node = nullptr;
	}
}

template<typename Type>
TreeNode<Type>* BinaryTree<Type>::insert(TreeNode<Type>* node, Type key)
{
	// 1. Perform the normal BST insertion
	if (node == nullptr)
		return new TreeNode<Type>(key);

	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed
		return node;

	// 2. Update height of the node
	node->updateHeight();

	if (balanced)
	{
		// 3. Get the balance factor of this node to check whether this node became unbalanced
		int balance = node->getBalance();

		// If this node becomes unbalanced, then there are 4 cases
		// Left Left Case
		if (balance > 1 && key < node->left->key)
			return node->rightRotate();

		// Left Right Case
		if (balance > 1 && key > node->left->key)
		{
			node->left = node->left->leftRotate();
			return node->rightRotate();
		}

		// Right Right Case
		if (balance < -1 && key > node->right->key)
			return node->leftRotate();

		// Right Left Case
		if (balance < -1 && key < node->right->key)
		{
			node->right = node->right->rightRotate();
			return node->leftRotate();
		}
	}

	return node;
}

template<typename Type>
inline TreeNode<Type>* BinaryTree<Type>::erase(TreeNode<Type>* node, Type key)
{
	// 1. Perform the normal BST delete
	if (node == nullptr)
		return node;

	if (key < node->key)
		node->left = erase(node->left, key);
	else if (key > node->key)
		node->right = erase(node->right, key);
	else // if key is same as node's key, then this is the node to be deleted
	{
		// node with only one child or no child
		if ((node->left == nullptr) || (node->right == nullptr))
		{
			auto temp = node->left ? node->left : node->right;

			delete node;
			node = temp;
		}
		else
		{
			// node with two children: Get the inorder successor (smallest in the right subtree)
			auto temp = minValueNode(node->right);

			// Copy the inorder successor's data to this node
			node->key = temp->key;

			// Delete the inorder successor
			node->right = erase(node->right, temp->key);
		}
	}

	// If the tree had only one node then return
	if (node == nullptr)
		return node;

	// 2. Update height of the node
	node->updateHeight();

	if (balanced)
	{
		// 3. Get the balance factor of this node to check whether this node became unbalanced
		int balance = node->getBalance();

		// If this node becomes unbalanced, then there are 4 cases

		// Left Left Case
		if (balance > 1 && node->left->getBalance() >= 0)
			return node->rightRotate();

		// Left Right Case
		if (balance > 1 && node->left->getBalance() < 0)
		{
			node->left = node->left->leftRotate();
			return node->rightRotate();
		}

		// Right Right Case
		if (balance < -1 && node->right->getBalance() <= 0)
			return node->leftRotate();

		// Right Left Case
		if (balance < -1 && node->right->getBalance() > 0)
		{
			node->right = node->right->rightRotate();
			return node->leftRotate();
		}
	}

	return node;
}

template<typename Type>
TreeNode<Type>* BinaryTree<Type>::search(TreeNode<Type>* node, Type key)
{
	if (leaf != nullptr)
	{
		if (key == node->key)
			return node;
		if (key < node->key)
			return search(key, node->left);
		else
			return search(key, node->right);
	}
	else
		return nullptr;
}

template<typename Type>
inline std::vector<TreeNode<Type>*> BinaryTree<Type>::preorder(TreeNode<Type>* node)
{
	std::vector<TreeNode<Type>*> out;

	std::vector<TreeNode<Type>*> left;
	std::vector<TreeNode<Type>*> right;

	if (node == nullptr)
		return out;

	left = preorder(node->left);
	right = preorder(node->right);

	out.emplace_back(node);

	out.insert(out.end(), left.begin(), left.end());
	out.insert(out.end(), right.begin(), right.end());

	return out;
}

template<typename Type>
inline std::vector<TreeNode<Type>*> BinaryTree<Type>::inorder(TreeNode<Type>* node)
{
	std::vector<TreeNode<Type>*> out;

	std::vector<TreeNode<Type>*> right;

	if (node == nullptr)
		return out;

	out = inorder(node->left);
	right = inorder(node->right);

	out.emplace_back(node);

	out.insert(out.end(), right.begin(), right.end());

	return out;
}

template<typename Type>
inline std::vector<TreeNode<Type>*> BinaryTree<Type>::postorder(TreeNode<Type>* node)
{
	std::vector<TreeNode<Type>*> out;

	std::vector<TreeNode<Type>*> right;

	if (node == nullptr)
		return out;

	out = postorder(node->left);
	right = postorder(node->right);

	out.insert(out.end(), right.begin(), right.end());

	out.emplace_back(node);

	return out;
}

template<typename Type>
inline void BinaryTree<Type>::print(TreeNode<Type>* node, int space)
{
	// Base case
	if (node == nullptr)
		return;

	// Increase distance between levels
	space += 10;

	// Process right child first
	print(node->right, space);

	// Print current node after space
	// count
	printf("\n");
	for (int i = 10; i < space; i++)
		printf(" ");

	printf("%d(%d, %d)\n", node->key, node->height, node->getBalance());

	// Process left child
	print(node->left, space);
}

template<typename Type>
BinaryTree<Type>::BinaryTree(bool b)
{
	m_root = nullptr;
	balanced = b;
}

template<typename Type>
BinaryTree<Type>::~BinaryTree()
{
	destroy();
}

template<typename Type>
void BinaryTree<Type>::destroy()
{
	destroy(m_root);
	m_root = nullptr;
}

template<typename Type>
void BinaryTree<Type>::insert(Type key)
{
	m_root = insert(m_root, key);
}

template<typename Type>
inline void BinaryTree<Type>::erase(Type key)
{
	erase(m_root, key);
}

template<typename Type>
TreeNode<Type>* BinaryTree<Type>::search(Type key)
{
	return search(m_root, key);
}

template<typename Type>
inline std::vector<Type> BinaryTree<Type>::preorder()
{
	std::vector<Type> out;

	for (TreeNode<Type>* node : preorder(m_root))
	{
		out.emplace_back(node->key);
	}

	return out;
}

template<typename Type>
inline std::vector<Type> BinaryTree<Type>::inorder()
{
	std::vector<Type> out;
	
	for (TreeNode<Type>* node : inorder(m_root))
	{
		out.emplace_back(node->key);
	}

	return out;
}

template<typename Type>
inline std::vector<Type> BinaryTree<Type>::postorder()
{
	std::vector<Type> out;

	for (TreeNode<Type>* node : postorder(m_root))
	{
		out.emplace_back(node->key);
	}

	return out;
}

template<typename Type>
void BinaryTree<Type>::print()
{
	print(m_root, 0);
}