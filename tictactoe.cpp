// ============================================================
//  Colorful Tic-Tac-Toe (Console, Windows)
//  Concepts used: 2D arrays, loops, if-else, functions
// ============================================================
#include <iostream>
#include <string>
#include <windows.h>   // for console colors and system("cls")
using namespace std;

// ---------- Color codes (Windows console attributes) ----------
const int RED    = 12;
const int GREEN  = 10;
const int CYAN   = 11;   // used for the grid borders
const int YELLOW = 14;
const int WHITE  = 7;    // default console color

// ---------- Global game data ----------
char board[3][3];                 // the 3x3 board
string name1, name2;              // player names
int gamesPlayed = 0, wins1 = 0, wins2 = 0, draws = 0;

// ------------------------------------------------------------
// setColor: changes the color of ALL text printed after it.
// Windows keeps a "handle" to the console window; we pass that
// handle and a number (the color) to SetConsoleTextAttribute.
// ------------------------------------------------------------
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// ------------------------------------------------------------
// resetBoard: puts '1'..'9' in the cells so the player can see
// which number to type. Cell number = row*3 + col + 1.
// ------------------------------------------------------------
void resetBoard() {
    char number = '1';
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            board[r][c] = number;
            number++;
        }
    }
}

// ------------------------------------------------------------
// printCell: prints ONE cell in its own color.
// X = red, O = green, an empty cell (a digit) = white.
// ------------------------------------------------------------
void printCell(char ch) {
    if (ch == 'X')      setColor(RED);
    else if (ch == 'O') setColor(GREEN);
    else                setColor(WHITE);
    cout << ch;
}

// ------------------------------------------------------------
// drawBoard: clears the screen, then draws the grid with
// box characters. Borders are cyan; the cells use printCell.
// ------------------------------------------------------------
void drawBoard() {
    system("cls");                       // wipe the old board

    setColor(YELLOW);
    cout << "\n      TIC - TAC - TOE\n\n";

    setColor(CYAN);
    cout << "     +-----+-----+-----+\n";
    for (int r = 0; r < 3; r++) {
        setColor(CYAN);
        cout << "     |";
        for (int c = 0; c < 3; c++) {
            setColor(CYAN);
            cout << "  ";
            printCell(board[r][c]);      // colored X / O / number
            setColor(CYAN);
            cout << "  |";
        }
        cout << "\n     +-----+-----+-----+\n";
    }
    setColor(WHITE);
    cout << "\n";
}

// ------------------------------------------------------------
// Move mapping: the player types 1-9, but the array is [0-2][0-2].
//   row    = (cell - 1) / 3     (integer division)
//   column = (cell - 1) % 3     (remainder)
// Example: cell 5 -> row 1, column 1 (the center).
// ------------------------------------------------------------

// ------------------------------------------------------------
// getMove: asks for a cell number and keeps asking until the
// move is valid. Returns the chosen cell number (1-9).
// ------------------------------------------------------------
int getMove(string playerName, char symbol) {
    int cell;
    while (true) {
        setColor(YELLOW);
        cout << playerName << " (" << symbol << "), choose a cell (1-9): ";
        setColor(WHITE);
        cin >> cell;

        // Check 1: input was not a number (e.g. a letter)
        if (cin.fail()) {
            cin.clear();                 // reset the error state
            cin.ignore(1000, '\n');      // throw away the bad input
            setColor(RED);
            cout << "Please type a NUMBER between 1 and 9.\n";
            continue;
        }
        // Check 2: number is out of range
        if (cell < 1 || cell > 9) {
            setColor(RED);
            cout << "Out of range! Choose between 1 and 9.\n";
            continue;
        }
        // Check 3: cell is already taken
        int row = (cell - 1) / 3;
        int col = (cell - 1) % 3;
        if (board[row][col] == 'X' || board[row][col] == 'O') {
            setColor(RED);
            cout << "That cell is already taken. Try another.\n";
            continue;
        }
        return cell;                     // all checks passed
    }
}

// ------------------------------------------------------------
// checkWin: returns true if 'symbol' has 3 in a line.
// There are only 8 winning lines: 3 rows, 3 columns, 2 diagonals.
// ------------------------------------------------------------
bool checkWin(char symbol) {
    for (int i = 0; i < 3; i++) {
        // row i is complete
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
            return true;
        // column i is complete
        if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)
            return true;
    }
    // main diagonal (top-left to bottom-right)
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
        return true;
    // other diagonal (top-right to bottom-left)
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
        return true;
    return false;
}

// ------------------------------------------------------------
// checkDraw: if every cell holds an 'X' or 'O' (no digits left)
// and nobody won, the game is a draw.
// ------------------------------------------------------------
bool checkDraw() {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (board[r][c] != 'X' && board[r][c] != 'O')
                return false;            // found an empty cell
        }
    }
    return true;
}

// ------------------------------------------------------------
// showScoreboard: prints the totals.
// ------------------------------------------------------------
void showScoreboard() {
    setColor(CYAN);
    cout << "\n  ========== SCOREBOARD ==========\n";
    setColor(WHITE);
    cout << "  Total games played : " << gamesPlayed << "\n";
    setColor(RED);
    cout << "  " << name1 << " (X) wins : " << wins1 << "\n";
    setColor(GREEN);
    cout << "  " << name2 << " (O) wins : " << wins2 << "\n";
    setColor(YELLOW);
    cout << "  Draws              : " << draws << "\n";
    setColor(CYAN);
    cout << "  ================================\n\n";
    setColor(WHITE);
}

// ------------------------------------------------------------
// playRound: plays one full game and updates the scores.
// ------------------------------------------------------------
void playRound() {
    resetBoard();
    int turn = 1;                        // 1 = Player 1, 2 = Player 2
    bool gameOver = false;

    while (!gameOver) {
        drawBoard();
        string currentName = (turn == 1) ? name1 : name2;
        char   currentSym  = (turn == 1) ? 'X'   : 'O';

        int cell = getMove(currentName, currentSym);
        board[(cell - 1) / 3][(cell - 1) % 3] = currentSym;   // place the move

        if (checkWin(currentSym)) {
            drawBoard();
            setColor(YELLOW);
            cout << "  *** " << currentName << " wins this round! ***\n";
            if (turn == 1) wins1++; else wins2++;
            gameOver = true;
        }
        else if (checkDraw()) {
            drawBoard();
            setColor(YELLOW);
            cout << "  It's a draw!\n";
            draws++;
            gameOver = true;
        }
        else {
            turn = (turn == 1) ? 2 : 1;  // switch player
        }
    }
    gamesPlayed++;
    showScoreboard();
}

int main() {
    setColor(YELLOW);
    cout << "Enter Player 1 name (X): ";
    setColor(WHITE);
    getline(cin, name1);
    setColor(YELLOW);
    cout << "Enter Player 2 name (O): ";
    setColor(WHITE);
    getline(cin, name2);

    char again;
    do {
        playRound();
        setColor(YELLOW);
        cout << "Play another round? (y/n): ";
        setColor(WHITE);
        cin >> again;
    } while (again == 'y' || again == 'Y');

    system("cls");
    setColor(YELLOW);
    cout << "\n  Thanks for playing!\n";
    showScoreboard();                    // final scoreboard on exit
    return 0;
}
