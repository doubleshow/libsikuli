
public class test {
   static {
      System.out.println(System.getProperty("java.library.path"));
      System.loadLibrary("sikuli");
   }
   
   public static void main(String argv[]) {
      System.out.println("test");
      Screen s = new Screen();
      sikuli.initOCR("images/arial.png");
      s.click("images/apple.png");
      
      Region r = s.inner(10,10,100,100);
      ScreenImage img = r.capture();
      img.save("t.png");
      //img.show();
      
   }
}
