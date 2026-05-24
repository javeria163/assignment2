#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <random>

using namespace std;
bool readInt(int& x) {
    cin >> x;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }

    cin.ignore(10000, '\n');
    return true;
}
struct BSTNode {
    int val;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    BSTNode* insertRec(BSTNode* node, int val, bool& inserted) {
        if (!node) { inserted = true; return new BSTNode(val); }
        if (val < node->val)
            node->left  = insertRec(node->left,  val, inserted);
        else if (val > node->val)
            node->right = insertRec(node->right, val, inserted);
        return node;
    }

    BSTNode* deleteNode(BSTNode* node, int val) {
        if (!node) return nullptr;
        if      (val < node->val) node->left  = deleteNode(node->left,  val);
        else if (val > node->val) node->right = deleteNode(node->right, val);
        else {
            if (!node->left)  { BSTNode* t = node->right; delete node; return t; }
            if (!node->right) { BSTNode* t = node->left;  delete node; return t; }
            BSTNode* succ = node->right;
            while (succ->left) succ = succ->left;
            node->val   = succ->val;
            node->right = deleteNode(node->right, succ->val);
        }
        return node;
    }

    void inorderRec(BSTNode* node, vector<int>& res) const {
        if (!node) return;
        inorderRec(node->left, res);
        res.push_back(node->val);
        inorderRec(node->right, res);
    }

    void preorderRec(BSTNode* node, vector<int>& res) const {
        if (!node) return;
        res.push_back(node->val);
        preorderRec(node->left,  res);
        preorderRec(node->right, res);
    }

    void postorderRec(BSTNode* node, vector<int>& res) const {
        if (!node) return;
        postorderRec(node->left,  res);
        postorderRec(node->right, res);
        res.push_back(node->val);
    }

    int heightRec(BSTNode* node) const {
        if (!node) return 0;
        return 1 + max(heightRec(node->left), heightRec(node->right));
    }

