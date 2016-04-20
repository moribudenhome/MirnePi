#if 0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
* 入力と出力のパイプを開いてコマンドを実行する。
*/
int popen2(char *command, int *fd_r, int *fd_w)
{
	int pipe_c2p[2], pipe_p2c[2];
	int pid;

	/* Create two of pipes. */
	if (pipe(pipe_c2p) < 0) {
		perror("popen2");
		return (-1);
	}
	if (pipe(pipe_p2c) < 0) {
		perror("popen2");
		close(pipe_c2p[0]);
		close(pipe_c2p[1]);
		return (-1);
	}

	/* Invoke processs */
	if ((pid = fork()) < 0) {
		perror("popen2");
		close(pipe_c2p[0]);
		close(pipe_c2p[1]);
		close(pipe_p2c[0]);
		close(pipe_p2c[1]);
		return (-1);
	}
	if (pid == 0) {   /* I'm child */
		close(pipe_p2c[1]);
		close(pipe_c2p[0]);
		dup2(pipe_p2c[0], 0);
		dup2(pipe_c2p[1], 1);
		close(pipe_p2c[0]);
		close(pipe_c2p[1]);
		if (execlp("sh", "sh", "-c", command, NULL) < 0) {
			perror("popen2");
			close(pipe_p2c[0]);
			close(pipe_c2p[1]);
			exit(1);
		}
	}

	close(pipe_p2c[0]);
	close(pipe_c2p[1]);
	*fd_w = pipe_p2c[1];
	*fd_r = pipe_c2p[0];

	return (pid);
}

/*
* メインルーチン。
*/
int main(void)
{
	char *str = "=?ISO-2022-JP?B?GyRCJDMkbCRPRnxLXDhsJE4kRiQ5JEgkSiRzJEAkKyRpGyhC?==?ISO-2022-JP?B?GyRCJE0kQxsoQg==?=";
	int fd_r, fd_w;
	char buffer[10240];
	size_t ret;

	printf("Now starting...\n");

	// コマンドとパイプの準備
	// ここではnkfコマンドでMIMEエンコードされた文字列を
	// UTF-8文字列にデコードして出力する
	popen2("omxplayer /home/pi/recorder/enc/灰と幻想のグリムガル/灰と幻想のグリムガル_リーダーの器じゃないけれど_10.mp4  灰と幻想のグリムガル_生と死の 間で_11.mp4", &fd_r, &fd_w);

	// パイプに入力する

	short int aa = 0x435b;
	sleep(3);
	ret = write(fd_w, &aa, 2);
	if (ret != -1)
	{
		printf("Write successfull.\n");
		close(fd_w);

		// パイプから読み出し
		ret = read(fd_r, buffer, 10240);
		close(fd_r);
		if (ret != -1)
		{
			printf("Read successfull.\n");
			// 読み出し結果の出力
			printf("%s\n", buffer);
		}
		else
		{
			perror("Read error");
		}
	}
	else
	{
		perror("Write error");
	}


	//ret = write(fd_w, &aa, 1);
	//ret = write(fd_w, &a, 1);
	//ret = write(fd_w, &a, 1);
	//close(fd_w);

	printf("Exitting...\n");

	return 0;
}

#endif

#if 0
#include "Player/OmxPlayer.h"
#include <unistd.h>

int main()
{
	//OMXPlayer a;
	//a.Play("/home/pi/recorder/enc/灰と幻想のグリムガル/灰と幻想のグリムガル_リーダーの器じゃないけれど_10.mp4  灰と幻想のグリムガル_生と死の 間で_11.mp4");
	//while (true)
	//{
	//	usleep(1);
	//}
	//sleep(5);
	//a.Close();
}

#endif

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>

#include "Player/OmxPlayer.h"

#define BUTTON_DATA_MAX (1024*1024)
#define STICK_DATA_MAX (1024*1024)

int main()
{
	int fd = open("/dev/input/js0", O_RDONLY);

	OMXPlayer a;
	a.Play("/home/pi/recorder/enc/灰と幻想のグリムガル/灰と幻想のグリムガル_リーダーの器じゃないけれど_10.mp4  灰と幻想のグリムガル_生と死の 間で_11.mp4");

	unsigned char  ButtonData[BUTTON_DATA_MAX];
	signed int     StickData[STICK_DATA_MAX];

	for (;;) {
		struct js_event  event;
		if (read(fd, &event, sizeof(struct js_event)) >= sizeof(struct js_event)) {
			switch (event.type & 0x7f) {
			case JS_EVENT_BUTTON:
				if (event.number < BUTTON_DATA_MAX) {
					ButtonData[event.number] = event.value != 0;
					printf("number=%d value~%d\n", event.number, event.value);
				}
				break;
			case JS_EVENT_AXIS:
				if (event.number < STICK_DATA_MAX) {
					StickData[event.number] = event.value;
				}
				break;
			}
		}
	}
	close(fd);
	a.Close();
}
#endif

