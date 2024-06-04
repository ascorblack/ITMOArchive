add_executable(SearchEngine SearchEngineMain.cpp)
add_executable(Indexer IndexerBin.cpp)
#add_executable(Main main.cpp)

target_link_libraries(SearchEngine PRIVATE SearchEngineSource)
target_link_libraries(Indexer PRIVATE SearchEngineSource)
#target_link_libraries(Main PRIVATE SearchEngineSource)

target_include_directories(SearchEngine PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(Indexer PUBLIC ${PROJECT_SOURCE_DIR})
#target_include_directories(Main PUBLIC ${PROJECT_SOURCE_DIR})

