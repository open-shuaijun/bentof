package com.et.bento;

import android.content.Context;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileUtils {

    public static void assetsCopyTo(Context context, String assetsFile, File out) {
        File dest = new File(out, assetsFile);
        if (dest.exists()) return;
        try (InputStream inputStream = context.getAssets().open(assetsFile);
             BufferedInputStream bufferedInputStream = new BufferedInputStream(inputStream);
             FileOutputStream fout = new FileOutputStream(dest);) {
            byte[] buf = new byte[4 * 1024];
            int len;
            while ((len = bufferedInputStream.read(buf)) != -1) {
                fout.write(buf, 0, len);
            }
            fout.flush();
        } catch (IOException e) {
            Log.e("TAG", "拷贝数据报错", e);
        }
    }

}
