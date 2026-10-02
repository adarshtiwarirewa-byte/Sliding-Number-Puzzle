#include<iostream>
#include<vector>
#include<time.h>
#include<numeric>
#include<stdlib.h>
#include<array>
#include<unordered_map>
#include<queue>
#include<algorithm>
#include<iomanip>
#include<string>

using BoardState = std::array<int,16>;

enum class Direction
{
    Left,
    Right,
    Up,
    Down
};



class  Matrix                  // To manage matrix along the gameplay
{
    private:

    std::vector<std::vector<int>>Grid;       
    public:
    Matrix()  : Grid(4,std::vector<int>(4))    // Constructor
    { 
        generateMatrix();
        removeEmpty();
    }

    BoardState getState() const
    {
        BoardState state{};

        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                state[i*4 + j] = Grid[i][j];
            }
        }
        return state;
    }


    void generateMatrix()      // Generates a random solvable matrix having numbers in range 1 to 16
    {
        int value = 1;
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                Grid[i][j] = value;
                value++;
            }
        }

        // Move seeding to a one_time check so restarting quickly still gets a different scramble =====
        static bool seeded = false;                                              

        if (!seeded)                                                             
        {
            srand(static_cast<unsigned int>(time(NULL)));                       
            seeded = true;                                                     
        }

        int emptyRow = 3;
        int emptyCol = 3;

        for(int step = 0;step<50;step++)
        {
            std::vector<std::pair<int,int>> choices;

            if(emptyRow>0)   choices.push_back({emptyRow-1,emptyCol});
            if(emptyRow<3)   choices.push_back({emptyRow+1,emptyCol});
            if(emptyCol>0)   choices.push_back({emptyRow,emptyCol-1});
            if(emptyCol<3)   choices.push_back({emptyRow,emptyCol+1});

            int choice = rand() % choices.size();
            int nextRow = choices[choice].first;
            int nextCol = choices[choice].second;

            std:: swap(Grid[emptyRow][emptyCol],Grid[nextRow][nextCol]);

            emptyRow = nextRow;
            emptyCol = nextCol;
        }
        return;
    }


    void removeEmpty()         // To remove the number 16 and make it empty
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(Grid[i][j]==16)
                {
                    Grid[i][j]=0;
                }
            }
        }
        return ;
    }



        // To get a  particular tile of the board i.e  (i,j)th value in the matrix
    int getTile(int row,int col) const
    {
        return Grid.at(row).at(col);
    }
    
    bool won()                   // checks the matrix elements that it in correct order from 1 to 15
    {
        int count = 1;
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(i==3 && j==3)    return Grid[i][j] == 0;
                if(Grid[i][j]!=count)  return false;
                count ++; 
            }
        }
        return true;
    }

    bool Update_Matrix(Direction move)               // It updates the matrix when called with given move
    {
        int emptyRow = -1;
        int emptyCol = -1;

        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(Grid[i][j] == 0)
                {
                    emptyRow = i;
                    emptyCol = j;
                    break;
                }
            }
        }

        if(emptyRow == -1)      return false;


        switch (move)
        {
        case Direction:: Left:
            if(emptyCol == 3)   return false;
            std:: swap(Grid[emptyRow][emptyCol], Grid[emptyRow][emptyCol+1]);
            break;
        
        case Direction:: Right:
            if(emptyCol == 0)   return false;
            std:: swap(Grid[emptyRow][emptyCol], Grid[emptyRow][emptyCol-1]);
            break;
        
        case Direction::Up:
            if (emptyRow == 3) return false;
            std::swap(Grid[emptyRow][emptyCol],
                    Grid[emptyRow + 1][emptyCol]);
            break;

        case Direction::Down:
            if (emptyRow == 0) return false;
            std::swap(Grid[emptyRow][emptyCol],
                    Grid[emptyRow - 1][emptyCol]);
            break;
        }
        return true;
    }

};

//..............................................................................................//

// For the solution of game we will use A star algorithm


class AstarSolver
{
    public:
    
