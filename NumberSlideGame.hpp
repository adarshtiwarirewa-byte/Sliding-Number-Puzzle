#include<iostream>
#include<vector>
#include<time.h>
#include<numeric>
#include<stdlib.h>



class  Matrix                  // To manage matrix along the gameplay
{
    private:

    std::vector<std::vector<int>>Grid;
    unsigned int store[2];  //  It stores the indices of the empty space
        
    public:
    Matrix()  : Grid(4,std::vector<int>(4))    // Constructor
    { 
        generateMatrix();
        removeEmpty();
    }


    void generateMatrix()      // Generates a random matrix having numbers in range 1 to 16
    {
        unsigned int random_val;
        srand(time(NULL));

        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                random_val = 1 + rand() % 16;
                while(!checkDistinct(i,j,random_val))
                {
                    random_val = random_val = 1 + rand() % 16;
                }
                Grid[i][j] = random_val;

            }
        }
        return;
    }

    
    bool checkDistinct(int i,int j,int new_value)   // Checks that all values are distinct or not
    {
        for(int k=0;k<=i;k++)
        {
            for(int l=0;l<4;l++)
            {
                if(k==i && l==j)  return true;
                if(Grid[k][l]==new_value)  return false;
            }
        }
           
        return true;
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



    void displayMatrix()     //  To display matrix to user
    {
        // updateGrid();

        for(int i=0;i<4;i++)
        {
            std::cout<<"[  ";
            for(int j=0;j<4;j++)
            {
                if(Grid[i][j]==0)       
                {
                    std::cout<<"    "; 
                    store[0] = i;
                    store[1] = j;
                }   
                else if(Grid[i][j]>9)   std::cout<<Grid[i][j]<<"  ";
                else                   std::cout<<Grid[i][j]<<"   ";
            }
            std::cout<<"]";
            std::cout<<"\n\n";
        }

        return;
    } 
    
    bool won()                   // checks the matrix elements that it in correct order from 1 to 15
    {
        int count = 1;
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(Grid[i][j]!=count)  return false;
                count ++; 
            }
        }
        return true;
    }

    void Update_Matrix(std::string &move)               // It updates the matrix when called with given move
    {
         if(move=="A" || move=="a")
        
        {
            if(store[1]==3)   return;  
            std::swap(Grid[store[0]][store[1]],Grid[store[0]][store[1]+1]);
            displayMatrix();
        }
        
        if(move=="D" || move=="d")
        {
            if(store[1]==0)     return ;
            std::swap(Grid[store[0]][store[1]],Grid[store[0]][store[1]-1]);
            displayMatrix();
        }
        
        if(move=="W" || move=="w")
        {
            if(store[0]==3)      return;
            std::swap(Grid[store[0]][store[1]],Grid[store[0]+1][store[1]]);
            displayMatrix();
        }

        if(move=="S" || move=="s")
        {
            if(store[0]==0)      return;
            std::swap(Grid[store[0]][store[1]],Grid[store[0]-1][store[1]]);
            displayMatrix();
        }
    }

};


//.................................................................//

class Game                              // To manage the gameplay
{
    private:
    Matrix M;                             // Proper encapsulation of matrix and moves
    std::string user_choice{};
    public:
    Game(std::string c)             //Constructor :  Takes choice input from user
    {
        user_choice = c;
    }

    void playGame()                      // It manages whole gameplay by calling other functions 
    {
        bool quit;
        std::string move;
        std::cout<<"\nNavigation Commands : \nA/a for left\t\tD/d for right\nW/w for up\t\tS/s for down\nQ/q for quit\n\n>>";

        std::cin>>move;

        while(!(move=="A"||move=="a"||move=="D"||move=="d"||move=="W"||move=="w"||move=="S"||move=="s"||move=="Q"||move=="q"))
        {
            std::cout<<"Invalid Input! Please Re-enter the command :  ";
            std::cin>>move;
        }

        if(move=="Q" || move == "q")   quit = Confirm_Quit();      // It asks the user does he really want to Quit

        if(quit)      return; 
        // else M.displayMatrix();    // user chooses yes quit then...

        processMove(move);

        if(!M.won())      playGame();

        return;
        
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
            M.displayMatrix();
            playGame();
        }
        
        else
        {
            std::cout<<"Okay! We may play later .... ";
            return ;
        }

        return;
    }


    void processMove(std::string &move)           // Process the moves choosen by user by calling upadate matrix function of Matrix class
    {
        M.Update_Matrix(move);
       
        return;
    }
    
    bool Confirm_Quit()          // To take confirmation of quitting the game
    {
        std::string quit;
        std::cout<<"Do you really want to Quit ?  \t0.No\t1. Yes \n\n  > ";
        std::cin>>quit;
        
        while(!(quit == "0" || quit == "1"))
        {
            std::cout<<"Invalid Input! Please re-enter ... : ";
            std::cin>>quit;
        }
        if(quit != "0")   
        {
            std::cout<<"Okay ! I will miss you...";
            return 1;
        }
        else       return 0;
        
    }

};