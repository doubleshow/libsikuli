#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


sikuli_module = Extension('_sikuli',
                           sources=[
                           'sikuli_wrap.cxx', 
                           'region.cpp','robot.cpp',
                           #'ocr.cpp','glgrab.c',
                           'glgrab.c',
                           #'cv-util.cpp','pyramid-template-matcher.cpp',
                           'finder.cpp','ocr.cpp','cv-util.cpp','pyramid-template-matcher.cpp'],
                           #'finder.cpp'],#,'ocr.cpp'],
                           extra_link_args=[
                           '-framework,AppplicationServices',
                           '-framework,OpenGL'],
                           libraries=[
                           'opencv_core.2.1.1',
                           'opencv_highgui.2.1.1',
                           'opencv_legacy.2.1.1',
                           'opencv_objdetect.2.1.1'
                           ]
                           )

setup (name = 'sikuli',
       version = '0.1',
       author      = "Tom Yeh",
       description = """Simple swig example from docs""",
       ext_modules = [sikuli_module],
       py_modules = ["sikuli"],
       )
