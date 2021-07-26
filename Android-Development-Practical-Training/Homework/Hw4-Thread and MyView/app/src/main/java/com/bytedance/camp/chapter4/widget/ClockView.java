package com.bytedance.camp.chapter4.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import androidx.annotation.Nullable;

import java.time.Clock;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

public class ClockView extends View {

    private static final int FULL_CIRCLE_DEGREE = 360;
    private static final int UNIT_DEGREE = 6;

    private static final float UNIT_LINE_WIDTH = 8; // 刻度线的宽度
    private static final float TEXT_NUMBER_SIZE = 65; // 数字文本的字号
    private static final int HIGHLIGHT_UNIT_ALPHA = 0xFF;
    private static final int NORMAL_UNIT_ALPHA = 0x80;
    private static final int TEXT_NUMBER_ALPHA = 0xFF;

    private static final float HOUR_NEEDLE_LENGTH_RATIO = 0.4f; // 时针长度相对表盘半径的比例
    private static final float MINUTE_NEEDLE_LENGTH_RATIO = 0.6f; // 分针长度相对表盘半径的比例
    private static final float SECOND_NEEDLE_LENGTH_RATIO = 0.8f; // 秒针长度相对表盘半径的比例
    private static final float HOUR_NEEDLE_WIDTH = 12; // 时针的宽度
    private static final float MINUTE_NEEDLE_WIDTH = 8; // 分针的宽度
    private static final float SECOND_NEEDLE_WIDTH = 4; // 秒针的宽度
    private static final float CIRCLE_STROKE_WIDTH = 10; // 指针装饰圆环边线宽度

    private Calendar calendar = Calendar.getInstance();

    private float radius = 0; // 表盘半径
    private float centerX = 0; // 表盘圆心X坐标
    private float centerY = 0; // 表盘圆心Y坐标

    private List<RectF> unitLinePositions = new ArrayList<>();
    private Paint unitPaint = new Paint();
    private Paint needlePaint = new Paint();
    private Paint numberPaint = new Paint();

    private Handler handler = new Handler();

    public ClockView(Context context) {
        super(context);
        init();
    }

    public ClockView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public ClockView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        unitPaint.setAntiAlias(true);
        unitPaint.setColor(Color.WHITE);
        unitPaint.setStrokeWidth(UNIT_LINE_WIDTH);
        unitPaint.setStrokeCap(Paint.Cap.ROUND);
        unitPaint.setStyle(Paint.Style.STROKE);

        needlePaint.setAntiAlias(true);
        needlePaint.setStrokeCap(Paint.Cap.ROUND);

