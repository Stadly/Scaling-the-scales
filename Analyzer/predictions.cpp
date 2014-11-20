#include "predictions.h"

void Predictions::addFile(std::string const filename) {
   m_filenames.push_back(filename);
   
   m_files.push_back(new std::ifstream(m_path+"predictions/"+m_dataset+"/"+filename));
   m_fails.push_back(0);
   m_failsExtremal.push_back(0);
   m_totalDiff.push_back(0);
   m_totalDiffExtremal.push_back(0);
   m_totalPredictors.push_back(0);
   m_totalPathCount.push_back(0);
   m_totalPathLength.push_back(0);
   m_totalArcCount.push_back(0);
   m_totalDistinctArcs.push_back(0);
   m_totalAvgCommonRatings.push_back(0);
   m_totalAvgErrorFunc.push_back(0);
   m_totalCount.push_back(0);
   m_totalCountExtremal.push_back(0);
   m_okDiff.push_back(0);
   m_okDiffExtremal.push_back(0);
   m_okPredictors.push_back(0);
   m_okPathCount.push_back(0);
   m_okPathLength.push_back(0);
   m_okArcCount.push_back(0);
   m_okDistinctArcs.push_back(0);
   m_okAvgCommonRatings.push_back(0);
   m_okAvgErrorFunc.push_back(0);
   m_table.push_back(std::vector<std::vector<int>>(m_scaleSize, std::vector<int>(m_scaleSize, 0)));
} // addFile

bool Predictions::isExtremal(int const rating) const {
//   return rating <= extremalThreshold;
//   return scaleSize-extremalThreshold < rating;
   return rating <= m_extremalThreshold || m_scaleSize-m_extremalThreshold < rating;
} // isExtremal

std::vector<int> Predictions::useFilter(const int filterSuccessRate, const double filterTotalError, const double filterTotalErrorExtremal) const {
   std::vector<int> filtered;
   for(int i = 0; i < m_filenames.size(); ++i) {
      m_files.at(i)->close();
      delete m_files.at(i);
      if(m_totalCount[i] > 0 && m_totalCountExtremal[i] > 0 && 100-100*(double)m_fails[i]/m_totalCount[i] > filterSuccessRate && ((double)m_totalDiff[i]/m_totalCount[i] <= filterTotalError || (double)m_totalDiffExtremal[i]/m_totalCountExtremal[i] <= filterTotalErrorExtremal)) {
         filtered.push_back(i);
         std::cout << "   predictions.addFile(\""+m_filenames[i]+"\");" << std::endl;
         std::cout << "   relations.addFile(\""+m_filenames[i]+"\");" << std::endl;
      } // end if
   } // end for
   return filtered;
} // useFilter

