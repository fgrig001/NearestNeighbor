
#ifndef _NNSEARCH_H_
#define _NNSEARCH_H_

#include <vector>
#include "Instance.h"
#include <cmath>
#include <algorithm>

class NNSearch{
   public:
      void RunForwardSelection(std::vector<Instance*> &_instances);
      void RunBackwardElimination(std::vector<Instance*> &_instances){}
      void RunCustom(std::vector<Instance*> &_instances){}
   private:
      /* Helper Functions */
      void RunForwardSelection(std::vector<Instance*> &_instances,
                               std::vector<int> &feature_indices);
      float NearestNeighbor(std::vector<Instance*> &_instances,
                            std::vector<int> &feature_indices);
      float GetDistance(Instance* a, Instance* b,std::vector<int> feature_indices);
      void PrintV(std::vector<int> &v);
      /* Variables */
      float best_percentage{0.f};
      float prev_best_percentage{0.f};
      std::vector<int> best_feature_indices;
      int num_features{0};
      int num_instances{0};
};

void NNSearch::RunForwardSelection(std::vector<Instance*> &_instances){
   /* Set up and start recursion for Forward Selection */
   std::vector<int> feature_indices;
   best_percentage = 0.f;
   prev_best_percentage = 0.f;
   num_features = _instances.at(0)->GetNumFeatures();
   num_instances = _instances.size();
   // Run Recursively 
   RunForwardSelection(_instances,feature_indices);
   // Output final results
   std::cout<<"\nFinished search!! The best feature subset is ";
   PrintV(best_feature_indices);
   std::cout<<", which has an accuracy of "<<100*best_percentage<<"%\n";
   // Flush global variables for next run
   best_feature_indices.clear();
}

void NNSearch::RunForwardSelection(std::vector<Instance*> &_instances,
                         std::vector<int> &feature_indices)
{
   // End recursion condition
   if(feature_indices.size() == num_features){
      return;
   }
   float local_best_percentage = 0.f;
   std::vector<int> local_best_feature_indices;
   for(int i=0; i < num_features; ++i){
      // Check if number is alread in list
      if(std::find(feature_indices.begin(),feature_indices.end(),i) 
         == feature_indices.end())
      {
         feature_indices.push_back(i);
      }else{ continue; }
      // Run NN 
      float accuracy  = NearestNeighbor(_instances,feature_indices);
      // Print results
      std::cout<<"Using feature(s) ";
      PrintV(feature_indices);
      std::cout<<" accuracy is "<<100*accuracy<<"%"<<std::endl;
      // Check for highest local accuracy
      if(accuracy > local_best_percentage){
         local_best_percentage = accuracy;
         local_best_feature_indices = feature_indices;
      }
      // Remove added feature
      feature_indices.pop_back();
   }
   // Print best local feature set
   if(feature_indices.size() != num_features-1){
      std::cout<<std::endl;
      if(local_best_percentage < prev_best_percentage){
         std::cout<<"(Warning, Accuracy has decreased! Continuing "
                  <<"search in case of local maxima\n"; 
      }
      prev_best_percentage = local_best_percentage;
      std::cout<<"Feature set ";
      PrintV(local_best_feature_indices);
      std::cout<<" was best, accuracy is "<<100*local_best_percentage<<"%"<<std::endl;
      std::cout<<std::endl;
   }
   // Check for highest global accuracy
   if(local_best_percentage > best_percentage){
      best_percentage = local_best_percentage;
      best_feature_indices = local_best_feature_indices;
   }
   // Recurse 
   RunForwardSelection(_instances,local_best_feature_indices);
}

// Returns acuracy as a %
float NNSearch::NearestNeighbor(std::vector<Instance*> &_instances,
                                std::vector<int> &feature_indices)
{
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
   return ((float)true_count) / total_count;
}

float NNSearch::GetDistance(Instance* a, 
                            Instance* b,
                            std::vector<int> feature_indices)
{
   float diff;
   float sum=0;
   // Iterate through features
   for(int i=0;i<feature_indices.size();++i){
      diff = a->GetFeature(feature_indices.at(i)) - b->GetFeature(feature_indices.at(i));
      sum += diff * diff;
   }
   return sqrt(sum);
}

void NNSearch::PrintV(std::vector<int> &v){
   std::cout<<"{";
   for(auto i = v.begin();i != v.end(); i++){
      if(i != v.begin()){
         std::cout<<",";
      }
      std::cout<<(*i)+1;
   }
   std::cout<<"}";
}

#endif
