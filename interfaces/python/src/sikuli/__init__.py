from libsikuli import *
import os

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

sikuli_event_handler_global_array = []
class SikuliCallbackEventHandler(SikuliEventHandler):

    def __init__(self, handler):
        SikuliEventHandler.__init__(self)
        self.handler = handler

    def handle(self, event):
        if len(inspect.getargspec(self.handler).args) == 1:
            self.handler(event)
        else:
            self.handler()


Region_onAppear_old = Region.onAppear
def Region_onAppear(self, target, handler):
    ae = SikuliCallbackEventHandler(handler)

    # Keep a reference to the handler in a 
    # global array. This is a trick to 
    # to keep handler objects in memory 
    # so that they don't get garbage collected 
    # after the call is returned
    sikuli_event_handler_global_array.append(ae)

    return Region_onAppear_old(self, target, ae)
Region.onAppear = Region_onAppear

Region_onVanish_old = Region.onVanish
def Region_onVanish(self, target, handler):
    ae = SikuliCallbackEventHandler(handler)
    sikuli_event_handler_global_array.append(ae)
    return Region_onVanish_old(self, target, ae)
Region.onVanish = Region_onVanish

Region_onChange_old = Region.onChange
def Region_onChange(self, handler):
    ae = SikuliCallbackEventHandler(handler)
    sikuli_event_handler_global_array.append(ae)
    return Region_onChange_old(self, ae)
Region.onChange = Region_onChange


def initSikuli():
   dict = globals()
   dict['SCREEN'] = Screen()
   dict['SCREEN']._exposeAllMethods(__name__)
   #print "Sikuli is initialized." 
   initOCR(determine_path() + "/data/arial.png")

def determine_path():
    root = __file__
    if os.path.islink(root):
        root = os.path.realpath(root)
    return os.path.dirname(os.path.abspath(root))

initSikuli()