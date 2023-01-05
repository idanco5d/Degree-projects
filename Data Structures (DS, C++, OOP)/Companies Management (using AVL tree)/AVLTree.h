#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include "Node.h"

namespace DS
{
    template <class Key, class Data>
    class AvlTree
    {
        typedef AvlNode<Key, Data>* Node;

    private:
        Node root;

        void rightRotation(Node root);
        void leftRotation(Node root);

        void rebalance(Node iterator);

        Node searchParentForInsert(Node n, const Key &key);

    public:
        unsigned int num_of_nodes;
        AvlTree() : root(nullptr), num_of_nodes(0){};
        ~AvlTree()
        {
            if (this->root)
                this->root->deleteAllChildren();
        }

        Node findNodeHelper(Node node, const Key &key) const;
        Node findNode(const Key &key) const;

        void insertNode(const Key &key, const Data &data); // going to insert a new node somewhere in the tree
        void removeNode(const Key &key);                   // removes the node with the given key
        void removeNode(const Key &key, const Data &data);
        void setRoot(Node newRoot);
        Node getRoot() const
        {
            return root;
        }
        AvlTree* clone () const;

        Node getMinNode() const
        {

            if(!this->root){
                return nullptr;
            }
            AvlNode<Key, Data>* temp_node = root;

            while(temp_node)
            {
                if(!temp_node->left)
                {
                    return temp_node;
                }
                temp_node = temp_node->left;
            }
        }
        Node getMaxNode() const
        {
            if(!this->root){
                return nullptr;
            }
            AvlNode<Key, Data>* temp_node = root;

             while(temp_node)
             {
                 if(!temp_node->right)
                 {
                     return temp_node;
                 }
                 temp_node = temp_node->right;
             }
             return temp_node;
        }

        template <class Func>
        void doInOrder(Node root, Func &functor);

        template <class Func>
        void doOppositeInOrder(Node root, Func &functor);

        template <class Func>
        void doPostOrder(Node root, Func &functor);
    };

    template <class Key, class Data>
    AvlNode<Key, Data>* AvlTree<Key, Data>::findNode(const Key &key) const
    {
        return findNodeHelper(this->root, key);
    }
    template <class Key, class Data>
    AvlNode<Key, Data>* AvlTree<Key, Data>::findNodeHelper(Node node, const Key &key) const
    {
        if (!node || key == node->key)
        {
            return node;
        }

        if (key < node->key)
        {
            return findNodeHelper(node->left, key);
        }
        return findNodeHelper(node->right, key);
    }

    template <class Key, class Data>
    void AvlTree<Key, Data>::rightRotation(Node root)
    {
        if(!root){
            return;
        }
        Node temp_node = root->left;
        root->left = temp_node->right;
        temp_node->right = root;

        if (root->left)
        {
            root->left->parent = root;
        }
        if (!root->parent)
        {
            this->root = temp_node;
        }
        else
        {
            if (root->parent->right == root)
                root->parent->right = temp_node;
            else
                root->parent->left = temp_node;
        }
        temp_node->parent = root->parent;
        root->parent = temp_node;

        root->updateHeight();
        if(temp_node -> right)
            temp_node->right->updateHeight();
        temp_node->updateHeight();

        root->calculateBf();
        temp_node->calculateBf();
    }

    template <class Key, class Data>
    void AvlTree<Key, Data>::leftRotation(Node root)
    {

        if(!root){
            return;
        }
        Node temp_node = root->right;

        root->right = temp_node->left;
        temp_node->left = root;

        if (root->right)
        {
            root->right->parent = root;
        }
        if (!root->parent)
        {
            this->root = temp_node;
        }
        else
        {
            if (root->parent->right == root)
                root->parent->right = temp_node;
            else
                root->parent->left = temp_node;
        }
        temp_node->parent = root->parent;
        root->parent = temp_node;

        root->updateHeight();
        if(temp_node->right)
            temp_node->right->updateHeight();
        temp_node->updateHeight();

        root->calculateBf();
        temp_node->calculateBf();
    }

