#include "SAUtils.h"
#include "cocos-ext.h"
#include "SA/SAHeader.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;


bool SAUtils::isContain(
	Node* pNode, 
	const Vec2& vec2WorldLocation,
	const Rect* const pRect /*= nullptr*/
)
{
	//转换成节点的相对坐标,此时以节点的左下角为原点
	Vec2 vec2NodeLocation = pNode->convertToNodeSpace(vec2WorldLocation);
	Size s = pNode->getContentSize();
	Rect r = !pRect ?
		Rect(0, 0, s.width, s.height) :
		*pRect;
	//返回是否包含
	return r.containsPoint(vec2NodeLocation);
}

Sprite* SAUtils::createWithFileConfig(const string& strFileName, int iConfig)
{
	if ((iConfig & LOAD_TEXTURE_FROM_SPR_FRAME) > 0)
		return (iConfig & SPR_TYPE_SCALE9) > 0 ? Scale9Sprite::createWithSpriteFrameName(strFileName) : Sprite::createWithSpriteFrameName(strFileName);
	else
		return (iConfig & SPR_TYPE_SCALE9) > 0 ? Scale9Sprite::create(strFileName) : Sprite::create(strFileName);
}

Sprite* SAUtils::createWithFileConfigAndSize(const Size& size, const string& strFileName, int iConfig)
{
	Sprite* pSpr = createWithFileConfig(strFileName, iConfig);
	(iConfig & SPR_TYPE_SCALE9) > 0 ? pSpr->setContentSize(size) : setNodeSize(size, pSpr);
	return pSpr;
}

void SAUtils::removeChildByTag(Node* pParent, int iTag)
{
	Node* pChild = pParent->getChildByTag(iTag);
	if (pChild)
	{
		pChild->removeFromParentAndCleanup(true);
		pChild = nullptr;
	}
}

void SAUtils::removeChildByTag(cocos2d::Node* pParent, int iTagBegin, int iTagEnd)
{
	for (int i = iTagBegin;i < iTagEnd; i++)
	{
		removeChildByTag(pParent, i);
	}
}

Size SAUtils::getTotalContentSize(Node* pNode)
{
	float xl = 0, xr = 0, yu = 0, yb = 0;
	getTotalContentOffset(pNode, xl, xr, yu, yb);
	return Size(xr - xl, yu - yb);
}

void SAUtils::getTotalContentOffset(Node* pNode, float& xl, float& xr, float& yu, float& yb)
{
	const Size& size = pNode->getContentSize();
	bool bEmptySize = (!size.width && !size.height);
	xl = 0;
	xr = size.width;
	yb = 0;
	yu = size.height;

	const Vector<Node*>& vecChildren = pNode->getChildren();
	for (auto itr = vecChildren.begin(); itr != vecChildren.end(); ++itr)
	{
		Node* pNodeChild = *itr;
		const Vec2& vec2PInP = pNodeChild->getAnchorPointInPoints();
		const Vec2& vec2Pos = pNodeChild->getPosition();
		Vec2 vec2OrgPos = (pNodeChild->isIgnoreAnchorPointForPosition() ? vec2Pos : vec2Pos - vec2PInP);

		float xlChild = 0, xrChild = 0, ybChild = 0, yuChild = 0;
		getTotalContentOffset(pNodeChild, xlChild, xrChild, yuChild, ybChild);
		const Size& sizeChild = Size(xrChild - xlChild, yuChild - ybChild);

		xlChild += vec2OrgPos.x;
		xrChild += vec2OrgPos.x;
		ybChild += vec2OrgPos.y;
		yuChild += vec2OrgPos.y;

		if (bEmptySize)
		{
			xl = xlChild;
			xr = xrChild;
			yb = ybChild;
			yu = yuChild;
			bEmptySize = false;
		}
		else
		{
			xl = MIN(xl, xlChild);
			xr = MAX(xr, xrChild);
			yb = MIN(yb, ybChild);
			yu = MAX(yu, yuChild);
		}
	}
}