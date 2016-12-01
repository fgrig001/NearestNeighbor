#ifndef _MENU_H
#define _MENU_H

#include <iostream>
#include <string>

class Menu{
   public:
      enum SearchAlgorithm{FORWARD_SELECTION=1,BACKWARD_ELIMINATION=2,CUSTOM=3};

      // Constructor 
      Menu() {}
      ~Menu();

      void GetUserInput();
      void PromptForExit();

      // Getters
      SearchAlgorithm SearchAlgorithmSelected(){ return searchType; }
      bool ExitSelected(){ return exit; }
      std::string FileSelected(){ return fileName; }

      // Helper Functions TODO: make private
      void GetFileInput();
      void GetSearchAlgorithmInput();

   private:
      // Variables
      SearchAlgorithm searchType{FORWARD_SELECTION};
      bool exit{0};
      std::string fileName;
};

Menu::~Menu(){
}

void Menu::GetUserInput(){
   GetFileInput();
   GetSearchAlgorithmInput();
}

void Menu::PromptForExit(){
   std::cout<<std::endl;
   std::cout<<"Type \"1\" to run again, or \"2\" to exit."<<std::endl; 
   int _temp;
   std::cin>>_temp;
   if(_temp == 2){ exit = 1; }
}


void Menu::GetFileInput(){
   std::cout<<"Type in the name of the file to test: "<<std::endl;
   std::cin>>fileName;
}

void Menu::GetSearchAlgorithmInput(){
   int tmp_int;
   do{
      std::cout<<std::endl;
      std::cout<<"Enter your choice of algorithm"<<std::endl; 
      std::cout<<"1. Forward Selection"<<std::endl; 
      std::cout<<"2. Backward Elimination"<<std::endl; 
      std::cout<<"3. Custom"<<std::endl; 
      std::cin>>tmp_int;
      searchType=(SearchAlgorithm)tmp_int;
      if(tmp_int<1 || tmp_int>3){
         std::cout<<"Selection Not Valid!"<<std::endl; 
      }
   }while(tmp_int<1 || tmp_int>3);
}

#endif
