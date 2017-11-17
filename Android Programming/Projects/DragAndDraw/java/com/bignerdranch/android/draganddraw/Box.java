package com.bignerdranch.android.draganddraw;

import android.graphics.PointF;

/**
 * Created by javidscool on 4/18/2017.
 */

public class Box {
    private PointF mOrigin;
    private PointF mCurrent;

    public PointF getCurrent() {
        return mCurrent;
    }

    public void setCurrent(PointF current) {
        mCurrent = current;
    }

    public PointF getOrigin() {
        return mOrigin;
    }

    public Box(PointF origin){
        mOrigin = origin;
        mCurrent = origin;
    }
}
