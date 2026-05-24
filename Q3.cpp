#include <iostream>
#include <vector>
#include <limits>

using namespace std;

bool readInt(int& val) {
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}
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
int main() {
    whiteAreaMenu();
    return 0;
}