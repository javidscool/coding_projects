package com.bignerdranch.android.draganddraw;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PointF;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by javidscool on 4/18/2017.
 */

public class BoxDrawingView extends View {
    private static final String TAG = "BoxDrawingView";

    private Box mCurrentBox;
    private List<Box> mBoxen = new ArrayList<>();

    private Paint mBoxPaint;
    private Paint mBackgroundPaint;

    //used when creating the view in code
    public BoxDrawingView(Context context){
        this(context, null);
    }

    //used when inflating the view from XML
    public BoxDrawingView(Context context, AttributeSet attrs){
        super(context, attrs);

        //paint the boxes a nice semitransparent red (ARGB)
        mBoxPaint = new Paint();
        mBoxPaint.setColor(0x22ff0000);

        //paint the background off-white
        mBackgroundPaint = new Paint();
        mBackgroundPaint.setColor(0xfff8efe0);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event){
        PointF current = new PointF(event.getX(), event.getY());
        String action = "";

        switch(event.getAction()){
            case MotionEvent.ACTION_DOWN:
                action = "ACTION_DOWN";

                //reset drawing state
                mCurrentBox = new Box(current);
                mBoxen.add(mCurrentBox);

                break;
            case MotionEvent.ACTION_MOVE:
                action = "ACTION_MOVE";

                if(mCurrentBox != null){
                    mCurrentBox.setCurrent(current);
                    invalidate();
                }

                break;
            case MotionEvent.ACTION_UP:
                action = "ACTION_UP";

                mCurrentBox = null;

                break;
            case MotionEvent.ACTION_CANCEL:
                action = "ACTION_CANCEL";

                mCurrentBox = null;

                break;
        }

        Log.i(TAG, action + " at x=" + current.x + ", y=" + current.y);

        return true;
    }

    @Override
    protected void onDraw(Canvas canvas){
        //fill the background
        canvas.drawPaint(mBackgroundPaint);

        for(Box box : mBoxen){

            float left = Math.min(box.getOrigin().x, box.getCurrent().x);
            float right = Math.max(box.getOrigin().x, box.getCurrent().x);
            float top = Math.min(box.getOrigin().y, box.getCurrent().y);
            float bottom = Math.max(box.getOrigin().y, box.getCurrent().y);

            canvas.drawRect(left, top, right, bottom, mBoxPaint);
        }
    }

    @Override
    protected Parcelable onSaveInstanceState(){
        //YOU MUST SET AN +id ON YOUR VIEW IN THE XML FILE OR SAVING WON'T WORK
        Bundle args = new Bundle();
        args.putParcelable("instanceState", super.onSaveInstanceState());
        args.putParcelableArrayList("boxArray", (ArrayList) mBoxen);
        return args;
    }

    @Override
    public void onRestoreInstanceState(Parcelable state){

        if (state instanceof Bundle) {
            Bundle bundle = (Bundle) state;
            mBoxen = (ArrayList)bundle.getParcelableArrayList("boxArray");
            super.onRestoreInstanceState(bundle.getParcelable("instanceState"));
            return;
        }
        else {
            super.onRestoreInstanceState(state);
        }
    }
}