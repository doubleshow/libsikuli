/*
 *  exceptions.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>

namespace sikuli{
   
class FindFailed : public exception {
   
public:
   FindFailed(string _msg) : msg(_msg) {};
   FindFailed(Pattern _ptn) : ptn(_ptn) {
      msg = ptn.toString() + " can not be found";
   };
   
   virtual const char* what() const throw(){
      return msg.c_str();
   }
   
   virtual ~FindFailed() throw() {};
   
private:
   
   string msg;
   Pattern ptn;
   
};

class FileNotFound : public exception {
   
public:
   
   FileNotFound(const char* filename) {
      msg = string(filename) + " can not be found";
   }
   
   virtual ~FileNotFound() throw() {};
   
   virtual const char* what() const throw(){
      return msg.c_str();
   }
   
private:
   
   string msg;
};

};

#endif