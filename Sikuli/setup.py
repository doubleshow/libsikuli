#!/usr/bin/env python

"""

setup.py file for libsikuli Python 

"""

from distutils.core import setup, Extension

sources = [
   'sikuli-python_wrap.cxx', 
   'region.cpp','screen.cpp',
   'robot.cpp','glgrab.c',
   'vision.cpp','finder.cpp','ocr.cpp',
   'cv-util.cpp','pyramid-template-matcher.cpp']



sikuli_module = Extension('_sikuli',
                           sources = ['src/' + x for x in sources],
                           extra_link_args=[
                           '-framework,AppplicationServices',
                           '-framework,OpenGL'],
                           libraries=[
                           'opencv_core',
                           'opencv_highgui',
                           'opencv_legacy',
                           'opencv_objdetect'
                           ]
                           )

setup (name = 'sikuli',
       version = '0.1',
       author      = "Tom Yeh",
       description = """libsikuli Python SWIG interface""",
       ext_modules = [sikuli_module],
       py_modules = ["sikuli"],
       )
