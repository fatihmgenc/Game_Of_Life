// Game_Of_Life_Task3.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <boost/program_options.hpp>
#include <vector>
#include <csignal>
#include <SFML/Graphics.hpp>


using namespace std;
namespace opt = boost::program_options;
const string CFileName = "settings.cfg";
const string LFileName = "loadfile.txt";

void CopyTables(vector<vector<bool>>& GameTable1, vector<vector<bool>>& GameTable2)
{
    for (int i = 0; i < GameTable1.size(); i++) {
        for (int j = 0; j < GameTable1[0].size(); j++) {
            GameTable1[i][j] = GameTable2[i][j];
        }
    }
}
void PrintTable(vector<vector<bool>> GameTable) {
    std::cout << "Game Table : \n";
    for (int i = 0; i < GameTable.size(); i++) {
        for (int j = 0; j < GameTable[0].size(); j++) {
            if (GameTable[i][j])
                std::cout << " 1";
            else
                std::cout << " 0";
        }
        std::cout << "\n";
    }
}
void SetCoordinates(vector<vector<bool>>& GameTable) {
    int tempAmount, x, y;
    std::cout << "\nhow many alive cell do you want to set ? : (for basic blinker enter '0' ) ";
    std::cin >> tempAmount;
    if (tempAmount == 0) {
        GameTable[(int)(GameTable.size() / 2)][(int)((GameTable[0].size() / 2) + 1)] = true;
        GameTable[(int)(GameTable.size() / 2)][(int)((GameTable[0].size() / 2))] = true;
        GameTable[(int)(GameTable.size() / 2)][(int)((GameTable[0].size() / 2) - 1)] = true;
        return;
    }
    for (int i = 0; i < tempAmount; i++) {
        std::cout << "\nrow and column for " << i << ". : ";
        std::cin >> x >> y;
        if (x > 0 && y > 0) {
            GameTable[x - 1][y - 1] = true;
            PrintTable(GameTable);
        }
        else {
            std::cout << "Coordinates can not be lower than 1 ";
            i--;
        }
    }
}
void CheckConditions(vector<vector<bool>>& GameTable) {
    vector<vector<bool>> tempGameTable(GameTable.size(),
        vector<bool>(GameTable[0].size(), 0));
    CopyTables(tempGameTable, GameTable);
    int tempCountAlive = 0;
    for (int i = 0; i < GameTable.size() - 1; i++) {
        for (int j = 0; j < GameTable[0].size() - 1; j++) {
            tempCountAlive = 0;
            for (int h = -1; h < 2; h++) {
                for (int v = -1; v < 2; v++) {
                    if ((i + h) >= 0 && //border condition
                        (i + h) < GameTable.size() &&
                        (j + v) >= 0 &&
                        (j + v) < GameTable.size() &&
                        (h != 0 || v != 0)) //& check if it is controling itself instead of neighberhood
                    {
                        if (GameTable[i + h][j + v])
                        {
                            ++tempCountAlive;
                        }
                    }
                }
            }
            //    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            //    Any live cell with two or three live neighbours lives on to the next generation.
            //    Any live cell with more than three live neighbours dies, as if by overpopulation.
            //    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            //    These rules, which compare the behavior of the automaton to real life, can be condensed into the following :

            //    Any live cell with two or three neighbors survives.
            //    Any dead cell with three live neighbors becomes a live cell.
            //    All other live cells die in the next generation.Similarly, all other dead cells stay dead.

            if (GameTable[i][j]) {
                //if cell alive
                if (tempCountAlive < 2)
                    tempGameTable[i][j] = false;
                else if ((tempCountAlive == 2) || (tempCountAlive == 3))
                    tempGameTable[i][j] = true;
                else if (tempCountAlive > 3)
                    tempGameTable[i][j] = false;
            }
            else {
                //if cell dead
                if (tempCountAlive == 3)
                    tempGameTable[i][j] = true;
            }
        }
    }
    CopyTables(GameTable, tempGameTable);
}
void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
bool IsExist(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}
string decodeLine(string s) {
    string decodedOutput = "";
    int secondIterator = 0;
    for (int firstIterator = 0; firstIterator < s.length(); firstIterator++)
    {
        string counter = ""; int letterCoefficient = 0;

        if (!isalpha(s[firstIterator]))
            continue;
        else
        {
            counter = s.substr(secondIterator, firstIterator - secondIterator);
            secondIterator = firstIterator + 1;
            istringstream(counter) >> letterCoefficient;
            for (int i = 0; i < letterCoefficient; i++)
                decodedOutput += s[firstIterator];
        }
    }
    return decodedOutput;
}
void SaveAsRLE(vector<vector<bool>> GameTable) {

    fstream Lfile;
    string temp;
    Lfile.open(LFileName, fstream::out);

    for (int i = 0; i < GameTable.size(); i++) {

        for (int j = 0; j < GameTable[0].size(); j++) {

            int count = 1;
            while (j < GameTable[0].size() - 1 && GameTable[i][j] == GameTable[i][j + 1]) {
                count++;
                j++;
            }
            if (GameTable[i][j] == 0) {
                temp = 'F';
            }
            else { temp = 'T'; }
            Lfile << count << temp;

        }
        Lfile << "\n";
    }
    Lfile.close();
}
void signalHandler(int signum) {
    string answer;
    std::cout << "\n Load file created/updated.";
    exit(signum);
}
void LoadFromRLE(vector<vector<bool>>& GameTable) {
    string myText, tempLine;
    int rowCounter = 0;
    ifstream inFile;
    inFile.open(LFileName);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    //while morel lines
    while (getline(inFile, myText)) {
        tempLine = decodeLine(myText);
        for (int i = 0; i < tempLine.size(); i++) {
            if (tempLine[i] == 'F') {
                GameTable[rowCounter][i] = false;
            }
            else {
                GameTable[rowCounter][i] = true;
            }
        }
        rowCounter++;

    }
    inFile.close();
}


