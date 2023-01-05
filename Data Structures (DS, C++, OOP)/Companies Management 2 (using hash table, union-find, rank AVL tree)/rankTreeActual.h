#ifndef WET2_RANKTREEACTUAL_H
#define WET2_RANKTREEACTUAL_H

#ifndef RANKTREE_H
#define RANKTREE_H

#include<iostream>
#include "SalaryIdKey.h"
#include "Employee.h"

namespace DS
{
    template<class Company>
    class RankTree
    {
    public:
        struct Node {

            salaryIdKey key;
            std::shared_ptr<Employee<Company>> data;
            int height;
            long long rank;
            long long sum;
            Node* left;
            Node* right;
            Node* parent;

            ~Node() = default;
//            ~Node() {
//                delete data;
//            };

            int getBalance() {
                int left_height = 0, right_height = 0;
                if (left) {
                    left_height = left->height;
                }
                if (right) {
                    right_height = right->height;
                }
                return left_height - right_height;
            }

            Node* getNodePlacedK (int k) {
                int place = rank;
                if (right) {
                    place -= right->rank;
                }
                if (place == k) {
                    return this;
                }
                if (place > k) {
                    return left->getNodePlacedK(k);
                }
                if (place < k) {
                    return right->getNodePlacedK(k-place);
                }
                return nullptr;
            }

            Node* getMin()
            {
                if (this->left == nullptr)
                    return this;
                return this->left->getMin();
            }


            void deleteAllChildren()
            {
                if (!this){
                    return;
                }
                if (this->left)
                    this->left->deleteAllChildren();
                if (this->right)
                    this->right->deleteAllChildren();
                delete this;
            }

            Node* findFirstInRange (int minSalary, int maxSalary) {
                if (!this) {
                    return nullptr;
                }
                int currSalary = key.getSalary();
                if (currSalary > maxSalary) {
                    if (left) {
                        return left->findFirstInRange(minSalary,maxSalary);
                    }
                    else {
                        return nullptr;
                    }
                }
                if (currSalary < minSalary) {
                    if (right) {
                        return right->findFirstInRange(minSalary,maxSalary);
                    }
                    else {
                        return nullptr;
                    }
                }
                return this;
            }

//            Node* findSmallestInRange (int minSalary) {
//                //assuming salary < max salary
//                if (left && left->data->getSalary() >= minSalary) {
//                    return left->findSmallestInRange(minSalary);
//                }
//                return this;
//            }
//
//            Node* findGreatestInRange (int maxSalary) {
//                //assuming salary > min salary
//                if (right && right->data->getSalary() <= maxSalary) {
//                    return right->findSmallestInRange(maxSalary);
//                }
//                return this;
//            }

            bool isLeaf()
            {
                return this->left == nullptr && this->right == nullptr;
            }

            bool hasTwoSons()
            {
                return this->left != nullptr && this->right != nullptr;
            }
        };
    private:
        Node* root;
        Node* smallest; // most left in the tree

        void rightRotation(Node* root);
        void leftRotation(Node* root);
        void updateSmallestNode();

        Node* findNodeHelper(Node* node, const salaryIdKey &key) const;
        //        Node findNodeHelper(Node node, const salaryIdKey &key, const std::shared_ptr<Employee<Company>> &data) const;

        int findNumOfLeftest(const salaryIdKey &key) const;
        Node* findNodeClosestTo(const salaryIdKey &key) const;
        void rebalance(Node* iterator);
        //to delete!
        void testRootsHeight(Node* root, bool* isHeightOk) const;
        //not to delete

        template <class Func>
        void searchAfterOrder(Node* root, const Func &functor);

        template <class Func>
        void searchBeforeOrder(Node* root, const Func &functor);

        Node* searchParentForInsert(Node* n, const salaryIdKey &key);