    template <class Key, class Data>
    void AvlTree<Key, Data>::rebalance(Node iterator)
    {
        if (iterator->bf > 1) // left subtree is bigger
        {
            if (iterator->left && iterator->left->bf > 0)
                rightRotation(iterator);
            else
            {
                leftRotation(iterator -> left);
                rightRotation(iterator);
            }
        }
        else if (iterator->bf < -1) // right subtree is bigger
        {

            if (iterator->right && iterator->right->bf < 0)
                leftRotation(iterator);
            else
            {
                rightRotation(iterator -> right);
                leftRotation(iterator);
            }
        }
    }


    template <class Key, class Data>
    void AvlTree<Key, Data>::removeNode(const Key &key)
    {
        Node node_to_remove = findNode(key);
        Node parent_of_node = nullptr;
        if (!node_to_remove)
            return;

        if (node_to_remove->isLeaf())
        {
            if (!node_to_remove->parent)
                this->root = nullptr;
            else {
                if (node_to_remove->parent->right == node_to_remove)
                node_to_remove->parent->right = nullptr;
            else
                node_to_remove->parent->left = nullptr;
            parent_of_node = node_to_remove->parent;
            }
            delete node_to_remove;
        }
        else if (node_to_remove->hasTwoSons())
        {
            Node succ = node_to_remove->right->getMin();

            node_to_remove->data = succ->data;
            node_to_remove->key = succ->key;
            if (succ->parent)
            {
                if (succ->parent->left == succ){
                    if(succ -> right) {
                        succ->parent->left = succ -> right;
                    } else{
                        succ->parent->left = nullptr;
                    }
                } else {
                    succ->parent->right = succ -> right;
                }
            }
            parent_of_node = succ->parent;

            delete succ;
        }
        else
        {
            if (node_to_remove->left)
            {
                node_to_remove->data = node_to_remove->left->data;
                node_to_remove->key = node_to_remove->left->key;
                delete node_to_remove->left;
                node_to_remove->left = nullptr;
            }
            else
            {
                node_to_remove->data = node_to_remove->right->data;
                node_to_remove->key = node_to_remove->right->key;
                delete node_to_remove->right;
                node_to_remove->right = nullptr;
            }
            parent_of_node = node_to_remove;
        }
        this->num_of_nodes--;

        if(parent_of_node) {
            if(parent_of_node->hasTwoSons()){
                if(parent_of_node -> left -> parent != parent_of_node){
                    parent_of_node -> left -> parent = parent_of_node;
                }
                if(parent_of_node -> right -> parent != parent_of_node){
                    parent_of_node -> right -> parent = parent_of_node;
                }
            } else if(!(parent_of_node->hasTwoSons()) && !(parent_of_node->isLeaf())) {
                if(!parent_of_node -> left) {
                    if(parent_of_node -> right -> parent != parent_of_node){
                        parent_of_node -> right -> parent = parent_of_node;
                    }
                } else {
                    if(parent_of_node -> left -> parent != parent_of_node){
                        parent_of_node -> left -> parent = parent_of_node;
                    }
                }
            }
        }

        for (AvlNode<Key, Data> *n = parent_of_node; n; n = n->parent)
        {
            n->updateHeight();
            n->calculateBf();
            rebalance(n);
        }
    }

