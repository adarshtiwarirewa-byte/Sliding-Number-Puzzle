#include<iostream>
#include"NumberSlideGame.hpp"

int main()
{
    std::string wish{};       // I am using this because when I used char and I fill more than one inputs it considers all inputs one by one .

    std::cout<<"\nThis is \" THE NUMBER SLIDE GAME \" !\n\n";
    std::cout<<"Do you want to play ?\n \"Enter -> Y/y/1 : Yes  and   N/n/0 : No\n\n>>";
    std::cin>>wish;
    std::cout<<"\n";

   
    Game G(wish);         
    G.want_to_play(); 


    
    return 0;
}