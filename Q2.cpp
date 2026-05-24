#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

#define CLEAR_CIN() do { cin.clear(); \
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); } while(0)

bool readInt(int& val) {
    if (!(cin >> val)) {
        CLEAR_CIN();
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}
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
int main() {
        TextEditor te;
        te.run();
        return 0;
}    