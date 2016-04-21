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

	boost::weak_ptr<WGSprite> WGSprite::createSprite(boost::weak_ptr<WGWidgetBase> parent, std::string name)
	{
		return createWidget(parent, boost::shared_ptr<WGSprite>(new WGSprite(name)));
	}
}
