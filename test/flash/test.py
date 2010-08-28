import unittest
from sikuli import *
import time

Settings.addImagePath("images")
#click("SikuliTester.png")
click("images/TestClick.png")
time.sleep(1)


def switchToTest(x):
    click(x)
    time.sleep(1)

def appear_callback(event):
    print event.pattern.getImageURL() + " appeared!"
    print event.match.toString()
    print event.region.hover(event.match)

def vanish_callback(event):
    print event.pattern.getImageURL() + " vanished!"

def flower_vanish_callback():
    print "flower vanished!"

def appear_and_stop_callback(event):
    print event.pattern.getImageURL() + " appeared!"
    EventManager.stop()
    print "stopping event manager.."

def change_callback(event):
    print "change is detected"

class TestFlash(unittest.TestCase):
    def setUp(self):
        pass
        
    def tearDown(self):
        time.sleep(1)
        
    def test_click(self):
        switchToTest("TestClick.png")
        click("clickHere.png")
        self.assertTrue(exists("success.png"))

    def test_doubleClick(self):
        switchToTest("TestDoubleClick.png")
        doubleClick("clickHere.png")
        self.assertTrue(exists("success.png"))

    def test_rightClick(self):
        rightClick("clickHere.png")
        self.assertTrue(exists("about_adobe_flash_player.png"))
        press(ESC)

    def test_type(self):
        switchToTest("TestType.png")
        type("textinput.png","abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ !@#$%^&*()_+ 1234567890-=`~;':,./<>?\"[]{}\\|")
        self.assertTrue(exists("success.png"))
        
    def test_keys(self):
        switchToTest("TestKeys.png")
        click("textinput.png");
        press(DOWN)
        press(UP)
        press(LEFT)
        press(RIGHT)
        press(F1)
        press(F2)
        press(F3)
        press(F4)
        press(F5)
        press(F6)
        press(F7)
        press(F8)
        press(F9)
        press(F10)
        press(F11)
        press(F12)
        press(BACKSPACE)
        press(DELETE)
        press(END)
        press(ENTER)
        press(ESC)
        press(PAGE_DOWN)
        press(PAGE_UP)
        press(SPACE)
        press(CAPSLOCK)
        press(INSERT)
        press(HOME)
        press(TAB)

   
    def test_OCR(self):
        switchToTest("TestOCR.png")
        initOCR("arial.png")
        click("/System Preferences/")
        click("/Configuration/")
        click("/Keyboard/")
        click("/My Favorites/")

    def test_dragDrop(self):
        switchToTest("TestDragDrop.png")
        dragDrop("hslider_thumb.png", "dropHere.png")
        self.assertTrue(exists("success.png"))

    def test_findAll(self):
        switchToTest("TestFindAll.png")
        ms = findAll("flower.png")
        self.assertEqual(len(ms), 6)

        ms = findAll("folder.png")
        self.assertEqual(len(ms), 5)

        ms = findAll("checkbox.png")
        self.assertEqual(len(ms), 5)

        for m in ms:
            click(m)

        self.assertTrue(exists("success.png"))


    def test_vanish(self):
        switchToTest("TestVanish.png")
        waitVanish("flower.png")
        self.assertTrue(not exists("flower.png",0))

    def test_spatialOp(self):
        switchToTest("TestSpatialOp.png")
      
        computer = find("computer.png")
        hover(computer)
      
        computer.left().find("clickButton.png").click(Location(10,10))
        computer.below().find("clickButton.png").click(Location(10,10))
        computer.right().find("clickButton.png").click(Location(10,10))
        computer.above().find("clickButton.png").click(Location(10,10))
      
        self.assertTrue(exists("success.png"))


    def test_ordering(self):
        switchToTest("TestOrdering.png")

        p = Pattern("button.png").leftMost()
        click(p)
        p = Pattern("button.png").bottomMost()
        click(p)
        p = Pattern("button.png").rightMost()
        click(p)
        p = Pattern("button.png").topMost()
        click(p)
      
        p = Pattern("button.png").order(LEFTRIGHT)
        click(findAll(p)[2]);
        p = Pattern("button.png").order(BOTTOMUP)
        click(findAll(p)[1]);
        p = Pattern("button.png").order(RIGHTLEFT)
        click(findAll(p)[2]);
        p = Pattern("button.png").order(TOPDOWN)
        click(findAll(p)[1])

        self.assertTrue(exists("success.png"))
      
    def test_paste(self):
        switchToTest("TestPaste.png")
        paste("paste_target.png", "paSte tHis text here!!!")
        self.assertTrue(exists("success.png"))
  
    def test_event(self):
        switchToTest("TestEvent.png")
        m = find("SikuliTester.png")
        r = inner(m.x,m.y,230,250)

        r.onChange(change_callback)
    
        r.onAppear(Pattern("computer.png"), appear_callback)
        r.onVanish(Pattern("computer.png"), vanish_callback)

        r.onAppear(Pattern("flower.png"), appear_callback)
        r.onVanish(Pattern("flower.png"), flower_vanish_callback)

        r.onAppear(Pattern("bug.png"), appear_and_stop_callback)

        EventManager.observe(20)
  
if __name__ == '__main__':
    unittest.main()
