#include "SAResolutionCalculator.h"

using namespace std;
using namespace cocos2d;

SAPosition SAResolutionCalculator::cal(float x, float y)
{
	return calP(x / m_absoluteDesign.x, y / m_absoluteDesign.y, m_design);
}

SAPosition SAResolutionCalculator::cal(const Vec2& vec2)
{
	return calP(vec2.x / m_absoluteDesign.x, vec2.y / m_absoluteDesign.y, m_design);
}

SAPosition SAResolutionCalculator::cal(const Size& size)
{
	return calP(size.width / m_absoluteDesign.x, size.height / m_absoluteDesign.y, m_design);
}

SAPosition SAResolutionCalculator::calP(float xPercent, float yPercent)
{
	return calP(xPercent, yPercent, m_design);
}

SAPosition SAResolutionCalculator::calP(float xPercent, float yPercent, const SAPosition& real)
{
	return SAPosition(xPercent * real.x, yPercent * real.y);
}

SAResolutionCalculator* SAResolutionCalculator::getDefaultCalculator()
{
	static SAResolutionCalculator instance;
	return &instance;
}