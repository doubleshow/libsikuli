#!/usr/bin/env python

"""

setup.py file for libsikuli Python 

"""

from distutils.core import setup, Extension

sources = ['sikuli-python_wrap.cxx']

sikuli_module = Extension('_sikuli',
                           sources = sources,
                           include_dirs=['../../src'],
                           library_dirs=['../../lib'],
                           extra_link_args=['-Rlib',
                           '-framework,AppplicationServices',
                           '-framework,OpenGL'],
                           libraries=[
                           'opencv_core',
                           'opencv_highgui',
                           'opencv_legacy',
                           'opencv_objdetect',
                           'curl',
                           'sikuli'
                           ]
                           )

setup (name = 'sikuli',
       version = '0.1',
       author      = "Tom Yeh",
       description = """libsikuli Python SWIG interface""",
       ext_modules = [sikuli_module],
       py_modules = ["sikuli"],
       )
