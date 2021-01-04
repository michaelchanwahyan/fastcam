cmake_minimum_required(VERSION 3.1.0)

project(fastcam)

include_directories(SYSTEM
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/lib
/usr/include
)

add_compile_options(-std=c++14)

add_library(fastcam_config lib/config.c)

add_library(fastcam_dec_bayer SHARED lib/dec_bayer.c)
add_library(fastcam_dec_grey  SHARED lib/dec_grey.c)
add_library(fastcam_dec_jpeg  SHARED lib/dec_jpeg.c)
target_link_libraries(fastcam_dec_jpeg
  fastcam_config
)
add_library(fastcam_dec_png   SHARED lib/dec_png.c)
add_library(fastcam_dec_rgb   SHARED lib/dec_rgb.c)
add_library(fastcam_dec_s561  SHARED lib/dec_s561.c)
add_library(fastcam_dec_yuv   SHARED lib/dec_yuv.c)

add_library(fastcam_dec SHARED lib/dec.c)
target_link_libraries(fastcam_dec
  fastcam_dec_bayer
  fastcam_dec_grey
  fastcam_dec_jpeg
  fastcam_dec_png
  fastcam_dec_rgb
  fastcam_dec_s561
  fastcam_dec_yuv
)

add_library(fastcam_log SHARED lib/log.c)
target_link_libraries(fastcam_log
  fastcam_config
)

add_library(fastcam_parse SHARED lib/parse.c)

add_library(fastcam_src_file SHARED lib/src_file.c)
add_library(fastcam_src_raw  SHARED lib/src_raw.c)
add_library(fastcam_src_v4l1 SHARED lib/src_v4l1.c)
add_library(fastcam_src_v4l2 SHARED lib/src_v4l2.c)

add_library(fastcam_src SHARED lib/src.c)
target_link_libraries(fastcam_src
  fastcam_src_file
  fastcam_src_raw
  fastcam_src_v4l1
  fastcam_src_v4l2
  fastcam_parse
)

add_executable (fastcam fswebcam.c)
target_link_libraries(fastcam
  /usr/lib/x86_64-linux-gnu/libgd.so
  #/usr/lib/arm-linux-gnueabihf/libgd.so
  fastcam_config
  fastcam_dec
  fastcam_log
  fastcam_parse
  fastcam_src
)

