
#include "SDLContext.h"

#include <SDL_image.h>

namespace mop
{

	Sprite::Sprite(SDLContext* context, SDL_Surface *surface)
	{
		if (surface == nullptr) {
			return;
		}
		dstRect.x = dstRect.y = srcRect.x = srcRect.y = 0;
		srcRect.w = dstRect.w = surface->w;
		srcRect.h = dstRect.h = surface->h;
		this->context = context;
		texture = SDL_CreateTextureFromSurface(context->renderer, surface);
	}

	Sprite::~Sprite()
	{
		SDL_DestroyTexture(texture);
	}

	void Sprite::Draw()
	{
		if (context == nullptr) { return; }
		SDL_RenderCopy(context->renderer, texture, &srcRect, &dstRect);
	}

	SDLContext* SDLContext::_instance = nullptr;

	SDLContext::SDLContext()
	{
	}

	SDLContext::~SDLContext()
	{
	}

	bool SDLContext::Initialize()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			return false;
		}

		// TODO 現在フルスクリーンのみ。後でオプション指定できるようにしよう
		SDL_DisplayMode mode;
		SDL_GetDesktopDisplayMode(0, &mode);

		if (TTF_Init() < 0) {
			return false;
		}

		fontRWops = SDL_RWFromFile(TTF_FONT_NAME, "rb");
		if (fontRWops == nullptr) {
			return false;
		}

		window = SDL_CreateWindow(
			"aaaaa", //ウィンドウタイトル
			0, //ウィンドウ位置x
			0, //y
			mode.w, //幅
			mode.h, //高さ
			SDL_WINDOW_OPENGL //flags 詳細はマニュアル参照
		);
		SDL_SetWindowFullscreen(window, SDL_TRUE);

		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	void SDLContext::Destroy()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_RWclose(fontRWops);
		for (auto itr = createdFonts.begin(); itr != createdFonts.end(); itr++) {
			TTF_CloseFont(std::get<1>(*itr));
		}
		createdFonts.clear();
		SDL_Quit();
	}

	void SDLContext::BeginRender()
	{
		SDL_RenderClear(renderer);
	}

	void SDLContext::EndRender()
	{
		SDL_RenderPresent(renderer);
	}

	////
	// 文字列が焼きこまれたスプライトを生成
	std::shared_ptr<Sprite> SDLContext::CreateStringSprite(int fontSize, const std::string str, SDL_Color color)
	{
		TTF_Font* font = nullptr;
		font = FindFont(fontSize);
		if (font == nullptr) {
			font = CreateFont(fontSize);
			if (font == nullptr) { return nullptr; }
			createdFonts.push_back(std::make_tuple(fontSize,font));
		}

		SDL_Surface* surface = TTF_RenderUTF8_Solid(font, str.c_str(), color);
		if (surface == nullptr) {
			return nullptr;
		}

		std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>(new Sprite(this, surface));
		SDL_FreeSurface(surface);

		return sprite;
	}

	////
	// 画像からスプライトを生成
	std::shared_ptr<Sprite> SDLContext::CreateSprite(const std::string name)
	{
		SDL_RWops* rwops = SDL_RWFromFile(name.c_str(), "rb");
		if (rwops == nullptr) {
			return nullptr;
		}

		SDL_Surface* surface = IMG_LoadPNG_RW(rwops);
		if (surface == nullptr) {
			return nullptr;
		}

		std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>(new Sprite(this, surface));
		SDL_FreeSurface(surface);

		return sprite;
	}

	TTF_Font* SDLContext::FindFont(int fontSize)
	{
		for (auto itr = createdFonts.begin(); itr != createdFonts.end(); itr++) {
			if (std::get<0>(*itr) == fontSize) {
				return std::get<1>(*itr);
			}
		}
		return nullptr;
	}

	TTF_Font* SDLContext::CreateFont(int fontSize)
	{
		return TTF_OpenFontRW(fontRWops, 0, fontSize);
	}
}