        numberPaint.setAntiAlias(true);
        numberPaint.setColor(Color.WHITE);
        numberPaint.setTextSize(TEXT_NUMBER_SIZE);
        numberPaint.setStyle(Paint.Style.FILL);
        numberPaint.setAlpha(TEXT_NUMBER_ALPHA);
        numberPaint.setTextAlign(Paint.Align.CENTER);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        configWhenLayoutChanged();
    }

    private void configWhenLayoutChanged() {
        float newRadius = Math.min(getWidth(), getHeight()) / 2f;
        if (newRadius == radius) {
            return;
        }
        radius = newRadius;
        centerX = getWidth() / 2f;
        centerY = getHeight() / 2f;

        for (int degree = 0; degree < FULL_CIRCLE_DEGREE; degree += UNIT_DEGREE) {
            double radians = Math.toRadians(degree);
            float startX = (float) (centerX + (radius * (1 - 0.05f)) * Math.cos(radians));
            float startY = (float) (centerY + (radius * (1 - 0.05f)) * Math.sin(radians));
            float stopX = (float) (centerX + radius * Math.cos(radians));
            float stopY = (float) (centerY + radius * Math.sin(radians));
            unitLinePositions.add(new RectF(startX, startY, stopX, stopY));
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        drawUnit(canvas);
        drawTimeNeedles(canvas);
        drawTimeNumbers(canvas);
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
//                Log.d("handler","Timeout!");
                invalidate(); //重新onDraw()
            }
        }, 100);
    }

    private void drawUnit(Canvas canvas) {
        for (int i = 0; i < unitLinePositions.size(); i++) {
            if (i % 5 == 0) {
                unitPaint.setAlpha(HIGHLIGHT_UNIT_ALPHA);
            } else {
                unitPaint.setAlpha(NORMAL_UNIT_ALPHA);
            }
            RectF linePosition = unitLinePositions.get(i);
            canvas.drawLine(linePosition.left, linePosition.top, linePosition.right, linePosition.bottom, unitPaint);
        }
    }

    private void drawNeedle(Canvas canvas,float needleWidth,float radiusRatio,double degree){
        double rad=Math.toRadians(degree);
        double r=radiusRatio*radius;
        double endX=centerX+Math.cos(rad)*r;
        double endY=centerY+Math.sin(rad)*r;
        needlePaint.setColor(Color.WHITE);
        needlePaint.setStrokeWidth(needleWidth);
        needlePaint.setStyle(Paint.Style.STROKE);
        canvas.drawLine(centerX,centerY,(float)endX,(float)endY,needlePaint);
    }

    private void drawNeedleCircle(Canvas canvas){
        float ring_radius=radius/20.0f;
        needlePaint.setColor(Color.WHITE);
        needlePaint.setStrokeWidth(CIRCLE_STROKE_WIDTH);
        needlePaint.setStyle(Paint.Style.STROKE);
        canvas.drawCircle(centerX, centerY, ring_radius, needlePaint);
        ring_radius-=CIRCLE_STROKE_WIDTH;
        needlePaint.setColor(Color.GRAY);
        needlePaint.setStyle(Paint.Style.FILL_AND_STROKE);
        canvas.drawCircle(centerX, centerY, ring_radius, needlePaint);
    }

    private void drawTimeNeedles(Canvas canvas) {
        Time time = getCurrentTime();
        int hour = time.getHours();
        int minute = time.getMinutes();
        int second = time.getSeconds();
        double angle_hour=FULL_CIRCLE_DEGREE*(hour%12+(double)minute/60)/12-FULL_CIRCLE_DEGREE/4;
        double angle_minute=FULL_CIRCLE_DEGREE*minute/60-FULL_CIRCLE_DEGREE/4;;
        double angle_second=FULL_CIRCLE_DEGREE*second/60-FULL_CIRCLE_DEGREE/4;;
        drawNeedle(canvas,HOUR_NEEDLE_WIDTH,HOUR_NEEDLE_LENGTH_RATIO,angle_hour);
        drawNeedle(canvas,MINUTE_NEEDLE_WIDTH,MINUTE_NEEDLE_LENGTH_RATIO,angle_minute);
        drawNeedle(canvas,SECOND_NEEDLE_WIDTH,SECOND_NEEDLE_LENGTH_RATIO,angle_second);
        drawNeedleCircle(canvas);
     }

    private void drawTimeNumbers(Canvas canvas) {
        double angle;
        for(int i=1;i<=12;i++){
            angle=FULL_CIRCLE_DEGREE*i/12-FULL_CIRCLE_DEGREE/4;
            drawNumber(canvas,i,angle);
        }
    }

    private void drawNumber(Canvas canvas,int number,double degree){
        double rad=Math.toRadians(degree);
        String text=String.format("%02d", number);
        Paint.FontMetrics fontMetrics=numberPaint.getFontMetrics();
        float textHeight=fontMetrics.bottom-fontMetrics.top;
        float textWidth=numberPaint.measureText(text);
        float x= (float) (centerX+Math.cos(rad)*(0.7f*radius+textWidth/2.0f));
        float y= (float) (centerY+Math.sin(rad)*(0.7f*radius+textHeight/2.0f));
        canvas.drawText(text,x,y,numberPaint);
    }

    private Time getCurrentTime() {
        calendar.setTimeInMillis(System.currentTimeMillis());
        return new Time(
                calendar.get(Calendar.HOUR),
                calendar.get(Calendar.MINUTE),
                calendar.get(Calendar.SECOND));
    }

}
