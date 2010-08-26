%module(directors="1") sikuli
%{
#define SWIG_FILE_WITH_INI
#include "sikuli.h"
using namespace sikuli;
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

   
