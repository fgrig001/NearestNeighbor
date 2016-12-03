
#ifndef _NNSEARCH_H_
#define _NNSEARCH_H_

#include <vector>
#include "Instance.h"
#include <cmath>

class NNSearch{
   public:
      void RunForwardSelection(std::vector<Instance*> _instances){
         std::vector<int> v;
         /*
         for(int i=0; i < _instances.at(0)->GetNumFeatures(); ++i){
            std::cout<<"Using feature(s) {"<<i+1<<"} accuracy is ";
            v.at(0) = i;
            std::cout<<NearestNeighbor(_instances,v)<<std::endl;
         }
         */
         v.push_back(5);
         v.push_back(8);
         v.push_back(9);
         std::cout<<"Using feature(s) {6,9,10} accuracy is ";
         std::cout<<NearestNeighbor(_instances,v)<<std::endl;
      }
      void RunBackwardElimination(std::vector<Instance*> _instances){}
      void RunCustom(std::vector<Instance*> _instances){}
   private:
      float NearestNeighbor(std::vector<Instance*> _instances,std::vector<int> feature_indices);
      float GetDistance(Instance* a, Instance* b,std::vector<int> feature_indices);
};

// Returns acuracy as a %
float NNSearch::NearestNeighbor(std::vector<Instance*> _instances,std::vector<int> feature_indices){
   int true_count=0;
   int total_count=0;
   Instance *nearest_inst;
   float nearest_dist = -1;
   float temp_dist;
   for(auto i = _instances.begin(); i != _instances.end(); ++i){
      Instance *nearest_inst=NULL;
      float nearest_dist = -1;
      for(auto j = _instances.begin(); j != _instances.end(); ++j){
         if( *i != *j){
            temp_dist = GetDistance(*i,*j,feature_indices);
            if(temp_dist < nearest_dist || nearest_dist == -1){
               nearest_dist = temp_dist;
               nearest_inst = *j;
            }
         }
      }
      if((*i)->GetClass() == nearest_inst->GetClass()){
         true_count++; 
      }
      total_count++;
   }
   return 100*(((float)true_count) / total_count);
}

float NNSearch::GetDistance(Instance* a, Instance* b,std::vector<int> feature_indices){
   float diff;
   float sum=0;
   // Iterate through features
   for(int i=0;i<feature_indices.size();++i){
      diff = a->GetFeature(feature_indices.at(i)) - b->GetFeature(feature_indices.at(i));
      sum += diff * diff;
   }
   return sqrt(sum);
}

#endif
