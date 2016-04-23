#pragma once

#include "../WGWidgetBase.h"
#include "../../SDL/SDLContext.h"

#include <string>

namespace widget
{
	class WGLabel : public WGWidgetBase
	{
		friend WGWidgetBase;
	private:
		WGLabel(int fontSize, std::string str, SDL_Color color);
	public:
		~WGLabel();
	private:
		virtual void Initialize();
	public:
		void SetText(int fontSize, std::string str, SDL_Color color);
	private:
		int fontSize = 60;
		SDL_Color fontColor = { 255, 255, 255, 0 };
		std::string str;
		std::shared_ptr<mop::Sprite> sprite;
	};
}

