# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\VSCode_Helper_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\VSCode_Helper_autogen.dir\\ParseCache.txt"
  "VSCode_Helper_autogen"
  )
endif()
