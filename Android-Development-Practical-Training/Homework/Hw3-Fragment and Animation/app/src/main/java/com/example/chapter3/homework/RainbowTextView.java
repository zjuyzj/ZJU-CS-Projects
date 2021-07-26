package com.example.chapter3.homework;

import android.animation.FloatEvaluator;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Shader;
import android.util.AttributeSet;

import androidx.appcompat.widget.AppCompatTextView;

/**
 * 如何创建自定义 View：https://developer.android.google.cn/guide/topics/ui/custom-components
 * Advanced TextView：https://chiuki.github.io/advanced-android-textview
 */
public class RainbowTextView extends AppCompatTextView {
    private float mRainbowPercent;

    public RainbowTextView(Context context) {
        super(context);
        autoPlay();
    }

    public RainbowTextView(Context context, AttributeSet attrs) {
        super(context, attrs);
        autoPlay();
    }

    public RainbowTextView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        autoPlay();
    }

    public void setRainbowPercent(float rainbowPercent) {
        mRainbowPercent = rainbowPercent;
        updatePaint(getWidth(), rainbowPercent);
    }

    public float getRainbowPercent() {
        return mRainbowPercent;
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        updatePaint(w, mRainbowPercent);
    }

    private void updatePaint(int w, float rainbowPercent) {
        int[] rainbow = getRainbowColors();
        Shader shader = new LinearGradient(0, 0, 0, w, rainbow,
                null, Shader.TileMode.MIRROR);
        Matrix matrix = new Matrix();
        matrix.setRotate(90);
        matrix.postTranslate(w * rainbowPercent, 0);
        shader.setLocalMatrix(matrix);
        getPaint().setShader(shader);
        invalidate();
    }

    private void autoPlay() {
        ObjectAnimator animator = ObjectAnimator.ofFloat(this,
                "rainbowPercent", 0, 100);
        animator.setDuration(60000);
        animator.setRepeatCount(ValueAnimator.INFINITE);
        animator.setEvaluator(new FloatEvaluator());
        animator.start();
    }

    private int[] getRainbowColors() {
        return new int[]{
                getResources().getColor(R.color.rainbow_red),
                getResources().getColor(R.color.rainbow_yellow),
                getResources().getColor(R.color.rainbow_green),
                getResources().getColor(R.color.rainbow_blue),
                getResources().getColor(R.color.rainbow_purple)
        };
    }
}
