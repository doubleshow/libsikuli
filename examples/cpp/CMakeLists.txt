include_directories("../../src")

add_executable(bejeweled play-bejeweled.cpp)
add_executable(whackamole play-whackamole.cpp)

target_link_libraries(bejeweled sikuli curl 
				${APPLE_LIBRARIES} 
				${OPENCV_LIBRARIES})

target_link_libraries(whackamole sikuli curl 
				${APPLE_LIBRARIES} 
				${OPENCV_LIBRARIES})
