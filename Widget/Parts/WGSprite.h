#pragma once

#include "../WGWidgetBase.h"
#include "../../SDL/SDLContext.h"

namespace widget
{
	class WGSprite : public WGWidgetBase
	{
	public:
		WGSprite(std::string name);
		~WGSprite();

	public:
		static boost::weak_ptr<WGSprite> createSprite(boost::weak_ptr<WGWidgetBase> parent, std::string name );
	private:
		std::shared_ptr<mop::Sprite> sprite;
	};
}

