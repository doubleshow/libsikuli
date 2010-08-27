from libsikuli import *
import urllib
import urlparse
import os.path

global_sikuli_image_paths = []

def addImagePath(path):
    global_sikuli_image_paths.append(path)

def removeImagePath(path):
    global_sikuli_image_paths.remove(path)

class pyImageReadHelper(ImageReadHelper):

    def __init__(self):
        ImageReadHelper.__init__(self)

    def resolveImageFilename(self, image_filename):

        if os.path.exists(image_filename):
            return image_filename

        for p in global_sikuli_image_paths:
            relpath = os.path.join(p, image_filename)
            print relpath

            if os.path.exists(relpath):
                return relpath

            if (urlparse.urlparse(relpath).scheme == 'http'):
                try:
                    (filename, headers) = urllib.urlretrieve(relpath)
                    print headers.type
                    if (headers.type == 'image/png' or headers.type == 'image/jpeg'):
                        return filename
                except:
                    pass
 
        raise FileNotFound(image_filename)

global_ImageReadHelper = pyImageReadHelper()
ImageReadHelper.set(global_ImageReadHelper)

#addImagePath("http://www.sikuli.org/images")
#addImagePath("images")
#global_ImageReadHelper.resolveImageFilename("apple.png")
#click("apple.png")
#click("mac.png")

