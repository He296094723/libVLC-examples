// ���� ssize_t �����Ĵ���
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <windows.h>
#include <vlc/vlc.h>
#include <QImage>
#include <QMutex>
#include <QCoreApplication>

// ���������Ƶ�ķֱ���
#define VIDEO_WIDTH   640
#define VIDEO_HEIGHT  480

struct context {
	QMutex mutex;
	uchar *pixels;
};

static void *lock(void *opaque, void **planes)
{
	struct context *ctx = (context *)opaque;
	ctx->mutex.lock();

	// ���� VLC ����������ݷŵ���������
	*planes = ctx->pixels;

	return NULL;
}

// ��ȡ argb ͼƬ�����浽�ļ���
static void unlock(void *opaque, void *picture, void *const *planes)
{
	Q_UNUSED(picture);

	struct context *ctx = (context *)opaque;
	unsigned char *data = (unsigned char *)*planes;
	static int frameCount = 1;

	QImage image(data, VIDEO_WIDTH, VIDEO_HEIGHT, QImage::Format_ARGB32);
	image.save(QString("frame_%1.png").arg(frameCount++));

	ctx->mutex.unlock();
}

static void display(void *opaque, void *picture)
{
	Q_UNUSED(picture);

	(void)opaque;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *mediaPlayer;
	libvlc_media_t *media;

	// �ȴ� 20 ��
	int waitTime = 1000 * 20;

	struct context ctx;
	ctx.pixels = new uchar[VIDEO_WIDTH * VIDEO_HEIGHT * 4];
	memset(ctx.pixels, 0, VIDEO_WIDTH * VIDEO_HEIGHT * 4);

	// ��������ʼ�� libvlc ʵ��
	vlcInstance = libvlc_new(0, NULL);

	// ����һ�� media��������һ��ý����Դλ�ã����磺��Ч�� URL����
	media = libvlc_media_new_location(vlcInstance, "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_175k.mov");

	libvlc_media_add_option(media, ":avcodec-hw=none");

	// ����һ�� media player ���Ż���
	mediaPlayer = libvlc_media_player_new_from_media(media);

	// ���ڣ�����Ҫ���� media ��
	libvlc_media_release(media);

	// ���ûص���������ȡ֡��������Ļ����ʾ��
	libvlc_video_set_callbacks(mediaPlayer, lock, unlock, display, &ctx);
	libvlc_video_set_format(mediaPlayer, "RGBA", VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH * 4);

	// ���� media player
	libvlc_media_player_play(mediaPlayer);

	// ��������һ��
	Sleep(waitTime);

	// ֹͣ����
	libvlc_media_player_stop(mediaPlayer);

	// �ͷ� media player
	libvlc_media_player_release(mediaPlayer);

	// �ͷ� libvlc ʵ��
	libvlc_release(vlcInstance);

	return a.exec();
}
