# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\uTorrent_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\uTorrent_autogen.dir\\ParseCache.txt"
  "src\\CMakeFiles\\uTorrentSrc_autogen.dir\\AutogenUsed.txt"
  "src\\CMakeFiles\\uTorrentSrc_autogen.dir\\ParseCache.txt"
  "src\\uTorrentSrc_autogen"
  "uTorrent_autogen"
  )
endif()
