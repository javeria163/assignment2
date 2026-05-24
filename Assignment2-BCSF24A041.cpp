// ============================================================
// Q1: Window Manager ADT (Doubly Linked List)
// Q2: Text Editor
// Q3: White Areas in N×N Grid (Recursive DFS)
// Q4: Binary Search Tree + Timing Driver
// ============================================================
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <limits>
using namespace std;

#define CLEAR_CIN() do { cin.clear(); \
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); } while(0)

bool readInt(int& val) {
    if (!(cin >> val)) { CLEAR_CIN(); return false; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// ============================================================
// QUESTION 1: Window Manager ADT (Doubly Linked List)
// ============================================================

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
        if (!cur) {
            cout << "Window \"" << title << "\" not found.\n"; return;
        }
        if (cur == head) {
            cout << "\"" << title << "\" is already active.\n"; return;
        }
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
        if (!cur) {
            cout << "Window \"" << title << "\" not found.\n"; return;
        }
        cur->width  = w;
        cur->height = h;
        cout << "Resized \"" << title << "\" to " << w << "x" << h << "\n";
    }

    void deleteWindow(const string& title) {
        WindowNode* cur = findNode(title);
        if (!cur) {
            cout << "Window \"" << title << "\" not found.\n"; return;
        }
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

// ============================================================
// QUESTION 2: Text Editor
// ============================================================

class TextEditor {
private:
    vector<string> lines;
    string currentFile;
    bool modified;
    vector<string> recentFiles;

    string fontName;
    string textColor;
    int    fontSize;

    int cursorLine;
    int cursorPos;

    const vector<string> fontList  = {"Arial", "Times New Roman",
                                      "Courier New", "Verdana", "Calibri"};
    const vector<string> colorList = {"Black", "White", "Red",
                                      "Blue",  "Green", "Yellow"};
    const vector<int>    sizeList  = {8, 10, 12, 14, 16, 18, 24, 36, 48};

    void clampCursor() {
        if (lines.empty()) { cursorLine = 1; cursorPos = 0; return; }
        if (cursorLine < 1) cursorLine = 1;
        if (cursorLine > (int)lines.size())
            cursorLine = (int)lines.size();
        int len = (int)lines[cursorLine - 1].size();
        if (cursorPos < 0)   cursorPos = 0;
        if (cursorPos > len) cursorPos = len;
    }

    void showStatusBar() const {
        cout << "------------------------------------------------------------\n";
        cout << "File : "
             << (currentFile.empty() ? "[New File]" : currentFile);
        if (modified) cout << " *";
        cout << "   Lines: " << lines.size()
             << "   Cursor: L" << cursorLine
             << ":C" << cursorPos + 1 << "\n";
        cout << "Font : " << fontName
             << "  |  Color: " << textColor
             << "  |  Size: "  << fontSize << "pt\n";
        cout << "------------------------------------------------------------\n";
    }

    void writeToFile(const string& fname) {
        ofstream fout(fname);
        if (!fout) {
            cout << "Error: Cannot write to \"" << fname << "\"\n"; return;
        }
        for (int i = 0; i < (int)lines.size(); i++) fout << lines[i] << "\n";
        fout.close();
        modified = false;
        cout << "Saved to \"" << fname << "\"\n";
    }

    void addRecent(const string& fname) {
        recentFiles.erase(
            remove(recentFiles.begin(), recentFiles.end(), fname),
            recentFiles.end());
        recentFiles.insert(recentFiles.begin(), fname);
        if ((int)recentFiles.size() > 8) recentFiles.resize(8);
    }

    bool confirmSave() {
        if (!modified) return true;
        cout << "Unsaved changes! Save first? (y/n): ";
        char c;
        if (!(cin >> c)) { CLEAR_CIN(); return false; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c == 'y' || c == 'Y') { saveFile(); return true; }
        return false;
    }

    bool guardUnsaved(const string& action) {
        if (!modified) return true;
        bool saved = confirmSave();
        if (saved) return true;
        cout << "Discard unsaved changes and " << action << "? (y/n): ";
        char c;
        if (!(cin >> c)) { CLEAR_CIN(); return false; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c == 'y' || c == 'Y') return true;
        cout << "Cancelled.\n";
        return false;
    }

public:
    TextEditor()
        : modified(false), fontName("Arial"), textColor("Black"),
          fontSize(12), cursorLine(1), cursorPos(0) {}

    void newFile() {
        if (!guardUnsaved("create new file")) return;
        lines.clear();
        currentFile = "";
        modified    = false;
        cursorLine  = 1; cursorPos = 0;
        cout << "New file created.\n";
    }

    void openFile() {
        if (!guardUnsaved("open file")) return;
        cout << "Enter filename to open: ";
        string fname; getline(cin, fname);
        if (fname.empty()) { cout << "Cancelled.\n"; return; }
        ifstream fin(fname);
        if (!fin) {
            cout << "Error: Cannot open \"" << fname << "\"\n"; return;
        }
        lines.clear();
        string line;
        while (getline(fin, line)) lines.push_back(line);
        fin.close();
        currentFile = fname;
        modified    = false;
        cursorLine  = 1; cursorPos = 0;
        addRecent(fname);
        cout << "Opened \"" << fname
             << "\" (" << lines.size() << " lines)\n";
    }

    void saveFile() {
        if (currentFile.empty()) { saveAs(); return; }
        writeToFile(currentFile);
    }

    void saveAs() {
        cout << "Save as (filename): ";
        string fname; getline(cin, fname);
        if (fname.empty()) { cout << "Cancelled.\n"; return; }
        writeToFile(fname);
        currentFile = fname;
        addRecent(fname);
    }

    void closeFile() {
        if (!guardUnsaved("close file")) return;
        lines.clear();
        currentFile = "";
        modified    = false;
        cursorLine  = 1; cursorPos = 0;
        cout << "File closed.\n";
    }

    void viewRecentFiles() {
        if (recentFiles.empty()) { cout << "No recent files.\n"; return; }
        cout << "\nRecent Files:\n";
        for (int i = 0; i < (int)recentFiles.size(); i++)
            cout << "  " << i + 1 << ". " << recentFiles[i] << "\n";
        cout << "Open which number (0 to cancel)? ";
        int n;
        if (!readInt(n) || n < 1 || n > (int)recentFiles.size()) {
            cout << "Cancelled.\n"; return;
        }
        string fname = recentFiles[n - 1];
        ifstream fin(fname);
        if (!fin) { cout << "File not found.\n"; return; }
        if (!guardUnsaved("open recent file")) return;
        lines.clear();
        string line;
        while (getline(fin, line)) lines.push_back(line);
        fin.close();
        currentFile = fname;
        modified    = false;
        cursorLine  = 1; cursorPos = 0;
        cout << "Opened \"" << fname << "\"\n";
    }

    void typeText() {
        cout << "Type text (Enter = new paragraph, "
                "type END alone to stop):\n";
        string line;
        while (true) {
            getline(cin, line);
            if (line == "END") break;
            lines.push_back(line);
            modified = true;
        }
        cursorLine = (int)lines.size();
        if (cursorLine < 1) cursorLine = 1;
        cursorPos  = lines.empty() ? 0 : (int)lines.back().size();
        cout << "Text added. Total lines: " << lines.size() << "\n";
    }

    void editLine() {
        displayBuffer();
        if (lines.empty()) { cout << "Nothing to edit.\n"; return; }
        cout << "Line number to edit (1-" << lines.size() << "): ";
        int n;
        if (!readInt(n) || n < 1 || n > (int)lines.size()) {
            cout << "Invalid line number.\n"; return;
        }
        cursorLine = n;
        cout << "Current [" << n << "]: " << lines[n - 1] << "\n"
             << "  1. Replace entire line\n"
             << "  2. Delete this line\n"
             << "  3. Insert new line before this\n"
             << "Choice: ";
        int c;
        if (!readInt(c)) { cout << "Cancelled.\n"; return; }

        if (c == 1) {
            cout << "New text: ";
            getline(cin, lines[n - 1]);
            cursorPos = (int)lines[n - 1].size();
            modified  = true;
            cout << "Line updated.\n";
        } else if (c == 2) {
            lines.erase(lines.begin() + n - 1);
            modified = true;
            clampCursor();
            cout << "Line deleted.\n";
        } else if (c == 3) {
            cout << "New line text: ";
            string nl; getline(cin, nl);
            lines.insert(lines.begin() + n - 1, nl);
            cursorPos = 0;
            modified  = true;
            cout << "Line inserted.\n";
        } else {
            cout << "Cancelled.\n";
        }
    }

    void backspace() {
        if (lines.empty()) { cout << "Nothing to delete.\n"; return; }
        clampCursor();
        string& line = lines[cursorLine - 1];
        if (cursorPos > 0) {
            line.erase(cursorPos - 1, 1);
            cursorPos--;
            modified = true;
            cout << "Backspace. Line " << cursorLine
                 << ": \"" << line << "\"\n";
        } else if (cursorLine > 1) {
            string& prev = lines[cursorLine - 2];
            cursorPos = (int)prev.size();
            prev += line;
            lines.erase(lines.begin() + cursorLine - 1);
            cursorLine--;
            modified = true;
            cout << "Lines merged. Line " << cursorLine
                 << ": \"" << lines[cursorLine - 1] << "\"\n";
        } else {
            cout << "Already at document start.\n";
        }
    }

    void deleteChar() {
        if (lines.empty()) { cout << "Nothing to delete.\n"; return; }
        clampCursor();
        string& line = lines[cursorLine - 1];
        if (cursorPos < (int)line.size()) {
            line.erase(cursorPos, 1);
            modified = true;
            cout << "Delete. Line " << cursorLine
                 << ": \"" << line << "\"\n";
        } else if (cursorLine < (int)lines.size()) {
            line += lines[cursorLine];
            lines.erase(lines.begin() + cursorLine);
            modified = true;
            cout << "Lines merged. Line " << cursorLine
                 << ": \"" << line << "\"\n";
        } else {
            cout << "Already at document end.\n";
        }
    }

    void insertAtCursor() {
        if (lines.empty()) {
            lines.push_back("");
            cursorLine = 1; cursorPos = 0;
        }
        clampCursor();
        cout << "Insert at L" << cursorLine
             << ":C" << cursorPos + 1 << ": ";
        string text; getline(cin, text);
        if (text.empty()) { cout << "Nothing inserted.\n"; return; }
        lines[cursorLine - 1].insert(cursorPos, text);
        cursorPos += (int)text.size();
        modified = true;
        cout << "Inserted. Line: \""
             << lines[cursorLine - 1] << "\"\n";
    }

    void moveCursor() {
        cout << "Move cursor:\n"
             << "  1. Up       2. Down\n"
             << "  3. Home     4. End\n"
             << "  5. PgUp     6. PgDn\n"
             << "  7. Left     8. Right\n"
             << "Choice: ";
        int c;
        if (!readInt(c)) return;

        switch (c) {
            case 1:
                if (cursorLine > 1) cursorLine--;
                clampCursor();
                cout << "Up -> L" << cursorLine << ":C" << cursorPos+1 << "\n";
                break;
            case 2:
                if (!lines.empty() && cursorLine < (int)lines.size())
                    cursorLine++;
                clampCursor();
                cout << "Down -> L" << cursorLine << ":C" << cursorPos+1 << "\n";
                break;
            case 3:
                cursorPos = 0;
                cout << "Home -> start of line " << cursorLine << "\n";
                break;
            case 4:
                if (!lines.empty()) {
                    clampCursor();
                    cursorPos = (int)lines[cursorLine - 1].size();
                }
                cout << "End -> end of line " << cursorLine << "\n";
                break;
            case 5:
                cursorLine = max(1, cursorLine - 10);
                clampCursor();
                cout << "PgUp -> L" << cursorLine << "\n";
                break;
            case 6:
                if (!lines.empty())
                    cursorLine = min((int)lines.size(), cursorLine + 10);
                clampCursor();
                cout << "PgDn -> L" << cursorLine << "\n";
                break;
            case 7:
                if (cursorPos > 0) {
                    cursorPos--;
                } else if (cursorLine > 1) {
                    cursorLine--;
                    clampCursor();
                    cursorPos = (int)lines[cursorLine - 1].size();
                }
                cout << "Left -> L" << cursorLine << ":C" << cursorPos+1 << "\n";
                break;
            case 8:
                if (!lines.empty()) {
                    clampCursor();
                    if (cursorPos < (int)lines[cursorLine-1].size()) {
                        cursorPos++;
                    } else if (cursorLine < (int)lines.size()) {
                        cursorLine++; cursorPos = 0;
                    }
                }
                cout << "Right -> L" << cursorLine << ":C" << cursorPos+1 << "\n";
                break;
            default:
                cout << "Invalid.\n";
        }
    }

    void findReplace() {
        cout << "Find:    "; string find; getline(cin, find);
        cout << "Replace: "; string rep;  getline(cin, rep);
        if (find.empty()) { cout << "Nothing to find.\n"; return; }
        int count = 0;
        for (int i = 0; i < (int)lines.size(); i++) {
            string& line = lines[i];
            size_t pos = 0;
            while ((pos = line.find(find, pos)) != string::npos) {
                line.replace(pos, find.size(), rep);
                pos += rep.size();
                count++;
            }
        }
        if (count) {
            modified = true;
            cout << "Replaced " << count << " occurrence(s).\n";
        } else {
            cout << "\"" << find << "\" not found.\n";
        }
    }

    void selectAll() {
        cout << "\n--- SELECT ALL ---\n";
        if (lines.empty()) { cout << "(empty)\n"; }
        else { for (int i = 0; i < (int)lines.size(); i++) cout << lines[i] << "\n"; }
        cout << "------------------\n"
             << "Total lines: " << lines.size() << "\n";
    }

    void changeFont() {
        cout << "Available fonts:\n";
        for (int i = 0; i < (int)fontList.size(); i++)
            cout << "  " << i+1 << ". " << fontList[i] << "\n";
        cout << "Choice: ";
        int n;
        if (!readInt(n) || n < 1 || n > (int)fontList.size()) {
            cout << "Invalid choice.\n"; return;
        }
        fontName = fontList[n - 1];
        cout << "Font set to: " << fontName << "\n";
    }

    void changeColor() {
        cout << "Available colors:\n";
        for (int i = 0; i < (int)colorList.size(); i++)
            cout << "  " << i+1 << ". " << colorList[i] << "\n";
        cout << "Choice: ";
        int n;
        if (!readInt(n) || n < 1 || n > (int)colorList.size()) {
            cout << "Invalid choice.\n"; return;
        }
        textColor = colorList[n - 1];
        cout << "Color set to: " << textColor << "\n";
    }

    void changeSize() {
        cout << "Available sizes:\n";
        for (int i = 0; i < (int)sizeList.size(); i++)
            cout << "  " << i+1 << ". " << sizeList[i] << "pt\n";
        cout << "Choice: ";
        int n;
        if (!readInt(n) || n < 1 || n > (int)sizeList.size()) {
            cout << "Invalid choice.\n"; return;
        }
        fontSize = sizeList[n - 1];
        cout << "Size set to: " << fontSize << "pt\n";
    }

    void wordWrap() {
        cout << "Wrap width (chars per line, min 10): ";
        int width;
        if (!readInt(width) || width < 10) {
            cout << "Invalid width (min 10).\n"; return;
        }
        vector<string> wrapped;
        for (int i = 0; i < (int)lines.size(); i++) {
            const string& line = lines[i];
            if (line.empty()) { wrapped.push_back(""); continue; }
            if ((int)line.size() <= width) { wrapped.push_back(line); continue; }
            string rem = line;
            while ((int)rem.size() > width) {
                int breakAt = width;
                for (int j = width - 1; j >= 0; j--) {
                    if (rem[j] == ' ') { breakAt = j; break; }
                }
                wrapped.push_back(rem.substr(0, breakAt));
                int skip = (breakAt < (int)rem.size() &&
                            rem[breakAt] == ' ') ? 1 : 0;
                rem = rem.substr(breakAt + skip);
            }
            if (!rem.empty()) wrapped.push_back(rem);
        }
        lines    = wrapped;
        modified = true;
        clampCursor();
        cout << "Word wrap applied (" << width << " chars). "
             << "Lines: " << lines.size() << "\n";
    }

    void newLine() {
        // If document is empty, seed it first
        if (lines.empty()) {
            lines.push_back("");
            cursorLine = 1;
            cursorPos  = 0;
        }
        // Insert blank line AFTER current cursor line
        lines.insert(lines.begin() + cursorLine, "");
        cursorLine++;
        cursorPos = 0;
        modified  = true;
        cout << "New line inserted. Now at L" << cursorLine << ":C1\n";
    }

    void displayBuffer() {
        cout << "\n============================================================\n";
        cout << "                     TEXT EDITOR\n";
        showStatusBar();
        if (lines.empty()) {
            cout << "  (empty document)\n";
        } else {
            for (int i = 0; i < (int)lines.size(); i++) {
                cout << (i + 1 == cursorLine ? " >> " : "    ");
                cout << setw(3) << i + 1 << ": " << lines[i] << "\n";
            }
        }
        cout << "============================================================\n";
    }

    void run() {
        int choice;
        do {
            cout << "\n============================================================\n";
            cout << "                     TEXT EDITOR\n";
            showStatusBar();
            cout << "  --- File ---\n"
                 << "   1. New File       2. Open File      3. Save\n"
                 << "   4. Save As        5. Close File     6. Recent Files\n"
                 << "  --- Edit ---\n"
                 << "   7. Type Text      8. Edit/Del Line  9. Find & Replace\n"
                 << "  10. Select All    11. Backspace     12. Delete Char\n"
                 << "  13. Insert Text   14. Move Cursor\n"
                 << "  --- Format ---\n"
                 << "  15. Change Font   16. Change Color  17. Change Size\n"
                 << "  18. Word Wrap     20. New Line\n"
                 << "  --- View ---\n"
                 << "  19. Display Buffer\n"
                 << "   0. Exit Editor\n"
                 << "============================================================\n"
                 << "Choice: ";

            if (!readInt(choice)) { choice = -1; continue; }

            switch (choice) {
                case  1: newFile();          break;
                case  2: openFile();         break;
                case  3: saveFile();         break;
                case  4: saveAs();           break;
                case  5: closeFile();        break;
                case  6: viewRecentFiles();  break;
                case  7: typeText();         break;
                case  8: editLine();         break;
                case  9: findReplace();      break;
                case 10: selectAll();        break;
                case 11: backspace();        break;
                case 12: deleteChar();       break;
                case 13: insertAtCursor();   break;
                case 14: moveCursor();       break;
                case 15: changeFont();       break;
                case 16: changeColor();      break;
                case 17: changeSize();       break;
                case 18: wordWrap();         break;
                case 19: displayBuffer();    break;
                case 20: newLine();          break;
                case 0:
                    if (modified) {
                        cout << "Unsaved changes! Save before exit? (y/n): ";
                        char c;
                        if (cin >> c) {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            if (c == 'y' || c == 'Y') saveFile();
                        } else { CLEAR_CIN(); }
                    }
                    cout << "Goodbye.\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

// ============================================================
// QUESTION 3: White Areas in N×N Grid (Recursive DFS)
// ============================================================

#define Q3_WHITE   0
#define Q3_BLACK   1
#define Q3_VISITED 2

static int recursiveDFS(vector<vector<int> >& grid,
                        int r, int c, int N) {
    if (r < 1 || r > N || c < 1 || c > N) return 0;
    if (grid[r][c] != Q3_WHITE) return 0;

    grid[r][c] = Q3_VISITED;
    int count = 1;

    count += recursiveDFS(grid, r - 1, c,     N);
    count += recursiveDFS(grid, r + 1, c,     N);
    count += recursiveDFS(grid, r,     c - 1, N);
    count += recursiveDFS(grid, r,     c + 1, N);

    return count;
}

void solveGrid() {
    int n;
    cout << "Enter grid size n (1-20): ";
    if (!readInt(n) || n < 1 || n > 20) {
        cout << "Invalid size (must be 1-20).\n"; return;
    }

    int padN = n + 2;
    vector<vector<int> > grid(padN, vector<int>(padN, Q3_BLACK));

    cout << "Enter " << n << "x" << n
         << " grid row by row (0 = white, 1 = black):\n"
         << "(Space-separated per row)\n";
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++) {
            int val;
            if (!(cin >> val)) { cin.clear(); val = Q3_BLACK; }
            grid[r][c] = (val == 0) ? Q3_WHITE : Q3_BLACK;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nOriginal Grid (. = white, # = black):\n";
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++)
            cout << (grid[r][c] == Q3_WHITE ? ". " : "# ");
        cout << "\n";
    }

    vector<int> areas;
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            if (grid[r][c] == Q3_WHITE) {
                int sz = recursiveDFS(grid, r, c, n);
                if (sz > 0) areas.push_back(sz);
            }

    cout << "\nMarked Grid (* = visited white, # = black):\n";
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++) {
            if      (grid[r][c] == Q3_BLACK)   cout << "# ";
            else if (grid[r][c] == Q3_VISITED) cout << "* ";
            else                                cout << ". ";
        }
        cout << "\n";
    }

    cout << "\n===== Results =====\n";
    cout << "Number of white areas: " << areas.size() << "\n";
    int total = 0;
    for (int i = 0; i < (int)areas.size(); i++) {
        cout << "  Area " << i+1 << ": " << areas[i] << " cell(s)\n";
        total += areas[i];
    }
    cout << "Total white cells: " << total << "\n";
    cout << "===================\n";
}

void runChessboardTest() {
    cout << "\n--- Test: 4x4 Chessboard ---\n";
    int n = 4, padN = n + 2;
    vector<vector<int> > grid(padN, vector<int>(padN, Q3_BLACK));
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            grid[r][c] = ((r+c) % 2 == 0) ? Q3_WHITE : Q3_BLACK;
    cout << "Grid:\n";
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++)
            cout << (grid[r][c] == Q3_WHITE ? ". " : "# ");
        cout << "\n";
    }
    vector<int> areas;
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            if (grid[r][c] == Q3_WHITE) {
                int sz = recursiveDFS(grid, r, c, n);
                if (sz > 0) areas.push_back(sz);
            }
    int total = 0;
    cout << "White areas: " << areas.size() << "  (expected 8)\n";
    for (int i = 0; i < (int)areas.size(); i++) {
        cout << "  Area " << i+1 << ": " << areas[i] << " cell(s)\n";
        total += areas[i];
    }
    cout << "Total white cells: " << total << "  (expected 8)\n";
}

void runAllBlackTest() {
    cout << "\n--- Test: 3x3 All-Black ---\n";
    int n = 3, padN = n + 2;
    vector<vector<int> > grid(padN, vector<int>(padN, Q3_BLACK));
    cout << "Grid (all black):\n";
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++) cout << "# ";
        cout << "\n";
    }
    vector<int> areas;
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            if (grid[r][c] == Q3_WHITE) {
                int sz = recursiveDFS(grid, r, c, n);
                if (sz > 0) areas.push_back(sz);
            }
    cout << "White areas: " << areas.size() << "  (expected 0)\n";
}

