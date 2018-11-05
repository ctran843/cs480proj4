#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <numeric>
#include <math.h>
using namespace std;

int nodeExpansions = 0;

string getFile(string fileName)
{
    cout << "Could not open file." << endl;
    cout << "Please enter a valid file name or enter 3 to exit the program." << endl;
    cin >> fileName;
    if (fileName == "3")
    {
        cout << "Exiting the program" << endl;
        return "0";
    }
    else
    {
        return fileName;
    }
}

void printAndSaveBoard(ifstream & file, vector< vector<string> > & board)
{
    string line;
    string piece;
    cout << "Turn: Player 1" << endl;
    // for loop both prints and saves the values from the file
    for(int i = 0; i < 4; i++)
    {
        vector<string> row;
        getline(file, line);
        for (int j = 0; j < 4; j++)
        {
            cout << line[j] << " ";
            piece = string(1, line[j]);
            row.push_back(piece);
        }
        board.push_back(row);
        cout << endl;
    }
    file.close();
}

void printResults(float time, float val)
{
    if(val == 1)
    {
        cout << "Root Node Value: 100 Player 1 wins" << endl;
    }
    else if(val == 0)
    {
        cout << "Root Node Value: -100 player 2 wins" << endl;
    }
    else
    {
        cout << "Root Node Value: " << val << " the game ends in a tie"<< endl;
    }

    cout << "Best Move Found: " << endl;
    cout << "Number of Nodes expanded: " << nodeExpansions << endl;
    cout << fixed << setprecision(6) << "CPU Time: " << time << " seconds." << endl;
    cout << endl;
    nodeExpansions = 0;
}

