#if 0

#include "SDL/SDLContext.h"
#include "Player/Explorer.h"
#include "SDL/JoyStick.h"

#include <unistd.h>
#include <stdlib.h>

int main()
{
	Explorer explorer;
	explorer.Build( "/home/pi/recorder/raw/" );

	OMXPlayer player;

	int seek = 0;
	system("clear");
	printf("%s\n", explorer.items[0]->fileName.c_str());

	JoyStick* js = new JoyStick();
	while (true) {
		js->Update();
		if (0 < js->GetAnalogValue(5)){
			seek--;
			if (seek < 0) {
				seek = explorer.items.size() - 1;
			}
			system("clear");
			printf("%s\n", explorer.items[seek]->fileName.c_str());
		}
		if (js->GetAnalogValue(5) < 0) {
			seek++;
			if (explorer.items.size() <= seek) {
				seek = 0;
			}
			system("clear");
			printf("%s\n", explorer.items[seek]->fileName.c_str());
		}
		if (js->GetInputState(2) == JoyStick::PushEnd) {
			explorer.items[seek]->Play(&player);
		}
		if (js->GetInputState(1) == JoyStick::PushEnd) {
			player.SendCommand(OMXPlayer::Stop);
		}
		usleep(100000);
	}
}


#endif

#if 1

#include "SDL/SDLContext.h"
#include "Widget/Parts/WGSprite.h"

int main()
{
	mop::SDLContext* sdl = mop::SDLContext::GetInstance();
	sdl->Initialize();

	//auto sprite = sdl.CreateSprite( "sample.png" );

	auto root = widget::WGWidgetBase::Create();
	auto sprite = CREATE_WIDGET(widget::WGSprite, root, "sample.png");
	sprite.lock()->SetLocalPos(widget::Vector2(5.0f, 5.0f));
	auto sprite2 = CREATE_WIDGET(widget::WGSprite, sprite, "sample.png");
	sprite2.lock()->SetLocalPos(widget::Vector2(50, 10));

	auto a = sprite.lock()->GetPos();
	sprite.lock()->GetTransformMatrix().dump();
	printf("sprite %f, %f\n", a.x, a.y);
	auto b = sprite2.lock()->GetPos();
	sprite2.lock()->GetTransformMatrix().dump();
	printf("sprite2 %f, %f\n", b.x, b.y);

	float hoge = 0.0f;
	while (true) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				return 0;
			case SDL_KEYDOWN:
				int key = ev.key.keysym.sym;
				if (key == SDLK_ESCAPE) {
					return 0;
				}
				break;
			}
		}
		sdl->BeginRender();
		root->SetLocalScale(widget::Vector2(hoge, hoge));
		root->Update(NULL);
		root->Draw(NULL);
		sdl->EndRender();

		hoge += 0.01f;
	}

	//auto wm = widget::WGWidgetManager::createWidgetManager();
	//auto w = widget::WGWidgetBase::createWidget<widget::WGSprite>();
	//auto w2 = widget::WGWidgetBase::createWidget<widget::WGSprite>();
	//w->addWidget(w2);
}

#endif

#if 0

#include "SDL/SDLContext.h"
#include "Widget/Parts/WGSprite.h"
#include <iostream>

int main()
{
	widget::Matrix3x3 mat1, mat2;
	mat1.translate(5.0f, 5.0f);
	//mat1.scale(0.5, 1.0);
	mat2.translate(1.0f, 1.0f);
	mat2.translate(1.0f, 1.0f);

	mat1.dump();
	mat2.dump();

	widget::Matrix3x3 mat3 = (mat1*mat2);
	mat3.dump();

	widget::Vector2 p; p.x = 0.0f; p.y = 0.0f;
	p = mat3.transformPoint(p);
	printf("%f, %f", p.x, p.y);
}

#endif