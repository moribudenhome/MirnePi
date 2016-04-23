#include "WGSprite.h"

namespace widget
{
	WGSprite::WGSprite(std::string name)
	{
		sprite = mop::SDLContext::GetInstance()->CreateSprite(name);
		OnDrawHandle.connect([this](boost::weak_ptr< WGWidgetBase > sender, WGEventArgs* e) {
			sprite->dstRect.x = GetPos().x;
			sprite->dstRect.y = GetPos().y;
			sprite->dstRect.w = GetSize().x;
			sprite->dstRect.h = GetSize().y;
			sprite->Draw();
		});
	}

	WGSprite::~WGSprite()
	{
	}

	/* 初期化 */
	void WGSprite::Initialize()
	{
		WGWidgetBase::Initialize();
		SetLocalSize(Vector2(sprite->srcRect.w, sprite->srcRect.h));
	}
}
