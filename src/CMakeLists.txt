find_package(curl)

add_library(sikuli region.cpp pattern.cpp location.cpp robot.cpp
event-manager.cpp settings.cpp ui.cpp sikuli.cpp vision.cpp
finder.cpp pyramid-template-matcher.cpp screen.cpp ocr.cpp cv-util.cpp)

add_subdirectory(test)
