%module(directors="1") sikuli
%{
#define SWIG_FILE_WITH_INI
#include "sikuli.h"
using namespace sikuli;
%}



%pythoncode
%{
_type = type
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
   if (name == "thisown"): return self.this.own(value)
   if (name == "this"):
      if _type(value).__name__ == 'SwigPyObject':
         self.__dict__[name] = value
         return
   method = class_type.__swig_setmethods__.get(name,None)
   if method: return method(self,value)
   if (not static) or hasattr(self,name):
      self.__dict__[name] = value
   else:
      raise AttributeError("You cannot add attributes to %s" % self)   
%}

%include "std_vector.i"
%template(Matches) std::vector<sikuli::Match>;

%feature("director") sikuli::SikuliEventHandler;   
   
%include "sikuli.h"
%include "keys.h"
%include "pattern.h"
%include "location.h"
%include "region.h"
%include "screen.h"
%include "settings.h"
%include "exceptions.h"
%include "event-manager.h"

   
%pythoncode
%{

import inspect
import sys
import __main__

def Region__enter__(self): 
   self._global_funcs = {}
   for name in dir(self):
      if inspect.ismethod(getattr(self,name)) and __main__.__dict__.has_key(name):
         self._global_funcs[name] = __main__.__dict__[name]
         #print "save " + name + " :" + str(__main__.__dict__[name])
         __main__.__dict__[name] = eval("self."+name)
   return self

def Region__exit__(self, type, value, traceback): 
   for name in self._global_funcs.keys():
      __main__.__dict__[name] = self._global_funcs[name]

Region.__enter__ = Region__enter__
Region.__exit__ = Region__exit__


def Screen_exposeAllMethods(self, mod):
   exclude_list = [ 'class', 'classDictInit', 'clone', 'equals', 'finalize', 
                   'getClass', 'hashCode', 'notify', 'notifyAll', 
                   'toGlobalCoord', 'toString',
                   'capture', 'selectRegion']
   dict = sys.modules[mod].__dict__
   for name in dir(self):
      if inspect.ismethod(getattr(self,name)) \
         and name[0] != '_' and name[:7] != 'super__' and \
         not name in exclude_list:
         #if DEBUG: print "expose " + name
         dict[name] = eval("self."+name)
         #__main__.__dict__[name] = eval("self."+name)
Screen._exposeAllMethods = Screen_exposeAllMethods


def initSikuli():
   dict = globals()
   dict['SCREEN'] = Screen()
   dict['SCREEN']._exposeAllMethods(__name__)
   #print "Sikuli is initialized." 
  
   
initSikuli()
   
%}

