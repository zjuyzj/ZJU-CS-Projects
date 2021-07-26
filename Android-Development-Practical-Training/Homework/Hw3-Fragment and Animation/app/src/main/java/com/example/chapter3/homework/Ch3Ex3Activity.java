package com.example.chapter3.homework;

import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentPagerAdapter;
import androidx.viewpager.widget.ViewPager;

import com.google.android.material.tabs.TabLayout;

public class Ch3Ex3Activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ch3ex3);

        ViewPager friends_list=findViewById(R.id.friends_container);
        TabLayout friends_tab=findViewById(R.id.friends_tab);
        friends_list.setOffscreenPageLimit(1);
        friends_list.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public int getCount() {
                return 3;
            }
            @Override
            public Fragment getItem(int i) {
                return PlaceholderFragment.newInstance(i+1);
            }
            @Nullable
            @Override
            public CharSequence getPageTitle(int position) {
                return "Group "+(position+1);
            }
        });
        friends_tab.setupWithViewPager(friends_list);
    }

}
