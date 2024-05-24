/**
 * @file simulationComponent.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "application/components/component.h"
#include "simulation/amplitudeGrid.h"

#include <memory>

class SimulationComponent : public Component
{
public:

	/*
	* Component doing all the simulation work
	*/
	SimulationComponent(const std::string& debugName);

	void init() override;
	void destroy() override;

	void onUpdate(float dt) override;
	void onRenderGui() override;

	AmplitudeGrid& getGrid() { return *m_simGrid; }
	bool isUpdateGird() { return m_isUpdateGrid; }

private:

	/*
	* function to add point disturbances on water surface 
	* 
	* @param viewportX x coordinate in screen space 
	* @param viewportY y coordinate in screen space
	*/
	void addPointDisturbance(double viewportX, double viewportY);

	bool m_isUpdateGrid = true;
	float m_timeMultiplier = 0.3f;
	float m_dtLast;


	/*
	* The simulation grid
	*/
	std::unique_ptr<AmplitudeGrid> m_simGrid;

	friend class VisualizationComponent;
};