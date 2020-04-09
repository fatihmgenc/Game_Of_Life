// Game_Of_Life_Task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
using namespace std;

const int TableSize = 10;

void CopyTables(bool GameTable1[TableSize][TableSize], bool GameTable2[TableSize][TableSize])
{
    for (int i = 0; i < TableSize; i++) {
        for (int j = 0; j < TableSize; j++) {
            GameTable1[i][j] = GameTable2[i][j];
        }
    }
}
void PrintTable(bool GameTable[TableSize][TableSize]) {
    std::cout << "Game Table : \n";
    for (int i = 0; i < TableSize; i++) {
        for (int j = 0; j < TableSize; j++) {
            if (GameTable[i][j])
                std::cout << " 1";
            else
                std::cout << " 0";
        }
        std::cout << "\n";
    }
}
void SetCoordinates(bool GameTable[TableSize][TableSize]) {
    int tempAmount, x, y;

    std::cout << "\nhow many alive cell do you want to set ? : (for basic blinker enter '0' ) ";
    std::cin >> tempAmount;
    if (tempAmount == 0) {
        GameTable[4][5] = true;
        GameTable[4][4] = true;
        GameTable[4][3] = true;
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
void CheckConditions(bool GameTable[TableSize][TableSize]) {
    bool tempGameTable[TableSize][TableSize] = {};
    CopyTables(tempGameTable, GameTable);
    int tempCountAlive = 0;
    for (int i = 0; i < TableSize - 1; i++) {
        for (int j = 0; j < TableSize - 1; j++) {
            tempCountAlive = 0;
            for (int h = -1; h < 2; h++) {
                for (int v = -1; v < 2; v++) {
                    if ((i + h) >= 0 && //border condition
                        (i + h) < TableSize &&
                        (j + v) >= 0 &&
                        (j + v) < TableSize &&
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
int main()
{
    bool GameTable[TableSize][TableSize] = { false };
    PrintTable(GameTable);
    SetCoordinates(GameTable);
    while (true) {
        CheckConditions(GameTable);
        PrintTable(GameTable);
        Sleep(2500);
        system("cls");
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
