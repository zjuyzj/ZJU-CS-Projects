package com.example.basicui;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RatingBar;
import android.widget.TextView;

public class RateActivity extends AppCompatActivity {
    private int progress;
    private Handler myHandler=new Handler(Looper.getMainLooper());
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("RateActivity","onCreate()");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rate);
        stateCounter counter=new stateCounter();
        TextView txtHello = findViewById(R.id.txtHello);
        TextView txtRating = findViewById(R.id.txtRating);
        Button btnNext = findViewById(R.id.btnNext);
        Button btnRating = findViewById(R.id.btnRating);
        ImageView imgFace = findViewById(R.id.imgFace);
        RatingBar starsRating = findViewById(R.id.starsRating);
        EditText inputName = findViewById(R.id.inputName);
        ProgressBar progressSubmit = findViewById(R.id.progressSubmit);
        txtRating.setVisibility(View.INVISIBLE);
        starsRating.setVisibility(View.INVISIBLE);
        btnRating.setVisibility(View.INVISIBLE);
        inputName.setVisibility(View.INVISIBLE);
        progressSubmit.setVisibility(View.INVISIBLE);
        btnNext.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                switch(counter.stateInc()){
                    case 1:
                        String name=getIntent().getStringExtra("name");
                        if(name.isEmpty()) name="ZJU Student";
                        txtHello.setText("My name is "+name);
                        break;
                    case 2:
                        String id=getIntent().getStringExtra("id");
                        if(id.isEmpty()) id="319xxxxxxx";
                        txtHello.setText("My ID is "+id);
                        break;
                    case 3:
                        txtHello.setVisibility(View.INVISIBLE);
                        btnNext.setVisibility(View.INVISIBLE);
                        txtRating.setVisibility(View.VISIBLE);
                        imgFace.setVisibility(View.VISIBLE);
                        starsRating.setVisibility(View.VISIBLE);
                        inputName.setVisibility(View.VISIBLE);
                        btnRating.setVisibility(View.VISIBLE);
                        break;
                }
            }
        });
        btnRating.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Log.d("Rating","Rating Start");
                progressSubmit.setVisibility(View.VISIBLE);
                progress=0;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        for(progress=0;progress<100;progress++){
                            for(int i=0;i<1000;i++)
                                for(int j=0;j<1000;j++);
                            myHandler.post(new Runnable(){
                                @Override
                                public void run() {
                                    progressSubmit.setProgress(progress);
                                }
                            });
                        }
                        //Log.d("Rating","[Person]"+inputName.getText());
                        //Log.d("Rating","[Stars]"+starsRating.getRating());
                        //Log.d("Rating","Rating End");
                        Intent intent=new Intent();
                        if(inputName.getText().toString().isEmpty()){
                            setResult(RESULT_CANCELED,intent);
                        }else{
                            intent.putExtra("ratingPerson",inputName.getText().toString());
                            intent.putExtra("stars",Math.round(starsRating.getRating()));
                            setResult(RESULT_OK,intent);
                        }
                        finish(); //Destory RateActivity Instance
                    }
                }).start();
            }
        });
    }
    @Override
    protected void onStart(){
        super.onStart();
        Log.d("RateActivity","onStart()");
    }
    @Override
    protected void onRestart(){
        super.onRestart();
        Log.d("RateActivity","onResart()");
    }
    @Override
    protected void onResume(){
        super.onResume();
        Log.d("RateActivity","onResume()");
    }
    @Override
    protected void onPause(){
        super.onPause();
        Log.d("RateActivity","onPause()");
    }
    @Override
    protected void onStop(){
        super.onStop();
        Log.d("RateActivity","onStop()");
    }
    @Override
    protected void onDestroy(){
        super.onDestroy();
        Log.d("RateActivity","onDestroy()");
    }
}