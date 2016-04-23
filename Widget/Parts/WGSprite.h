#pragma once

#include "../WGWidgetBase.h"
#include "../../SDL/SDLContext.h"

namespace widget
{
	class WGSprite : public WGWidgetBase
	{
		friend WGWidgetBase;
	private:
		WGSprite(std::string name);
	public:
		~WGSprite();
	public:
		/* 初期化 */
		virtual void Initialize();
	private:
		std::shared_ptr<mop::Sprite> sprite;
	};
}

