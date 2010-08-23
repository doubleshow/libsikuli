include_directories("../../src")

add_executable(bejeweled play-bejeweled.cpp)
add_executable(whackamole play-whackamole.cpp)

target_link_libraries(bejeweled sikuli curl 
				${APP_SERVICES_LIBRARY} 
				${OPENCV_LIBRARIES})

target_link_libraries(whackamole sikuli curl 
				${APP_SERVICES_LIBRARY} 
				${OPENCV_LIBRARIES})