    int manhattanDistance(const BoardState& state) const
    {
        int totalDistance = 0;
        for(int i=0;i<16;i++)
        {
            int tile = state[i];
            
            if(tile == 0)   continue;

            int goalIndex = tile-1;            // We want state[i]th entry in (state[i]-1)th index ...

            int currentRow = i / 4;
            int currentCol = i % 4;
            int goalRow = goalIndex / 4;
            int goalCol = goalIndex % 4;

            int rowDistance = abs(currentRow - goalRow);
            int colDistance = abs(currentCol - goalCol);

            totalDistance += rowDistance + colDistance;            // Manhattan Distane : d((a,b),(c,d)) = |c-a| + |d-b|;
        }

        return totalDistance;
    }


    std::vector<Direction> solve(const BoardState& start) const
    {
        struct Node
        {
            BoardState state;
            int blank;
            int moves;
            int estimate;
            int parent;
            Direction move;
        };

        struct Entry
        {
            int score;
            int estimate;
            int nodeIndex;
        };

        struct LaterEntry
        {
            bool operator()(const Entry& a, const Entry& b) const
            {
                if (a.score != b.score)
                    return a.score > b.score;

                return a.estimate > b.estimate;
            }
        };

        struct StateHash
        {
            std::size_t operator()(const BoardState& state) const
            {
                std::size_t hash = 0;

                for (int tile : state)
                    hash = hash * 17 + tile;

                return hash;
            }
        };

        std::vector<Node> nodes;
        std::priority_queue<Entry, std::vector<Entry>, LaterEntry> open;
        std::unordered_map<BoardState, int, StateHash> bestMoves;

        int startBlank = 0;
        for (int i = 0; i < 16; i++)
        {
            if (start[i] == 0)
            {
                startBlank = i;
                break;
            }
        }

        int startEstimate = manhattanDistance(start);

        nodes.push_back({start, startBlank, 0, startEstimate, -1, Direction::Left});
        open.push({startEstimate, startEstimate, 0});
        bestMoves[start] = 0;

        while (!open.empty())
        {
            Entry entry = open.top();
            open.pop();

            Node current = nodes[entry.nodeIndex];

            if (bestMoves[current.state] != current.moves)
                continue;

            if (current.estimate == 0)
            {
                std::vector<Direction> path;
                int index = entry.nodeIndex;

                while (nodes[index].parent != -1)
                {
                    path.push_back(nodes[index].move);
                    index = nodes[index].parent;
                }

                std::reverse(path.begin(), path.end());
                return path;
            }

            int row = current.blank / 4;
            int col = current.blank % 4;

            for (int directionIndex = 0; directionIndex < 4; directionIndex++)
            {
                int nextBlank = -1;
                Direction direction = Direction::Left;

                if (directionIndex == 0 && col < 3)
                {
                    nextBlank = current.blank + 1;
                    direction = Direction::Left;
                }
                else if (directionIndex == 1 && col > 0)
                {
                    nextBlank = current.blank - 1;
                    direction = Direction::Right;
                }
                else if (directionIndex == 2 && row < 3)
                {
                    nextBlank = current.blank + 4;
                    direction = Direction::Up;
                }
                else if (directionIndex == 3 && row > 0)
                {
                    nextBlank = current.blank - 4;
                    direction = Direction::Down;
                }

                if (nextBlank == -1)
                    continue;

                BoardState nextState = current.state;
                std::swap(nextState[current.blank], nextState[nextBlank]);

                int nextMoves = current.moves + 1;
                auto found = bestMoves.find(nextState);

                if (found != bestMoves.end() && found->second <= nextMoves)
                    continue;

                bestMoves[nextState] = nextMoves;

                int estimate = manhattanDistance(nextState);
                int nextNode = static_cast<int>(nodes.size());

                nodes.push_back({nextState, nextBlank, nextMoves, estimate, entry.nodeIndex, direction});

                open.push({nextMoves + estimate, estimate, nextNode});
            }
        }

        return {};
    }
};


//.................................................................//
// To manage the gameplay

class Game
{
    private:
    Matrix M;                             // Proper encapsulation of matrix and moves
    std::string user_choice{};
    int moves_ = 0;
    AstarSolver solver;


    char directionKey(Direction direction) const
    {
        if (direction == Direction::Left)  return 'A';
        if (direction == Direction::Right) return 'D';
        if (direction == Direction::Up)    return 'W';
        return 'S';
    }

    public:
    Game(std::string c)             //Constructor :  Takes choice input from user
    {
        user_choice = c;
    }


