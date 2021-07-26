package com.bytedance.mediademo;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import androidx.annotation.Nullable;

/**
 * @author milo
 * @data 2020/7/26
 */
class PView extends View {

    public PView(Context context) {
        super(context);
    }

    public PView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public PView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public PView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int mode = MeasureSpec.getMode(widthMeasureSpec);
        int i = MeasureSpec.UNSPECIFIED;
        int i2 = MeasureSpec.AT_MOST;
        int i3 = MeasureSpec.EXACTLY;
        Log.e("123","mode:"+mode + " i:" + i + " i2:" +i2+" i3:"+ i3);
    }
}
