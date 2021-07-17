public class Simple {
    static {
        System.loadLibrary("ThreadNative");
    }

    public static void main(String[] args) {
        System.out.println("main thread begin...");

        new MyThread(() -> System.out.println("my thread method run...")).start();
    }
}