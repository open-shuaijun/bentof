package com.et.bentof;

public class Bentof {

    public native String mp4mux(String h264Path, String mp4Path);

    public native String mp42hls(String mp4InputPath,String outDirectory, String outFileName, String outTsName);

    public native String mp42ts();

    private static class Holder {
        private static final Bentof ins = new Bentof();
    }

    private Bentof() {
        System.loadLibrary("bentof");
    }

    public static Bentof getInstance() {
        return Holder.ins;
    }
}
