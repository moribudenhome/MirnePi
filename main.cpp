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

#include "SDL/SDLContext.h"
#include "Widget/Parts/WGSprite.h"

int main()
{
	mop::SDLContext* sdl = mop::SDLContext::GetInstance();
	sdl->Initialize();

	//auto sprite = sdl.CreateSprite( "sample.png" );

	auto root = widget::WGWidgetBase::create();
	auto sprite = widget::WGSprite::createSprite(root, "sample.png");
	sprite.lock()->setPos( 50, 50 );
	auto sprite2 = widget::WGSprite::createSprite(sprite, "sample.png");
	sprite2.lock()->setPos(50, 10);

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
		root->update(NULL);
		root->draw(NULL);
		sdl->EndRender();
	}

	//auto wm = widget::WGWidgetManager::createWidgetManager();
	//auto w = widget::WGWidgetBase::createWidget<widget::WGSprite>();
	//auto w2 = widget::WGWidgetBase::createWidget<widget::WGSprite>();
	//w->addWidget(w2);
}