#ifndef AVL_NODE_H
#define AVL_NODE_H

#include <iostream>
#include <memory>

namespace DS
{
    template <class Key, class Data>
    struct AvlNode
    {
        typedef AvlNode* NodePtr;
        Key key;         // holds the key
        Data data;       // holds the data
        AvlNode* parent; // pointer to the parent
        AvlNode* left;   // pointer to left child
        AvlNode* right;  // pointer to right child
        int height;
        int bf; // balance factor of the node

        // creates new AvlNode with the given parameters
        AvlNode(const Key &key, const Data &data, AvlNode* parent = nullptr) : key(key), data(data),
                                                                               parent(parent), left(nullptr), right(nullptr), height(0), bf(0){};
        // copy constructor - default
        AvlNode(const AvlNode &node) = default;
        // destructor
        ~AvlNode() = default;

        AvlNode &operator=(const AvlNode &node) = default;

        friend bool operator<(const AvlNode &curr, const AvlNode &other)
        {
            return curr->key < other->key;
        }
        friend bool operator>(const AvlNode &curr, const AvlNode &other)
        {
            return curr->key > other->key;
        }
        friend bool operator==(const AvlNode &curr, const AvlNode &other)
        {
            if(curr == nullptr && other == nullptr) {
                return true;
            } else if (curr != nullptr && other != nullptr) {
                return curr->key == other->key;
            } else {
                return false;
            }

        }
        friend bool operator<=(const AvlNode &curr, const AvlNode &other)
        {
            return curr.key <= other->key;
        }
        friend bool operator>=(const AvlNode &curr, const AvlNode &other)
        {
            return curr.key >= other->key;
        }

        void deleteAllChildren()
        {
            if (this) {
            if (this->left)
                this->left->deleteAllChildren();
            if (this->right)
                this->right->deleteAllChildren();
            delete this;
            }
        }

        NodePtr getMin()
        {
            if (!this->left)
                return this;
            return this->left->getMin();
        }

        Data getData()
        {
            return this->data;
        }

        void setData(Data newData) {
            data = newData;
        }

        void setKey(Key newKey)
        {
            key = newKey;
        }

        void calculateBf()
        {
            const int left_hight = getHeight(left);
            const int right_height = getHeight(right);
            bf = left_hight - right_height;
        }

        static const int getHeight(const NodePtr node)
        {
            return (!node) ? -1 : node->height;
        }

        void updateHeight()
        {
            if(this == nullptr){
                return;
            }
            this->height = std::max(this->getHeight(this->left), this->getHeight(this->right)) + 1;
        }

        bool isLeaf()
        {
            return !this->left && !this->right;
        }

        bool hasTwoSons()
        {
            return this->left && this->right;
        }

        NodePtr clone() const {
            return new AvlNode<Key,Data>(*this);
        }

        NodePtr cloneThisAndAllChildren() const {
            NodePtr cloned=clone();
            if (cloned->right)
            {
                cloned->right->cloneThisAndAllChildren();
            }
            if (cloned->left)
            {
                cloned->left->cloneThisAndAllChildren();
            }
            return cloned;
        }

        static void swapData(NodePtr &node_a, NodePtr &node_b)
        {
            const Key key_temp = node_a->key;
            const Data data_temp = node_a->data;
            node_a->key = node_b->key;
            node_a->data = node_b->data;

            node_b->key = key_temp;
            node_b->data = data_temp;
        }
    };

    template <class Key, class Data>
    std::ostream &operator<<(std::ostream &os, const AvlNode<Key, Data> &node)
    {

        os << "(" << node.key << ")";

        return os;
    }

}

#endif
