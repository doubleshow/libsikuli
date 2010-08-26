/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.0
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_Sikuli_WRAP_H_
#define SWIG_Sikuli_WRAP_H_

namespace Swig {
  class Director;
}


class SwigDirector_SikuliEventHandler : public sikuli::SikuliEventHandler, public Swig::Director {

public:
    SwigDirector_SikuliEventHandler(VALUE self);
    virtual ~SwigDirector_SikuliEventHandler();
    virtual void handle(sikuli::Event event);
};


class SwigDirector_SikuliUI : public sikuli::SikuliUI, public Swig::Director {

public:
    SwigDirector_SikuliUI(VALUE self);
    virtual ~SwigDirector_SikuliUI();
    virtual int handleFindFailedException(sikuli::Region region, sikuli::Pattern target);
    virtual int handleMatchFound(sikuli::Region region, sikuli::Pattern target, std::vector< sikuli::Match,std::allocator< sikuli::Match > > matches);
};


#endif
