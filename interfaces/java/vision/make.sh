swig -c++ -package vision -java -I../../../src/main/cpp/vision vision.i
g++ -c -fpic vision_wrap.cxx -I/System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Headers/ -I../../../src/main/cpp/vision
g++ -shared vision_wrap.o -o libsikulicv.jnilib -llibcvgui -L../../../build/Debug -framework ApplicationServices -lopencv_core -lopencv_highgui -lopencv_legacy -lopencv_imgproc -llibpng -L../../../lib/opencv -llibjpeg -framework AppKit -lzlib -framework Cocoa -lopencv_objdetect -lopencv_lapack -ltesseract_full
