#ifndef ANALYZER_RELATIONS_H
#define ANALYZER_RELATIONS_H

#include "patch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <algorithm>

class Relations {
public:
   Relations(std::string const dataset, std::string const path) : m_dataset(dataset), m_path(path) {}
   void addFile(std::string const filename);
   void analyze();
   void print() const;
   int fileCount() const {return m_files.size();}
private:
   std::string const m_dataset;
   std::string const m_path;
   std::vector<std::string> m_filenames;
   
	std::vector<std::ifstream *> m_files;
   std::vector<int> m_count;
   std::vector<std::vector<double>> m_scale;
   std::vector<double> m_totalScale;
   std::vector<int> m_scaleDown;
   std::vector<int> m_scaleUp;
   std::vector<int> m_inverseCount;
   std::vector<std::vector<double>> m_inverseScale;
   std::vector<double> m_inverseTotalScale;
   std::vector<int> m_inverseScaleDown;
   std::vector<int> m_inverseScaleUp;
};

#endif // ANALYZER_RELATIONS_H

