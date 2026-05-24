#include <iostream>
#include <string>
#include <limits>
using namespace std;

#define CLEAR_CIN() do { cin.clear(); \
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); } while(0)

bool readInt(int& val) {
    if (!(cin >> val)) { CLEAR_CIN(); return false; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

struct WindowNode {
    string title, color;
    int width, height;
    WindowNode* prev;
    WindowNode* next;
    WindowNode(string t, string c, int w, int h)
        : title(t), color(c), width(w), height(h),
          prev(nullptr), next(nullptr) {}
};

class WindowList {
private:
    WindowNode* head;
    int size;

    WindowNode* findNode(const string& title) const {
        WindowNode* cur = head;
        while (cur) {
            if (cur->title == title) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

public:
    WindowList() : head(nullptr), size(0) {}

    ~WindowList() {
        WindowNode* cur = head;
        while (cur) {
            WindowNode* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }

    void createWindow(const string& title,
                      const string& color = "Blue",
                      int width = 300, int height = 200) {
        if (findNode(title)) {
            cout << "Window \"" << title << "\" already exists.\n";
            return;
        }
        if (width <= 0 || height <= 0) {
            cout << "Invalid size. Width and height must be positive.\n";
            return;
        }
        string col = color.empty() ? "Blue" : color;
        WindowNode* node = new WindowNode(title, col, width, height);
        if (!head) {
            head = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        size++;
        cout << "Created: \"" << title << "\" ("
             << width << "x" << height << ") [" << col << "]\n";
    }

    void activate(const string& title) {
        if (!head) { cout << "No windows open.\n"; return; }
        WindowNode* cur = findNode(title);
        if (!cur) { cout << "Window \"" << title << "\" not found.\n"; return; }
        if (cur == head) { cout << "\"" << title << "\" is already active.\n"; return; }
        if (cur->prev) cur->prev->next = cur->next;
        if (cur->next) cur->next->prev = cur->prev;
        cur->prev  = nullptr;
        cur->next  = head;
        head->prev = cur;
        head       = cur;
        cout << "Activated: \"" << title << "\"\n";
    }

    void resizeWindow(const string& title, int w, int h) {
        if (w <= 0 || h <= 0) { cout << "Invalid size.\n"; return; }
        WindowNode* cur = findNode(title);
        if (!cur) { cout << "Window \"" << title << "\" not found.\n"; return; }
        cur->width  = w;
        cur->height = h;
        cout << "Resized \"" << title << "\" to " << w << "x" << h << "\n";
    }

    void deleteWindow(const string& title) {
        WindowNode* cur = findNode(title);
        if (!cur) { cout << "Window \"" << title << "\" not found.\n"; return; }
        if (cur->prev) cur->prev->next = cur->next;
        if (cur->next) cur->next->prev = cur->prev;
        if (cur == head) head = cur->next;
        if (head) head->prev = nullptr;
        delete cur;
        size--;
        cout << "Deleted: \"" << title << "\"\n";
    }

    void display() const {
        cout << "\n======================================\n";
        cout << "   WINDOW STACK    Total: " << size << "\n";
        cout << "======================================\n";
        if (!head) {
            cout << "  (no windows open)\n";
        } else {
            WindowNode* cur = head;
            int i = 1;
            while (cur) {
                cout << (i == 1 ? "  [ACTIVE] " : "  [      ] ");
                cout << "\"" << cur->title << "\""
                     << "  " << cur->width << "x" << cur->height
                     << "  Color: " << cur->color << "\n";
                cur = cur->next;
                i++;
            }
        }
        cout << "======================================\n";
    }
};

void windowManagerMenu() {
    WindowList wm;
    wm.createWindow("Browser",      "Blue",  800, 600);
    wm.createWindow("File Manager", "Green", 600, 400);
    wm.createWindow("Terminal",     "Black", 500, 300);

    int choice;
    string title, color;
    int w, h;

    do {
        cout << "\n====== Window Manager ======\n"
             << "1. Create Window\n"
             << "2. Activate Window\n"
             << "3. Resize Window\n"
             << "4. Delete Window\n"
             << "5. Display All Windows\n"
             << "0. Back to Main Menu\n"
             << "Choice: ";

        if (!readInt(choice)) { choice = -1; continue; }

        switch (choice) {
            case 1:
                cout << "Title:  "; getline(cin, title);
                if (title.empty()) { cout << "Title cannot be empty.\n"; break; }
                cout << "Color (leave blank for Blue): ";
                getline(cin, color);
                if (color.empty()) color = "Blue";
                cout << "Width:  ";  if (!readInt(w)) break;
                cout << "Height: "; if (!readInt(h)) break;
                wm.createWindow(title, color, w, h);
                break;
            case 2:
                cout << "Title to activate: "; getline(cin, title);
                if (title.empty()) { cout << "Title cannot be empty.\n"; break; }
                wm.activate(title);
                break;
            case 3:
                cout << "Title to resize:  "; getline(cin, title);
                if (title.empty()) { cout << "Title cannot be empty.\n"; break; }
                cout << "New Width:  "; if (!readInt(w)) break;
                cout << "New Height: "; if (!readInt(h)) break;
                wm.resizeWindow(title, w, h);
                break;
            case 4:
                cout << "Title to delete: "; getline(cin, title);
                if (title.empty()) { cout << "Title cannot be empty.\n"; break; }
                wm.deleteWindow(title);
                break;
            case 5:
                wm.display();
                break;
            case 0:
                cout << "Exiting Window Manager.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    windowManagerMenu();
    return 0;
}