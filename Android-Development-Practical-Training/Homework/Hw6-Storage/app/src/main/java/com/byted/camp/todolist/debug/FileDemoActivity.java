package com.byted.camp.todolist.debug;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.byted.camp.todolist.R;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class FileDemoActivity extends AppCompatActivity {

    private String mFileName = null;

    private EditText editText;
    private TextView tvText;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_demo);

        mFileName =  getFilesDir().getAbsolutePath() + File.separator + "demoFile.txt";

        editText = findViewById(R.id.edit_text);
        tvText = findViewById(R.id.tv_file);

        findViewById(R.id.btn_save).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                saveToFile(editText.getText().toString());
            }
        });

        findViewById(R.id.btn_read).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                readFromFile();
            }
        });

    }

    private void saveToFile(final String content) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                File file = new File(mFileName);
                if (!file.exists()) {
                    try {
                        boolean isSuccess = file.createNewFile();
                        if (!isSuccess) {
                            throw new IOException("create exception error.");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                //
                FileOutputStream outputStream = null;
                try {
                    outputStream = new FileOutputStream(file);
                    outputStream.write(content.getBytes());
                }  catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        if (null != outputStream) {
                            outputStream.close();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    private void readFromFile() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                File file = new File(mFileName);
                if (!file.exists()) {
                    try {
                        boolean isSuccess = file.createNewFile();
                        if (!isSuccess) {
                            throw new IOException("create exception error.");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                try {
                    FileInputStream inputStream = new FileInputStream(file);
                    byte[] bytes = new byte[1024];
                    final StringBuffer sb = new StringBuffer();
                    while (inputStream.read(bytes) != -1) {
                        sb.append(new String(bytes));
                    }
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            tvText.setText(sb.toString());
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        }).start();
    }

}
