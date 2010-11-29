swig -c++ -java -I../../src sikuli-java.i 
g++ -c -fpic sikuli-java_wrap.cxx -I/System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Headers/ -I../../src
g++ -shared sikuli-java_wrap.o -o libsikuli.jnilib -lsikuli -L../../lib -framework ApplicationServices -lopencv_core -lopencv_highgui -lopencv_legacy -lopencv_imgproc -llibpng -L../../lib/opencv -llibjpeg -framework AppKit -lzlib -framework Cocoa -lopencv_objdetect -lopencv_lapack
g++ -shared vision_wrap.o -o sikulicv.jnilib -llibcvgui -L../../build/Debug -framework ApplicationServices -lopencv_core -lopencv_highgui -lopencv_legacy -lopencv_imgproc -llibpng -L../../lib/opencv -llibjpeg -framework AppKit -lzlib -framework Cocoa -lopencv_objdetect -lopencv_lapack -ltesseract_full
