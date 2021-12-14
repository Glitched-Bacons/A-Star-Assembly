#pragma once
#include <string>
#include <vector>
#include <AStarCpp/AStarModernCpp.h>

#include "Settings.h"



struct AlgorithmMeasurments
{
	std::string algorithmName;
	std::vector<float> timeElapsed;
	int dataOffset;
	int numberOfValuesToDisplay;
};

class StatisticDisplayer
{
public:
	StatisticDisplayer();

	void imGuiUpdateSelectedAlgorithm(Algorithm& currentAlgorithm);
	void imGuiDisplayLastMeasurements(const std::vector<AlgorithmMeasurments>& measurments);
	void imGuiUpdateSelectedDistanceFunctionCpp(as::AstarModernCpp::Distance& distanceFunction);

private:
	std::vector<const char*> algorithmLabels;
	std::vector<const char*> distanceCppLabels;
};
