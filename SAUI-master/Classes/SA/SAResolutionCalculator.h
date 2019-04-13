#pragma once
#include "cocos2d.h"
#include "SAExternal.h"

class SAResolutionCalculator
{
public:
	void setDesignResolution(const cocos2d::Size& size) { m_design.set(size); }
	cocos2d::Size getDesignResolution() { return m_design.toSize(); }

	void setAbsoluteDesignResolution(const cocos2d::Size& size) { m_absoluteDesign.set(size); }
	cocos2d::Size getAbsoluteDesignResolution() { return m_absoluteDesign.toSize(); }

	SAPosition cal(const cocos2d::Size& size);
	SAPosition cal(const cocos2d::Vec2& vec2);
	SAPosition cal(float x, float y);

	SAPosition calP(float xPercent, float yPercent);
	SAPosition calP(float xPercent, float yPercent, const SAPosition& real);

	static SAResolutionCalculator* getDefaultCalculator();

private:
	SAPosition m_design;
	SAPosition m_absoluteDesign;
};