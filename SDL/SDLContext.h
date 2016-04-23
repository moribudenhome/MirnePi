#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <tuple>
#include <vector>

namespace mop
{
	class Sprite;

	class SDLContext
	{
		////
		// Singlton
	private:
		static SDLContext* _instance;
	public:
		static SDLContext* GetInstance() {
			if (_instance == nullptr) {
				_instance = new SDLContext();
			}
			return _instance;
		}
	private:
		// 使用するフォントファイル名
		const char* TTF_FONT_NAME = "sazanami-gothic.ttf";
	private:
		SDLContext();
	public:
		~SDLContext();
	public:
		bool Initialize();
		void Destroy();
		void BeginRender();
		void EndRender();

		// 文字列が焼きこまれたスプライトを生成
		std::shared_ptr<Sprite> CreateStringSprite(int fontSize, const std::string str, SDL_Color color);
		// 画像からスプライトを生成
		std::shared_ptr<Sprite> CreateSprite(const std::string name);

		// 生成済みフォント管理
	private:
		std::vector<std::tuple<int, TTF_Font*>> createdFonts;
	private:
		TTF_Font* FindFont(int fontSize);
		TTF_Font* CreateFont(int fontSize);

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