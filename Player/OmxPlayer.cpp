#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

#include "OmxPlayer.h"

OMXPlayer::OMXPlayer()
{
	fdWriteOmxPlayer = 0;
	fbReadOmxPlayer = 0;
	omxPid = 0;
}

OMXPlayer::~OMXPlayer()
{
	Close();
}

// 動画を再生する
int OMXPlayer::Play(std::string name)
{
	printf("Now starting... %s\n", name.c_str());

	int fbr;
	std::string command = "omxplayer "; command += name;
	omxPid = Popen2(command.c_str(), &fbReadOmxPlayer, &fdWriteOmxPlayer);
}

// プレイヤーにコマンドを送信
int OMXPlayer::SendCommand(PlayerCommand Command)
{
	if (fdWriteOmxPlayer < 0 || fbReadOmxPlayer < 0) {
		return -1;
	}
	int ret = 0;
	switch (Command) 
	{
		case OMXPlayer::Stop:
		{	
			char c = 'q';
			ret = write(fdWriteOmxPlayer, &c, 1);
			break;
		}
		case OMXPlayer::Pause:
		{
			char c = 'p';
			ret = write(fdWriteOmxPlayer, &c, 1);
			break;
		}
		case OMXPlayer::VolUP:
		{
			char c = '+';
			ret = write(fdWriteOmxPlayer, &c, 1);
			break;
		}
		case OMXPlayer::VolDown:
		{
			char c = '-';
			ret = write(fdWriteOmxPlayer, &c, 1);
			break;
		}
		case OMXPlayer::SeekSub30:
		{
			short int c = 0x445b;
			ret = write(fdWriteOmxPlayer, &c, 2);
			break;
		}
		case OMXPlayer::SeekAdd30:
		{
			short int c = 0x435b;
			ret = write(fdWriteOmxPlayer, &c, 2);
			break;
		}
		case OMXPlayer::SeekSub600:
		{
			short int c = 0x425b;
			ret = write(fdWriteOmxPlayer, &c, 2);
			break;
		}
		case OMXPlayer::SeekAdd600:
		{
			short int c = 0x415b;
			ret = write(fdWriteOmxPlayer, &c, 2);
			break;
		}
	}
	return ret;
}

void OMXPlayer::Close()
{
	// プレーヤーに終了コマンド送信
	SendCommand(OMXPlayer::Stop);
	// 念の為子プロセスの終了を待つ
	int status;
	pid_t r = waitpid(omxPid, &status, 0);

	close(fdWriteOmxPlayer);
	close(fbReadOmxPlayer);
}

// 入力と出力のパイプを開いてコマンドを実行する。
int OMXPlayer::Popen2(const char *command, int *fd_r, int *fd_w)
{
	int pipe_c2p[2], pipe_p2c[2];
	int pid;

	if (pipe(pipe_c2p) < 0) {
		return (-1);
	}
	if (pipe(pipe_p2c) < 0) {
		close(pipe_c2p[0]);
		close(pipe_c2p[1]);
		return (-1);
	}

	if ((pid = fork()) < 0) {
		close(pipe_c2p[0]);
		close(pipe_c2p[1]);
		close(pipe_p2c[0]);
		close(pipe_p2c[1]);
		return (-1);
	}
	if (pid == 0) {
		close(pipe_p2c[1]);
		close(pipe_c2p[0]);
		dup2(pipe_p2c[0], 0);
		dup2(pipe_c2p[1], 1);
		close(pipe_p2c[0]);
		close(pipe_c2p[1]);
		if (execlp("sh", "sh", "-c", command, NULL) < 0) {
			close(pipe_p2c[0]);
			close(pipe_c2p[1]);
			//exit(1);
		}
	}

	close(pipe_p2c[0]);
	close(pipe_c2p[1]);
	*fd_w = pipe_p2c[1];
	*fd_r = pipe_c2p[0];

	return (pid);
}
