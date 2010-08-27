#!/usr/bin/env python

"""

setup.py file for libsikuli Python 

"""

from distutils.core import setup, Extension
from distutils.util import *

sources = ['src/sikuli/sikuli-python_wrap.cxx']

platform = get_platform()

if platform == 'win32':
    opencv_include_dir = ['C:\\OpenCV2.1\\include\opencv']
    opencv_library_dir = ['C:\\OpenCV2.1\\lib']
    opencv_libraries = ['cv210','cxcore210','highgui210']
    os_libraries = ['kernel32','user32','gdi32',
                    'winspool','shell32','ole32',
                    'oleaut32','uuid','comdlg32']

if platform.find('macosx') >= 0:
  # opencv_include_dir = ['/usr/local/include/opencv','/opt/local/include/opencv']
   #opencv_library_dir = ['/usr/local/lib','/opt/local/lib']
   opencv_libraries = ['opencv_core','opencv_highgui','opencv_legacy','opencv_objdetect','opencv_imgproc']
   opencv_dependency_libraries = ['libpng','libjpeg','libtiff','opencv_lapack','zlib']
#   opencv_libraries = ['cv','cxcore','highgui',]
   os_libraries = []

sikuli_module = Extension('sikuli/_libsikuli',
                           sources = sources,
                           include_dirs=['../../src','../../include','../../include/opencv'],
                           library_dirs=['../../lib','../../lib/opencv'],#+ opencv_library_dir,
                           extra_link_args=[
                           '-framework ApplicationServices',
                           '-framework AppKit',
                           '-framework OpenGL',
                           '-framework Cocoa',
                           '-framework Foundation'],
                           libraries= ['sikuli'] +
                              opencv_libraries +
                              opencv_dependency_libraries +
                              os_libraries
                           )

setup (name = 'sikuli',
       packages = ['sikuli'],
       package_dir = {'sikuli': 'src/sikuli'},
       package_data = {'sikuli': ['data/*.png']},
       version = '0.1',
       author      = "Tom Yeh",
       description = """libsikuli Python SWIG interface""",
       ext_modules = [sikuli_module],
       )