float eval(vector< vector<string> > board)
{   // this function looks at the current board state and returns 1 if
    // its a win for player one, a 0 if its a win for player 2, 0.5
    // if it's a tie and a -1 if it isn't a valid board.
    int ones = 0;
    int twos = 0;
    int dif = 0;

    // vectors to indicate whether a row or column has been filled for a given number
    vector<int> rowP1 (4);
    vector<int> colP1 (4);
    vector<int> rowP2 (4);
    vector<int> colP2 (4);

    // variables to hold the total value of each of the above vectors
    int rowP1Total = 0;
    int colP1Total = 0;
    int rowP2Total = 0;
    int colP2Total = 0;

    // booleans to indicate if a player has enough columns and rows to win
    bool p1Done;
    bool p2Done;

    // for loop that iterates through the board and finds 2s and 1s.
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            string curVal = board[i][j];
            if(curVal == "1")
            {
                ones++;
                rowP1[i] = 1;
                colP1[j] = 1;
            }
            else if(curVal == "2")
            {
                twos++;
                rowP2[i] = 1;
                colP2[j] = 1;
            }
        }
    }

    if(ones >= twos)
    {
        dif = ones - twos;
    }
    else if(twos > ones)
    {
        return -10000;
    }

    // difference between amount of ones and twos is too great
    // and therefore it is not a valid board.
    if(dif > 1)
    {
        return -10000;
    }

    //evaluating the number of ways to win in the board starting with p1
    int P1 = 0;
    int P2 = 0;
    vector<int> Ctaken{0,0,0,0};
    //starting top row
    for(int i = 0;i <4;i++)
    {
        if(board[i][0]=="0" || board[i][0]=="1")
        {
            Ctaken[i] = 1;
            //searches middle top row
            for(int j = 0;j < 4;j++)
            {
                if ((!Ctaken[j]==1)&&(board[j][1]=="0" || board[j][1]=="1"))
                {
                    Ctaken[j] = 1;
                    //searches middle bottom row
                    for(int k = 0;k < 4;k++)
                    {
                        if ((!Ctaken[k]==1)&&(board[k][2]=="0" || board[k][2]=="1"))
                        {
                            Ctaken[k] = 1;
                            //searches middle bottom row
                            for(int l = 0;l < 4;l++)
                            {

                                if ((!Ctaken[l]==1)&&(board[l][3]=="0" || board[l][3]=="1"))
                                {
                                    P1++;
                                }

                            }
                            Ctaken[k]=0;
                        }

                    }
                    Ctaken[j]=0;
                }

            }
            Ctaken[i]=0;
        }
    }
    //evaluating the number of ways to win with p2
    Ctaken={0,0,0,0};
    for(int i = 0;i <4;i++)
    {
        if(board[i][0]=="0" || board[i][0]=="2")
        {
            Ctaken[i] = 1;
            //searches middle top row
            for(int j = 0;j < 4;j++)
            {
                if ((!Ctaken[j]==1)&&(board[j][1]=="0" || board[j][1]=="2"))
                {
                    Ctaken[j] = 1;
                    //searches middle bottom row
                    for(int k = 0;k < 4;k++)
                    {

                        if ((!Ctaken[k]==1)&&(board[k][2]=="0" || board[k][2]=="2"))
                        {
                            Ctaken[k] = 1;
                            //searches middle bottom row
                            for(int l = 0;l < 4;l++)
                            {
                                if ((!Ctaken[l]==1)&&(board[l][3]=="0" || board[l][3]=="2"))
                                {
                                    P2++;
                                }

                            }
                            Ctaken[k]=0;
                        }

                    }
                    Ctaken[j]=0;
                }

            }
            Ctaken[i]=0;
        }

    }
    Ctaken={0,0,0,0};


    // total up the values of the vectors
    rowP1Total = accumulate(rowP1.begin(), rowP1.end(), 0);
    colP1Total = accumulate(colP1.begin(), colP1.end(), 0);
    rowP2Total = accumulate(rowP2.begin(), rowP2.end(), 0);
    colP2Total = accumulate(colP2.begin(), colP2.end(), 0);

    bool player1W = false;
    bool player2W = false;
    //starting top row
    for(int i = 0;i <4;i++)
    {
        if(board[i][0]=="1")
        {
            Ctaken[i] = 1;
            //searches middle top row
            for(int j = 0;j < 4;j++)
            {
                if ((!Ctaken[j]==1)&&(board[j][1]=="1"))
                {
                    Ctaken[j] = 1;
                    //searches middle bottom row
                    for(int k = 0;k < 4;k++)
                    {

                        if ((!Ctaken[k]==1)&&(board[k][2]=="1"))
                        {
                            Ctaken[k] = 1;
                            //searches middle bottom row
                            for(int l = 0;l < 4;l++)
                            {
                                if ((!Ctaken[l]==1)&&(board[l][3]=="1"))
                                {
                                    player1W = true;
                                }
                            }
                            Ctaken[k]=0;
                        }
                    }
                    Ctaken[j]=0;
                }
            }
            Ctaken[i]=0;
        }
    }

    Ctaken={0,0,0,0};
    for(int i = 0;i <4;i++)
    {
        if(board[i][0]=="2")
        {
            Ctaken[i] = 1;
            //searches middle top row
            for(int j = 0;j < 4;j++)
            {
                if ((!Ctaken[j]==1)&&(board[j][1]=="2"))
                {
                    Ctaken[j] = 1;
                    //searches middle bottom row
                    for(int k = 0;k < 4;k++)
                    {

                        if ((!Ctaken[k]==1)&&(board[k][2]=="2"))
                        {
                            Ctaken[k] = 1;
                            //searches middle bottom row
                            for(int l = 0;l < 4;l++)
                            {
                                if ((!Ctaken[l]==1)&&(board[l][3]=="2"))
                                {
                                    player2W = true;
                                }

                            }
                            Ctaken[k]=0;
                        }
                    }
                    Ctaken[j]=0;
                }
            }
            Ctaken[i]=0;
        }
    }
    Ctaken={0,0,0,0};
    bool boardFull = true;
    for(int i = 0;i<4;i++)
        for(int j = 0;j<4;j++)
        {
            if(board[i][j]=="0")
            {
                boardFull = false;
            }
        }

    // if both true then it is a tie
    if((!player1W && !player2W)&& boardFull)
    {
        return 0;
    }
        // if p1Done is true and p2Done is false then player one has won
    else if(player1W)
    {
        return 100;
    }
        // if p1Done is false and p2Done is true then player two has won
    else if(player2W)
    {
        return -100;
    }
        // the game has not winner and there is no tie
    else
    {
        return (P1-P2);
    }
}

bool checkStatus(float status,vector<vector<string>> board)
{   // checks to see if the game has been solved or not.

    bool boardFull = true;
    for(int i = 0;i<4;i++)
        for(int j = 0;j<4;j++)
        {
            if(board[i][j]=="0")
            {
                boardFull = false;
            }
        }
    if(status == -10000)
    {
        cout << "\n" << "This board is invalid. Please provide a valid board." << endl;
        return true;
    }
    else if(status == 100)
    {
        cout << "\n" << "Player 1 has already won the game." << endl;
        return true;
    }
    else if(status == -100)
    {
        cout << "\n" << "Player 2 has already won the game." << endl;
        return true;
    }
    else if(boardFull)
    {
        cout << "\n" << "The board is in a final tied game state" << endl;
        return true;
    }
    else
        return false;
}


