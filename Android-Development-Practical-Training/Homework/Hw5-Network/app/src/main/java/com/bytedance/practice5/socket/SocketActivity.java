package com.bytedance.practice5.socket;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.bytedance.practice5.R;

public class SocketActivity extends AppCompatActivity {
    private TextView textView;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_socket);
        textView = findViewById(R.id.tv_response);
        SocketCallback callback =  new SocketCallback() {
            @Override
            public void onResponse(String s) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        textView.setText(s);
                    }
                });
            }
        };
        findViewById(R.id.btn_send).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new ClientSocketThread(callback).start();
            }
        });
    }

    public static interface SocketCallback{
        void onResponse(String s);
    }
}
