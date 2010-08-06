require 'sikuli'
include Sikuli

def initSikuli
   s = Screen.new
   methods_to_expose = [
      'find','findAll',
      'waitVanish','wait',
      'inside','above','left','right','below','nearby',
      'click','doubleClick','rightClick','hover',
      'drag','dropAt','dragDrop','type',
      'mouseDown','mouseUp','keyDown','keyUp']

   methods_to_expose.each { |method|
      self.class.send(:define_method, method) { |*arg| s.send(method,*arg)}
   }
end
initSikuli


click('apple.png')
type("sys\n")
click('sound.png')
click('mute.png')