    void destroyTree(BSTNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void printTree(BSTNode* root_) const {
        if (!root_) { cout << "(empty tree)\n"; return; }
        queue<pair<BSTNode*, int> > q;
        q.push(make_pair(root_, 0));
        int curLevel = 0;
        cout << "Level 0: ";
        while (!q.empty()) {
            BSTNode* node  = q.front().first;
            int      level = q.front().second;
            q.pop();
            if (level != curLevel) {
                cout << "\nLevel " << level << ": ";
                curLevel = level;
            }
            cout << node->val << " ";
            if (node->left)  q.push(make_pair(node->left,  level + 1));
            if (node->right) q.push(make_pair(node->right, level + 1));
        }
        cout << "\n";
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroyTree(root); }

    bool insert(int val) {
        bool inserted = false;
        root = insertRec(root, val, inserted);
        return inserted;
    }

    bool remove(int val) {
        if (!search(val)) return false;
        root = deleteNode(root, val);
        return true;
    }

    bool isEmpty()  const { return root == nullptr; }
    int  height()   const { return heightRec(root); }

    bool search(int val) const {
        BSTNode* cur = root;
        while (cur) {
            if      (val == cur->val) return true;
            else if (val  < cur->val) cur = cur->left;
            else                      cur = cur->right;
        }
        return false;
    }

    vector<int> inorder()   const { vector<int> r; inorderRec(root,   r); return r; }
    vector<int> preorder()  const { vector<int> r; preorderRec(root,  r); return r; }
    vector<int> postorder() const { vector<int> r; postorderRec(root, r); return r; }

    void printLevelOrder() const { printTree(root); }
};

void runTimingDriver() {
    srand((unsigned)time(nullptr));
 
    const int NUM_SETS = 5;
    const int SET_SIZE = 100;
    const int REPEAT   = 1000;   // repeat each op so timing is visible
 
    double totalIns = 0, totalTrav = 0;
 
    cout << "\n"
         << "=================================================================\n"
         << left
         << setw(8)  << "Set #"
         << setw(26) << "Insertion Time (us)"
         << setw(26) << "Inorder Traversal (us)"
         << setw(12) << "Tree Height"
         << "\n"
         << "=================================================================\n";
 
    for (int s = 1; s <= NUM_SETS; s++) {
 
        // 1. Generate 100 unique random numbers
        set<int> unique;
        while ((int)unique.size() < SET_SIZE)
            unique.insert(rand() % 10000 + 1);
 
        // 2. Copy into vector — still sorted at this point
        vector<int> data(unique.begin(), unique.end());
 
        // 3. SHUFFLE so BST doesn't degenerate into a linked list
        random_shuffle(data.begin(), data.end());
 
        BST tree;
 
        // 4. Time insertions (repeat 1000x so it's measurable)
        chrono::high_resolution_clock::time_point t0 =
            chrono::high_resolution_clock::now();
 
        for (int repeat = 0; repeat < REPEAT; repeat++)
            for (int i = 0; i < (int)data.size(); i++)
                tree.insert(data[i]);   // duplicates silently ignored
 
        chrono::high_resolution_clock::time_point t1 =
            chrono::high_resolution_clock::now();
 
        double insUs =
            chrono::duration<double, chrono::microseconds::period>(t1 - t0)
            .count() / REPEAT;          // average per single run
 
        // 5. Time inorder traversal (repeat 1000x, use result to block optimizer)
        chrono::high_resolution_clock::time_point t2 =
            chrono::high_resolution_clock::now();
 
        for (int repeat = 0; repeat < REPEAT; repeat++) {
            vector<int> result = tree.inorder();
            volatile int sink = 0;
            for (int i = 0; i < (int)result.size(); i++)
                sink += result[i];      // force compiler to keep the call
        }
 
        chrono::high_resolution_clock::time_point t3 =
            chrono::high_resolution_clock::now();
 
        double travUs =
            chrono::duration<double, chrono::microseconds::period>(t3 - t2)
            .count() / REPEAT;
 
        totalIns  += insUs;
        totalTrav += travUs;
 
        cout << left
             << setw(8)  << s
             << setw(26) << fixed << setprecision(4) << insUs
             << setw(26) << fixed << setprecision(4) << travUs
             << setw(12) << tree.height()
             << "\n";
    }
 
    cout << "-----------------------------------------------------------------\n"
         << left
         << setw(8)  << "Average"
         << setw(26) << fixed << setprecision(4) << totalIns  / NUM_SETS
         << setw(26) << fixed << setprecision(4) << totalTrav / NUM_SETS
         << "\n"
         << "=================================================================\n";
}
 

void bstDemo() {
    cout << "\n--- BST Demo ---\n";
    BST tree;
    int vals[] = {50, 30, 70, 20, 40, 60, 80, 10, 25};
    int valsSize = sizeof(vals) / sizeof(vals[0]);
    cout << "Inserting: ";
    for (int i = 0; i < valsSize; i++) { cout << vals[i] << " "; tree.insert(vals[i]); }
    cout << "\nTree height: " << tree.height() << "\n";

    vector<int> res;

    res = tree.inorder();
    cout << "Inorder    (must be sorted): ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    res = tree.preorder();
    cout << "Preorder                   : ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    res = tree.postorder();
    cout << "Postorder                  : ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    cout << "Level-order view:\n"; tree.printLevelOrder();
    cout << "Search 40  : " << (tree.search(40) ? "FOUND" : "NOT FOUND") << "\n";
    cout << "Search 99  : " << (tree.search(99) ? "FOUND" : "NOT FOUND") << "\n";

    bool dup = tree.insert(50);
    cout << "Insert(50) duplicate: " << (dup ? "inserted" : "DUPLICATE - ignored") << "\n";
    res = tree.inorder();
    cout << "After insert(50) duplicate : ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    tree.remove(10);
    res = tree.inorder();
    cout << "After delete(10)  leaf:         ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    tree.remove(20);
    res = tree.inorder();
    cout << "After delete(20)  one child:    ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    tree.remove(30);
    res = tree.inorder();
    cout << "After delete(30)  two children: ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    tree.remove(50);
    res = tree.inorder();
    cout << "After delete(50)  root:         ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";

    bool found999 = tree.remove(999);
    cout << "Delete(999): " << (found999 ? "removed" : "NOT FOUND - no change") << "\n";
    res = tree.inorder();
    cout << "After delete(999) not found: ";
    for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
    cout << "\n";
}

void bstMenu() {
    BST tree;
    int choice;
    do {
        cout << "\n====== BST Menu ======\n"
             << "1. Insert\n"
             << "2. Search\n"
             << "3. Inorder Traversal\n"
             << "4. Preorder Traversal\n"
             << "5. Postorder Traversal\n"
             << "6. Delete\n"
             << "7. Tree Height\n"
             << "8. Print Level-Order View\n"
             << "9. Run Timing Driver (5 sets x 100 numbers)\n"
             << "10. Run Demo\n"
             << "0. Back\n"
             << "Choice: ";

        if (!readInt(choice)) { choice = -1; continue; }

        int val;
        vector<int> res;

        switch (choice) {
            case 1:
                cout << "Value to insert: ";
                if (!readInt(val)) break;
                if (tree.insert(val))
                    cout << "Inserted " << val << "\n";
                else
                    cout << val << " already exists (duplicate ignored).\n";
                break;
            case 2:
                cout << "Value to search: ";
                if (!readInt(val)) break;
                cout << (tree.search(val) ? "FOUND\n" : "NOT FOUND\n");
                break;
            case 3:
                res = tree.inorder();
                cout << "Inorder:   ";
                for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
                cout << "\n";
                break;
            case 4:
                res = tree.preorder();
                cout << "Preorder:  ";
                for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
                cout << "\n";
                break;
            case 5:
                res = tree.postorder();
                cout << "Postorder: ";
                for (int i = 0; i < (int)res.size(); i++) cout << res[i] << " ";
                cout << "\n";
                break;
            case 6:
                cout << "Value to delete: ";
                if (!readInt(val)) break;
                if (tree.remove(val))
                    cout << val << " deleted.\n";
                else
                    cout << val << " not found.\n";
                break;
            case 7:
                cout << "Tree height: " << tree.height() << "\n";
                break;
            case 8:
                tree.printLevelOrder();
                break;
            case 9: runTimingDriver(); break;
            case 10: bstDemo();        break;
            case 0: break;
            default: cout << "Invalid.\n";
        }
    } while (choice != 0);
}
int main() {
    bstMenu();
    return 0;
}