#include "main.h"

int main(int argc, char** argv) {
   
//   Set parameters for the predictions and relations to be analyzed
   std::string const dataset = "hetrec-2011-ml";
   int const scaleSize = 10;
   int const extremalThreshold = 2;
   
//   std::string const dataset = "ira";
//   int const scaleSize = 13;
//   int const extremalThreshold = 3;
   
//   std::string const dataset = "scaling";
//   int const scaleSize = 13;
//   int const extremalThreshold = 3;
   
//   std::string const dataset = "ml-100k";
//   int const scaleSize = 5;
//   int const extremalThreshold = 1;
   
   std::string const path = "../RecommenderSystem/";
   Predictions predictions(dataset, scaleSize, extremalThreshold, path);
   Relations relations(dataset, path);
   
//   Add files that should be analyzed
   predictions.addFile("ira-scaling-4-1-0.2.txt");
   relations.addFile("ira-scaling-4-1-0.2.txt");
   predictions.addFile("ira-ira-median-sort-4-1-0.2.txt");
   relations.addFile("ira-ira-median-sort-4-1-0.2.txt");
   predictions.addFile("ira-scaling-7-1-0.2.txt");
   relations.addFile("ira-scaling-7-1-0.2.txt");
   predictions.addFile("ira-ira-median-sort-7-1-0.2.txt");
   relations.addFile("ira-ira-median-sort-7-1-0.2.txt");
   
//   predictions.addFile("scaling-scaling-6-1-0.2.txt");
//   relations.addFile("scaling-scaling-6-1-0.2.txt");
//   predictions.addFile("scaling-ira-median-sort-6-1-0.2.txt");
//   relations.addFile("scaling-ira-median-sort-6-1-0.2.txt");
//   predictions.addFile("scaling-scaling-8-1-0.6.txt");
//   relations.addFile("scaling-scaling-8-1-0.6.txt");
//   predictions.addFile("scaling-ira-median-sort-8-1-0.6.txt");
//   relations.addFile("scaling-ira-median-sort-8-1-0.6.txt");
   
//   predictions.addFile("ml-100k-scaling-4-1-0.8.txt");
//   relations.addFile("ml-100k-scaling-4-1-0.8.txt");
//   predictions.addFile("ml-100k-ira-median-sort-4-1-0.8.txt");
//   relations.addFile("ml-100k-ira-median-sort-4-1-0.8.txt");
//   predictions.addFile("ml-100k-scaling-11-1-0.4.txt");
//   relations.addFile("ml-100k-scaling-11-1-0.4.txt");
//   predictions.addFile("ml-100k-ira-median-sort-11-1-0.4.txt");
//   relations.addFile("ml-100k-ira-median-sort-11-1-0.4.txt");
   
//   predictions.addFile("hetrec-2011-ml-scaling-4-1-1.4.txt");
//   relations.addFile("hetrec-2011-ml-scaling-4-1-1.4.txt");
//   predictions.addFile("hetrec-2011-ml-ira-median-sort-4-1-1.4.txt");
//   relations.addFile("hetrec-2011-ml-ira-median-sort-4-1-1.4.txt");
//   predictions.addFile("hetrec-2011-ml-scaling-3-1-0.5.txt");
//   relations.addFile("hetrec-2011-ml-scaling-3-1-0.5.txt");
//   predictions.addFile("hetrec-2011-ml-ira-median-sort-3-1-0.5.txt");
//   relations.addFile("hetrec-2011-ml-ira-median-sort-3-1-0.5.txt");
   
//   Add ranges of files to be analyzed
//   if(0 == predictions.fileCount() && 0 == relations.fileCount()) {
////      std::string filter = "-scaling-";
//      std::string filter = "-ira-median-sort-";
//      std::ifstream file;
////      for(int i = 3; i <= 15; ++i)
////      for(int i = 16; i <= 27; ++i)
////         for(double j = 0.4; j <= 1.0; j += 0.2)
////            for(double k = 0.2; k <= 1.0; k += 0.2) {
////      for(int i = 3; i <= 58; i += 1)
////         for(double j = 0.1; j <= 1.0; j += 0.1)
////            for(double k = 0.2; k <= 1.8; k += 0.4) {
//      for(int i = 3; i <= 8; i += 1)
//         for(double j = 0.2; j <= 1.0; j += 0.2)
//            for(double k = 0.2; k <= 1.0; k += 0.2) {
//               std::string const filename = dataset+filter+patch::to_string(i)+"-"+patch::to_string(j)+"-"+patch::to_string(k)+".txt";
//               file.open(path+"relations/"+dataset+"/"+filename);
//               if(file.is_open()) {
//                  relations.addFile(filename);
//                  file.close();
//               }
//               file.open(path+"predictions/"+dataset+"/"+filename);
//               if(file.is_open()) {
//                  predictions.addFile(filename);
//                  file.close();
//               }
//            } // end for
//   } // end if

   predictions.analyze();
   
//   Filter out files giving bad results
   bool const useFilter = false;
   int const filterSuccessRate = 10;
   double const filterTotalError = 1.255;
   double const filterTotalErrorExtremal = 1.255;
   std::vector<int> filtered;
   if(useFilter)
      filtered = predictions.useFilter(filterSuccessRate, filterTotalError, filterTotalErrorExtremal);
   else
      for(int i = 0; i < predictions.fileCount(); ++i)
         filtered.push_back(i);
   
//   Print relationships between real and predicted ratings
   predictions.printTables(filtered);
//   Print statistics when considering all prediction attempts
   predictions.printTotal(filtered);
//   Print statistics when only considering predictions successfully calculated by all recommender systems
   predictions.printOk(filtered);
//   Print info about the dataset
   predictions.printDataset();
   
   relations.analyze();
//   Print statistics for the relations
   relations.print();
   
   return 0;
} // main
