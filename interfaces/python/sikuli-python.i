%module(directors="1") libsikuli
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
%feature("director") sikuli::SikuliUI;
   
%include "sikuli.h"
%include "keys.h"
%include "pattern.h"
%include "location.h"
%include "region.h"
%include "screen.h"
%include "settings.h"
%include "exceptions.h"
%include "event-manager.h"
%include "ui.h"

