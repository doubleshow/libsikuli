#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


sikuli_module = Extension('_sikuli',
                           sources=[
                           'sikuli_wrap.cxx', 
                           'region.cpp','screen.cpp',
                           'robot.cpp','glgrab.c',
                           'vision.cpp','finder.cpp','ocr.cpp','cv-util.cpp','pyramid-template-matcher.cpp'],
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
       description = """Simple swig example from docs""",
       ext_modules = [sikuli_module],
       py_modules = ["sikuli"],
       )
