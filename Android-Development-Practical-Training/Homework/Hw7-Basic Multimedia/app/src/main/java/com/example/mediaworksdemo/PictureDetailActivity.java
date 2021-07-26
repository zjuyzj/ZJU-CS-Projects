package com.example.mediaworksdemo;

import android.os.Bundle;
import android.widget.Button;
import android.widget.ImageView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.bumptech.glide.Glide;
import static com.bumptech.glide.load.resource.drawable.DrawableTransitionOptions.withCrossFade;
import com.bumptech.glide.load.MultiTransformation;
import com.bumptech.glide.load.resource.bitmap.CenterCrop;
import com.bumptech.glide.load.resource.bitmap.CenterInside;
import com.bumptech.glide.load.resource.bitmap.RoundedCorners;
import com.bumptech.glide.load.resource.drawable.DrawableTransitionOptions;

public class PictureDetailActivity extends AppCompatActivity {

    private static final String successURL="https://www.zju.edu.cn/_upload/article/images/71/23/a3cb28e046c2a6061a517f7c11b9/8b38452e-2c5f-4255-b360-2e6dbe8e7e5b.jpg";
    private static final String failureURL="http://www.zju.edu.cn/1.png";
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_picture_detail);
        ImageView imageView = findViewById(R.id.iv_detail);
        Button btnSuccess = findViewById(R.id.btn_load_success);
        Button btnFailure = findViewById(R.id.btn_load_fail);
        Button btnRound = findViewById(R.id.btn_rounded_corners);

        btnSuccess.setOnClickListener( v -> {
            Glide.with(this).load(successURL)
                    .placeholder(R.drawable.loading_green)
                    .error(R.drawable.error_red)
                    .into(imageView);
        });

        btnFailure.setOnClickListener( v -> {
            Glide.with(this).load(failureURL)
                    .placeholder(R.drawable.loading_green)
                    .error(R.drawable.error_red)
                    .into(imageView);
        });

        btnRound.setOnClickListener( v-> {
            MultiTransformation imgOptions=new MultiTransformation(
                    new CenterInside(),
                    new RoundedCorners(40)
            );
            Glide.with(this).load(successURL)
                    .placeholder(R.drawable.loading_green)
                    .error(R.drawable.error_red)
                    .transition(withCrossFade())
                    .transform(imgOptions)
                    .into(imageView);
        });
    }
}