    // To display the board ............
    void displayBoard() const
    {
        int colWidth = 4;
        std::cout<<"\n"<<std::string(30,'-')<<"\n";
        for(int i=0;i<4;i++)
        {
            std::cout<<"|";
            for(int j=0;j<4;j++)
            {
                int tile = M.getTile(i,j);
                if(tile == 0)    std::cout<<"      |";
                else             std::cout<<std::setw(colWidth)<<tile<<"  |";
            }

            std::cout<<"\n"<<std::string(30,'-')<<"\n";
        }
    }


    void playGame()                    // It manages all the game-play by call all other functions in the class
    {
        std::string move;

        while (!M.won())
        {
            std::cout << "\nNavigation: A/a left, D/d right, W/w up, S/s down\n";
            std::cout << "H/h show solution     ? hint     R/r restart     Q/q quit\n";
            std::cout << "Enter a command: ";

            if (!(std::cin >> move))
                return;

            if (move == "Q" || move == "q")
            {
                if (Confirm_Quit())
                    return;

                continue;
            }

            if (move == "H" || move == "h")
            {
                std::vector<Direction> solution = solver.solve(M.getState());

                if (solution.empty())
                {
                    if (M.won())
                        std::cout << "\n\nThe puzzle is already solved!\n";
                    else
                        std::cout << "\n\nNo solution was found.\n";
                        
                }
                else
                {
                    std::cout << "\n\nShortest solution (" << solution.size() << " moves):\n";

                    for (Direction direction : solution)
                        std::cout << directionKey(direction) << " ";

                    std::cout << "\n";
                }

                continue;
            }

            if (move == "?")
            {
                std::vector<Direction> solution = solver.solve(M.getState());

                if (solution.empty())
                    std::cout << "\n\nThe puzzle is already solved!\n";
                else
                    std::cout << "\n\nHint: move " << directionKey(solution.front()) << ".\n";

                continue;
            }

            if (move == "R" || move == "r")
            {
                M = Matrix();        // new object created and then again newly shuffled matrix;
                moves_ = 0;
                displayBoard();
                continue;
            }

            if (!(move == "A" || move == "a" ||
                move == "D" || move == "d" ||
                move == "W" || move == "w" ||
                move == "S" || move == "s"))
            {
                std::cout << "\nInvalid command. Please try again.\n";
                continue;
            }

            if (processMove(move))
            {
                displayBoard();
                moves_++;
                std::cout << "\nMoves so far: " << moves_ << "\n";
            }
            else
            {
                std::cout << "\nThat move is blocked by the edge of the board.\n";
            }
        }

        std::cout << "\n\nPuzzle solved! Well done!\n";
    }

    

    void want_to_play()                // To ask whether user wants to play the game or not
    {
        while(!(user_choice=="Y" || user_choice=="y"|| user_choice=="1"||user_choice=="N"||user_choice=="n"||user_choice=="0")) 
        {
            std::cout<<"\"Invalid Input!\" Please re-enter your choice : ";
            std::cin>>user_choice;
        }


        if(user_choice=="Y"|| user_choice=="y"|| user_choice=="1")
        {
            std::cout<<"\nWelcome to \" THE NUMBER SLIDE GAME \" !\n\n";
            displayBoard();
            playGame();
        }
        
        else
        {
            std::cout<<"\nOkay! We may play later .... ";
            return ;
        }

        return;
    }


    bool processMove(std::string &move)           // Process the moves choosen by user by calling upadate matrix function of Matrix class
    {
        if(move == "A" || move == "a")
            return M.Update_Matrix(Direction::Left);
        else if(move == "D" || move == "d")
            return M.Update_Matrix(Direction::Right);
        else if(move == "W" || move == "w")
            return M.Update_Matrix(Direction::Up);
        else if(move == "S" || move == "s")
             return M.Update_Matrix(Direction::Down);

        return false;
    }
    
    bool Confirm_Quit()          // To take confirmation of quitting the game
    {
        std::string quit;
        std::cout<<"\nDo you really want to Quit ?  \t0.No\t1. Yes \n\n  > ";
        std::cin>>quit;
        
        while(!(quit == "0" || quit == "1"))
        {
            std::cout<<"\nInvalid Input! Please re-enter ... : ";
            std::cin>>quit;
        }
        if(quit != "0")   
        {
            std::cout<<"\n\nOkay ! I will miss you...";
            return 1;
        }
        else       return 0;
        
    }

};
