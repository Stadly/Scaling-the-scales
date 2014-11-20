#include "relations.h"

void Relations::addFile(std::string const filename) {
   m_filenames.push_back(filename);
   m_files.push_back(new std::ifstream(m_path+"relations/"+m_dataset+"/"+filename));
   m_count.push_back(0);
   m_scale.push_back(std::vector<double>());
   m_scaleDown.push_back(0);
   m_scaleUp.push_back(0);
   m_totalScale.push_back(0);
   m_inverseCount.push_back(0);
   m_inverseScale.push_back(std::vector<double>());
   m_inverseScaleDown.push_back(0);
   m_inverseScaleUp.push_back(0);
   m_inverseTotalScale.push_back(0);
} // addFile

void Relations::analyze() {
   for(int i = 0; i < m_filenames.size(); ++i) {
		while(*m_files[i]) {
			std::string s;
			std::getline(*m_files[i], s, '|'); // predictor
			if(!s.empty()) {
				int predictor = atoi(s.c_str());
				std::getline(*m_files[i], s, '|'); // predicted
				int predicted = atoi(s.c_str());
				std::getline(*m_files[i], s, '|'); // commonRatings
				int commonRatings = atoi(s.c_str());
				std::getline(*m_files[i], s, '|'); // funcError
				double funcError = atof(s.c_str());
				std::getline(*m_files[i], s); // func
            
            int min1, max1, min2, max2;
            std::stringstream ss(s);
            std::getline(ss, s, ','); // min1
            min1 = atoi(s.c_str());
            std::getline(ss, s, ','); // max1
            max1 = atoi(s.c_str());
            std::getline(ss, s, ','); // min2
            if(ss) {
               min2 = atoi(s.c_str());
               std::getline(ss, s); // max2
               max2 = atoi(s.c_str());
            } else {
               min2 = max1;
               max2 = max1+min1;
               min1 = 0;
               max1 = 1;
            }
            
            ++m_count[i];
            if(std::abs(max2-min2) < std::abs(max1-min1))
               ++m_scaleDown[i];
            if(std::abs(max1-min1) < std::abs(max2-min2))
               ++m_scaleUp[i];
            m_scale[i].push_back((double)std::abs(max2-min2)/std::abs(max1-min1));
            m_totalScale[i] += (double)std::abs(max2-min2)/std::abs(max1-min1);
            
            if((max2-min2)*(max1-min1) < 0) {
               ++m_inverseCount[i];
               if(std::abs(max2-min2) < std::abs(max1-min1))
                  ++m_inverseScaleDown[i];
               if(std::abs(max1-min1) < std::abs(max2-min2))
                  ++m_inverseScaleUp[i];
               m_inverseScale[i].push_back((double)std::abs(max2-min2)/std::abs(max1-min1));
               m_inverseTotalScale[i] += (double)std::abs(max2-min2)/std::abs(max1-min1);
            }
			} // end if
		} // end while
   } // end for
} // analyze

void Relations::print() const {
   std::cout << std::fixed << std::setprecision(2);
   
   std::cout << "\\begin{tabu}{p{4cm}";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << "X[c]";
   std::cout << "}\n";
   
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & " << m_filenames[i];
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Relations";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << m_count[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Inverse relations";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\SI{" << (int)round(100*(double)m_inverseCount[i]/m_count[i]) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Scaled relations";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\SI{" << (int)round(100*(double)(m_scaleUp[i]+m_scaleDown[i])/m_count[i]) << "}{\\percent}";
   std::cout << " \\tabularnewline\n\n";
   
   std::cout << "Scale up";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << m_scaleUp[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Scale down";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << m_scaleDown[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Avg scale";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << (double)m_totalScale[i]/m_count[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Inverse scaled relations";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\SI{" << (int)round(100*(double)(m_inverseScaleUp[i]+m_inverseScaleDown[i])/m_inverseCount[i]) << "}{\\percent}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Inverse scale up";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << m_inverseScaleUp[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Inverse scale down";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << m_inverseScaleDown[i] << "}";
   std::cout << " \\tabularnewline\n";
   
   std::cout << "Inverse avg scale";
   for(int i = 0; i < m_filenames.size(); ++i)
      std::cout << " & \\num{" << (double)m_inverseTotalScale[i]/m_inverseCount[i] << "}";
   std::cout << " \\tabularnewline\n";
   std::cout << "\\end{tabu}\n\n";
   
   std::cout << "Scales\n";
   for(int i = 0; i < m_filenames.size(); ++i) {
      std::cout << m_filenames[i] << std::endl;
      std::vector<double> a = m_scale[i];
      if(0 < a.size()) {
         std::sort(a.begin(), a.end());
         double scalePrev = a.at(0);
         int scaleCount = 0;
         for(double scale : a) {
            if(scale != scalePrev) {
               std::cout << scalePrev << " " << scaleCount << " times\n";
               scaleCount = 0;
            }
            ++scaleCount;
            scalePrev = scale;
         } // end for
         std::cout << scalePrev << " " << scaleCount << " times" << std::endl << std::endl;
      } // end if
   } // end for
   
   std::cout << "Inverse scales\n";
   for(int i = 0; i < m_filenames.size(); ++i) {
      std::cout << m_filenames[i] << std::endl;
      std::vector<double> a = m_inverseScale[i];
      if(0 < a.size()) {
         std::sort(a.begin(), a.end());
         double scalePrev = a.at(0);
         int scaleCount = 0;
         for(double scale : a) {
            if(scale != scalePrev) {
               std::cout << scalePrev << " " << scaleCount << " times\n";
               scaleCount = 0;
            }
            ++scaleCount;
            scalePrev = scale;
         } // end for
         std::cout << scalePrev << " " << scaleCount << " times" << std::endl << std::endl;
      } // end if
   } // end for
} // print
