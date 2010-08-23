#require 'gem'
require 'rubygems'
require 'mkrf'

Mkrf::Generator.new('sikuli') do |g|
    g.include_library('opencv_core', 'opencv_highgui','opencv_legacy','opencv_objdetect')
    g.sources = ['src/*.cpp']
end


#require 'mkmf'
#have_library 'opencv_core'
#have_library 'opencv_highgui'
#have_library 'opencv_legacy'
#have_library 'opencv_objdetect'

#create_makefile 'sikuli'