    public:
        unsigned long long num_of_nodes;
        RankTree() : root(nullptr), smallest(nullptr), num_of_nodes(0){};
        ~RankTree()
        {
            if (this->root)
                this->root->deleteAllChildren();
        }

        class sumGradesFunctor {
            int m;
            long long* sum;
        public:
            sumGradesFunctor(int m, long long* sum) : m(m), sum(sum) {}
            bool operator() (RankTree<Company>::Node* node, bool isExitRule) {
                if (isExitRule) {
                    if (m<=0) {
                        return false;
                    }
                    return true;
                }
                (*sum)+=node->data->getGrade();
                m--;
                return true;
            }
        };

        void updateHeights(Node* node);
        void updateRanks(Node* node);
        void updateSums(Node* node);
        void insertNode(const salaryIdKey &key, std::shared_ptr<Employee<Company>> data); // going to insert a new node somewhere in the tree
        void removeNode(const salaryIdKey &key);                   // removes the node with the given key
        void setRoot(Node* newRoot);
        bool doesKeyExist(const salaryIdKey &key);
        void updateSumOfNodeAndAllParents (Node* node);
        bool isNodeInTree(Node* node);
        Node* getNodePlacedK (int k);
        long long getSumGrades(int m);
        Node* findFirstNodeInRange(int minSalary, int maxSalary) const;
        Node* findNode(const salaryIdKey &key) const;
        //        Node findNode(const salaryIdKey &key, const std::shared_ptr<Employee<Company>> &data) const;
        Node* getRoot() const
        {
            return root;
        }

        //to delete!
        class calcAvgFunc {
            int lowerSalary;
            int higherSalary;
            int* sum;
            int* rank;
        public:
            calcAvgFunc(int lower, int higher, int* sum, int* rank) : lowerSalary(lower), higherSalary(higher), sum(sum), rank(rank) {}
            bool operator() (Node* node, bool isExitRule) {
                std::shared_ptr<Employee<Company>> employee = node->data;
                if (isExitRule) {
                    return true;
                }
                if (employee->getSalary() <= higherSalary && employee->getSalary() >= lowerSalary) {
                    (*sum)+=employee->getGrade();
                    (*rank)++;
                }
                return true;
            }
        };

        double calcAvgNaively (int lowerSalary, int higherSalary, int sum, int rank) {
            if (!root) {
                return 0;
            }
            //sum and rank should include the hash sum rank
            calcAvgFunc calcSumRank(lowerSalary,higherSalary,&sum, &rank);
            doInOrder(root,calcSumRank);
            return (double)sum/rank;
        }

        class nullCheckFunc {
            bool* nullNode;
        public:
            nullCheckFunc(bool* nullNode) : nullNode(nullNode) {}
            bool operator() (Node* node, bool isExitRule) {
                if (isExitRule) {
                    if (node && !node->data) {
                        return false;
                    }
                    return true;
                }
                if (node && !node->data) {
                    (*nullNode)=true;
                }
                return true;
            }
        };

        bool testForNullNodeData(bool toPrint) {
            bool isThereNullNode=false;
            nullCheckFunc checker(&isThereNullNode);
            doInOrder(root,checker);
            if (isThereNullNode && toPrint) {
                displayAvl(root);
            }
            return isThereNullNode;
        }
        //not to delete

        Node* getMinNode() const
        {

            if(!this->root){
                return nullptr;
            }
            Node* temp_node = root;

            while(temp_node)
            {
                if(!temp_node->left)
                {
                    return temp_node;
                }
                temp_node = temp_node->left;
            }
            return temp_node;
        }

        Node* findNextInSize(Node* node) const{
            if(node->parent){
                if(node->parent->right && node->parent->right != node) {
                    return node->parent->right;
                }
                return node->parent;
            }
            return node->right;
        }

