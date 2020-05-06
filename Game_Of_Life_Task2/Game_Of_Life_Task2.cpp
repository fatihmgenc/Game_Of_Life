// Game_Of_Life_Task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <boost/program_options.hpp>
#include <vector>

namespace opt = boost::program_options;
using namespace std;
const string CFileName = "settings.cfg";

void CopyTables(vector<vector<bool>> &GameTable1, vector<vector<bool>> &GameTable2)
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
void SetCoordinates(vector<vector<bool>> &GameTable) {
    int tempAmount,x, y;
    std::cout << "\nhow many alive cell do you want to set ? : (for basic blinker enter '0' ) ";
    std::cin >> tempAmount;
    if (tempAmount == 0) {
        GameTable[(int)(GameTable.size() / 2)][(int)((GameTable[0].size() / 2)+1)] = true;
        GameTable[(int)(GameTable.size() / 2)][(int)((GameTable[0].size() / 2))] = true;
        GameTable[(int)(GameTable.size() / 2)][(int)((GameTable[0].size() / 2)-1)] = true;
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
void CheckConditions(vector<vector<bool>> &GameTable) {
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
int main(int argc, char* argv[])
{
    cout << "Do you want set new table size and turn to default parameters? (Y/N)";
    char answer;
    cin >> answer;
    // chechk if config file already exist.
    if (IsExist(CFileName)){
        
    }
    else if(!IsExist(CFileName)){
     //creating all file
     fstream cfile;
     cfile.open(CFileName, fstream::out);
     cfile << "sizeX=10\nsizeY=10\nspeed=1\niterations=100\n";
     cfile.close();
    }
    if (answer == 'Y' || answer == 'y' ) {
        string tempX, tempY;
        cout << "Set table size widght = ";
        cin >> tempY;
        cout << "Set table size height = ";
        cin >> tempX;
        fstream cfile;
        cfile.open(CFileName, fstream::out);
        cfile << "sizeX=" + tempX + "\nsizeY=" + tempY+ "\nspeed=1\niterations=100\n";
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
        opt::store(opt::parse_config_file("settings.cfg",desc),vm);
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
    //if (vm.count("help")) {
    //    std::cout << desc << "\n";
    //    return 1;
    //}

    // taking our sizes and speed back from config file
    int TableSizeX = vm["sizeX"].as<int>();
    int TableSizeY = vm["sizeY"].as<int>();
    int speed = vm["speed"].as<int>();
    int iterations  = vm["iterations"].as<int>();
    // creating 2d boardgame 
    vector<vector<bool>> GameTable(TableSizeX, vector<bool>(TableSizeY, 0));

    //game flow
    PrintTable(GameTable);
    SetCoordinates(GameTable);
    while (iterations>0) {
        system("cls");
        CheckConditions(GameTable);
        PrintTable(GameTable);
        cout << to_string(iterations) + " iterations left";
        Sleep(2500/speed);
        iterations--;
    }
}

