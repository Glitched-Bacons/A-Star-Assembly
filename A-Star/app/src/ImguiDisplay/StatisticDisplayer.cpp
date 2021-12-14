#include "StatisticDisplayer.h"

#include <numeric>
#include <vector>
#include <AStarCpp/AStar.h>
#include <imgui/imgui.h>
#include <implot/implot.h>

StatisticDisplayer::StatisticDisplayer()
{
	for(int i = 0; i < static_cast<int>(Algorithm::COUNTER); ++i)
	{
		algorithmLabels.push_back(toString(static_cast<Algorithm>(i)));
	}

	for (int i = 0; i < static_cast<int>(as::AstarModernCpp::Distance::COUNTER); ++i)
	{
		distanceCppLabels.push_back(toString(static_cast<as::AstarModernCpp::Distance>(i)));
	}
}

void StatisticDisplayer::imGuiUpdateSelectedAlgorithm(Algorithm& currentAlgorithm)
{
	ImGui::TextWrapped("Select algorithm of which result should be displayed");
	if (ImGui::BeginCombo("Algorithm", algorithmLabels[static_cast<int>(currentAlgorithm)]))
	{
		for (int n = 0; n < algorithmLabels.size(); n++)
		{
			const bool is_selected = (static_cast<int>(currentAlgorithm) == n);
			if (ImGui::Selectable(algorithmLabels[n], is_selected))
			{
				currentAlgorithm = static_cast<Algorithm>(n);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void StatisticDisplayer::imGuiDisplayLastMeasurements(const std::vector<AlgorithmMeasurments>& measurments)
{
	if (ImPlot::BeginPlot("Last 10 executions")) {
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.2f");
		ImPlot::SetupAxes("Executions", "Times (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		for (const auto& measurment : measurments)
		{
			ImPlot::PlotStems(measurment.algorithmName.c_str(),
				measurment.timeElapsed.data() + measurment.dataOffset, measurment.numberOfValuesToDisplay);
		}
		ImPlot::EndPlot();
	}

	std::vector<const char*> algorithmNames;
	std::vector<float> averageExecutions;
	for (const auto& measurment : measurments)
	{
		algorithmNames.push_back(measurment.algorithmName.c_str());
		averageExecutions.push_back(
			std::accumulate(measurment.timeElapsed.begin(), measurment.timeElapsed.end(), 0.f) / measurment.timeElapsed.size()
		);
	}

	std::vector<double> positions;
	for (int i = 0; i <= algorithmNames.size(); ++i) { positions.emplace_back(i); }

	if (ImPlot::BeginPlot("Average of all previous executions")) {
		ImPlot::SetupAxes("Performance", "Times (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::SetupAxisTicks(ImAxis_X1, positions.data(), 3, algorithmNames.data());
		ImPlot::PlotBars("", averageExecutions.data(), averageExecutions.size());
		ImPlot::EndPlot();
	}
}

void StatisticDisplayer::imGuiUpdateSelectedDistanceFunctionCpp(as::AstarModernCpp::Distance& distanceFunction)
{
	if (ImGui::BeginCombo("Dist. Func.", distanceCppLabels[static_cast<int>(distanceFunction)]))
	{
		for (int n = 0; n < distanceCppLabels.size(); n++)
		{
			const bool is_selected = (static_cast<int>(distanceFunction) == n);
			if (ImGui::Selectable(distanceCppLabels[n], is_selected))
			{
				distanceFunction = static_cast<as::AstarModernCpp::Distance>(n);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}