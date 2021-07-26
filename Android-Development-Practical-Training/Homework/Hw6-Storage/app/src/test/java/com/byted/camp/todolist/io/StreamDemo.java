package com.byted.camp.todolist.io;

import org.junit.Test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.SimpleTimeZone;
import java.util.logging.SimpleFormatter;

public class StreamDemo {

    @Test
    public void main() {
        inputStream();
        outputStream();
    }

    private void inputStream() {

        String path = System.getProperty("user.dir");

        FileInputStream fis = null; // 声明文件输入流对象
        try {
            fis = new FileInputStream(path + File.separator + "test.txt"); // test.txt文件在当前工程目录下事先创建好
            byte input[] = new byte[30];
            fis.read(input); // 读入到一个字节数组

            String str = new String(input, "UTF-8"); // 字符编码要与读入的文件对应
            System.out.println(str);

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                fis.close(); // 关闭输入流
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void outputStream() {
        FileOutputStream fos = null;
        try {
            File file = new File(System.getProperty("user.dir")+File.separator+"test2.txt");
            if (!file.exists() && !file.createNewFile()) {
                throw new IOException("create file error");
            }
            fos = new FileOutputStream("test2.txt");
            String str = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss a").format(new Date());
            byte[] outStr = str.getBytes("UTF-8"); // 读入字节数组，并指定编码方式
            fos.write(outStr); // 使用文件输出流写出到文件

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

}