        Node* findOneSmaller(Node* node) const {
            if (!node) {
                return nullptr;
            }
            if (node->right) {
                if (node->left) {
                    if (node->left->right) {
                        return node->left->right;
                    }
                    return node->left;
                }
                if (node->parent && node->parent->right == node) {
                    return node->parent;
                }
                return nullptr;
            }
            else {
                if (node->left) {
                    return node->left;
                }
                if (node->parent && node->parent->right == node) {
                    return node->parent;
                }
            }
            return nullptr;
        }

        Node* getMaxNode() const
        {
            if(!root){
                return nullptr;
            }
            Node* temp_node = root;

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

        int calcSum (Node* root, bool* isTreeOk) {
            if (!root) {
                return 0;
            }
            int sumLeft = 0, sumRight = 0;
            if (root->left) {
                sumLeft+=calcSum(root->left, isTreeOk);
            }
            if (root->right) {
                sumRight+=calcSum(root->right, isTreeOk);
            }
            if (sumLeft + sumRight + root->data->getGrade() != root->sum) {
                (*isTreeOk)=false;
            }
            return sumLeft + sumRight + root->data->getGrade();
        }

        int calcRank (Node* root, bool* isTreeOk) {
            if (!root) {
                return 0;
            }
            int rankLeft = 0, rankRight = 0;
            if (root->left) {
                rankLeft+=calcRank(root->left, isTreeOk);
            }
            if (root->right) {
                rankRight+=calcRank(root->right, isTreeOk);
            }
            if (rankLeft + rankRight + 1 != root->rank) {
                (*isTreeOk)=false;
            }
            return rankLeft + rankRight + 1;
        }

        bool isSumAndRankOk (bool toPrint) {
            bool isTreeSumOk=true, isTreeRankOk=true;
            if (!root) {
                return true;
            }
            int calculatedSum = calcSum(root, &isTreeSumOk);
            int calculatedRank = calcRank(root,&isTreeRankOk);
            if ((!isTreeRankOk || !isTreeSumOk) && toPrint) {
                displayAvl(root);
                std::cout << "The expected root's rank was " << calculatedRank << " and the actual root's rank in the tree is " << root->rank << std::endl;
                std::cout << "The expected root's sum was " << calculatedSum << " and the actual root's sum in the tree is " << root->sum << std::endl;
            }
            return isTreeSumOk && isTreeRankOk;
        }

        unsigned int calcNumNodes(Node* node) {
            if (!node) {
                return 0;
            }
            unsigned int currNumNodes = 1;
            if (node->left) {
                currNumNodes += calcNumNodes(node->left);
            }
            if (node->right) {
                currNumNodes += calcNumNodes(node->right);
            }
            return currNumNodes;
        }

        bool isNumNodesOk(bool toPrint) {
            unsigned int trueNumNodes = calcNumNodes(root);
            if (trueNumNodes != num_of_nodes && toPrint) {
                displayAvl(root);
                std::cout << "The expected tree num of nodes is " << trueNumNodes << " but the actual num is " << num_of_nodes << std::endl;
            }
            return trueNumNodes == num_of_nodes;
        }

        void displayAvl(Node* t) const
        {
            if (!t)
                return;
            if (!t->data) {
                std::cout << "OH NO! This node is faulty!" << std::endl;
                return;
            }
            std::cout << "salary: " << t->key.getSalary() << ", id-" << t->data->getId() << ", Rank-"<< t->rank << ", Sum-"<<t->sum << ", height-" << t->height << ", grade-" << t->data->getGrade() << std::endl;
            //printf("salaryIdKey-%d, Rank-%d, Sum-%d", t->key.getSalary(), t->rank, t->sum);


            if (t->left != nullptr)
                std::cout << "left son salary: " << t->left->key.getSalary();
            else {
                std::cout << "left son is NULL";
            }
            std::cout << std::endl;
                // printf("(L:%d)", t->left->key);
            if (t->right != nullptr)
                std::cout << "right son salary: " << t->right->key.getSalary();
            else {
                std::cout << "right son is NULL";
            }
            std::cout << std::endl;
                //printf("(R:%d)", t->right->key);
                // printf("\n");
            this->displayAvl(t->left);
            this->displayAvl(t->right);
        }

        // print the tree structure on the screen
        void prettyPrint()
        {
            this->displayAvl(this->root);
            // printHelper(this->root, "", true);
        }

        template <class Func>
        void doInOrder(Node* root, Func &functor);

        template <class Func>
        void doOppositeInOrder(Node* root, Func &functor);
        int getSize(){
            return num_of_nodes;
        }

        template <class Func>
        void doPostOrder(Node* root, Func &functor);

        int getInRange(salaryIdKey min, salaryIdKey max);
    };

