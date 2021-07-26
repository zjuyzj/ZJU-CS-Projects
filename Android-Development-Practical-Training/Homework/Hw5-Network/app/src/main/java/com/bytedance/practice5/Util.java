package com.bytedance.practice5;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class Util {
    public static final byte[] inputStream2bytes(InputStream inStream)
            throws IOException {
        ByteArrayOutputStream swapStream = new ByteArrayOutputStream();
        byte[] buff = new byte[8192];
        int rc = 0;
        while ((rc = inStream.read(buff, 0, 8192)) > 0) {
            swapStream.write(buff, 0, rc);
        }
        byte[] in2b = swapStream.toByteArray();
        inStream.close();
        swapStream.close();
        return in2b;
    }

}
