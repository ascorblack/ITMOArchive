add_library(argparser ArgParser.cpp)

project(HammingArchiver)
set(SOURCES FileBlockReader.cpp ArchiveFile.cpp HammingArchiver.cpp HammingEncoder.cpp HammingDecoder.cpp ArchiveHeader.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
