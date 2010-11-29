cmake_minimum_required(VERSION 2.6)
project(Sikuli)


if (W32)

add_definitions(-DWINDOWS)




find_library(OPENCV_CORE_LIBRARY 
			    NAMES cxcore210 cxcore
			    PATHS C:/OpenCV2.1/lib /opt/local/lib)
find_library(OPENCV_HIGHGUI_LIBRARY 
			    NAMES highgui210 highgui
			    PATHS C:/OpenCV2.1/lib /opt/local/lib)
find_library(OPENCV_CV_LIBRARY 
			    NAMES cv210 cv
			    PATHS C:/OpenCV2.1/lib /opt/local/lib)

set(OPENCV_LIBRARIES 
		     ${OPENCV_CORE_LIBRARY} 
		     ${OPENCV_HIGHGUI_LIBRARY}
		     ${OPENCV_CV_LIBRARY})

option(OPENCV_NEW "use new opencv" NO)

endif (W32)


link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib 
                 ${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)

set(OPENCV_LIBRARIES opencv_contrib
   opencv_legacy opencv_core opencv_imgproc opencv_highgui opencv_objdetect opencv_lapack
   libpng libjpeg zlib)
   
   
if (OPENCV211)

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
endif (OPENCV211)

if (APPLE)

	add_definitions(-DMAC)

	include_directories(/Developer/Headers/FlatCarbon )
	find_library(APP_SERVICES_LIBRARY ApplicationServices)
#	find_library(CARBON_LIBRARY Carbon)      
	find_library(COCOA_LIBRARY Cocoa)      
   
	mark_as_advanced(APP_SERVICES_LIBRARY COCOA_LIBRARY)

   set(APPLE_LIBRARIES 
      ${APP_SERVICES_LIBRARY} 
#      ${CARBON_LIBRARY}
      ${COCOA_LIBRARY})

endif (APPLE)

configure_file(src/config.h.cmake src/config.h)

add_subdirectory(examples)
add_subdirectory(src)

