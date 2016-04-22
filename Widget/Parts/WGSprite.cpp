#include "WGSprite.h"

namespace widget
{
	WGSprite::WGSprite(std::string name)
	{
		sprite = mop::SDLContext::GetInstance()->CreateSprite(name);
		onDrawHandle.connect([this](boost::weak_ptr< WGWidgetBase > sender, WGEventArgs* e) {
			sprite->dstRect.x = this->getPosX();
			sprite->dstRect.y = this->getPosY();
			sprite->Draw();
		});
	}

	WGSprite::~WGSprite()
	{
	}
}
