cmake_minimum_required(VERSION 2.6)
project(Sikuli)

find_package(curl)

find_library(OPENCV_CORE_LIBRARY 
			    NAMES opencv_core 
			    PATHS /usr/local/lib)
find_library(OPENCV_HIGHGUI_LIBRARY 
			    NAMES opencv_highgui 
			    PATHS /usr/local/lib)
find_library(OPENCV_LEGACY_LIBRARY 
			    NAMES opencv_legacy 
			    PATHS /usr/local/lib)
find_library(OPENCV_IMGPROC_LIBRARY 
			    NAMES opencv_imgproc
			    PATHS /usr/local/lib)
find_library(OPENCV_OBJDETECT_LIBRARY 
			    NAMES opencv_objdetect
			    PATHS /usr/local/lib)

set(OPENCV_LIBRARIES 
		     ${OPENCV_CORE_LIBRARY} 
		     ${OPENCV_HIGHGUI_LIBRARY}
		     ${OPENCV_LEGACY}
		     ${OPENCV_IMGPROC_LIBRARY}
		     ${OPENCV_OBJDETECT_LIBRARY})

include_directories(/Developer/Headers/FlatCarbon )
find_library(APP_SERVICES_LIBRARY ApplicationServices)
mark_as_advanced(APP_SERVICES_LIBRARY)

add_subdirectory(examples)
add_subdirectory(src)

