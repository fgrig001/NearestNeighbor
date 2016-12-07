
#ifndef _NNSEARCH_H_
#define _NNSEARCH_H_

#include <vector>
#include <queue>
#include "Instance.h"
#include <cmath>
#include <algorithm>

struct Node{
   std::vector<int> featureIndices;
   float percentage;
   Node &operator=(const Node &other){
      featureIndices = other.featureIndices;
      percentage = other.percentage;
   }
};

class NNSearch{
   public:
      void RunForwardSelection(std::vector<Instance*> &_instances);
      void RunBackwardElimination(std::vector<Instance*> &_instances);
      void RunCustom(std::vector<Instance*> &_instances);
   private:
      /* Helper Functions */
      void RunForwardSelection(std::vector<Instance*> &_instances,
                               std::vector<int> &feature_indices);
      void RunBackwardElimination(std::vector<Instance*> &_instances,
                               std::vector<int> &feature_indices);
      void RunCustom(std::vector<Instance*> &_instances,
                               std::vector<int> &feature_indices);

      float NearestNeighbor(std::vector<Instance*> &_instances,
                            std::vector<int> &feature_indices);
      float GetDistance(Instance* a, Instance* b,std::vector<int> feature_indices);
      void PrintV(std::vector<int> &v);

      void NormalizeData(std::vector<Instance*> &_instances);

      /* Variables */
      float best_percentage{0.f};
      float prev_best_percentage{0.f};
      std::vector<int> best_feature_indices;
      int num_features{0};
      int num_instances{0};

      struct CompareNode{
         bool operator()(const Node lhs,const Node rhs) {
            return lhs.percentage < rhs.percentage;
         }
      };
      std::priority_queue<Node, std::vector<Node>, CompareNode> pQueue;

		bool IsEqual(Node &a,Node &b){
			bool foundFlag = false;
			if(a.percentage != b.percentage) return false;
			if(a.featureIndices.size() != b.featureIndices.size()) return false;
			for(int i=0;i<a.featureIndices.size();++i){
				for(int j=0;j<a.featureIndices.size();++j){
					if(a.featureIndices.at(i)==b.featureIndices.at(j)){
						break;
					}else if(j==a.featureIndices.size()-1){
						return false;
					}
				}
			}
		}
};


