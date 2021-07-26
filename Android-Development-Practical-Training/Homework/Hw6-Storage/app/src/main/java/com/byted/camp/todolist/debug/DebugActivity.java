package com.byted.camp.todolist.debug;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.byted.camp.todolist.R;

import java.io.File;
import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.Map;

public class DebugActivity extends AppCompatActivity {

    private static int REQUEST_CODE_STORAGE_PERMISSION = 1001;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug);
        setTitle(R.string.action_debug);

        final Button printBtn = findViewById(R.id.btn_print_path);
        final TextView pathText = findViewById(R.id.text_path);
        printBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                StringBuilder sb = new StringBuilder();
                sb.append("===== Internal Private =====\n").append(getInternalPath())
                        .append("===== External Private =====\n").append(getExternalPrivatePath())
                        .append("===== External Public =====\n").append(getExternalPublicPath());
                pathText.setText(sb);
            }
        });

        final Button permissionBtn = findViewById(R.id.btn_request_permission);
        permissionBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int state = ActivityCompat.checkSelfPermission(DebugActivity.this,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE);
                if (state == PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(DebugActivity.this, "already granted",
                            Toast.LENGTH_SHORT).show();
                    return;
                }
                ActivityCompat.requestPermissions(DebugActivity.this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        REQUEST_CODE_STORAGE_PERMISSION);
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (permissions.length == 0 || grantResults.length == 0) {
            return;
        }
        if (requestCode == REQUEST_CODE_STORAGE_PERMISSION) {
            int state = grantResults[0];
            if (state == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(DebugActivity.this, "permission granted",
                        Toast.LENGTH_SHORT).show();
            } else if (state == PackageManager.PERMISSION_DENIED) {
                Toast.makeText(DebugActivity.this, "permission denied",
                        Toast.LENGTH_SHORT).show();
            }
        }
    }

    private String getInternalPath() {
        Map<String, File> dirMap = new LinkedHashMap<>();
        dirMap.put("cacheDir", getCacheDir());
        dirMap.put("filesDir", getFilesDir());
        dirMap.put("customDir", getDir("custom", MODE_PRIVATE));
        return getCanonicalPath(dirMap);
    }

    private String getExternalPrivatePath() {
        Map<String, File> dirMap = new LinkedHashMap<>();
        dirMap.put("cacheDir", getExternalCacheDir());
        dirMap.put("filesDir", getExternalFilesDir(null));
        dirMap.put("picturesDir", getExternalFilesDir(Environment.DIRECTORY_PICTURES));
        return getCanonicalPath(dirMap);
    }

    private String getExternalPublicPath() {
        Map<String, File> dirMap = new LinkedHashMap<>();
        dirMap.put("rootDir", Environment.getExternalStorageDirectory());
        dirMap.put("picturesDir",
                Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES));
        return getCanonicalPath(dirMap);
    }

    private static String getCanonicalPath(Map<String, File> dirMap) {
        StringBuilder sb = new StringBuilder();
        try {
            for (String name : dirMap.keySet()) {
                sb.append(name)
                        .append(": ")
                        .append(dirMap.get(name).getCanonicalPath())
                        .append('\n');
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return sb.toString();
    }
}
