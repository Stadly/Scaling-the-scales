#include "main.h"

// How to use the recommender system is explained in the comments in this file

int main(int argc, char** argv) {
   srand(time(NULL));
   
//   Set a dataset
   Parser const &parser = DsIra();
//   Parser const &parser = DsScaling();
//   Parser const &parser = MovieLens100k();
//   Parser const &parser = HetRec2011MovieLens();
   
//   Parse the dataset
   Dataset const dataset = parser.parse();
   
//   Generate a file containing ratings to predict
   dataset.createPredictionFile("predictions.txt", 1000);
   
//   Create a RecommenderSystem, that can calculate the same predictions using different algorithms
   RecommenderSystem rs(dataset);
   
//   Add algorithms to the RecommenderSystem
   rs.addFilter(new Ira(dataset, 3, 1, 0.5, 5));
   rs.addFilter(new Scaling(dataset, 3, 1, 0.5, 5));
   rs.addFilter(new Firefly(dataset, 0.8));
   rs.addFilter(new LikeMinds(dataset, 0.8));
   
//   Initialize and calculate predictions
   rs.init();
   rs.predictFile("predictions.txt");
   
//   Ask the user for which user-item-pairs to calculate predictions
   rs.predictManual();
   
//   Create separate recommendation algorithm instances
   Scaling scaling(dataset, 3, 0.4, 1, 5);
   Ira ira(dataset, 3, 0.4, 1, 5);
   Firefly firefly(dataset, 0.6);
   LikeMinds likeminds(dataset, 0.6);
   
//   Initialize
   scaling.init();
   ira.init();
   firefly.init();
   likeminds.init();
   
//   Calculate prediction relations, based on the relations calculated by the recommendation algorithm
   scaling.relate(4, 1, 0.8);
   ira.relate(4, 1, 0.8);
   firefly.relate(1);
   likeminds.relate(1);
   
//   Manually calculate predictions
   scaling.predict(1, 2);
   ira.predict(1, 2);
   firefly.predict(1, 2);
   likeminds.predict(1, 2);
   
//   Calculate prediction relations using combinations of parameters, based on the relations calculated by the recommendation algorithm
   ira.relateRanges(3, 8, 1, 0.4, 1, 0.2, 0.2, 1, 0.2);
   scaling.relateRanges(3, 8, 1, 0.4, 1, 0.2, 0.2, 1, 0.2);
   firefly.relateRanges(0.6, 1, 0.1);
   likeminds.relateRanges(0.6, 1, 0.1);
   
//   Calculate prediction using combinations of parameters
   ira.predictRanges("predictions.txt", 3, 8, 1, 0.4, 1, 0.2, 0.2, 1, 0.2);
   scaling.predictRanges("predictions.txt", 3, 8, 1, 0.4, 1, 0.2, 0.2, 1, 0.2);
   firefly.predictRanges("predictions.txt", 0.6, 1, 0.1);
   likeminds.predictRanges("predictions.txt", 0.6, 1, 0.1);
   
   return 0;
} // main
