#ifndef _MENU_H
#define _MENU_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Instance.h"

class Menu{
   public:
      enum SearchAlgorithm{FORWARD_SELECTION=1,BACKWARD_ELIMINATION=2,CUSTOM=3};

      // Constructor 
      Menu() {}

      void GetUserInput();
      void PromptForExit();

      // Getters
      SearchAlgorithm SearchAlgorithmSelected(){ return searchType; }
      bool ExitSelected(){ return exit; }
      std::string FileSelected(){ return fileName; }
      std::vector<Instance*> &GetInstances(){ return instances; }

      // Helper Functions TODO: make private
      void GetFileInput();
      void GetSearchAlgorithmInput();
      void Print(){
         for( auto i = instances.begin(); i != instances.end(); ++i){
            (*i)->PrintFeatures();
         }
      }

   private:
      // Variables
      SearchAlgorithm searchType{FORWARD_SELECTION};
      bool exit{0};
      std::string fileName;
      std::vector<Instance*> instances; 
};

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
   // Local variables 
   Instance *tmp;
   std::string ln;
   std::string s;
   std::ifstream inFile;
   float f;
   bool classFlag;
   bool validInput = false;
   // Intro
   std::cout<<"\nWelcome to Frederick Grigsbys Feature Selection Algorithm\n";
   do{
      // Get file name
      std::cout<<"Type in the name of the file to test: ";
      std::cin>>fileName;

      // Open file and read in instances
      inFile.open(fileName.c_str());
      if(inFile.is_open()){
         while( getline(inFile,ln) ){
            classFlag = true;
            tmp = new Instance;
            std::istringstream ss(ln);
            while( ss>>s ){
               f = atof(s.c_str());
               if(classFlag == true){
                  tmp->SetClass((int)f);
                  classFlag = false;
               }else{ 
                  tmp->AddFeature(f);
               }
            }
            instances.push_back(tmp);
            tmp = NULL;
         }
         validInput = true;
      }else{
         std::cout<<"Error opening file!\n";
      }
   }while(validInput == false);
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
