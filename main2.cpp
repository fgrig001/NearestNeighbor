#include "Menu.h"
#include "PuzzleState.h"
#include "PuzzleSolver.h"

#include <iostream>
using namespace std;


int main(int argc, char *argv[]){
   PUZZLE_SIZE = 3;
   // Object Declarations
   Menu menu;
   PuzzleSolver puzzleSolver;

   // Menu loop
   do{
      menu.GetUserInput();
      switch(menu.SearchAlgorithmSelected()){
         case Menu::UNIFORM_COST_SEARCH:
            puzzleSolver.RunUniformCostSearch(menu.GetPuzzle());
            break;
         case Menu::A_STAR_MANHATTAN_DISTANCE:
            puzzleSolver.RunManhattanDistance(menu.GetPuzzle());
            break;
         case Menu::A_STAR_MISPLACED_TILE:
            puzzleSolver.RunMisplacedTile(menu.GetPuzzle());
            break;
         default:
            break;
      };
      //TODO: add delay to go from solution to getting input
      menu.PromptForExit();
   }while(!menu.ExitSelected());
   return 0;
}
