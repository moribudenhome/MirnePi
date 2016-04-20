#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>

namespace mop
{
	class Sprite;

	class SDLContext
	{
	public:
		SDLContext();
		~SDLContext();
	private:
		// 使用するフォントファイル名
		const char* TTF_FONT_NAME = "sazanami-gothic.ttf";
	public:
		bool Initialize();
		void Destroy();
		void BeginRender();
		void EndRender();

		// 文字列が焼きこまれたスプライトを生成
		std::shared_ptr<Sprite> CreateStringSprite(TTF_Font* font, const std::string str, SDL_Color color);

	public:
		SDL_Window* window;
		SDL_Renderer *renderer;
		SDL_RWops *fontRWops;
	};

	class Sprite
	{
	public:
		Sprite(SDLContext* context, SDL_Surface *surface);
		~Sprite();
		void Draw();
	private:
		SDLContext* context;
		SDL_Texture* texture;
	public:
		SDL_Rect srcRect;
		SDL_Rect dstRect;
	};

}