void Predictions::analyze() {
   int fileCount = m_files.size();
   
   double diff[fileCount];
   double prediction[fileCount];
   int predictors[fileCount];
   int pathCount[fileCount];
   int pathLength[fileCount];
   int arcCount[fileCount];
   double avgCommonRatings[fileCount];
   double avgErrorFunc[fileCount];
   
   while(true) {
      for(int i = 0; i < m_filenames.size(); ++i)
         if(!*m_files[i])
            return;
      bool failed = false;
      int rating = 0;
      for(int i = 0; i < m_filenames.size(); ++i) {
         std::string s;
         std::getline(*m_files[i], s, '\t'); // user
         if(!s.empty()) {
            std::getline(*m_files[i], s, '\t'); // item
            std::getline(*m_files[i], s, '\t'); // rating
            rating = atoi(s.c_str());
            std::getline(*m_files[i], s, '\t'); // prediction
            prediction[i] = std::min(std::max(1.0, atof(s.c_str())), (double)m_scaleSize);
            std::getline(*m_files[i], s, '\t'); // difference
            diff[i] = atof(s.c_str());
            std::getline(*m_files[i], s, '\t'); // predictors
            predictors[i] = atoi(s.c_str());
            std::getline(*m_files[i], s, '\t'); // path count
            pathCount[i] = atoi(s.c_str());
            std::getline(*m_files[i], s, '\t'); // path length
            pathLength[i] = atoi(s.c_str());
            std::getline(*m_files[i], s, '\t'); // arc count
            arcCount[i] = atoi(s.c_str());
            std::getline(*m_files[i], s, '\t'); // avg common ratings
            avgCommonRatings[i] = atof(s.c_str());
            std::getline(*m_files[i], s); // avg error func
            avgErrorFunc[i] = atof(s.c_str());

            if(diff[i] < 0) {
               ++m_fails[i];
               if(isExtremal(rating))
                  ++m_failsExtremal[i];
               failed = true;
            } else {
               ++m_totalCount[i];
               if(isExtremal(rating))
                  ++m_totalCountExtremal[i];
               m_totalDiff[i] += std::abs(rating-prediction[i]);
               if(isExtremal(rating))
                  m_totalDiffExtremal[i] += std::abs(rating-prediction[i]);
               m_totalPredictors[i] += predictors[i];
               m_totalPathCount[i] += pathCount[i];
               m_totalPathLength[i] += pathLength[i];
               m_totalArcCount[i] += arcCount[i];
               m_totalDistinctArcs[i] += (double)arcCount[i]/(pathCount[i]*pathLength[i]);
               m_totalAvgCommonRatings[i] += avgCommonRatings[i];
               m_totalAvgErrorFunc[i] += avgErrorFunc[i];
            } // end if
         } else {
            failed = true;
         } // end if
      } // end for
      
      ++m_count;
      if(isExtremal(rating))
         ++m_countExtremal;
         
      if(!failed) {
         ++m_okCount;
         if(isExtremal(rating))
            ++m_okCountExtremal;
         for(int i = 0; i < m_filenames.size(); ++i) {
            m_table[i][rating-1][(int)std::round(prediction[i]-1)]++;
            m_okDiff[i] += std::abs(rating-prediction[i]);
            if(isExtremal(rating))
               m_okDiffExtremal[i] += std::abs(rating-prediction[i]);
            m_okPredictors[i] += predictors[i];
            m_okPathCount[i] += pathCount[i];
            m_okPathLength[i] += pathLength[i];
            m_okArcCount[i] += arcCount[i];
            m_okDistinctArcs[i] += (double)arcCount[i]/(pathCount[i]*pathLength[i]);
            m_okAvgCommonRatings[i] += avgCommonRatings[i];
            m_okAvgErrorFunc[i] += avgErrorFunc[i];
         } // end for
      } // end if
   } // end while
} // analyzePredictions

void Predictions::printTables(std::vector<int> filtered) const {
   for(int const i : filtered) {
      std::cout << "\\begin{tabu}{>{\\raggedleft\\arraybackslash}p{0.4cm}|";
      for(int i = 0; i < m_scaleSize; ++i)
         std::cout << "X[c]";
      std::cout << "}\n\\multicolumn{"<<m_scaleSize+1<<"}{c}{\\textbf{" << m_filenames[i] << "}} \\tabularnewline\n";
      for(int j = 0; j < m_scaleSize; ++j)
         std::cout << " & \\num{" << j+1 << "}";
      std::cout << " \\tabularnewline \\hline\n & & & & \\\\[-2ex]\n";
      for(int j = 0; j < m_scaleSize; ++j) {
         int total = 0;
         for(int k = 0; k < m_scaleSize; ++k)
            total += m_table[i][j][k];
         std::cout << "\\num{" << j+1 << "}";
         for(int k = 0; k < m_scaleSize; ++k)
            std::cout << " & \\num{" << (0 == total ? 0 : (int)round(100*(double)m_table[i][j][k]/total)) << "}";
         std::cout << " \\tabularnewline\n";
      } // end for
      std::cout << "\\end{tabu}\n\n";
   } // end for
} // printTables

