package com.byted.camp.todolist.debug;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.byted.camp.todolist.R;

public class SpDemoActivity extends AppCompatActivity {

    private EditText editTextWriteKey;
    private EditText editTextWriteValue;
    private EditText editTextReadKey;
    private TextView tvReadValue;

    private final String SP_DEMO = "sp_demo";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sp_demo);

        editTextWriteKey = findViewById(R.id.edit_key);
        editTextWriteValue = findViewById(R.id.edit_value);
        editTextReadKey = findViewById(R.id.edit_read_key);
        tvReadValue = findViewById(R.id.tv_read);

        findViewById(R.id.btn_save).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String key = editTextWriteKey.getText().toString();
                String value = editTextWriteValue.getText().toString();

                SharedPreferences sp = SpDemoActivity.this.getSharedPreferences(SP_DEMO, MODE_PRIVATE);
                SharedPreferences.Editor editor = sp.edit();
                editor.putString(key, value);
                editor.apply();
            }
        });

        findViewById(R.id.btn_read).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String key = editTextReadKey.getText().toString();
                SharedPreferences sp = SpDemoActivity.this.getSharedPreferences(SP_DEMO, MODE_PRIVATE);
                String value = sp.getString(key, "no value from key = " + key);
                tvReadValue.setText(value);
            }
        });
    }
}
