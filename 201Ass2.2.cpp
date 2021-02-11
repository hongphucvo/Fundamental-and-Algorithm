#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class BusParking{
public:
    class Interval{
        int start;
        int end;
        Interval(int start = 0, int end = 0) {
            this->start = start;
            this->end = end;
        }
        int minPark(vector<Interval> lInterval) {
            vector<int> in;
            vector <int> out;
            int len=lInterval.size();
            for(unsigned i=0; i<len; i++){
    	        in.push_back(lInterval[i].start);
    	        out.push_back(lInterval[i].end);
	        }
	        sort(in.begin(), in.begin()+len);
	        sort(out.begin(), out.begin()+len);
            int guests_in = 1, max_guests = 1, time = in[0];
            int i = 1, j = 0;
            while (i < len && j < len)
            {
                if (in[i] <= out[j])
                {
                    guests_in++;
                    if (guests_in > max_guests)
                    {
                        max_guests = guests_in;
                        time = in[i];
                    }
                    i++; 
                }
                else {
                    guests_in--;
                    j++;
                }
            }
            return max_guests;
        }
    }    
    class Node {
    public:
        Interval* entry;
        Interval* maxOvl;
        int maxOvlTime;
        Node* left;
        Node* right;
        int balance;
        int height;

        Node(Interval* entry = NULL, Node* left = NULL, Node* right = NULL) {
            this->entry = entry;
            this->left = left;
            this->right = right;
            this->Interval maxOvl = entry;
            this->maxOvlTime = 0;
            this->height = 0;
            this->balance = 0;
        }
        ~Node() {
            delete entry;
            entry = NULL;
            left = NULL;
            right = NULL;
        }
    };

    public:
    Node* root;
    AVLTree() : root(NULL) {};
    ~AVLTree() { this->clear(); };
        
    
    void add(int s, int t) {
        Interval* entry = new Interval(s, t);
        Node* added = new Node(entry, NULL, NULL);
        root = add(root, added);
    }
    int Height(node){
        int L = 0, R = 0;
        if(node->left != NULL)
            L = node->left->height;
        if(node->right != NULL)
            R = node->right->height;
        return 1 + max(L, R);
    }
    Node* add(Node* node, Node* added) {
        if (node == NULL) {
            return added;
        }
        if (node->start > added->start) {
            int oldheight = node->left->height;
            node->left = add(node->left, added);
            node->left->height = Height(node->left);
            if (node->left->height > oldheight) {
                node->left = Balance(node->left);
                node = Balance(node);
            }
            return node;
        }
        int oldheight = node->right->height;
        node->right = add(node->right, added);
        node->right->height = Height(node->right);
        if (node->right->height > oldheight) {
            node->right = Balance(node->right);
            node = Balance(node);
        }
        return node;
    }
    void remove(int s, int t) {
        root = deleteItem(root, s, t);
    }
    Node* deleteItem(Node* node, int item, int t) {
        if (node == NULL)
            throw ("Not found");
        int Item = node->start;
        if (item < Item) {
            node->left = deleteItem(node->left, item);
            node->left = Balance(node->left);
        }
        else if (item > Item) {
            node->right = deleteItem(node->right, item);
            node->right = Balance(node->right);
        }
        else {
            if(node->entry->finish == t)
                node = deleteRoot(node);
            else {
                node->left = deleteItem(node->left, item);
                node->left = Balance(node->left);
            }
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
        if (node->balance == 0)
            return node;
        else if (node->balance == 1) {
            if (node->right->balance == -1)
                return LeftRight(node);
            return rotateLeft(node);//
        }
        else {
            if (node->left->balance == 1) 
                return LeftRight(node);
            return rotateRight(node);
        }
    }
    int updateBalance(Node* node) {
        int hL = height(node->left);
        int hR = height(node->right);
        node->height = 1 + max(node);
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
    Interval updateInterval(Interval* left, Interval* right, int& time){
        Interval* result;
        result->start = min(left->start, right->start);
        result->finish = min(left->finish, right->finish);
        if (result->start > result->finish)
            time = 0;
        else time = 1;
    }