void Predictions::printTotal(std::vector<int> filtered) const {
   std::cout << std::fixed << std::setprecision(2);
   
   std::cout << "\\begin{tabu}{p{4cm}";
   for(int const i : filtered)
      std::cout << "X[c]";
   std::cout << "}\n";
   
   for(int const i : filtered)
      std::cout << " & " << m_filenames[i];
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Success rate";
   for(int const i : filtered)
      std::cout << " & \\SI{" << (int)round(100-100*(double)m_fails[i]/m_count) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Success rate extremal";
   for(int const i : filtered)
      std::cout << " & \\SI{" << (int)round(100-100*(double)m_failsExtremal[i]/m_countExtremal) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Error";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalDiff[i]/m_totalCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Error extremal";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalDiffExtremal[i]/m_totalCountExtremal[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Prediction sources";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalPredictors[i]/m_totalCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Paths";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalPathCount[i]/m_totalCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Path length";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalPathLength[i]/m_totalCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Distinct edges";
   for(int const i : filtered)
      std::cout << " & \\SI{" << (int)round(100*(double)m_totalDistinctArcs[i]/m_totalCount[i]) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Common ratings";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalAvgCommonRatings[i]/m_totalCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Trans func error";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_totalAvgErrorFunc[i]/m_totalCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   std::cout << "\\end{tabu}\n\n";
} // printTotal

void Predictions::printOk(std::vector<int> filtered) const {
   std::cout << std::fixed << std::setprecision(2);
   
   std::cout << "\\begin{tabu}{p{4cm}";
   for(int const i : filtered)
      std::cout << "X[c]";
   std::cout << "}\n";
   
   for(int const i : filtered)
      std::cout << " & " << m_filenames[i];
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Error";
   for(int const i : filtered)
      std::cout << " & \\SI{" << (int)round(100-100*(double)m_okDiff[i]/m_okCount) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Error extremal";
   for(int const i : filtered)
      std::cout << " & \\SI{" << (int)round(100-100*(double)m_okDiffExtremal[i]/m_okCountExtremal) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Prediction sources";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_okPredictors[i]/m_okCount << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Paths";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_okPathCount[i]/m_okCount << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Path length";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_okPathLength[i]/m_okCount << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Distinct edges";
   for(int const i : filtered)
      std::cout << " & \\num{" << (int)round(100*(double)m_okDistinctArcs[i]/m_okCount) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Common ratings";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_okAvgCommonRatings[i]/m_okCount << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Ok Trans func error";
   for(int const i : filtered)
      std::cout << " & \\num{" << (double)m_okAvgErrorFunc[i]/m_okCount << "}";
   std::cout << " \\tabularnewline\n";
   std::cout << "\\end{tabu}\n\n";
} // printOk

void Predictions::printDataset() const {
   std::cout << std::fixed << std::setprecision(2);
   
   std::ifstream info;
   info.open(m_path+"dataset/"+m_dataset+"/u.info");
   std::cout << "\\begin{tabu}{rl}\n";
   std::cout << "Predictions & \\num{" << m_count << "} \\tabularnewline\n";
   std::cout << "Predictions extremal & \\num{" << m_countExtremal << "} \\tabularnewline\n";
   std::cout << "Ok Predictions & \\num{" << m_okCount << "} \\tabularnewline\n";
   std::cout << "Ok Predictions extremal & \\num{" << m_okCountExtremal << "} \\tabularnewline\n";
   std::cout << "Extremal values & \\numlist{" << 1;
   for(int i = 2; i <= m_extremalThreshold; ++i)
      std::cout << ";" << i;
   for(int i = m_scaleSize-m_extremalThreshold+1; i <= m_scaleSize; ++i)
      std::cout << ";" << i;
   std::cout << "} \\tabularnewline\n";
   while(info) {
      std::string s;
      std::getline(info, s, ':');
      if(!s.empty()) {
         std::cout << s;
         std::getline(info, s);
         std::cout << " &" << s << " \\tabularnewline\n";
      } // end if
   } // end while
   std::cout << "\\end{tabu}\n\n";
} // printDataset