void NNSearch::RunForwardSelection(std::vector<Instance*> &_instances){
   // Local variables 
   std::vector<int> feature_indices;
   best_percentage = 0.f;
   prev_best_percentage = 0.f;
   num_features = _instances.at(0)->GetNumFeatures();
   num_instances = _instances.size();
   // Print dataset info
   std::cout<<"This dataset has "<<num_features
            <<" features (not including the class attribute), with "
            <<num_instances<<" instances\n\n";
   // Normalize data
   std::cout<<"Please wait while I normalize the data... ";
   NormalizeData(_instances);
   std::cout<<"Done!\n\n";
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

void NNSearch::RunBackwardElimination(std::vector<Instance*> &_instances){
   // Local variables 
   std::vector<int> feature_indices;
   best_percentage = 0.f;
   prev_best_percentage = 0.f;
   num_features = _instances.at(0)->GetNumFeatures();
   num_instances = _instances.size();
   for(int i=0;i<num_features;++i){
      feature_indices.push_back(i);
   }
   // Print dataset info
   std::cout<<"This dataset has "<<num_features
            <<" features (not including the class attribute), with "
            <<num_instances<<" instances\n\n";
   // Normalize data
   std::cout<<"Please wait while I normalize the data... ";
   NormalizeData(_instances);
   std::cout<<"Done!\n\n";
   // First Run 
   best_percentage = NearestNeighbor(_instances,feature_indices);
   best_feature_indices = feature_indices;
   // Print results
   std::cout<<"\nBeginning search.\n";
   std::cout<<"Using feature(s) ";
   PrintV(feature_indices);
   std::cout<<" accuracy is "<<100*best_percentage<<"%\n\n";
   // Run Recursively 
   RunBackwardElimination(_instances,feature_indices);
   // Output final results
   std::cout<<"\nFinished search!! The best feature subset is ";
   PrintV(best_feature_indices);
   std::cout<<", which has an accuracy of "<<100*best_percentage<<"%\n";
   // Flush global variables for next run
   best_feature_indices.clear();
}

void NNSearch::RunBackwardElimination(std::vector<Instance*> &_instances,
                         std::vector<int> &feature_indices)
{
   float local_best_percentage = 0.f;
   std::vector<int> local_best_feature_indices;
   std::vector<int> temp_feature_indices;
   for(int i=num_features-1; i >= 0; --i){
      temp_feature_indices = feature_indices;
      // If value exists remove it 
      auto val = std::find(temp_feature_indices.begin(),temp_feature_indices.end(),i);
      if(val != temp_feature_indices.end()){
         temp_feature_indices.erase(val);
      }else{ continue; }
      // Run NN 
      float accuracy  = NearestNeighbor(_instances,temp_feature_indices);
      // Print results
      std::cout<<"Using feature(s) ";
      PrintV(temp_feature_indices);
      std::cout<<" accuracy is "<<100*accuracy<<"%"<<std::endl;
      // Check for highest local accuracy
      if(accuracy > local_best_percentage){
         local_best_percentage = accuracy;
         local_best_feature_indices = temp_feature_indices;
      }
   }
   // Print best local feature set
   if(temp_feature_indices.size() != 1){
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
   // End recursion condition
   if(local_best_feature_indices.size() == 1){
      return;
   }
   // Recurse 
   RunBackwardElimination(_instances,local_best_feature_indices);
}


void NNSearch::RunCustom(std::vector<Instance*> &_instances){
   num_features = _instances.at(0)->GetNumFeatures();
   num_instances = _instances.size();
	std::vector<int> features;
	std::vector<int> best_features;
	std::vector<int> second_best_features;
	float percentage;
	float best_percentage=0;
	float second_best_percentage=0;
	features.push_back(0);
	features.push_back(0);
   for(int i=0;i<num_features-1;++i){
   	for(int j=i+1;j<num_features;++j){
			features.at(0)=i;
			features.at(1)=j;
			percentage = NearestNeighbor(_instances,features);
			std::cout<<"Using feature(s) ";
			PrintV(features);
			std::cout<<" accuracy is "<<100*percentage<<"%"<<std::endl;
			if(percentage > best_percentage){
				best_percentage=percentage;
				best_features = features;
			}else if(percentage > second_best_percentage){
				second_best_percentage=percentage;
				second_best_features = features;
			}
		}
	}
	std::cout<<std::endl;
	std::cout<<"Feature set ";
	PrintV(best_features);
	std::cout<<" was best, accuracy is "<<100*best_percentage<<"%"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"Feature set ";
	PrintV(second_best_features);
	std::cout<<" was second best, accuracy is "<<100*second_best_percentage<<"%"<<std::endl;
	std::cout<<std::endl;

	// Combine first and second best features
	features.pop_back();
	features.pop_back();
	features.push_back(best_features.at(0));
	features.push_back(best_features.at(1));
	if(second_best_features.at(0) != best_features.at(0)
	  && second_best_features.at(0) != best_features.at(1))
	{
		features.push_back(second_best_features.at(0));
	}
	if(second_best_features.at(1) != best_features.at(0)
	  && second_best_features.at(1) != best_features.at(1))
	{
		features.push_back(second_best_features.at(1));
	}
	percentage = NearestNeighbor(_instances,features);
	std::cout<<std::endl;
	std::cout<<"Combined feature set ";
	PrintV(features);
	std::cout<<" accuracy is "<<100*percentage<<"%"<<std::endl;
	std::cout<<std::endl;

	/*
   num_features = _instances.at(0)->GetNumFeatures();
   num_instances = _instances.size();
   Node node;
   Node new_node;
   Node best_node;
   // Set up 
   for(int i=0;i<num_features-1;++i){
      new_node = node;
      new_node.featureIndices.push_back(i);
      new_node.featureIndices.push_back(i+1);
      new_node.percentage = NearestNeighbor(_instances,new_node.featureIndices);
      pQueue.push(new_node);
   }
   while(!pQueue.empty()){
		// Grab first node in queueu
      node = pQueue.top();
      pQueue.pop(); 
		new_node = pQueue.top();
		// Remove duplicates
		while(NNSearch::IsEqual(new_node,node)){
			pQueue.pop(); 
			new_node = pQueue.top();
		}
      std::cout<<"Using feature(s) ";
      PrintV(node.featureIndices);
      std::cout<<" accuracy is "<<100*node.percentage<<"%\n";
      // Check if top node is best node
      if(node.percentage > best_node.percentage){
         best_node = node;
      }
      // Expand node
      for(int i = 0;i<num_features;++i){
         new_node = node;
         // If node does not contain index then add it
         if(std::find(node.featureIndices.begin(),node.featureIndices.end(),i) 
            == node.featureIndices.end())
         {
            new_node.featureIndices.push_back(i);
         }else{ continue; }
         // calculate NN for new node
         new_node.percentage = NearestNeighbor(_instances,new_node.featureIndices);
         // add new node only if it is better then old node
         if(new_node.percentage > node.percentage){
            pQueue.push(new_node);
         }
      }
   }
   std::cout<<"Using feature(s) ";
   PrintV(best_node.featureIndices);
   std::cout<<" accuracy is "<<100*best_node.percentage<<"%\n";
	*/
}

void NNSearch::RunCustom(std::vector<Instance*> &_instances,
               std::vector<int> &feature_indices){

}

// Returns acuracy as a fraction
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

void NNSearch::NormalizeData(std::vector<Instance*> &_instances){
   for(int j=0;j<_instances.at(0)->GetNumFeatures();++j){
      // Find mean
      int numInstances = _instances.size();
      double mean=0;
      for( auto i = _instances.begin(); i != _instances.end(); ++i){
         mean += (*i)->GetFeature(j) / numInstances;
      }

      // Find standard dev
      double std_dev=0;
      double std_mean=0;
      for( auto i = _instances.begin(); i != _instances.end(); ++i){
         std_mean += pow(mean - (*i)->GetFeature(j),2) / numInstances;
      }
      std_dev = sqrt(std_mean); 

      // Normalize data
      for( auto i = _instances.begin(); i != _instances.end(); ++i){
         (*i)->SetFeature(j,((*i)->GetFeature(j) - mean)/std_dev);
      }
   }
}

#endif
