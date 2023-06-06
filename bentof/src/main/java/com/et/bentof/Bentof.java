package com.et.bentof;

public class Bentof {

    public native String mp4mux();

    public native String mp42hls();

    public native String mp42ts();

    private static class Holder {
        private static final Bentof ins = new Bentof();
    }

    private Bentof() {

    }

    public static Bentof getInstance() {
        return Holder.ins;
    }
}
