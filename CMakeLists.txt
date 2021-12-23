cmake_minimum_required(VERSION 2.8)
project(yangh264decoder)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -ffunction-sections -fdata-sections")

# 头文件目录
include_directories(. ffmpeg)

# 发现目录下的源文件
aux_source_directory(. DIR_SRCS)
aux_source_directory(ffmpeg/libavcodec DIR_SRCS)
aux_source_directory(ffmpeg/libavutil DIR_SRCS)

include(utils.cmake)
# 排除不参与编译的文件
list_excluding(DIR_SRCS
    ".*videodsp_template.c"
    ".*simple_idct_template.c"
    ".*hpel_template.c"
    ".*h264qpel_template.c"
    ".*h264pred_template.c"
    ".*h264idct_template.c"
    ".*h264dsp_template.c"
    ".*h264chroma_template.c"
    ".*h264addpx_template.c"
    ".*h264_mc_template.c"
    ".*h264_mb_template.c"
    ".*bit_depth_template.c"
    )

add_library(yangh264decoder ${DIR_SRCS})