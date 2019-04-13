#include "SAMessageInbox.h"

using namespace std;
using namespace cocos2d;

bool SAMessageInbox::init(
	const cocos2d::Size& sizeCell,
	InboxFillDirect direct,
	size_t maxSize /*= 0*/,
	float fSpaceLenX /*= 0*/,
	float fSpaceLenY /*= 0*/
)
{
	if (!SANode::init())
		return false;

	return true;
}