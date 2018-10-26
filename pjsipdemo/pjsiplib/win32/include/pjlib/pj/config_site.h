/* Activate Android specific settings in the 'config_site_sample.h' */
#define PJMEDIA_HAS_VIDEO	        1
//#define PJMEDIA_HAS_OPENH264_CODEC	1
#define PJMEDIA_HAS_LIBYUV		1
#define PJMEDIA_VIDEO_DEV_HAS_SDL       1
#define PJMEDIA_VIDEO_DEV_HAS_DSHOW     1
#define PJMEDIA_HAS_FFMPEG	        1
#include <pj/config_site_sample.h>