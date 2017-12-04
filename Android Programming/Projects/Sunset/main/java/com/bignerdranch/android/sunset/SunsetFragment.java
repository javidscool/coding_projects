package com.bignerdranch.android.sunset;

import android.animation.AnimatorSet;
import android.animation.ArgbEvaluator;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.ShapeDrawable;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AccelerateInterpolator;
import android.widget.ImageView;

import java.nio.channels.SelectionKey;

/**
 * Created by javidscool on 8/21/2015.
 */
public class SunsetFragment extends Fragment {

    private static final String TAG = "Sunset";

    private View mSceneView;
    private ImageView mSunView;
    private ImageView mShadowView;
    private View mSkyView;

    private int mBlueSkyColor;
    private int mSunsetSkyColor;
    private int mNightSkyColor;

    private int mBrightSun;
    private int mHeatSunColor;
    private int mCoolSunColor;

    //for keeping track of the positions of the sun and it's shadow
    private float mSunYCurrent = Float.NaN;
    private float mShadowYCurrent = Float.NaN;

    //for keeping track of the sunset and night sky color transitions
    private int mSunsetSkyCurrentColor = 0;
    private int mNightSkyCurrentColor = 0;

    //for keeping track of pulsate animation
    private long mPulsateCurrentTime = 0;
    private long mPulsatePrevTime = 0;
    private boolean isReversedPulsate = false;

    public static final int DURATION = 3000;

    private boolean isReversedSunset = false;

    AnimatorSet sunriseAnimatorSet = null;
    AnimatorSet sunsetAnimatorSet = null;
    AnimatorSet pulsateAnimatorSet = null;

    public static SunsetFragment newInstance(){
        return new SunsetFragment();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fragment_sunset, container, false);

        mSceneView = view;
        mSunView = (ImageView)view.findViewById(R.id.sun);
        mShadowView = (ImageView)view.findViewById(R.id.shadow);
        mSkyView = view.findViewById(R.id.sky);

        Resources resources = getResources();
        mBlueSkyColor = resources.getColor(R.color.blue_sky);
        mSunsetSkyColor = resources.getColor(R.color.sunset_sky);
        mNightSkyColor = resources.getColor(R.color.night_sky);

        mHeatSunColor = resources.getColor(R.color.heat_sun);
        mCoolSunColor = resources.getColor(R.color.cold_sun);
        mBrightSun = resources.getColor(R.color.bright_sun);

        mSceneView.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                //we check to see if we need to play the sunset or sunrise animations
                if(!isReversedSunset) {
                    startSunsetAnimation();

                    //if we click and our animator set is already running, stop it and set it to null
                    if(sunriseAnimatorSet != null){
                        sunriseAnimatorSet.end();
                        sunriseAnimatorSet = null;
                    }
                }

                else{
                    startSunriseAnimation();

                    //if we click and our animator set is already running, stop it and set it to null
                    if(sunsetAnimatorSet != null){
                        sunsetAnimatorSet.end();
                        sunsetAnimatorSet = null;
                    }
                }