#if 0
#include <SDL2/SDL.h>//ここはパスの通し方等で少し変えないといけないかもしれない

#include <cstdio>

int main() {

	SDL_Init(SDL_INIT_VIDEO);//SDL2の初期化

	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);

	SDL_Window *window = SDL_CreateWindow(
		"aaaaa", //ウィンドウタイトル
		0, //ウィンドウ位置x
		0, //y
		mode.w, //幅
		mode.h, //高さ
		SDL_WINDOW_OPENGL //flags 詳細はマニュアル参照

	);

	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	SDL_SetWindowFullscreen(window, SDL_TRUE);

	image = SDL_LoadBMP("sample.bmp");

	SDL_Delay(5000); //delay ms
	SDL_DestroyWindow(window);
	SDL_Quit(); //SDL2のクリーンアップ

	return 0;

}

#endif

#if 0

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#define SAMPLE_SPRITE "sample.png"
#define SAMPLE_FONT "sazanami-gothic.ttf"
#define SAMPLE_FONT_SIZE (32)
#define SAMPLE_STRING "モップmop"

static SDL_Window *gWindow;
static int gWindowWidth;
static int gWindowHeight;

static SDL_Renderer *gRenderer;

static SDL_RWops *gFontRWops;
static TTF_Font *gFont;
static int gFontWidth;
static int gFontHeight;
static SDL_Color gFontColor = { 255, 255, 0 };

static bool initializeFont()
{
	gFontRWops = SDL_RWFromFile(SAMPLE_FONT, "rb");
	if (gFontRWops == nullptr)
		return false;

	gFont = TTF_OpenFontRW(gFontRWops, 0, SAMPLE_FONT_SIZE);
	if (gFont == nullptr)
		goto err1;

	if (TTF_SizeUTF8(gFont, "a", &gFontWidth, &gFontHeight) < 0)
		goto err2;

	return true;

err2:
	TTF_CloseFont(gFont);
err1:
	SDL_RWclose(gFontRWops);
	return false;
}

static void finalizeFont()
{
	TTF_CloseFont(gFont);
	SDL_RWclose(gFontRWops);
}

static bool initialize(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);

	if (TTF_Init() < 0)
		goto err1;

	gWindow = SDL_CreateWindow(
		"aaaaa", //ウィンドウタイトル
		0, //ウィンドウ位置x
		0, //y
		mode.w, //幅
		mode.h, //高さ
		SDL_WINDOW_OPENGL //flags 詳細はマニュアル参照

	);
	SDL_SetWindowFullscreen(gWindow, SDL_TRUE);

	//if (SDL_CreateWindowAndRenderer(width, height, 0,
	//	&gWindow, &gRenderer) < 0)
	//	goto err2;

	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

	if (!initializeFont())
		goto err3;

	SDL_GetWindowSize(gWindow, &gWindowWidth, &gWindowHeight);
	return true;

err3:
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
err2:
	TTF_Quit();
err1:
	SDL_Quit();
	return false;
}

static void finalize()
{
	finalizeFont();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	TTF_Quit();
	SDL_Quit();
}

SDL_Texture *createSprite(const char *spriteName)
{
	SDL_RWops *rwops;
	SDL_Surface *surface;
	SDL_Texture *texture;

	rwops = SDL_RWFromFile(spriteName, "rb");
	if (rwops == nullptr)
		return nullptr;

	surface = IMG_LoadPNG_RW(rwops);
	if (surface == nullptr)
		goto err;

	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);
err:
	SDL_RWclose(rwops);
	return texture;
}

SDL_Texture *createString(const char *string)
{
	SDL_Surface *surface;
	SDL_Texture *texture;

	surface = TTF_RenderUTF8_Solid(gFont, string, gFontColor);
	if (surface == nullptr) {
		std::cerr << "utf8" << std::endl;
		return nullptr;
	}

	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

int main()
{
	SDL_Texture *sprite, *string;
	int ret = 1;

	if (!initialize(400, 400)) {
		std::cerr << "Initialize error" << std::endl;
		return 1;
	}

	sprite = createSprite(SAMPLE_SPRITE);
	if (sprite == nullptr) {
		std::cerr << "Sprite error" << std::endl;
		goto err1;
	}

	string = createString(SAMPLE_STRING);
	if (string == nullptr) {
		std::cerr << "String error" << std::endl;
		goto err2;
	}

	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, sprite, nullptr, nullptr);
	SDL_RenderCopy(gRenderer, string, nullptr, nullptr);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(100000);
	ret = 0;

	SDL_DestroyTexture(string);
err2:
	SDL_DestroyTexture(sprite);
err1:
	finalize();
	return ret;
}

#endif
