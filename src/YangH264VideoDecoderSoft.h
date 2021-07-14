#ifndef YANGDECODER_SRC_YANGH2645VIDEODECODERFFMPEG_H_
#define YANGDECODER_SRC_YANGH2645VIDEODECODERFFMPEG_H_
#include <stdio.h>
#include <stdlib.h>

#include "ffmpeg/libavcodec/avcodec.h"
#include "YangH264Dec.h"

struct YangH264Context{
	AVCodec *m_codec;
	AVCodecContext *m_codecCtx;
	AVCodecParserContext *m_parser;
	AVPacket packet;
	AVFrame *m_frame;

};


#endif /* YANGDECODER_SRC_YANGH2645VIDEODECODERFFMPEG_H_ */
