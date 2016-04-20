
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