package cn.zjuyzj.qvideo;

import androidx.appcompat.app.AppCompatActivity;

import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.res.Configuration;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.Toast;
import android.widget.VideoView;

import com.airbnb.lottie.LottieAnimationView;
import cn.zjuyzj.qvideo.var.variables;

public class PlayerActivity extends AppCompatActivity {
    private float oldYAxis,newYAxis;
    private long lastClickTime=0;
    private MediaController mediaController;
    private VideoView videoView;
    private ImageView imageLike;
    private LottieAnimationView loadingAnimation;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);
        String url=getIntent().getStringExtra("video_url");
        imageLike=findViewById(R.id.img_like);
        videoView=findViewById(R.id.player_video);
        videoView.setVideoURI(Uri.parse(url));
        mediaController=new MediaController(this);
        videoView.setMediaController(mediaController);
        videoView.setAlpha(0);
        loadingAnimation=findViewById(R.id.animation_loading);
        videoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                ObjectAnimator.ofFloat(loadingAnimation, "alpha", 1,0)
                        .setDuration(500)
                        .start();
                loadingAnimation.pauseAnimation();
                ObjectAnimator.ofFloat(videoView, "alpha", 0,1)
                        .setDuration(500)
                        .start();
                videoView.start();
            }
        });
        videoView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch (event.getAction()){
                    case MotionEvent.ACTION_DOWN:
                        if((System.currentTimeMillis()-lastClickTime<250)&&(variables.settingGlobal[1])){
                            ObjectAnimator animatorScaleX = ObjectAnimator.ofFloat(imageLike, "ScaleX", 2,5);
                            animatorScaleX.setDuration(500);
                            ObjectAnimator animatorScaleY = ObjectAnimator.ofFloat(imageLike,"ScaleY",2,5);
                            animatorScaleY.setDuration(500);
                            ObjectAnimator animatorAlpha = ObjectAnimator.ofFloat(imageLike,"alpha",0,1);
                            animatorAlpha.setDuration(500);
                            AnimatorSet animatorSet = new AnimatorSet();
                            animatorSet.playTogether(animatorScaleX);
                            animatorSet.playTogether(animatorScaleY);
                            animatorSet.playTogether(animatorAlpha);
                            animatorSet.start();
                            ObjectAnimator.ofFloat(imageLike, "alpha", 1,0)
                                    .setDuration(500)
                                    .start();
                            Toast.makeText(v.getContext(), "感谢点赞QwQ",Toast.LENGTH_SHORT).show();
                            //Upload LIKE information to server
                            //Waiting for backend to support storing LIKEs for each video
                        }
                        lastClickTime=System.currentTimeMillis();
                        oldYAxis=event.getY();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        newYAxis=event.getY();
                        break;
                    case MotionEvent.ACTION_UP:
                        if(System.currentTimeMillis()-lastClickTime>300){
                            if(!mediaController.isShowing())
                                mediaController.show();
                        }
                        if((newYAxis-oldYAxis>150)&&(variables.settingGlobal[2]))
                            finish();
                        break;
                }
                return true;
            }
        });
        videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                if(variables.settingGlobal[3]){
                    videoView.seekTo(0);
                    videoView.start();
                } else finish();
            }
        });
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }

}