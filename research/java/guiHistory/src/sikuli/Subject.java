package sikuli;

public interface Subject {
   public void addObserver(Observer o);
   public void notifyObserver();
}

