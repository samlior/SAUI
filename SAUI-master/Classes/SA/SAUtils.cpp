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
	const Size& size = pNode->getContentSize();
	bool bEmptySize = (!size.width && !size.height);
	float xl = 0;
	float xr = size.width;
	float yb = 0;
	float yu = size.height;

	const Vector<Node*>& vecChildren = pNode->getChildren();
	for (auto itr = vecChildren.begin(); itr != vecChildren.end(); ++itr)
	{
		Node* pNodeChild = *itr;
		const Vec2& vec2PInP = pNodeChild->getAnchorPointInPoints();
		const Vec2& vec2Pos = pNodeChild->getPosition();
		Vec2 vec2OrgPos = vec2Pos - vec2PInP;

		const Size& sizeChild = getTotalContentSize(pNodeChild);

		float xlChild = vec2OrgPos.x;
		float xrChild = vec2OrgPos.x + sizeChild.width;
		float ybChild = vec2OrgPos.y;
		float yuChild = vec2OrgPos.y + sizeChild.height;

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

	return Size(xr - xl, yu - yb);
}