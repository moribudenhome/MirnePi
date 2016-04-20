#pragma once

#include <string>

class OMXPlayer
{
public:
	// // https://github.com/JugglerLKR/omxplayer-web-controls-php/blob/master/omx_control.php
	enum PlayerCommand
	{
		Stop,
		Pause,
		VolUP,
		VolDown,
		SeekSub30,
		SeekAdd30,
		SeekSub600,
		SeekAdd600,
	};
public:
	OMXPlayer();
	~OMXPlayer();
public:
	// 動画を再生
	int Play(std::string name);
	// プレイヤーにコマンドを送信
	int SendCommand(PlayerCommand Command);
	void Close();
private:
	// 入力と出力のパイプを開いてコマンドを実行
	int Popen2(const char *command, int *fd_r, int *fd_w);
private:
	// omxplayerへの標準入力
	int fdWriteOmxPlayer;
	int fbReadOmxPlayer;
	int omxPid;
};