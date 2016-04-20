#include <SDL.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>

#include "JoyStick.h"

JoyStick::JoyStick()
{
	thread = std::thread(std::bind(&JoyStick::EventMonitoring, this));
}


JoyStick::~JoyStick()
{
}

void JoyStick::Update()
{
	// PushEndのものをFreeへ,PushStartのものをPushHoldへ
	for (auto itr = inputStates.begin(); itr != inputStates.end(); itr++) {
		if (itr->second == InputState::PushEnd) {
			itr->second = InputState::Free;
		}
		if (itr->second == InputState::PushStart) {
			itr->second = InputState::PushHold;
		}
	}

	mtx.lock();
	{
		for (auto itr = inputDatas.begin(); itr != inputDatas.end(); itr++) {
			int key = std::get<0>(*itr);
			if (inputStates.count(key) == 0) {
				inputStates[key] = InputState::Free;
			}
			InputState& state = inputStates[key];

			// 前回離して、今回押した→PushStart
			if (state == InputState::Free && std::get<1>(*itr)) {
				state = InputState::PushStart;
			}
			// 前回押しして、今回離した→PushEnd
			else if ( state == InputState::PushHold && !std::get<1>(*itr)){
				state = InputState::PushEnd;
			}
		}
		inputDatas.clear();
	}
	mtx.unlock();
}

////
// キーの入力状態を取得
JoyStick::InputState JoyStick::GetInputState(int key)
{
	auto itr = inputStates.find(key);
	return (itr != inputStates.end() ? (itr->second) : InputState::Free);
}

////
// アナログ入力値を取得
int JoyStick::GetAnalogValue(int key)
{
	int result = 0;
	mtx.lock();
	{
		auto itr = analogDatas.find(key);
		result = (itr != analogDatas.end() ? (itr->second) : 0);
	}
	mtx.unlock();
	return result;
}

////
// ゲームパッドへの入力を監視する.
void JoyStick::EventMonitoring()
{
	int fd = open("/dev/input/js0", O_RDONLY);

	for (;;) {
		struct js_event  event;
		if (read(fd, &event, sizeof(struct js_event)) >= sizeof(struct js_event)) {
			mtx.lock();
			{
				switch (event.type & 0x7f) {
				case JS_EVENT_BUTTON:
					inputDatas.push_back(std::make_tuple(event.number, event.value != 0));
					break;
				case JS_EVENT_AXIS:
					analogDatas[event.number] = event.value;
					//printf("number=%d value=%d\n", event.number, event.value);
					break;
				}
			}
			mtx.unlock();
		}
	}
	close(fd);
}