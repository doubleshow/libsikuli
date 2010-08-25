cmake_minimum_required(VERSION 2.6)
project(Sikuli)


if (W32)

add_definitions(-DWINDOWS)

find_library(OPENCV_CORE_LIBRARY 
			    NAMES cxcore2.1
			    PATHS C:\\OpenCV2.1\lib)
find_library(OPENCV_HIGHGUI_LIBRARY 
			    NAMES highgui2.1
			    PATHS C:\\OpenCV2.1\lib)
find_library(OPENCV_CV_LIBRARY 
			    NAMES cv2.1
			    PATHS C:\\OpenCV2.1\lib)

set(OPENCV_LIBRARIES 
		     ${OPENCV_CORE_LIBRARY} 
		     ${OPENCV_HIGHGUI_LIBRARY}
		     ${OPENCV_CV_LIBRARY})

option(OPENCV_NEW "use new opencv" NO)

endif (W32)

if (APPLE)

add_definitions(-DMAC)

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

endif (APPLE)

configure_file(src/config.h.cmake src/config.h)

add_subdirectory(examples)
add_subdirectory(src)

