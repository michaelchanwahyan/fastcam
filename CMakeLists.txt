cmake_minimum_required(VERSION 3.1.0)

project(fastcam)

include_directories(SYSTEM
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/lib
/usr/include
)

add_compile_options(-std=c++14)

add_library(fastcam_config lib/fastcam/config.cpp)

add_library(fastcam_dec_bayer SHARED lib/fastcam/dec_bayer.cpp)
add_library(fastcam_dec_grey  SHARED lib/fastcam/dec_grey.cpp)
add_library(fastcam_dec_jpeg  SHARED lib/fastcam/dec_jpeg.cpp)
target_link_libraries(fastcam_dec_jpeg
  fastcam_config
)
add_library(fastcam_dec_png   SHARED lib/fastcam/dec_png.cpp)
add_library(fastcam_dec_rgb   SHARED lib/fastcam/dec_rgb.cpp)
add_library(fastcam_dec_s561  SHARED lib/fastcam/dec_s561.cpp)
add_library(fastcam_dec_yuv   SHARED lib/fastcam/dec_yuv.cpp)

add_library(fastcam_dec SHARED lib/fastcam/dec.cpp)
target_link_libraries(fastcam_dec
  fastcam_dec_bayer
  fastcam_dec_grey
  fastcam_dec_jpeg
  fastcam_dec_png
  fastcam_dec_rgb
  fastcam_dec_s561
  fastcam_dec_yuv
)

add_library(fastcam_log SHARED lib/fastcam/log.cpp)
target_link_libraries(fastcam_log
  fastcam_config
)

add_library(fastcam_parse SHARED lib/fastcam/parse.cpp)

add_library(fastcam_src_file SHARED lib/fastcam/src_file.cpp)
add_library(fastcam_src_raw  SHARED lib/fastcam/src_raw.cpp)
add_library(fastcam_src_v4l1 SHARED lib/fastcam/src_v4l1.cpp)
add_library(fastcam_src_v4l2 SHARED lib/fastcam/src_v4l2.cpp)

add_library(fastcam_src SHARED lib/fastcam/src.cpp)
target_link_libraries(fastcam_src
  fastcam_src_file
  fastcam_src_raw
  fastcam_src_v4l1
  fastcam_src_v4l2
  fastcam_parse
)

add_executable (fastcam src/fswebcam.cpp)
target_link_libraries(fastcam
  /usr/lib/x86_64-linux-gnu/libgd.so
  #/usr/lib/arm-linux-gnueabihf/libgd.so
  fastcam_config
  fastcam_dec
  fastcam_log
  fastcam_parse
  fastcam_src
)

