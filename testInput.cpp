#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "Instance.h"

using namespace std;

int main(){

   vector<Instance*> instances; 
   Instance *tmp;

   float f;
   string ln;
   string s;
   ifstream inFile;
   inFile.open("test.txt");
   bool classFlag;
   while( getline(inFile,ln) ){
      classFlag = true;
      tmp = new Instance;
      istringstream ss(ln);
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
   
   for( auto i = instances.begin(); i != instances.end(); ++i){
      cout<<"Class: "<<(*i)->GetClass()<<endl;
      cout<<"Features: ";
      (*i)->PrintFeatures(); 
   }

   return 0;
}