    template <class Company>
    typename RankTree<Company>::Node* RankTree<Company>::findNode(const salaryIdKey &key) const
    {
        return findNodeHelper(this->root, key);
    }
    template <class Company>
    typename RankTree<Company>::Node* RankTree<Company>::findNodeHelper(Node* node, const salaryIdKey &key) const
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

    template <class Company>
    typename RankTree<Company>::Node*  RankTree<Company>::getNodePlacedK(int k) {
        return root->getNodePlacedK(k);
    }

    template <class Company>
    typename RankTree<Company>::Node* RankTree<Company>::findNodeClosestTo(const salaryIdKey &key) const{
        Node* curr = root;
        while ((curr != nullptr) && (curr->key != key)) {
            if (key < curr->key) { // left subtree
                if (curr->left == nullptr) {
                    return curr;
                } else {
                    curr = curr->left;
                }
            } else if (key > curr->key) { // right subtree
                if (curr->right == nullptr) {
                    return curr;
                } else {
                    curr = curr->right;
                }
            } else { // equal
                return curr;
            }
        }
        return curr;
    }

    template<class Company>
    bool RankTree<Company>::isNodeInTree(Node *node) {
        return findNode(node->key) != nullptr;
    }

    template<class Company>
    bool RankTree<Company>::doesKeyExist(const salaryIdKey &key) {
        return (findNode(key) != nullptr);
    }

    // Updating the smallest key. O(log(k)).
    template<class Company>

    void RankTree<Company>::updateSmallestNode() {
        Node* curr = root;
        if (!curr) {
            this->smallest = nullptr;
            return;
        }
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        this->smallest = curr;
    }


    template<class Company>
    int RankTree<Company>::findNumOfLeftest(const salaryIdKey &key) const {
        int smaller_nodes = 0;
        Node* curr = this->findNodeClosestTo(key);
        if (curr == nullptr) {
            return 0;
        }

        if (curr->key < key || curr->key == key) {
            smaller_nodes += 1; // count parent node
            if (curr->left) // cont left childs
                smaller_nodes += curr->left->rank;
        }

        while (curr != root) {
            Node* parent = curr->parent;
            if (parent->left && parent->left->key == curr->key) { // if current is left child of it's parent
                // do nothing
            } else { // if current is right child of it's parent
                smaller_nodes += 1; // count parent node
                if (parent->left) // cont left childs
                    smaller_nodes += parent->left->rank;
            }
            curr = parent;
        }
        return smaller_nodes;
    }

    template<class Company>
    int RankTree<Company>::getInRange(salaryIdKey min, salaryIdKey max) {
        int smallerThenMax = findNumOfLeftest(max);
        int smallerThenMin = findNumOfLeftest(min);
        int range = smallerThenMax - smallerThenMin;
        if (doesKeyExist(min))
            range++;
        return range;
    }

    template<class Company>
    void RankTree<Company>::updateSumOfNodeAndAllParents(Node *node) {
        Node* tmp = node;
        while (tmp) {
            updateSums(tmp);
            tmp = tmp->parent;
        }
    }

