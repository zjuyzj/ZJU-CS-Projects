package com.example.basicui;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class UIActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ui);
        findViewById(R.id.water_btn).setOnClickListener(v -> {
            finish();
        });
    }
}