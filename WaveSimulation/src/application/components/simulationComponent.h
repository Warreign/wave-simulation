#pragma once

#include "application/components/component.h"
#include "simulation/amplitude_grid.h"

#include <memory>

class SimulationComponent : public Component
{
public:
	SimulationComponent(const std::string& debugName);

	void init() override;
	void destroy() override;

	void onUpdate() override;
	void onRenderGui() override;

	AmplitudeGrid& getGrid() { return *m_simGrid; }
	bool isUpdateGird() { return m_isUpdateGrid; }

private:

	void addPointDisturbance(double viewportX, double viewportY);

	bool m_isUpdateGrid = true;
	float m_timeMultiplier = -0.9f;
	float m_defaultAmplitude = 0.2f;
	int m_defaultDirection = 3;
	float m_windSpeed = 8.0f;


	std::unique_ptr<AmplitudeGrid> m_simGrid;
};