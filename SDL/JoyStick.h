#pragma once

#include <future>
#include <mutex>
#include <vector>
#include <map>

////
// JoyStickへの入力を管理
class JoyStick
{
public:
	enum InputState {
		PushStart,	// ボタンを押した瞬間
		PushHold,	// ボタンを押し続けている
		PushEnd,	// ボタンを離した瞬間
		Free,		// ボタンを押していない
	};
public:
	JoyStick();
	~JoyStick();
public:
	void Update();
	// キーの入力状態を取得
	InputState GetInputState(int key);
	// アナログ入力値を取得
	int GetAnalogValue(int key);
private:
	std::vector<std::tuple<int, bool>> inputDatas;
	std::map<int, InputState> inputStates;
	std::map<int, int> analogDatas;
private:
	std::mutex mtx;
	std::thread thread;
private:
	void EventMonitoring();


};

