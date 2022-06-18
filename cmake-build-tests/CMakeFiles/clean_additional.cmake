# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BitsAndDroidsConnector_autogen"
  "CMakeFiles\\BitsAndDroidsConnector_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BitsAndDroidsConnector_autogen.dir\\ParseCache.txt"
  "tests\\ConnectorTests\\CMakeFiles\\OutputTests_autogen.dir\\AutogenUsed.txt"
  "tests\\ConnectorTests\\CMakeFiles\\OutputTests_autogen.dir\\ParseCache.txt"
  "tests\\ConnectorTests\\OutputTests_autogen"
  )
endif()
