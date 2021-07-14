/*
 * YangH264VideoDecoderSoft.cpp
 *
 *  Created on: 2020骞�10鏈�20鏃�
 *      Author: yang
 */

#include "YangH264VideoDecoderSoft.h"

//#include "stdlib.h"
extern AVCodec ff_h264_decoder;
extern AVCodecParser ff_h264_parser;



struct YangH264DecContext *getYangH264DecContext(){
	struct YangH264DecContext* cont=(struct YangH264DecContext*)calloc(1,sizeof(struct YangH264DecContext));
	cont->context=(struct YangH264Context*)calloc(1,sizeof(struct YangH264Context));
	return cont;
}


void init_H264DecCont(struct YangH264DecContext *cont,uint8_t *headers, int headerLen) {
	struct YangH264Context *p264=(struct YangH264Context *)cont->context;
	avcodec_register(&ff_h264_decoder);
	av_register_codec_parser(&ff_h264_parser);

	p264->m_codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	p264->m_codecCtx = avcodec_alloc_context3(p264->m_codec);

	p264->m_codecCtx->extradata = (uint8_t*) av_malloc(
			headerLen + AV_INPUT_BUFFER_PADDING_SIZE);
	p264->m_codecCtx->extradata_size = headerLen;
	memcpy(p264->m_codecCtx->extradata, headers, headerLen);

	p264->m_parser = av_parser_init(AV_CODEC_ID_H264);
	if (!p264->m_parser) {
		fprintf(stderr, "Could not create H264 parser\n");
		exit(1);
	}
	int ret = avcodec_open2(p264->m_codecCtx, p264->m_codec, NULL);
	if (ret < 0)
		printf("\navcodec_open2 failure................\n");
	//AVPixelFormat fmt = AV_PIX_FMT_YUV420P;


	av_yang_initExtra(p264->m_parser,p264->m_codecCtx,&cont->m_width,&cont->m_height,&cont->m_fps);
	cont->yLen = cont->m_width * cont->m_height ;
	cont->uLen = cont->yLen / 4;
	cont->allLen = cont->yLen * 3 / 2;
	p264->m_frame = av_frame_alloc();

	av_init_packet(&p264->packet);

	//m_isInit = 1;
}



int decode(struct YangH264DecContext *cont,int isIframe,unsigned char *pData, int nSize,enum YangYuvType pyuvType,
		unsigned char *dest, int *pnFrameReturned) {
	struct YangH264Context *p264=(struct YangH264Context *)cont->context;
	uint8_t *data = NULL;
			int t_size = 0;
			int bytes_used = av_parser_parse2(p264->m_parser, p264->m_codecCtx, &data, &t_size, pData,
					nSize, 0, 0, AV_NOPTS_VALUE);

			if (t_size == 0) {
					 if (bytes_used == nSize) {
				                av_parser_parse2(p264->m_parser, p264->m_codecCtx, &data, &t_size, pData, nSize, 0, 0, AV_NOPTS_VALUE);

				            }
			}
			if (bytes_used > 0) {
						av_init_packet(&p264->packet);
						p264->packet.data = data;
						p264->packet.size = t_size;
						int got_frame = 0;
							int len = avcodec_decode_video2(p264->m_codecCtx, p264->m_frame, &got_frame, &p264->packet);
							if (len < 0) {
								//printf("\ndecoder failure!");
								//  fprintf(stderr, "Error while decoding frame %d\n", picDecodeNumber);
								return 1;
							}

							if (got_frame) {
								//printf("\ndecoder success!");
								if(pyuvType==YangYv12){
									memcpy(dest, p264->m_frame->data[0], cont->yLen);
									memcpy(dest + cont->yLen, p264->m_frame->data[2], cont->uLen);
									memcpy(dest + cont->yLen + cont->uLen, p264->m_frame->data[1], cont->uLen);
								}else if(pyuvType==YangI420){
									memcpy(dest, p264->m_frame->data[0], cont->yLen);
									memcpy(dest + cont->yLen, p264->m_frame->data[1], cont->uLen);
									memcpy(dest + cont->yLen + cont->uLen, p264->m_frame->data[2], cont->uLen);
								}
							   // u8 *u = y + height * width, *v = u + height/2 * width/2;
							 //  for (int i = 0; i < cont->m_height; i++) {
							  //      memcpy(dest + i * cont->m_width, p264->m_frame->data[0] + i * p264->m_frame->linesize[0], cont->m_width);
							 //   }
							 //   for (int i = 0; i < cont->m_height / 2; i++) {
							  //      memcpy(dest + cont->yLen+i * cont->m_width / 2,p264-> m_frame->data[1] + i * p264->m_frame->linesize[1], cont->m_width / 2);
							 //   }
							 //   for (int i = 0; i < cont->m_height / 2; i++) {
							  //      memcpy(dest + cont->yLen+ cont->uLen+ i * cont->m_width / 2, p264->m_frame->data[2] + i * p264->m_frame->linesize[1], cont->m_width / 2);
							  //  }
								//	vpl->putVideo(dest, allLen, ptimestamp);
								*pnFrameReturned = cont->allLen;
							}
					}
			p264=NULL;
			data=NULL;
		return 0;
}


void decode_close(struct YangH264DecContext *cont) {
//    av_parser_close(pCodecParserCtx);
	struct YangH264Context *p264=(struct YangH264Context *)cont->context;
	av_frame_free(&p264->m_frame);
	p264->m_frame = NULL;
	if (p264->m_codecCtx) {
		avcodec_close(p264->m_codecCtx);
		av_free(p264->m_codecCtx);
	}
	//m_codecCtx = NULL;

	p264->m_codecCtx = NULL;
	if(p264->m_parser)	av_parser_close(p264->m_parser);
	p264->m_parser=NULL;
	av_frame_free(&p264->m_frame);
	p264=NULL;
	if(cont){
		if(cont->context) free(cont->context);
		cont->context=NULL;
		free(cont);
		cont=NULL;
	}

//	if (buffer)		av_free(buffer);
//	buffer = NULL;

}

