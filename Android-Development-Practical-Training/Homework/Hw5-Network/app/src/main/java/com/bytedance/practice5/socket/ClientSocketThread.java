package com.bytedance.practice5.socket;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;

import com.bytedance.practice5.Util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.net.Socket;

public class ClientSocketThread extends Thread {
    public ClientSocketThread(SocketActivity.SocketCallback callback) {
        this.callback = callback;
    }

    private SocketActivity.SocketCallback callback;

    //head请求内容
    private static String content = "HEAD / HTTP/1.1\r\nHost:www.zju.edu.cn\r\n\r\n";


    @Override
    public void run() {
        String result;
        int receivedLen;
        byte[] buffer=new byte[1024*8];
        try{
            Socket socket=new Socket("www.zju.edu.cn",80);
            BufferedInputStream is = new BufferedInputStream(socket.getInputStream());
            BufferedOutputStream os= new BufferedOutputStream(socket.getOutputStream());
            os.write(content.getBytes());
            os.flush();
            receivedLen=is.read(buffer);
            result=new String(buffer,0,receivedLen);
            Log.d("HEAD",result);
            callback.onResponse(result);
        }catch(Exception e){
            e.printStackTrace();
        }
    }
}