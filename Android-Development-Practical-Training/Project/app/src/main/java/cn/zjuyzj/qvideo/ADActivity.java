package cn.zjuyzj.qvideo;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class ADActivity extends AppCompatActivity {
    Button skipButton;
    ImageView imgView;
    private int time=5;
    private boolean isCountDown;
    private boolean isTimerRunning;
    private final int MSG_TIMEOUT=0;
    Handler mHandler=new Handler(Looper.getMainLooper()){
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if(msg.what==MSG_TIMEOUT){
                if(isCountDown==true) time--;
                skipButton.setText("跳过("+time+"秒)");
                if(time==0){
                    startAPP();
                }
            }
        }
    };
    Thread timer=new Thread(new Runnable() {
        @Override
        public void run() {
            try {
                while(isTimerRunning){
                    Thread.sleep(1000);
                    mHandler.sendEmptyMessage(MSG_TIMEOUT);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_advtisement);
        imgView=findViewById(R.id.img_ad);
        imgView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent webIntent = new Intent(Intent.ACTION_VIEW);
                webIntent.setData(Uri.parse("http://www.zju.edu.cn"));
                startActivity(webIntent);
            }
        });
        skipButton=findViewById(R.id.btn_skip);
        skipButton.setText("跳过("+time+"秒)");
        skipButton.setOnClickListener(v -> {
            startAPP();
        });
        isCountDown=true;
        isTimerRunning=true;
        timer.start();
    }

    private void startAPP(){
        timer.interrupt();
        Intent adIntent=new Intent(ADActivity.this,MainActivity.class);
        startActivityForResult(adIntent,0);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        finish();
    }

    @Override
    protected void onPause() {
        super.onPause();
        isCountDown=false;
    }

    @Override
    protected void onResume() {
        super.onResume();
        isCountDown=true;
    }

}