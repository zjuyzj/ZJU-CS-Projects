package com.bytedance.practice5;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.bytedance.practice5.model.Message;
import com.bytedance.practice5.model.MessageListResponse;
import com.bytedance.practice5.socket.SocketActivity;
import com.facebook.drawee.backends.pipeline.Fresco;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.List;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "chapter5";
    private FeedAdapter adapter = new FeedAdapter();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Fresco.initialize(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        RecyclerView recyclerView = findViewById(R.id.rv_list);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.setAdapter(adapter);
        findViewById(R.id.btn_upload).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,UploadActivity.class);
                startActivity(intent);
            }
        });
        findViewById(R.id.btn_mine).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getData(Constants.STUDENT_ID);
            }
        });

        findViewById(R.id.btn_all).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getData(null);
            }
        });
        findViewById(R.id.btn_socket).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, SocketActivity.class);
                startActivity(intent);
            }
        });

    }

    private void getData(String studentId){
        new Thread(new Runnable(){
            @Override
            public void run() {
                List<Message> msgs=getMessage(studentId);
                if(msgs!=null&&!msgs.isEmpty()){
                    new Handler(getMainLooper()).post(new Runnable() {
                        @Override
                        public void run() {
                            adapter.setData(msgs);
                        }
                    });
                }else{
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(MainActivity.this, "留言板为空", Toast.LENGTH_SHORT).show();
                        }
                    });
                    Log.d("Waring","An empty message list is fetched!");
                }
            }
        }).start();
    }

    private List<Message> getMessage(String studentId){
        List<Message> result=null;
        String urlStr;
        if(studentId==null)
            urlStr="https://api-android-camp.bytedance.com/zju/invoke/messages";
        else
            urlStr=String.format("https://api-android-camp.bytedance.com/zju/invoke/messages?student_id=%s",studentId);
        Log.d("URL",urlStr);
        try{
            URL url=new URL(urlStr);
            HttpURLConnection conn=(HttpURLConnection)url.openConnection();
            conn.setConnectTimeout(6000);
            conn.setRequestMethod("GET");
            conn.setRequestProperty("token",Constants.token);
            if(conn.getResponseCode()==200){
                InputStream is=conn.getInputStream();
                BufferedReader reader=new BufferedReader(new InputStreamReader(is, StandardCharsets.UTF_8));
                MessageListResponse res=new Gson().fromJson(reader, new TypeToken<MessageListResponse>(){}.getType());
                result=res.feeds;
                reader.close();
                is.close();
            }else{
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "列表拉取失败", Toast.LENGTH_SHORT).show();
                    }
                });
                Log.d("Error","Status code is not 200!");
            }
            conn.disconnect();
        }catch(Exception e){
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(MainActivity.this, "网络异常", Toast.LENGTH_SHORT).show();
                }
            });
            e.printStackTrace();
        }
        return result;
    }
}