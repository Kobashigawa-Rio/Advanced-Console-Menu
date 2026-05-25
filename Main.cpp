#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <sstream>

//For testing porpuses, you can delete it
#include <cstdlib>
#include <ctime>

// Internal Libaries
#include "Player.h"

using namespace System;
using namespace std;

void EnabledRGBColours() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// Macros ANSI Foreground and Background Colors
#define RGB_FG(r, g, b) "\033[38;2;" << r << ";" << g << ";" << b << "m"
#define RGB_BG(r, g, b) "\033[48;2;" << r << ";" << g << ";" << b << "m"
#define RESET_COLOUR "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

// Alternative method to clear screen using ANSI escape codes
//#define Clear_Screen "\033[2J\033[H"

string FgRGB(int r, int g, int b) { 
    return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

string BgRGB(int r, int g, int b) { 
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

//App DarkTheme Colors
const string TitleColour = FgRGB(255, 215, 0) + BOLD; // Golden
const string DefaultColour = FgRGB(180, 180, 220); // Lavander
const string CursorFgColour = FgRGB(15, 15, 30); // Semi-Black
const string CursorBgColour = BgRGB(80, 200, 255); // Glowing Cyan
const string BorderColour = FgRGB(100, 100, 255); // Electric Blue
const string FooterColour = FgRGB(150, 150, 255); // Light Blue
const string ErrorColour = FgRGB(220, 20, 60) + BOLD; // Crimson Red

int getConsoleWideth() { 
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int getConsoleHeight() { 
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void PrintCentered(const string& text, const string& ANSI_Colour = "") { 
    int consoleWidth = getConsoleWideth();
    int leingth = (int)text.size();
    int padding = (consoleWidth - text.length()) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << ANSI_Colour << text << (ANSI_Colour.empty() ? "" : RESET_COLOUR) << endl;
}

void MoveCursor(int x, int y) { 
    cout << "\033[" << x << ";" << y << "H";
}

struct Options { 
    string label;
    int value;
};

int ShowMenu(const string& title, const vector<Options>& options) {
    int selection = 0;
    const int total = (int)options.size();

    auto draw = [&]() {
        system("CLS");

        // Alternative method to clear screen using ANSI escape codes
        //cout << Clear_Screen;
        
        int wideth = getConsoleWideth();
        int height = getConsoleHeight();
        // Border
        cout << BorderColour;
        PrintCentered(string(40, '='));

        //title
        PrintCentered("");
        PrintCentered(title, TitleColour);
        PrintCentered("");

        //down border
        cout << BorderColour;
        PrintCentered(string(40, '='));

        //options
        for (int i = 0; i < total; i++) {
            string line = (i == selection) ? " > " + options[i].label + " < " : "   " + options[i].label + "   ";

            if (i == selection) {
                int Wideth = wideth;
                int Leingth = (int)line.size();
                int Padding = (Wideth - Leingth) / 2;
                if (Padding < 0) Padding = 0;

                cout << RESET_COLOUR << string(Padding, ' ') << CursorBgColour << CursorFgColour << BOLD << line << RESET_COLOUR << endl;
            }
            else {
                PrintCentered(line, DefaultColour);
            }
        }

        //footer
        PrintCentered("");
        cout << BorderColour;
        PrintCentered(string(40, '='));
        PrintCentered("Use the Arrows [Up/Down] or [w/s] to navegate                Use [Enter] to accept", FooterColour);
        cout << RESET_COLOUR;
        };

    //Hide cursor
    Console::CursorVisible = false;

	// Alternative method to hide cursor using Windows API
    //CONSOLE_CURSOR_INFO cci;
    //GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
    //CONSOLE_CURSOR_INFO cciHide = cci;
    //cciHide.bVisible = false;
    //SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cciHide);

    //Input
    while (true) {
        draw();

        int key = _getch();

        if (key == 0 || key == 0xE0 || key == 'w' || key == 's') {
            key = _getch();

            switch (key) {
            case 72: // Up Arrow
                selection = (selection - 1 + total) % total;
                break;
            case 'w':
                selection = (selection - 1 + total) % total;
                break;
            case 80: // Down Arrow
                selection = (selection + 1) % total;
                break;
            case 's':
                selection = (selection + 1) % total;
                break;
            }
        }
        else if (key == 13) { // Enter
            break;
        }
    }

    Console::CursorVisible = true;
	
    // Alternative method to restore original cursor visibility using Windows API
    //SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci); // Restore original cursor visibility
    
    return options[selection].value;
}

void RecordPlayers(int score) {
    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

	ofstream PlayersFile("players.txt", ios::app);

    if (PlayersFile.is_open()) {
        string name;

        PrintCentered(string(40, '='), BorderColour);
        PrintCentered("New Record!!", TitleColour);
        PrintCentered(string(40, '='), BorderColour);

        string prompt = "Write your name: ";
        int padding = (getConsoleWideth() - (int)prompt.size()) / 2;
        cout << string(padding, ' ') << DefaultColour << prompt << RESET_COLOUR;
        cin >> name;

        Player<string, int>* player = new Player<string, int>(name, score);

		PlayersFile << player->getName() << "           " << player->getScore() << endl;
		PlayersFile.close();
    }
    else {
        cerr << ErrorColour << "Error opening players file!" << endl;
        cout << RESET_COLOUR;
    }
}

bool fileExists(const string& Path) {
    ifstream File(Path);
    return File.good();
}

void ScoreBoard(int score) {
    string path = "players.txt";
    int i = 1;

    vector<string> Lines;
    if (fileExists(path)) {
        ifstream PlayersFile(path);
        if (PlayersFile.is_open()) {
            string line;
            while (getline(PlayersFile, line)) {
                Lines.push_back(line);
            }
            PlayersFile.close();
        }
    }

    int Score2 = 0;
    if (!Lines.empty()) {
        string lastLine = Lines.back();
        istringstream ss(lastLine);
        string text;
        ss >> text >> Score2;
    }

    if (score > Score2) {
        RecordPlayers(score);

        Lines.clear();
        ifstream PlayersFile(path);
        if (PlayersFile.is_open()) {
            string line;
            while (getline(PlayersFile, line)) {
                Lines.push_back(line);
            }
            PlayersFile.close();
        }
    }
	cout << "Hiest Score: " << Score2 << endl;

    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

    PrintCentered(string(40, '='), BorderColour);
    PrintCentered("Score Board", TitleColour);
    PrintCentered(string(40, '='), BorderColour);
    cout << RESET_COLOUR << endl << endl;

    int total = Lines.size();
    for (int i = total - 1; i >= 0; i--) {
        int index = total - i;
		string actualLine = to_string(index) + ". " + Lines[i];
        PrintCentered(actualLine, DefaultColour);
        cout << endl;
    }
}

// For Testing the Menu System
void NewGame() {
    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

    PrintCentered("", BorderColour);
    PrintCentered("Starting a new game...", TitleColour);
    PrintCentered("", BorderColour);
    cout << RESET_COLOUR;
    _sleep(2000);
    
    // Alternative method to pause
    //Sleep(2000);
}

void LoadGame() {
    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

    PrintCentered("", BorderColour);
    PrintCentered("Loading game...", TitleColour);
    PrintCentered("", BorderColour);
    cout << RESET_COLOUR;
    _sleep(2000);
    
    // Alternative method to pause
    //Sleep(2000);
}

void Settings() {
    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

    PrintCentered("", BorderColour);
    PrintCentered("Opening settings...", TitleColour);
    PrintCentered("", BorderColour);
    cout << RESET_COLOUR;
    _sleep(2000);
    
    // Alternative method to pause
    //Sleep(2000);
}

void Credits() {
    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

    PrintCentered(string(40, '='), BorderColour);
    PrintCentered("Credits", TitleColour);
    PrintCentered(string(40, '='), BorderColour);
    PrintCentered("Autor: Kobashigawa Rio", TitleColour);
    PrintCentered("Software Engineering", TitleColour);
    
    cout << RESET_COLOUR;
    _sleep(2000);
    
    // Alternative method to pause
    //Sleep(2000);
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    int score = (rand() % 1000) + 1; // For testing porpuses, you must to replace for the next line
	//int score; //you must to get the score from the game logic and pass it to this function when the game ends
    system("CLS");

    // Alternative method to clear screen using ANSI escape codes
    //cout << Clear_Screen;

	EnabledRGBColours();

    SetConsoleTitleA("[Game Name]");

    vector<Options> options = {
        { "New Game",   1 },
        { "Load Game",  2 },
        { "Settings",   3 },
        { "Credits",    4 },
        { "Exit",       0 },
    };

	bool running = true;

    while (running) {
		int opt = ShowMenu("Main Menu", options);

        switch (opt)
        {
            case 0:
                running = false;
                system("CLS");

                // Alternative method to clear screen using ANSI escape codes
                //cout << Clear_Screen;

				PrintCentered("", BorderColour);
				PrintCentered("Exiting the game... Goodbye!", TitleColour);
                PrintCentered("", BorderColour);
				cout << RESET_COLOUR;

                _sleep(1000);
                
				// Alternative method to pause
                //Sleep(1000);
                break;
            case 1:
                NewGame();
                _sleep(1000);

                // Alternative method to pause
                //Sleep(1000);

                ScoreBoard(score);
                _sleep(3000);

                // Alternative method to pause
                //Sleep(3000);
				running = false;
                break;
            case 2:
                LoadGame();
                break;
            case 3:
				Settings();
                break;
            case 4:
                Credits();
                break;
            default:
                break;
        }
    }
    return 0;
}
