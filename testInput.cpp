#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

using namespace std;

int main(){
   float f;
   string ln;
   string s;
   ifstream inFile;
   inFile.open("test.txt");
   while( getline(inFile,ln) ){
      istringstream ss(ln);
      while( ss>>s ){
         f = atof(s.c_str());
         cout<<f<<"  ";
      }
      cout<<endl;
   }
   return 0;
}
