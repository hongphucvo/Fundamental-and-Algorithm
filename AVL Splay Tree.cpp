#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) {}
    };

public:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) {
        avl=new AVLTree();
        splay = new SplayTree();
        this->maxNumOfKeys = maxNumOfKeys;
    }
    ~BKUTree() { this->clear(); }
    void add(K key, V value) {
        int size = keys.size();
        if (size == maxNumOfKeys) {
            keys.pop();
        }
        keys.push(key);        
        Entry* entry1 = new Entry(key, value);
        Entry* entry2 = new Entry(key, value);
        typename AVLTree::Node* avladd = new typename AVLTree::Node(entry1, NULL, NULL);
        avl->root = avl->add(avl->root, avladd);
        typename SplayTree::Node* splayadd = new typename SplayTree::Node(entry2, NULL, NULL);
        splay->root = splay->add(splay->root, splayadd);
        splay->root = splay->Splay(splayadd, 0);
        avladd->corr = splayadd;
        splayadd->corr = avladd;
    }
    void remove(K key) {
        splay->remove(key);
        avl->remove(key);
        K newkey = splay->root->entry->key;
        int size = keys.size();
        if (contains(keys, key)) {
            for (int i = 0; i < size; i++) {
                if (keys.front() != key) {
                    keys.push(keys.front());
                }
                keys.pop();
            }
            keys.push(newkey);
        }
    }
    bool contains(queue<K> keyqueue, K key) {
        if (keyqueue.empty()) {
            return false;
        }
        if (keyqueue.front() == key) {
            return true;
        }
        keyqueue.pop();
        return contains(keyqueue, key);
    }
    V search(K key, vector<K>& traversedList) {
        V result;
        int size = keys.size();
        if (size == maxNumOfKeys)
            keys.pop();
        if (splay->root->entry->key == key) {
            keys.push(key);//add truoc hay add sa
            return splay->root->entry->value;
        }
        if (contains(keys,key)) {
            typename SplayTree::Node* found = splay->BinarySearch(splay->root, key, traversedList);
            result = found->entry->value;
            splay->root = splay->Splay(found, 1);
        }
        else {
            typename AVLTree::Node* rAVL = splay->root->corr;
            typename AVLTree::Node* found = avl->searchBKU(rAVL, key, traversedList);
            if (found != NULL) {
                result = found->entry->value;
            }
            else {
                typename AVLTree::Node* refound = avl->searchBKU(avl->root, key, traversedList, rAVL);
                if (refound == NULL)throw ("Not found");
                result = refound->entry->key;
                splay->root = splay->Splay(refound->corr, 1);
            }
        }
        keys.push(key);//add truoc hay add sau
        return result;
    }
    void traverseNLROnAVL(void (*func)(K key, V value)) {
        avl->traverseNLR(func);
    }
    void traverseNLROnSplay(void (*func)(K key, V value)) {
        splay->traverseNLR(func);
    }
    void clear() {
        delete avl;
        delete splay;
        avl = NULL;
        splay = NULL;
        clrqueue();
        this->maxNumOfKeys = 0;
    }
    void clrqueue() {
        while (!this->keys.empty()) {
            keys.pop();
        }
    }
    class SplayTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;
            typename AVLTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->parent = parent;
                this->corr = NULL;
            }
            ~Node() {
                delete entry;
                left = NULL;
                right = NULL;
                parent = NULL;
                corr = NULL;
            }
        };

    public:
        Node* root;

        SplayTree() : root(NULL) {};
        ~SplayTree() { this->clear(); };


        void add(K key, V value) {
            Entry* entry = new Entry(key, value);
            Node* added = new Node(entry, NULL, NULL, NULL);
            root = add(root, added);
            root = Splay(added);
        }
        void add(Entry* entry) {
            Node* added = new Node(entry, NULL, NULL, NULL);
            root = add(root, added);
            root = Splay(added);
        }
        Node* add(Node* node, Node* added) {
            if (node == NULL) {
                node = added;
                return added;
            }
            if (node->entry->key == added->entry->key)
                throw ("Duplicate key");
            if (node->entry->key > added->entry->key) {
                if (node->left == NULL)
                    added->parent = node;//node->left=added; else{}
                node->left = add(node->left, added);
            }
            else if (node->entry->key < added->entry->key)
            {
                if (node->right == NULL)
                    added->parent = node;
                node->right = add(node->right, added);
            }
            return node;
        }
        void remove(K key) {
            Node* rmv = find(root, key);
            rmv = Splay(rmv);
            Node* R = rmv->right;
            Node* L = rmv->left;
            delete rmv;
            R->parent = NULL;
            L->parent = NULL;
            root = L;
            Node* successor = largest(L);
            root = Splay(successor);
            root->right = R;
            R->parent = root;
        }
        Node* largest(Node* Lnode) {
            if (Lnode->right == NULL)
                return Lnode;
            return largest(Lnode->right);
        }
        V search(K key) {
            Node* temp = find(root, key);//tr
            temp = Splay(temp);
            return temp->entry->key;
        }
        Node* Splay(Node* node, int mode = 0) {
            Node* ancestor;
            if (node == root)
                return node;
            if (node->parent != NULL) {
                if (node->parent->parent != NULL) {
                    ancestor= node->parent->parent;
                }
                else {
                    ancestor = node->parent;
                }
                if (ancestor == root) {
                    ancestor = SplayOnce(ancestor, node->entry->key);
                    root = ancestor;
                }
                else 
                    ancestor = SplayOnce(ancestor, node->entry->key);
                if (mode == 0) 
                    return Splay(ancestor, mode);
                else return ancestor;
            }
            else {
                return node;
            }
        }
        Node* SplayOnce(Node* node, K key) {
            if (node->entry->key > key) {
                if (node->left->left != NULL)
                    if (node->left->left->entry->key == key)
                        return ZigZig(node);
                if (node->left->right != NULL)
                    if (node->left->right->entry->key == key) {
                        return ZigZag(node);
                    }
                if (node->left->entry->key == key)
                    return Zig(node);
                return node;
            }
            if (node->right->left != NULL)
                if (node->right->left->entry->key == key)
                    return ZagZig(node);
            if (node->right->right != NULL)
                if (node->right->right->entry->key == key)
                    return ZagZag(node);
            if (node->right->entry->key == key)
                return Zag(node);
            return node;
        }
        Node* Zig(Node* Parent) {
            Node* savedTree = Parent->left;
            Parent->left = savedTree->right;
            savedTree->right = Parent;
            Node* ancestor = Parent->parent;
            if (ancestor != NULL) {
                if (ancestor->left == Parent)
                    ancestor->left = savedTree;
                else ancestor->right = savedTree;
            }
            savedTree->parent = ancestor;
            if (Parent->left != NULL)
                Parent->left->parent = Parent;
            Parent->parent = savedTree;
            return savedTree;
        }
        Node* Zag(Node* Parent) {
            Node* savedTree = Parent->right;
            Parent->right = savedTree->left;
            savedTree->left = Parent;
            Node* ancestor = Parent->parent;
            if (ancestor != NULL) {
                if (ancestor->left == Parent)
                    ancestor->left = savedTree;
                else ancestor->right = savedTree;
            }
            savedTree->parent = ancestor;
            if (Parent->right != NULL)
                Parent->right->parent = Parent;
            Parent->parent = savedTree;
            return savedTree;
        }
        Node* ZigZig(Node* Parent) {
            Parent = Zig(Parent);
            return Zig(Parent);
        }
        Node* ZigZag(Node* Parent) {
            Parent->left = Zag(Parent->left);
            return Zig(Parent);
        }
        Node* ZagZig(Node* Parent) {
            Parent->right = Zig(Parent->right);
            return Zag(Parent);
        }
        Node* ZagZag(Node* Parent) {
            Parent = Zag(Parent);
            return Zag(Parent);
        }
        void traverseNLR(void (*func)(K key, V value)) {
            traverseSplay(root, func);
        }
        void traverseSplay(Node* node, void(*func)(K key, V value)) {
            if (node == NULL)
                return;
            func(node->entry->key, node->entry->value);
            traverseSplay(node->left, func);
            traverseSplay(node->right, func);

        }
        void clear() {
            clr(root);
            this->root = NULL;
        }
        void clr(Node* node) {
            if (node == NULL) {
                return;
            }  
            clr(node->left);
            clr(node->right);
            delete node;
        }
        Node* BinarySearch(Node* node, K key, vector<K>& traversList) {
            if (node == NULL) return node;
            if (node->entry->key > key) {
                traversList.push_back(node->entry->key);
                return BinarySearch(node->left, key, traversList);
            }
            if (node->entry->key < key) {
                traversList.push_back(node->entry->key);
                return BinarySearch(node->right, key, traversList);
            }
            return node;
        }
        Node* find(Node* node, K key) {
            if (node == NULL) throw ("Not found");//neu tim khong ra thi
            if (node->entry->key > key) {
                return find(node->left, key);
            }
            if (node->entry->key < key) {
                return find(node->right, key);
            }
            return node;
        }
    };

    class AVLTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
            }
            ~Node() {
                delete entry;
                entry = NULL;
                left = NULL;
                right = NULL;
                corr = NULL;
            }
        };

    public:
        Node* root;
        AVLTree() : root(NULL) {};
        ~AVLTree() { this->clear(); };
        
        
        void add(K key, V value) {
            Entry* entry = new Entry(key, value);
            Node* added = new Node(entry, NULL, NULL);
            root = add(root, added);
        }
        void add(Entry* entry) {
            Node* added = new Node(entry, NULL, NULL);
            root = add(root, added);
        }
        Node* add(Node* node, Node* added) {
            if (node == NULL) {
                return added;
            }
            if (node->entry->key == added->entry->key) {
                throw ("Duplicate key");
            }
            if (node->entry->key > added->entry->key) {
                int oldheight = height(node->left);
                node->left = add(node->left, added);
                if (height(node->left) > oldheight) {
                    node->left = Balance(node->left);
                    node = Balance(node);
                }
                return node;
            }
            //else if (node->entry->key < added->entry->key) {
            int oldheight = height(node->right);
            node->right = add(node->right, added);
            if (height(node->right) > oldheight) {
                node->right = Balance(node->right);
                node = Balance(node);
            }
            return node;
        }
        void remove(K key) {
            root = deleteItem(root, key);
        }
        Node* deleteItem(Node* node, K item) {
            if (node == NULL)
                throw ("Not found");
            K Item = node->entry->key;
            if (item < Item) {
                node->left = deleteItem(node->left, item);
                node->left = Balance(node->left);
            }
            else if (item > Item) {
                node->right = deleteItem(node->right, item);
                node->right = Balance(node->right);
            }
            else {
                node = deleteRoot(node);
            }
            node = Balance(node);
            return node;
        }
        Node* deleteRoot(Node* node) {
            Node* temp = node;
            Node* R = node->right;
            if (node->left == NULL) {
                delete temp;
                return R;
            }
            node = deleteSuccessor(node->left);
            node->right = R;
            delete temp;
            return node;
        }
        Node* deleteSuccessor(Node* node) {
            Node* Successor = node;
            Node* temp = Prune(node, Successor);
            Successor->left = temp;
            return Successor;
        }
        Node* Prune(Node* Lnode, Node*& Successor) {
            if (Lnode->right == NULL) {
                Successor = Lnode;
                return Lnode->left;
            }
            Lnode->right = Prune(Lnode->right, Successor);
            Lnode->right = Balance(Lnode->right);
            return Lnode;
        }
        V search(K key) {
            Node* found = search(this->root, key);
            return found->entry->value;
        }
        Node* search(Node* node, K key) {
            if (node == NULL)
                return node;
            if (node->entry->key == key)
                return node;
            if (node->entry->key > key) {
                return search(node->left, key);
            }
            return search(node->right, key);
        }
        Node* searchBKU(Node* node, K key, vector<K>& traverseList, Node* child = NULL) {
            if (node == NULL)
                return node;
            if (node == child)
                throw ("Not found");
            if (node->entry->key == key)
                return node;
            if (node->entry->key > key) {
                traverseList.push_back(node->entry->key);
                return searchBKU(node->left, key, traverseList, child);
            }
            traverseList.push_back(node->entry->key);
            return searchBKU(node->right, key, traverseList, child);
        }
        Node* rotateLeft(Node* Rnode) {
            Node* savedTree = Rnode->right;
            Rnode->right = savedTree->left;
            savedTree->left = Rnode;
            updateBalance(savedTree);
            updateBalance(Rnode);
            if (Rnode == root)
                root = savedTree;
            return savedTree;
        }
        Node* rotateRight(Node* Lnode) {
            Node* savedTree = Lnode->left;
            Lnode->left = savedTree->right;
            savedTree->right = Lnode;
            updateBalance(savedTree);
            updateBalance(Lnode);
            if (Lnode == root)
                root = savedTree;
            return savedTree;
        }
        Node* LeftRight(Node* node) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        Node* RightLeft(Node* node) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        Node* Balance(Node* node) {
            if (node == NULL)
                return node;
            int update = updateBalance(node);
            if (update == 0)
                return node;
            else if (update == 1) {
                if (node->right->balance == -1)
                    return LeftRight(node);
                return rotateLeft(node);//
            }
            else {
                if (node->left->balance == 1) 
                    return LeftRight(node);
                return rotateRight(node);//else
            }
        }
        int updateBalance(Node* node) {
            int hL = height(node->left);
            int hR = height(node->right);
            if (hL == hR) {
                node->balance = 0;
                return 0;
            }
            if (hL > hR) {
                node->balance = -1;
                if (hL - hR > 1) 
                    return -1;
                return 0;
            }
            node->balance = 1;
            if (hL - hR < -1) 
                return 1;
            return 0;
        }
        int height(Node* node) {
            if (node == NULL) 
                return 0;
            return 1 + max(height(node->left), height(node->right));
        }
        void traverseNLR(void (*func)(K key, V value)) {
            traverseAVL(root, func);
        }
        void traverseAVL(Node* node, void(*func)(K key, V value)) {
            if (node == NULL) 
                return;
            func(node->entry->key, node->entry->value);
            traverseAVL(node->left, func);
            traverseAVL(node->right, func);
        }
        void clear() {
            clr(root);
            this->root = NULL;
        }
        void clr(Node* node) {
            if (node == NULL) 
                return;
            clr(node->left);
            clr(node->right);
            delete node;
        }
    };
};
void printKey(int key, int value) {
    cout << key << endl;
    value = 0;
}

int main() {
    BKUTree<int, int>::AVLTree tree;
    int keys[] = { 1, 3, 5, 7, 9, 2, 4 };
    for (int i = 0; i < 7; i++) tree.add(keys[i], keys[i]);

    tree.clear();
    if (tree.root == NULL) cout << "Fuck: " << endl;
}