    template <class Key, class Data>
    void AvlTree<Key, Data>::removeNode(const Key &key, const Data &data)
    {
        Node node_to_remove = findNode(key, data);
        Node parent_of_node;
        if (!node_to_remove)
            return;

        if (node_to_remove->isLeaf())
        {
            if (!node_to_remove->parent)
                this->root = nullptr;
            if (node_to_remove->parent->right == node_to_remove)
                node_to_remove->parent->right = nullptr;
            else
                node_to_remove->parent->left = nullptr;
            parent_of_node = node_to_remove->parent;
            delete node_to_remove;
        }
        else if (node_to_remove->hasTwoSons())
        {
            Node succ = node_to_remove->right->getMin();
            
            node_to_remove->data = succ->data;
            node_to_remove->key = succ->key;

            if (succ->parent->left == succ)
                succ->parent->left = nullptr;
            else
                succ->parent->right = nullptr;
            parent_of_node = succ->parent;
            delete succ;
        }
        else
        {
            if (node_to_remove->left)
            {
                node_to_remove->data = node_to_remove->left->data;
                node_to_remove->key = node_to_remove->left->key;
                delete node_to_remove->left;
                node_to_remove->left = nullptr;
            }
            else
            {
                node_to_remove->data = node_to_remove->right->data;
                node_to_remove->key = node_to_remove->right->key;
                delete node_to_remove->right;
                node_to_remove->right = nullptr;
            }
            parent_of_node = node_to_remove;
        }
        this->num_of_nodes--;

        for (AvlNode<Key, Data>* n = parent_of_node; n; n = n->parent)
        {
            n->updateHeight();
            n->calculateBf();
            rebalance(n);
        }
    }

    template <class Key, class Data>
    void AvlTree<Key,Data>::setRoot(Node newRoot) {
        root=newRoot;
    }

    template <class Key, class Data>
    template <class Func>
    void AvlTree<Key, Data>::doInOrder(Node root, Func &functor)
    {
        if (!root)
        {
            return;
        }
        if (root->left && functor(root->left,true))
        {
            doInOrder<Func>(root->left, functor);
        }
        if (functor(root,true))
        {
            functor(root, false);
        }
        if (root->right && functor(root->right,true))
        {
            doInOrder<Func>(root->right, functor);
        }
    }

    template <class Key, class Data>
    template <class Func>
    void AvlTree<Key, Data>::doOppositeInOrder(Node root, Func &functor)
    {
        if (!root)
        {
            return;
        }
        if (root->right && functor(root->right,true))
        {
            doOppositeInOrder<Func>(root->right, functor);
        }
        if (functor(root,true))
        {
            functor(root, false);
        }
        if (root->left && functor(root->left,true))
        {
            doOppositeInOrder<Func>(root->left, functor);
        }
    }

    template <class Key, class Data>
    template <class Func>
    void AvlTree<Key, Data>::doPostOrder(Node root, Func &functor)
    {
        if (!root)
        {
            return;
        }
        if (root->left && functor(root->left,true))
        {
            doInOrder<Func>(root->left, functor);
        }
        if (root->right && functor(root->right,true))
        {
            doInOrder<Func>(root->right, functor);
        }
        if (functor(root,true))
        {
            functor(root, false);
        }
    }

    template <class Key, class Data>
    void AvlTree<Key, Data>::insertNode(const Key &key, const Data &data)
    {
        AvlNode<Key, Data>* new_node (new AvlNode<Key, Data>(key, data));
        this->num_of_nodes++;
        if (!this->root)
            this->root = new_node;
        else
        {
            AvlNode<Key, Data>* parent = searchParentForInsert(this->root, key);
            if (key < parent->key)
                parent->left = new_node;
            else
                parent->right = new_node;
            new_node->parent = parent;

            for (AvlNode<Key, Data>* n = parent; n; n = n->parent)
            {
                n->updateHeight();
                n->calculateBf();
                rebalance(n);
            }
        }
    }

    template <class Key, class Data>
    AvlNode<Key, Data>* AvlTree<Key, Data>::searchParentForInsert(Node n, const Key &key)
    {
        if (key < n->key)
        {
            if (!n->left)
                return n;
            return searchParentForInsert(n->left, key);
        }
        if (!n->right)
            return n;
        return searchParentForInsert(n->right, key);
    }

    template <class Key, class Data>
    AvlTree<Key,Data>* AvlTree<Key, Data>::clone () const {
        Node copiedRoot = root->cloneThisAndAllChildren();
        AvlTree<Key,Data>* clonedTree = new AvlTree<Key,Data>();
        clonedTree->setRoot(copiedRoot);
        return clonedTree;
    }

}

#endif