int maxNode(vector< vector<string> > board)
{
    int player1 = 0;
    int player2 = 0;
    //int why = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(board[i][j] == "1")
            {
                player1++;
            }
            else if(board[i][j] == "2")
            {
                player2++;
            }
        }
    }

    if(player1 > player2)
    {
        return 2;
    }
    else
    {
        return 1;
    }

}

vector< vector< vector<string> > > succ(vector< vector<string> > board, int whosPlay)
{
    vector< vector< vector<string> > > successors;
    vector<vector <string>> tmp = board;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(board[i][j] == "0")
            {
                if(whosPlay == 1)
                {
                    tmp[i][j] = "1";
                }
                else
                {
                    tmp[i][j] = "2";
                }
                nodeExpansions++;
                successors.push_back(tmp);
                vector<vector <string>> tmp = board;
            }
        }
    }

    return successors;
}

float minMax(vector <vector<string> > board, int whosPlay)
{// minMax algorithm

    // algorithm goes here
    vector<vector<vector <string> >> successors;
    float val = 0;
    float evalVal = eval(board);
    if(evalVal == 100 || evalVal == -100 || evalVal == 0)
    {
        return evalVal;
    }
    if(whosPlay == 1)
    {
        val = INFINITY;
    }
    else
    {
        val = -INFINITY;
    }

    successors = succ(board, whosPlay);
    for(int i = 0; i < successors.size(); i++)
    {
        if(whosPlay == 1)
        {
            val = max(val, minMax(successors[i], whosPlay+1));
        }
        else
        {
            val = min(val, minMax(successors[i], whosPlay-1));
        }
    }
    if(val == INFINITY)
    {
        return 1;
    }
    else if (val == -INFINITY)
    {
        return 0;
    }
    return val;

}

float alphaBetaPruning(vector <vector<string> > board, int player)
{// alpha-beta pruning algorithm

    // algorithm goes here

}

int main()
{
    int choice = 0;
    float status = 0;
    float val = 0;
    int player = 0;
    while(choice != 3)
    {
        cout << "Enter the corresponding number to choose one of the options below: " << endl;
        cout << "1) Minmax" << endl;
        cout << "2) Alpha-beta Pruning" << endl;
        cout << "3) Quit" << endl;

        cin >> choice;
        cout << endl;

        // holds the string values of the items in the board
        vector< vector<string> > board;
        string fileName;
        if (choice == 1)
        {// use minmax algorithm
            cout << "Enter a .txt file containing a 4x4 tic toe board" << endl;
            cin >> fileName;
            ifstream file;
            file.open(fileName);
            while(!file.is_open())
            {
                fileName = getFile(fileName);
                if(fileName == "0")
                    return 0;

                file.open(fileName);
            }
            cout << endl;
            printAndSaveBoard(file, board);
            status = eval(board);
            if(checkStatus(status,board))
            {   // the game is either won or has an invalid board
                cout << "Provide another board to be solved." << "\n" << endl;
            }
            else
            {
                player = maxNode(board);
                cout << endl;

                float time;
                clock_t t;
                t = clock();

                val = minMax(board, player);

                t = clock() - t;
                time = (float(t)/CLOCKS_PER_SEC);
                printResults(time, val);
            }

        }
        else if (choice == 2)
        {// perform alpha-beta pruning
            cout << "Enter a .txt file containing a 4x4 tic toe board" << endl;
            cin >> fileName;
            ifstream file;
            file.open(fileName);
            while(!file.is_open())
            {
                fileName = getFile(fileName);
                if(fileName == "0")
                    return 0;

                file.open(fileName);
            }
            cout << endl;
            printAndSaveBoard(file, board);
            status = eval(board);
            if(checkStatus(status,board))
            {   // the game is either won or has an invalid board
                cout << "Provide another board to be solved." << "\n" << endl;
            }
            else
            {
                cout << endl;

                player = maxNode(board);
                float time;
                clock_t t;
                t = clock();

                val = alphaBetaPruning(board, player);

                t = clock() - t;
                time = (float(t))/CLOCKS_PER_SEC;
                printResults(time, val);
            }
        }
        else if (choice == 3)
        {// exit the program
            cout << "Exiting the program" << endl;
            return 0;
        }
        else
        {// user made an invalid choice
            cout << "Please enter either 1, 2, or 3" << endl;
        }
    }
}