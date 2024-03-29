# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/esp/espressif/v5.0.5/esp-idf/components/bootloader/subproject"
  "E:/espproject/hid_mouse/build/bootloader"
  "E:/espproject/hid_mouse/build/bootloader-prefix"
  "E:/espproject/hid_mouse/build/bootloader-prefix/tmp"
  "E:/espproject/hid_mouse/build/bootloader-prefix/src/bootloader-stamp"
  "E:/espproject/hid_mouse/build/bootloader-prefix/src"
  "E:/espproject/hid_mouse/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/espproject/hid_mouse/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/espproject/hid_mouse/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
