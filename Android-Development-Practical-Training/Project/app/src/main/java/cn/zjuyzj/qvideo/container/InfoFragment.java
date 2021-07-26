package cn.zjuyzj.qvideo.container;

import android.animation.ObjectAnimator;
import android.content.Intent;
import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.airbnb.lottie.LottieAnimationView;

import java.util.ArrayList;
import java.util.List;

import cn.zjuyzj.qvideo.PlayerActivity;
import cn.zjuyzj.qvideo.R;
import cn.zjuyzj.qvideo.model.VideoInfo;
import cn.zjuyzj.qvideo.util.NetworkUtility;
import cn.zjuyzj.qvideo.util.StorageUtility;
import cn.zjuyzj.qvideo.var.variables;

public class InfoFragment extends Fragment {
    private boolean isMyself;
    private List<VideoInfo> videoInfo=new ArrayList<VideoInfo>();
    private CoverAdapter videoCoverAdapter;
    private RecyclerView videoWaterFall;
    private LottieAnimationView animationLoading;
    private NetworkUtility netUtil;
    private StorageUtility stoUtil;

    public class UINetCallback implements NetworkUtility.NetCallback{
        @Override
        public void onRequest(List<VideoInfo> videos) {
            if(!variables.searchKey.isEmpty()){
                videoInfo=new ArrayList<VideoInfo>();
                for(int i=0;i<videos.size();i++){
                    if(videos.get(i).getInfo().getString("uname").contains(variables.searchKey))
                        videoInfo.add(videos.get(i));
                }
            }else videoInfo=videos;
            videoCoverAdapter.refreshInfo(videoInfo);
            ObjectAnimator.ofFloat(animationLoading, "alpha",1,0)
                            .setDuration(1000)
                            .start();
            ObjectAnimator.ofFloat(videoWaterFall, "alpha", 0,1)
                            .setDuration(1000)
                            .start();
        }

        public void onError(String reason){
            Toast.makeText(getContext(), reason, Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onRefresh() {
            ObjectAnimator.ofFloat(videoWaterFall, "alpha", 1,0)
                            .setDuration(1000)
                            .start();
            ObjectAnimator.ofFloat(animationLoading, "alpha",0,1)
                            .setDuration(1000)
                            .start();
        }

        @Override
        public void onSuccess() {
            return;
        }
    }

    private class UIInteractCallback implements CoverAdapter.CoverCallback{
        @Override
        public void onClick(String videoURL) {
            Intent videoIntent=new Intent(getActivity(), PlayerActivity.class);
            videoIntent.putExtra("video_url", videoURL);
            startActivity(videoIntent);
        }

        @Override
        public void onLongClick(String videoURL) {
            if(variables.settingGlobal[0])
                stoUtil.downloadVideo(videoURL);
            return;
        }
    }

    private void initView(){
        animationLoading=getView().findViewById(R.id.animation_refresh);
        videoWaterFall=getView().findViewById(R.id.recycler_content);
        videoWaterFall.setHasFixedSize(true);
        videoWaterFall.setLayoutManager(new GridLayoutManager(getView().getContext(),2));
        videoCoverAdapter=new CoverAdapter(videoInfo);
        videoWaterFall.setAdapter(videoCoverAdapter);
        videoCoverAdapter.setCallback(new UIInteractCallback());
    }

    private void initNetwork(){
        netUtil=new NetworkUtility();
        netUtil.initNetwork(new UINetCallback());
        stoUtil=new StorageUtility(getContext());
    }

    public static InfoFragment newInstance(boolean isMyself) {
        InfoFragment fragment = new InfoFragment();
        Bundle args = new Bundle();
        args.putBoolean("is_myself",isMyself);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments()!=null){
            Bundle args=getArguments();
            isMyself=args.getBoolean("is_myself");
        }else isMyself=false;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_info, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initNetwork();
        initView();
        if(isMyself) netUtil.getVideos(variables.myID);
        else netUtil.getVideos("");
    }
}