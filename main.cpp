
#include <iostream>
#include "Menu.h"
#include "NNSearch.h"

int main(){
   Menu menu;
   NNSearch nnSearch;

   do{
      menu.GetUserInput();
      switch(menu.SearchAlgorithmSelected()){
         case Menu::FORWARD_SELECTION:
            nnSearch.RunForwardSelection(menu.GetInstances());
            //menu.Print();
            break;
         case Menu::BACKWARD_ELIMINATION:
            nnSearch.RunBackwardElimination(menu.GetInstances());
            break;
         case Menu::CUSTOM:
            nnSearch.RunCustom(menu.GetInstances());
            break;
         default:
            break;
      };
      //TODO: add delay to go from solution to getting input
      menu.PromptForExit();
   }while(!menu.ExitSelected());

   return 0;
}