void runAllWhiteTest() {
    cout << "\n--- Test: 3x3 All-White ---\n";
    int n = 3, padN = n + 2;
    vector<vector<int> > grid(padN, vector<int>(padN, Q3_BLACK));
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            grid[r][c] = Q3_WHITE;
    cout << "Grid (all white):\n";
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++) cout << ". ";
        cout << "\n";
    }
    vector<int> areas;
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            if (grid[r][c] == Q3_WHITE) {
                int sz = recursiveDFS(grid, r, c, n);
                if (sz > 0) areas.push_back(sz);
            }
    int total = 0;
    cout << "White areas: " << areas.size() << "  (expected 1)\n";
    for (int i = 0; i < (int)areas.size(); i++) total += areas[i];
    cout << "Total white cells: " << total << "  (expected " << n*n << ")\n";
}

void runSingleCellTest() {
    cout << "\n--- Test: 1x1 White ---\n";
    int n = 1, padN = n + 2;
    vector<vector<int> > grid(padN, vector<int>(padN, Q3_BLACK));
    grid[1][1] = Q3_WHITE;
    vector<int> areas;
    for (int r = 1; r <= n; r++)
        for (int c = 1; c <= n; c++)
            if (grid[r][c] == Q3_WHITE) {
                int sz = recursiveDFS(grid, r, c, n);
                if (sz > 0) areas.push_back(sz);
            }
    cout << "White areas: " << areas.size()
         << "  (expected 1), size=" << (areas.empty() ? 0 : areas[0])
         << "  (expected 1)\n";
}

