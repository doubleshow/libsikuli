from org.sikuli import Tutor
def test():
    print "hello"

def showClick(target_imagename, message):
    tt = Tutor()
    tt.click(target_imagename,message)
    