                if(pulsateAnimatorSet != null) {
                    pulsateAnimatorSet.end();
                }
                //start the pulsating animations to change and pulsate the colors of the sun and its reflection
                startSunPulsateAnimation();
                Log.i(TAG, "CLICK!!!");
                //reverse the animation flag so that the proper animation gets played next time
                isReversedSunset = !isReversedSunset;

            }
        });

        return view;
    }


    private void startSunsetAnimation(){

        //we're placing the sun in such a way that it's top is right under the sea
        //we set the start to our saved position if we in fact saved a position
        float sunYStart = (Float.valueOf(mSunYCurrent).isNaN()) ? mSunView.getTop() : mSunYCurrent;
        float sunYEnd = mSkyView.getHeight();

        //similarly for the shadow, we're placing the shadow in such a way
        //so that its bottom is right above the sea line
        //again, we set the shadow start to our saved position if we in fact saved a position
        float shadowYStart = (Float.valueOf(mShadowYCurrent).isNaN()) ? mShadowView.getTop() : mShadowYCurrent;
        float shadowYEnd = -mShadowView.getHeight();

        //we set the duration for how long both the sunset and night animations should last
        float num = Math.abs((float)mSkyView.getHeight() - (float)mSunView.getTop());
        float den = Math.abs((float)mSkyView.getHeight() - mSunYCurrent);
        float process = (DURATION / num);
        float finalAnswer = process * den;
        long duration = (Float.valueOf(mSunYCurrent).isNaN()) ? DURATION :
                (long)(finalAnswer);

        int numColor = Math.abs(mNightSkyColor - mSunsetSkyColor);
        int denColor = Math.abs(mNightSkyColor - mNightSkyCurrentColor);
        float processColor = (DURATION / (float)numColor);
        float finalAnswerColor = processColor * denColor;
        long nightDuration = (mNightSkyCurrentColor == 0) ? DURATION :
                (long)(finalAnswerColor);

        //setting our current sunset and night sky colors because they change dynamically now
        int sunsetSkyColorStart = (mSunsetSkyCurrentColor == 0) ? mBlueSkyColor : mSunsetSkyCurrentColor;
        int nightSkyColorStart = (mNightSkyCurrentColor == 0) ? mSunsetSkyColor : mNightSkyCurrentColor;


        //we first set the animator controlling the sun's height
        ObjectAnimator heightAnimator = ObjectAnimator
                .ofFloat(mSunView, "y", sunYStart, sunYEnd)
                .setDuration(duration);
        heightAnimator.setInterpolator(new AccelerateInterpolator());

        //we set an animator update listener to keep track of the position of the sun every frame
        heightAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mSunYCurrent = (float) animator.getAnimatedValue();
            }
        });

        //then we set the animator controlling the shadow's height
        ObjectAnimator revHeightAnimator = ObjectAnimator
                .ofFloat(mShadowView, "y", shadowYStart, shadowYEnd)
                .setDuration(duration);
        revHeightAnimator.setInterpolator(new AccelerateInterpolator());

        //we also set an animator update listener to keep track of the position of the shadow every frame
        revHeightAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mShadowYCurrent = (float) animator.getAnimatedValue();
            }
        });


        //we set the animation to change the sky to sunset
        //we use lower-case duration here because this is the time during which the sun and shadow set
        ObjectAnimator sunsetSkyAnimator = ObjectAnimator
                .ofInt(mSkyView, "backgroundColor", sunsetSkyColorStart, mSunsetSkyColor)
                .setDuration(duration);
        sunsetSkyAnimator.setEvaluator(new ArgbEvaluator());

        //updating where our color is on the sunset
        sunsetSkyAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mSunsetSkyCurrentColor = (int) animator.getAnimatedValue();
            }
        });


        //we set the animation to change the sunset to night
        //we use nightDuration here because we could be in the middle of night
        ObjectAnimator nightSkyAnimator = ObjectAnimator
                .ofInt(mSkyView, "backgroundColor", nightSkyColorStart, mNightSkyColor)
                .setDuration(nightDuration);
        nightSkyAnimator.setEvaluator(new ArgbEvaluator());

        //updating where our color is on the night
        nightSkyAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mNightSkyCurrentColor = (int) animator.getAnimatedValue();
            }
        });

        //set the animation and play it
        //first we change the color of the sky to sunset while animating the sun and its shadow
        //then we change the color to night
        sunsetAnimatorSet = new AnimatorSet();
        sunsetAnimatorSet
                .play(heightAnimator)
                .with(revHeightAnimator)
                .with(sunsetSkyAnimator)
                .before(nightSkyAnimator);

        sunsetAnimatorSet.start();
    }

    private void startSunriseAnimation(){

        //what we're trying to set up here is to get the sun to come up from right
        //under the sea to its original place
        float sunYStart = (Float.valueOf(mSunYCurrent).isNaN()) ? mSkyView.getHeight() : mSunYCurrent;
        float sunYEnd = mSunView.getTop();

        //also setting shit up so that our reflection comes back downwards to the water
        float shadowYStart = (Float.valueOf(mShadowYCurrent).isNaN()) ? -mShadowView.getHeight() : mShadowYCurrent;
        float shadowYEnd = mShadowView.getTop();

        //we set the duration for how long both the sunset and night animations should last
        float num = Math.abs((float)mSunView.getTop() - (float)mSkyView.getHeight());
        float den = Math.abs((float)mSunView.getTop() - mSunYCurrent);
        float process = (DURATION / num);
        float finalAnswer = process * den;
        long duration = (Float.valueOf(mSunYCurrent).isNaN()) ? DURATION :
                (long)(finalAnswer);

        int numColor = Math.abs(mSunsetSkyColor - mNightSkyColor);
        int denColor = Math.abs(mSunsetSkyColor - mNightSkyCurrentColor);
        float processColor = DURATION / (float)numColor;
        float finalAnswerColor = processColor * denColor;
        long nightDuration = (mNightSkyCurrentColor == 0) ? DURATION :
                (long)(finalAnswerColor);

        //we add this check in case we clicked the screen when the sun hasn't set all the way yet
        //in this case, we don't even need to play the night-to-dawn animation
        if(mSunYCurrent != mSkyView.getHeight()){
            nightDuration = 0;
        }

        //setting our current sunset and night sky colors because they change dynamically now
        int sunsetSkyColorStart = (mSunsetSkyCurrentColor == 0) ? mSunsetSkyColor : mSunsetSkyCurrentColor;
        int nightSkyColorStart = (mNightSkyCurrentColor == 0) ? mNightSkyColor : mNightSkyCurrentColor;



        //we set the animation to change the sky color from night back to dawn
        ObjectAnimator dawnSkyAnimator = ObjectAnimator
                .ofInt(mSkyView, "backgroundColor", nightSkyColorStart, mSunsetSkyColor)
                .setDuration(nightDuration);
        dawnSkyAnimator.setEvaluator(new ArgbEvaluator());

        //updating where our color is on the dawn
        dawnSkyAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mNightSkyCurrentColor = (int) animator.getAnimatedValue();
            }
        });


        //set the animation to change the sky color from dawn to day
        ObjectAnimator sunriseSkyAnimator = ObjectAnimator
                .ofInt(mSkyView, "backgroundColor", sunsetSkyColorStart, mBlueSkyColor)
                .setDuration(duration);
        sunriseSkyAnimator.setEvaluator(new ArgbEvaluator());

        //updating where our color is on the sunrise
        sunriseSkyAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mSunsetSkyCurrentColor = (int) animator.getAnimatedValue();
            }
        });


        //creating our sun animator
        ObjectAnimator heightAnimator = ObjectAnimator
                .ofFloat(mSunView, "y", sunYStart, sunYEnd)
                .setDuration(duration);
        heightAnimator.setInterpolator(new AccelerateInterpolator());

        //updating the position of our sun
        heightAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mSunYCurrent = (float) animator.getAnimatedValue();
            }
        });


        //also creating our shadow animator
        ObjectAnimator revHeightAnimator = ObjectAnimator
                .ofFloat(mShadowView, "y", shadowYStart, shadowYEnd)
                .setDuration(duration);
        revHeightAnimator.setInterpolator(new AccelerateInterpolator());

        //updating the position of our shadow
        revHeightAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mShadowYCurrent = (float) animator.getAnimatedValue();
            }
        });

        //set the animation and play it, similarly designed to the sunset animation
        sunriseAnimatorSet = new AnimatorSet();
        sunriseAnimatorSet
                .play(heightAnimator)
                .with(revHeightAnimator)
                .with(sunriseSkyAnimator)
                .after(dawnSkyAnimator);

        sunriseAnimatorSet.start();
    }

    private void startSunPulsateAnimation(){
        //what we do for both sun and shadow is get the drawable that is within the view,
        //then change the color of that drawable using an object animator

        GradientDrawable sunBackground = (GradientDrawable)mSunView.getDrawable();

        ObjectAnimator sunPulsateAnimator;

        if(isReversedPulsate) {
            sunPulsateAnimator = ObjectAnimator.ofObject(sunBackground, "color",
                    new ArgbEvaluator(), mCoolSunColor, mBrightSun, mHeatSunColor, mBrightSun)
                    .setDuration(2000);

            sunPulsateAnimator.setCurrentPlayTime(mPulsateCurrentTime);
        }
        else {
            sunPulsateAnimator = ObjectAnimator.ofObject(sunBackground, "color",
                    new ArgbEvaluator(), mBrightSun, mHeatSunColor, mBrightSun, mCoolSunColor)
                    .setDuration(2000);

            sunPulsateAnimator.setCurrentPlayTime(mPulsateCurrentTime);
        }

        GradientDrawable shadowBackground = (GradientDrawable)mShadowView.getDrawable();
        ObjectAnimator shadowPulsateAnimator;

        if(isReversedPulsate) {
            shadowPulsateAnimator = ObjectAnimator.ofObject(shadowBackground, "color",
                    new ArgbEvaluator(), mCoolSunColor, mBrightSun, mHeatSunColor, mBrightSun)
                    .setDuration(2000);

            shadowPulsateAnimator.setCurrentPlayTime(mPulsateCurrentTime);
        }
        else {
            shadowPulsateAnimator = ObjectAnimator.ofObject(shadowBackground, "color",
                    new ArgbEvaluator(), mBrightSun, mHeatSunColor, mBrightSun, mCoolSunColor)
                    .setDuration(2000);

            shadowPulsateAnimator.setCurrentPlayTime(mPulsateCurrentTime);
        }

        //we set the repeat mode to be reverse so that we can loop shit properly and don't start at the
        //beginning and make a jump in color
        //we also set the repeat count to infinite so that the pulsating goes on forever
        sunPulsateAnimator.setRepeatMode(ValueAnimator.REVERSE);
        sunPulsateAnimator.setRepeatCount(ValueAnimator.INFINITE);

        shadowPulsateAnimator.setRepeatMode(ValueAnimator.REVERSE);
        shadowPulsateAnimator.setRepeatCount(ValueAnimator.INFINITE);

        //updating where we are on our pulsate
        sunPulsateAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animator) {
                mPulsatePrevTime = mPulsateCurrentTime;

                mPulsateCurrentTime = animator.getCurrentPlayTime();

                //Log.i(TAG, "animator current play time: " + mPulsateCurrentTime);
                //Log.i(TAG, "animator prev play time: " + mPulsatePrevTime);

                if(mPulsatePrevTime >= mPulsateCurrentTime){
                    isReversedPulsate = !isReversedPulsate;
                    //Log.i(TAG, "SWITCH!!!");
                }
            }
        });

        //play the pulsating of the sun and its shadow together
        pulsateAnimatorSet = new AnimatorSet();
        pulsateAnimatorSet
                .play(sunPulsateAnimator)
                .with(shadowPulsateAnimator);

        pulsateAnimatorSet.start();
    }
}