/*
 *  glgrab.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <ApplicationServices/ApplicationServices.h>

extern "C" {
CGImageRef grabViaOpenGL(CGDirectDisplayID display, CGRect srcRect);
};