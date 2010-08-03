//#include <iostream>
//
//#include "region.h"
//using namespace sikuli;

//#include "robot.h"
//#import <Cocoa/Cocoa.h>
//#include "glgrab.h"

//#include "ApplicationServices/ApplicationServices.h"
/*
void write_image(CGImageRef imgRef){
   // Create a CFURLRef to the new file
   
   CFStringRef _exportPath = CFSTR("/Users/tomyeh/Desktop");
   CFStringRef thumbnailPath = CFSTR("test1.jpg");
   
   CFURLRef baseURLRef = 
   CFURLCreateWithFileSystemPath(kCFAllocatorDefault, 
                                 _exportPath, 
                                 kCFURLPOSIXPathStyle, true);   
   
   CFURLRef thumbnailFileURLRef = 
   CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, 
                                               thumbnailPath, 
                                               kCFURLPOSIXPathStyle, 
                                               false, baseURLRef);
   
   // Write the image
   
   CGImageDestinationRef thumbnailDestinationRef = 
   CGImageDestinationCreateWithURL(thumbnailFileURLRef, kUTTypeJPEG, 1, NULL);
   
   CGImageDestinationAddImage(thumbnailDestinationRef, imgRef, NULL);
   
   CGImageDestinationFinalize(thumbnailDestinationRef);
   
   
   
   CFRelease(thumbnailFileURLRef);
   
   CFRelease(thumbnailDestinationRef);   
   
   CFRelease(baseURLRef);
}
*/

//#include <opencv2/opencv.hpp>

//#include "opencv/cv.h"
//#include "opencv2/opencv.hpp"
//using namespace cv;


//#include <cxxtest/ErrorPrinter.h>
//
//int main (int argc, char * const argv[]) {
//   
//   CxxTest::ErrorPrinter::runAllTests();
//   return 0;
// //   Rectangle r;
//   //Robot robot;
//   //cv::Mat m = robot.capture();
//   //ScreenImage img(r,m);
//   
//   //Pattern p("/Users/tomyeh/Desktop/target.png");
//   
//   
//   
//   //Pattern p("apple.png");///Users/tomyeh/Desktop/target.png");
//   
//   
////   Region screen;
////   screen.click(Location(10,10),1);
////   screen.click(Location(10,10),1);
//   
//   //screen.click(p,0);
//
//   //sleep(1);
//   //screen.type(p, string("a"), 0);
//               
//   //Match mc = Vision::find(img,p);
//   //robot.mouseMove(mc.getX(),mc.getY());
//   
//   //Region r(0,0,20,20);
//   //r.click(Location(10,10),1);
//   
//   
////   Robot robot;
////   Mat bgr = robot.capture();
////   
////   namedWindow("test", CV_WINDOW_AUTOSIZE);
////   imshow("test", bgr);
////   waitKey(0);
//
//   
///*   
//   Robot r;
////   r.mouseMove(10,10);
////   r.mousePress(0);
////   r.mouseRelease(0);
//// 
////   sleep(1);
////  
////   r.mouseMove(20,100);
////   r.mousePress(0);
////   r.mouseRelease(0);
// 
////   sleep(1);
//   
//   r.mouseMove(1250,10);
// 
//   sleep(1);
//   
//   r.mousePress(0);
//   r.mouseRelease(0);
//
//   sleep(1);
// 
//   r.keyPress(56);   
//   r.keyPress(6);
//   r.keyRelease(6);
//   r.keyRelease(56);
//   
//   
////   
//    // insert code here...
//    std::cout << "Hello, World!\n";*/
//    return 0;
//}
