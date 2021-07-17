class MyThread implements Runnable {
    private Runnable target;

    public MyThread(Runnable target) {
        this.target = target;
    }

    @Override
    public void run() {
        if (target != null) {
            target.run();
        }
    }

    public synchronized void start() {
        start0();
    }

    private native void start0();
}