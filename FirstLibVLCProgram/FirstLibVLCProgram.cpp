// FirstLibVLCProgram.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 避免 ssize_t 引发的错误
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <windows.h>  
#include <vlc/vlc.h>

int main()
{
	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *mediaPlayer;
	libvlc_media_t *media;

	// 等待 20 秒
	int waitTime = 1000 * 20;

	// 创建并初始化 libvlc 实例
	vlcInstance = libvlc_new(0, NULL);

	// 创建一个 media，参数是一个媒体资源位置（例如：有效的 URL）。 
	media = libvlc_media_new_location(vlcInstance, "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_175k.mov");

	// 创建一个 media player 播放环境
	mediaPlayer = libvlc_media_player_new_from_media(media);

	// 现在，不需要保留 media 了
	libvlc_media_release(media);

	// 播放 media player  
	libvlc_media_player_play(mediaPlayer);

	// 让它播放一会
	Sleep(waitTime);

	// 停止播放
	libvlc_media_player_stop(mediaPlayer);

	// 释放 media player  
	libvlc_media_player_release(mediaPlayer);

	// 释放 libvlc 实例
	libvlc_release(vlcInstance);

	return 0;
}