#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <vector>
#include <iostream>

class Instance{
   public:
      Instance(){}
      int GetClass(){ return classIndex; }
      void SetClass(int class_index){ classIndex = class_index; }
      float GetFeature(int index){ return features.at(index); }
      void SetFeature(int index, float value){ features.at(index) = value; }
      int GetNumFeatures(){ return numFeatures; }
      void AddFeature(float feature);
      void PrintFeatures();

   private:
      std::vector<float> features;
      int classIndex;
      int numFeatures{0};
};

void Instance::AddFeature(float feature){
   features.push_back(feature); 
   numFeatures++;
}

void Instance::PrintFeatures(){
   for( auto i = features.begin(); i != features.end();++i){
      std::cout<<*i<<" ";
   }
   std::cout<<std::endl;
}

#endif
