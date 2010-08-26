#!/usr/bin/env python

"""

setup.py file for libsikuli Python 

"""

from distutils.core import setup, Extension
from distutils.util import *

sources = ['sikuli-python_wrap.cxx']

platform = get_platform()

if platform == 'win32':
    opencv_include_dir = 'C:\\OpenCV2.1\\include\opencv'
    opencv_library_dir = 'C:\\OpenCV2.1\\lib'
    opencv_libraries = ['cv210','cxcore210','highgui210']
    os_libraries = ['kernel32','user32','gdi32',
                    'winspool','shell32','ole32',
                    'oleaut32','uuid','comdlg32']
    
sikuli_module = Extension('_sikuli',
                           sources = sources,
                           include_dirs=['../../src',opencv_include_dir],
                           library_dirs=['../../lib',opencv_library_dir],
                           extra_link_args=[
                           '-framework,AppplicationServices'],
                           libraries= ['sikuli'] +
                              opencv_libraries +
                              os_libraries
                           )

setup (name = 'sikuli',
       version = '0.1',
       author      = "Tom Yeh",
       description = """libsikuli Python SWIG interface""",
       ext_modules = [sikuli_module],
       py_modules = ["sikuli"],
       )
