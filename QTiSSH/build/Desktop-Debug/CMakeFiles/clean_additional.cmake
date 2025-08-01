# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/QTiSSH_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/QTiSSH_autogen.dir/ParseCache.txt"
  "QTiSSH_autogen"
  )
endif()
