from org.sikuli import Tutor
def test():
    print "hello"

def showClick(target_imagename, message):
    tt = Tutor()
    tt.click(target_imagename,message)
    
def tell(target_imagename, message, clip):
   tt = Tutor()
   tt.tell(target_imagename, message, clip)

