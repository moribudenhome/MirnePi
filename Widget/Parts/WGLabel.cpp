#include "WGLabel.h"

namespace widget
{
	WGLabel::WGLabel(int fontSize, std::string str, SDL_Color color)
	{
		SetText(fontSize, str, color);
		OnDrawHandle.connect([this](boost::weak_ptr< WGWidgetBase > sender, WGEventArgs* e) {
			if (sprite == nullptr) { return; }
			sprite->dstRect.x = GetPos().x;
			sprite->dstRect.y = GetPos().y;
			sprite->dstRect.w = GetSize().x;
			sprite->dstRect.h = GetSize().y;
			sprite->Draw();
		});
	}

	WGLabel::~WGLabel()
	{
	}

	void WGLabel::Initialize()
	{
		WGWidgetBase::Initialize();
		SetLocalSize(Vector2(sprite->srcRect.w, sprite->srcRect.h));
	}

	void WGLabel::SetText(int fontSize, std::string str, SDL_Color color)
	{
		this->fontSize = fontSize;
		this->fontColor = color;
		this->str = str;
		sprite = mop::SDLContext::GetInstance()->CreateStringSprite(fontSize, str, color);
	}

}