void whiteAreaMenu() {
    int choice;
    do {
        cout << "\n====== White Area Finder ======\n"
             << "1. Enter custom grid\n"
             << "2. Run chessboard test (4x4)\n"
             << "3. Run all-black test  (3x3)\n"
             << "4. Run all-white test  (3x3)\n"
             << "5. Run 1x1 white test\n"
             << "0. Back\n"
             << "Choice: ";
        if (!readInt(choice)) { choice = -1; continue; }
        switch (choice) {
            case 1: solveGrid();         break;
            case 2: runChessboardTest(); break;
            case 3: runAllBlackTest();   break;
            case 4: runAllWhiteTest();   break;
            case 5: runSingleCellTest(); break;
            case 0: break;
            default: cout << "Invalid.\n";
        }
    } while (choice != 0);
}

// ============================================================
// QUESTION 4: Binary Search Tree
// ============================================================

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

// ============================================================
// MAIN ENTRY POINT
// ============================================================

int main() {
    int choice;
    do {
        cout << "\n+==============================+\n"
             << "|        MAIN MENU             |\n"
             << "+==============================+\n"
             << "|  1. Window Manager  (Q1)     |\n"
             << "|  2. Text Editor     (Q2)     |\n"
             << "|  3. White Areas     (Q3)     |\n"
             << "|  4. BST             (Q4)     |\n"
             << "|  0. Exit                     |\n"
             << "+==============================+\n"
             << "Choice: ";

        if (!readInt(choice)) { choice = -1; continue; }

        switch (choice) {
            case 1: windowManagerMenu();           break;
            case 2: { TextEditor te; te.run(); }   break;
            case 3: whiteAreaMenu();                break;
            case 4: bstMenu();                      break;
            case 0: cout << "Bye!\n";               break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}