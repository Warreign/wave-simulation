#pragma once

#include "application/components/component.h"
#include "simulation/amplitudeGrid.h"

#include <memory>

class SimulationComponent : public Component
{
public:
	SimulationComponent(const std::string& debugName);

	void init() override;
	void destroy() override;

	void onUpdate(float dt) override;
	void onRenderGui() override;

	AmplitudeGrid& getGrid() { return *m_simGrid; }
	bool isUpdateGird() { return m_isUpdateGrid; }

private:

	void addPointDisturbance(double viewportX, double viewportY);

	bool m_isUpdateGrid = true;
	float m_timeMultiplier = 0.0f;
	float m_dtLast;

	std::unique_ptr<AmplitudeGrid> m_simGrid;

	friend class VisualizationComponent;
};