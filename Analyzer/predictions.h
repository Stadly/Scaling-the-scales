#ifndef ANALYZER_PREDICTIONS_H
#define ANALYZER_PREDICTIONS_H

#include "patch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>

class Predictions {
public:
   Predictions(std::string const dataset, int const scaleSize, int const extremalThreshold, std::string const path)
   : m_dataset(dataset), m_scaleSize(scaleSize), m_extremalThreshold(extremalThreshold), m_path(path) {}
   void addFile(std::string const filename);
   bool isExtremal(int const rating) const;
   std::vector<int> useFilter(int const filterSuccessRate, double const filterTotalError, double const filterTotalErrorExtremal) const;
   void analyze();
   void printTables(std::vector<int> filtered) const;
   void printTotal(std::vector<int> filtered) const;
   void printOk(std::vector<int> filtered) const;
   void printDataset() const;
   int fileCount() const {return m_files.size();}
private:
   std::vector<std::string> m_filenames;
   std::string const m_dataset;
   int const m_scaleSize;
   int const m_extremalThreshold;
   std::string const m_path;
   int m_count = 0;
   int m_countExtremal = 0;
   
	std::vector<std::ifstream *> m_files;
   std::vector<int> m_fails;
   std::vector<int> m_failsExtremal;
   std::vector<double> m_totalDiff;
   std::vector<double> m_totalDiffExtremal;
   std::vector<int> m_totalPredictors;
   std::vector<int> m_totalPathCount;
   std::vector<int> m_totalPathLength;
   std::vector<int> m_totalArcCount;
   std::vector<double> m_totalDistinctArcs;
   std::vector<double> m_totalAvgCommonRatings;
   std::vector<double> m_totalAvgErrorFunc;
   std::vector<int> m_totalCount;
   std::vector<int> m_totalCountExtremal;
   
   std::vector<std::vector<std::vector<int>>> m_table;
   std::vector<double> m_okDiff;
   std::vector<double> m_okDiffExtremal;
   std::vector<int> m_okPredictors;
   std::vector<int> m_okPathCount;
   std::vector<int> m_okPathLength;
   std::vector<int> m_okArcCount;
   std::vector<double> m_okDistinctArcs;
   std::vector<double> m_okAvgCommonRatings;
   std::vector<double> m_okAvgErrorFunc;
   int m_okCount = 0;
   int m_okCountExtremal = 0;
};

#endif // ANALYZER_PREDICTIONS_H