    template<class Company>
    long long RankTree<Company>::getSumGrades(int m) {
        //third try
//        if (!root || m > num_of_nodes) {
//            return 0;
//        }
//        Node* lowestNodeThatMatches = getNodePlacedK(num_of_nodes - m + 1);
//        int sum = lowestNodeThatMatches->sum;
//        if (lowestNodeThatMatches->left) {
//            sum -= lowestNodeThatMatches->left->sum;
//        }
//        return sum;
//second try -- works perfectly, but is higher than required complexity
        long long sum = 0;
        sumGradesFunctor summarizer(m,&sum);
        doOppositeInOrder(root,summarizer);
        return sum;
//first implementation
//        int sum = 0;
//        Node* max_node = getMaxNode();
//        if (!max_node) {
//            return 0;
//        }
//        while(m > 0 && max_node) {
//            sum += max_node->data->getGrade();
//            max_node = findOneSmaller(max_node);
//            m--;
//        }
//        return sum;
    }

    template <class Company>
    void RankTree<Company>::rightRotation(Node* root)
    {
        if(!root){
            return;
        }
        Node* temp_node = root->left;
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


        updateHeights(temp_node->right);
        updateHeights(temp_node);
        updateHeights(root);
        updateRanks(temp_node->right);
        updateRanks(temp_node);
        updateRanks(root);
        updateSums(temp_node->right);
        updateSums(temp_node);
        updateSums(root);
    }

    template <class Company>
    void RankTree<Company>::leftRotation(Node* root)
    {

        if(!root){
            return;
        }
        Node* temp_node = root->right;

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


        updateHeights(temp_node->right);
        updateRanks(temp_node->right);
        updateSums(temp_node->right);
        updateHeights(root);
        updateHeights(temp_node);
        updateRanks(root);
        updateRanks(temp_node);
        updateSums(root);
        updateSums(temp_node);
    }

    template <class Company>
    void RankTree<Company>::rebalance(Node* iterator)
    {
        //to delete!!!
//        bool isTreeSumAndRankOK = isSumAndRankOk(false);
        //not to delete
        if (iterator->getBalance() > 1) // left subtree is bigger
        {
            if (iterator->left && iterator->left->getBalance() > 0)
                rightRotation(iterator);
            else
            {
                leftRotation(iterator -> left);
                rightRotation(iterator);
            }
        }
        else if (iterator->getBalance() < -1) // right subtree is bigger
        {

            if (iterator->right && iterator->right->getBalance() < 0)
                leftRotation(iterator);
            else
            {
                rightRotation(iterator -> right);
                leftRotation(iterator);
            }
        }
        updateSmallestNode();
        //to delete!!!
//        if (isTreeSumAndRankOK && !isSumAndRankOk(true)) {
//            std::cout << "The error occurred after rebalance of node of " << *iterator->data << std::endl;
//        }
        //not to delete
    }

    // Updating the height property of the node and his ancestors.
    template<class Company>
    void RankTree<Company>::updateHeights(Node* node) {
        if (!node) {
            return;
        }
        Node* curr = node;
        //while (curr != nullptr) {
            int left_height = 0, right_height = 0;
            if (curr->left) {
                left_height = curr->left->height;
            }
            if (curr->right) {
                right_height = curr->right->height;
            }
            curr->height = ((left_height > right_height) ? left_height : right_height) + 1;
            //curr = curr->parent;
        //}
    }

    // Updating the rank property of the node and his ancestors.
    template<class Company>
    void RankTree<Company>::updateRanks(Node* node) {
        if (!node) {
            return;
        }
        Node* curr = node;
        //while (curr != nullptr) {
            long long left_rank = 0, right_rank = 0;
            if (curr->left) {
                left_rank = curr->left->rank;
            }
            if (curr->right) {
                right_rank = curr->right->rank;
            }
            curr->rank = left_rank + right_rank + 1;
            //curr = curr->parent;
        //}
    }

