package com.example.basicui;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private void initView(){
        findViewById(R.id.toast_btn).setOnClickListener((v -> {
            Toast.makeText(this, "Hi!", Toast.LENGTH_SHORT).show();
        }));
        findViewById(R.id.phone_btn).setOnClickListener(v -> {
            Intent intent=new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse("tel:10086"));
            //intent.setAction(Intent.ACTION_DIAL);
            startActivity(intent);
        });
        findViewById((R.id.web_btn)).setOnClickListener(v -> {
            Intent intent=new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse("http://www.baidu.com"));
            startActivity(intent);
        });
        findViewById(R.id.hello_btn).setOnClickListener(v -> {
            Intent intent=new Intent(MainActivity.this,RateActivity.class);
            //intent.putExtra("name","");
            //intent.putExtra("id","");
            intent.putExtra("name","xxx");
            intent.putExtra("id","319xxxxxxx");
            startActivityForResult(intent,1);
        });
        findViewById(R.id.ui_btn).setOnClickListener(v -> {
            Intent intent=new Intent(MainActivity.this,UIActivity.class);
            startActivity(intent);
        });
        findViewById(R.id.toplist_btn).setOnClickListener(v -> {
            Intent intent=new Intent(MainActivity.this,RecyclerViewActivity.class);
            startActivity(intent);
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,@Nullable Intent data){
        super.onActivityResult(requestCode,resultCode,data);
        if(requestCode!=1) return;
        if(resultCode==RESULT_OK){
            String rating=data.getStringExtra("ratingPerson")+"为我点亮了"+data.getIntExtra("stars",0)+"颗星！";
            Toast.makeText(this,rating,Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(this,"无有效评分QwQ",Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }
}