int main(int argc, char* argv[])
{
    const int CELL_SIZE = 30;
    const sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);

#define WHITE sf::Color::White
#define Red sf::Color::Red

    signal(SIGINT, signalHandler);
    std::cout << "Do you want set new table size and turn to default parameters or continiou to loaded game ? (Y/N) ";
    char answer;
    std::cin >> answer;
    // chechk if config file already exist.
    if (IsExist(CFileName)) {

    }
    else if (!IsExist(CFileName)) {
        //creating all file
        fstream cfile;
        cfile.open(CFileName, fstream::out);
        cfile << "sizeX=10\nsizeY=10\nspeed=1\niterations=100\n";
        cfile.close();
    }

    if (answer == 'Y' || answer == 'y') {
        string tempX, tempY;
        cout << "Set table size widght = ";
        cin >> tempY;
        cout << "Set table size height = ";
        cin >> tempX;
        fstream cfile;
        cfile.open(CFileName, fstream::out);
        cfile << "sizeX=" + tempX + "\nsizeY=" + tempY + "\nspeed=1\niterations=100\n";
        cfile.close();
    }

    string line;
    // defining parameters
    opt::options_description desc("options");
    desc.add_options()
        ("sizeX", opt::value<int>(), "X Range of game table")
        ("sizeY", opt::value<int>(), "Y Range of game table")
        ("speed", opt::value<int>(), "speed from 1 to 20")
        ("iterations", opt::value<int>(), "number of iterations")
        ("help", "produce help message")
        ;
    opt::variables_map vm;
    //parsing and storing parameters in boost-options library
    opt::store(opt::parse_command_line(argc, argv, desc), vm);
    try {
        opt::store(opt::parse_config_file("settings.cfg", desc), vm);
    }
    catch (const opt::reading_file& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    try {
        opt::notify(vm);
    }
    catch (const opt::required_option& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 2;
    }

    // taking our sizes and speed back from config file
    int TableSizeX = vm["sizeX"].as<int>();
    int TableSizeY = vm["sizeY"].as<int>();
    int speed = vm["speed"].as<int>();
    int iterations = vm["iterations"].as<int>();


    // creating 2d boardgame 
    vector<vector<bool>> GameTable(TableSizeX, vector<bool>(TableSizeY, 0));
    char answerT;
    cout << "Do you want to load from save file ?(Y/N) ";
    cin >> answerT;
    if (answerT == 'Y' || answerT == 'y') {
        LoadFromRLE(GameTable);
    }
    else
    {
        SetCoordinates(GameTable);
    }

    //PrintTable(GameTable);


    sf::Font font;
    font.loadFromFile("./arial.ttf");

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * TableSizeX, CELL_SIZE * TableSizeY + 50), "Game of Life");
    while (window.isOpen() && iterations > 0)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
            window.clear(WHITE);
            for (int x = 0; x < TableSizeX ; x++)
            {
                for (int y = 0; y < TableSizeY ;y++)
                {

                    sf::RectangleShape cell;
                    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    cell.setSize(CELL_VECTOR);
                    cell.setOutlineThickness(1);
                    cell.setOutlineColor(sf::Color::Green);
                    if (GameTable[x][y] == 1)
                        cell.setFillColor(Red);
                    else
                        cell.setFillColor(WHITE);
                    window.draw(cell);

                }
            }
            window.display();

            SaveAsRLE(GameTable);
            system("cls");
            CheckConditions(GameTable);
            PrintTable(GameTable);
            std::cout << to_string(iterations) + " iterations left";
            Sleep(500 / speed);
            iterations--;


        }

    }
}