    // Updating the sum property of the node and his ancestors.
    template<class Company>
    void RankTree<Company>::updateSums(Node* node) {
        if (!node) {
            return;
        }
        Node* curr = node;
        //while (curr != nullptr) {
            long long left_sum = 0, right_sum = 0;
            if (curr->left) {
                left_sum = curr->left->sum;
            }
            if (curr->right) {
                right_sum = curr->right->sum;
            }
            curr->sum = left_sum + right_sum + curr->data->getGrade();
            //curr = curr->parent;
        //}
    }


    //to delete!!
    template <class Company>
    void RankTree<Company>::testRootsHeight(Node *root, bool *isHeightOk) const {
        if (!*isHeightOk || !root) {
            return;
        }
        if (root->left) {
            testRootsHeight(root->left,isHeightOk);
        }
        if (root->right) {
            testRootsHeight(root->right,isHeightOk);
        }
        if (!root->right && !root->left) {
            if (root->height>1) {
                (*isHeightOk)=false;
            }
        }
    }
    //not to delete

    template <class Company>
    void RankTree<Company>::removeNode(const salaryIdKey &key)
    {
        //to delete!!! for testing height issues
//        bool isThereNullNode = testForNullNodeData(false);
//        bool allRootsHeightsOk=true;
//        testRootsHeight(root,&allRootsHeightsOk);
//        bool isSumRankOK = isSumAndRankOk(false);
        //not to delete
        Node* node_to_remove = findNode(key);
        if (!node_to_remove)
            return;
        Node* parent_of_node = node_to_remove->parent;
        if (node_to_remove->isLeaf())
        {
            if (!node_to_remove->parent) {
                this->root = nullptr;
                this->smallest = nullptr;
            }
            //added else - seg fault
            else {
                if (node_to_remove->parent->right == node_to_remove)
                    node_to_remove->parent->right = nullptr;
                else
                    node_to_remove->parent->left = nullptr;
            }
            parent_of_node = node_to_remove->parent;
            delete node_to_remove;
        }
        else if (node_to_remove->hasTwoSons())
        {
            Node* succ = node_to_remove->right->getMin();

            node_to_remove->data = succ->data;
            node_to_remove->key = succ->key;

            if (succ->parent->left == succ){
                if(succ -> right) {
                    succ->parent->left = succ -> right;
                } else{
                    succ->parent->left = nullptr;
                }
            } else {
                succ->parent->right = succ -> right;
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
            //added this - what if parent is null
            if (parent_of_node) {
                parent_of_node = node_to_remove;
            }
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


        for (Node* n = parent_of_node; n; n = n->parent)
        {
            updateHeights(n);
            updateRanks(n);
            updateSums(n);
            rebalance(n);
        }
        if (num_of_nodes < 2 && root) {
            updateHeights(root);
            updateSums(root);
            updateRanks(root);
        }
        //to delete!!!! testing height issue
//        if (!isThereNullNode && testForNullNodeData(true)) {
//            std::cout << "After removal of node with salary " << key.getSalary() << " and id " << key.getId() << " there is a node with null data. Printing tree:" << std::endl;
//            displayAvl(root);
//        }
//        if (allRootsHeightsOk) {
//            testRootsHeight(root,&allRootsHeightsOk);
//            if (!allRootsHeightsOk) {
//                std::cout << "AHA!! After removing employee Id " << key.getId() << " salary: " << key.getSalary()
//                << ", one of the leaves are now with height > 1. Printing the tree: " << std::endl;
//                displayAvl(root);
//            }
//        }
//        if (isSumRankOK && !isSumAndRankOk(true)) {
//            std::cout << "The error occurred after removing a node with salary " << key.getSalary() << " and id " << key.getId() << std::endl;
//        }
        //not to delete
    }

    template <class Company>
    void RankTree<Company>::setRoot(Node* newRoot) {
        Node* temp=root;
        root=newRoot;
        delete temp;
    }

    template <class Company>
    typename RankTree<Company>::Node* RankTree<Company>::findFirstNodeInRange(int minSalary, int maxSalary) const {
        return root->findFirstInRange(minSalary,maxSalary);
    }


    template <class Company>
    template <class Func>
    void RankTree<Company>::searchAfterOrder(Node* root, const Func &functor)
    {
        if (!root)
        {
            return;
        }
        searchAfterOrder<Func>(root->left, functor);
        searchAfterOrder<Func>(root->right, functor);
        functor(root);
    }

    template <class Company>
    template <class Func>
    void RankTree<Company>::searchBeforeOrder(Node* root, const Func &functor)
    {
        if (!root)
        {
            return;
        }
        functor(root);
        searchBeforeOrder<Func>(root->left, functor);
        searchBeforeOrder<Func>(root->right, functor);
    }

    template <class Company>
    template <class Func>
    void RankTree<Company>::doInOrder(Node* root, Func &functor)
    {
        if (!root || !functor(root, true))
        {
            return;
        }
        doInOrder<Func>(root->left, functor);
        if (functor(root,true)) {
            functor(root, false);
        }
        doInOrder<Func>(root->right, functor);
    }

    template <class Company>
    template <class Func>
    void RankTree<Company>::doPostOrder(Node* root, Func &functor)
    {
        if (!root || !functor(root, true))
        {
            return;
        }
        doPostOrder<Func>(root->left, functor);
        doPostOrder<Func>(root->right, functor);
        if (functor(root,true)) {
            functor(root, false);
        }
    }

    template <class Company>
    template <class Func>
    void RankTree<Company>::doOppositeInOrder(Node* root, Func &functor)
    {
        if (!root || !functor(root, true))
        {
            return;
        }
        doOppositeInOrder<Func>(root->right, functor);
        if (functor(root,true)) {
            functor(root, false);
        }
        doOppositeInOrder<Func>(root->left, functor);
    }

    template <class Company>
    void RankTree<Company>::insertNode(const salaryIdKey &key,std::shared_ptr<Employee<Company>> data)
    {
        //to delete!!! for testing height issues
//        bool allRootsHeightsOk=true;
//        testRootsHeight(root,&allRootsHeightsOk);
//        bool isSumRankOk = isSumAndRankOk(false);
        //not to delete
        Node* new_node = new Node();
        new_node->height = 1; // height of the subtree with this node as root.
        new_node->rank = 1;
        new_node->sum = data->getGrade();
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->parent = nullptr;
        new_node->key = key;
        new_node->data = data;

        this->num_of_nodes++;
        if (!this->root) {
            this->root = new_node;
            this->smallest = new_node;
        } else {
            Node* parent = searchParentForInsert(this->root, key);
            if (key < parent->key)
                parent->left = new_node;
            else
                parent->right = new_node;
            new_node->parent = parent;

            for (Node* n = parent; n; n = n->parent)
            {
                updateHeights(n);
                updateRanks(n);
                updateSums(n);
                rebalance(n);
            }
        }
        //to delete!!!! testing height issue
//        if (allRootsHeightsOk) {
//            testRootsHeight(root,&allRootsHeightsOk);
//            if (!allRootsHeightsOk) {
//                std::cout << "AHA!! After adding employee Id " << key.getId() << " salary: " << key.getSalary()
//                          << ", one of the leaves are now with height > 1. Printing the tree: " << std::endl;
//                displayAvl(root);
//            }
//        }
//        if (isSumRankOk && !isSumAndRankOk(true)) {
//            std::cout << "The error occurred after adding the node with salary " << key.getSalary() << " and id " << key.getId() << std::endl;
//        }
        //not to delete
    }

    template <class Company>
    typename RankTree<Company>::Node* RankTree<Company>::searchParentForInsert(Node* n, const salaryIdKey &key)
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

}


#endif


#endif //WET2_RANKTREEACTUAL_H
