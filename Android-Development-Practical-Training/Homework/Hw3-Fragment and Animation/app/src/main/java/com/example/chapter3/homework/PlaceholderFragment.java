package com.example.chapter3.homework;


import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.FrameLayout;
import android.widget.ListView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.airbnb.lottie.LottieAnimationView;

import java.util.ArrayList;
import java.util.List;

public class PlaceholderFragment extends Fragment {
    private int groupID;

    public static PlaceholderFragment newInstance(int groupID){
        PlaceholderFragment instance=new PlaceholderFragment();
        Bundle args=new Bundle();
        args.putInt("groupID",groupID);
        instance.setArguments(args);
        return instance;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Bundle args=getArguments();
        this.groupID=args.getInt("groupID");
        return inflater.inflate(R.layout.fragment_placeholder, container, false);
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        //不要在这里获取View，此时View可能还没有创建
    }

    @Override
    public void onViewCreated(@NonNull @org.jetbrains.annotations.NotNull final View view, @Nullable @org.jetbrains.annotations.Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        view.postDelayed(new Runnable() {
            @Override
            public void run() {
                List<String> friends_data = new ArrayList<String>();
                for(int i=0;i<20;i++)
                    friends_data.add("测试数据 Group "+groupID+"-"+(i+1));
                ArrayAdapter<String> friends_adapter=new ArrayAdapter<String>(getContext(), android.R.layout.simple_list_item_1,friends_data);
                LottieAnimationView animationLoading=view.findViewById(R.id.friends_loading);
                ListView friendsList=view.findViewById(R.id.friends_list);
                friendsList.setAdapter(friends_adapter);
                ObjectAnimator animatorLoading = ObjectAnimator.ofFloat(animationLoading,"alpha",1,0);
                animatorLoading.setDuration(1000);
                animatorLoading.start();
                ObjectAnimator animatorList=ObjectAnimator.ofFloat(friendsList, "alpha",0,1);
                animatorList.setDuration(1000);
                animatorList.start();
            }
        }, 5000);
    }

}
