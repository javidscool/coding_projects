package com.bignerdranch.android.photogallery;

import android.app.Activity;
import android.app.Notification;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationManagerCompat;
import android.util.Log;

/**
 * Created by javidscool on 4/18/2017.
 */

public class NotificationReceiver extends BroadcastReceiver {

    private static final String TAG = "NotificationReceiver";

    @Override
    public void onReceive(Context c, Intent i){
        Log.i(TAG, "received result: " + getResultCode());

        if(getResultCode() != Activity.RESULT_OK){
            //a foreground activity cancelled the
            //broadcast
            return;
        }

        //note that both PollService and PollJobService use PollService.REQUEST_CODE and PollService.NOTIFICATION
        int requestCode = i.getIntExtra(PollService.REQUEST_CODE, 0);

        Notification notification = (Notification) i.getParcelableExtra(PollService.NOTIFICATION);
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(c);
        notificationManager.notify(requestCode, notification);
    }
}

