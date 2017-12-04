package com.bignerdranch.android.photogallery;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.job.JobInfo;
import android.app.job.JobParameters;
import android.app.job.JobScheduler;
import android.app.job.JobService;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import java.util.List;
import java.util.concurrent.TimeUnit;

/**
 * Created by javidscool on 10/5/2015.
 */
@TargetApi(21)
public class PollJobService extends JobService {

    private static final String TAG = "PollJobService";
    private static final int JOB_ID = 1;

    //FOR NOUGAT DEVICES AND ABOVE THE POLL INTERVAL MUST BE AT LEAST 15 MINUTES
    private static final long POLL_INTERVAL = TimeUnit.MINUTES.toMillis(1); //1 minute
    private static final long BACKOFF_INTERVAL = TimeUnit.MINUTES.toMillis(1); //1 minute

    private PollTask mCurrentTask;

    public static void setJobServiceAlarm(Context context, boolean isOn){

        //get the job scheduler so we can schedule a service
        JobScheduler scheduler = (JobScheduler)context.getSystemService(Context.JOB_SCHEDULER_SERVICE);

        if(isOn){
            //first we check whether or not our job has already been scheduled
            boolean hasBeenScheduled = false;
            for(JobInfo jobInfo : scheduler.getAllPendingJobs()){
                if(jobInfo.getId() == JOB_ID){
                    hasBeenScheduled = true;
                }
            }

            if(!hasBeenScheduled){
                ComponentName serviceName = new ComponentName(context, PollJobService.class);
                JobInfo jobInfo = new JobInfo.Builder(JOB_ID, serviceName)
                        .setRequiredNetworkType(JobInfo.NETWORK_TYPE_UNMETERED)
                        .setPeriodic(POLL_INTERVAL)
                        .setBackoffCriteria(BACKOFF_INTERVAL, JobInfo.BACKOFF_POLICY_LINEAR)
                        .setPersisted(true)
                        .build();
                scheduler.schedule(jobInfo);
            }
        }
        else{
            //we cancel our job if we toggled off the polling
            scheduler.cancel(JOB_ID);
        }

        QueryPreferences.setAlarmOn(context, isOn);
    }

    public static boolean isJobServiceAlarmOn(Context context){

        //get the job scheduler so we can check if the job has been scheduled
        JobScheduler scheduler = (JobScheduler) context.getSystemService(Context.JOB_SCHEDULER_SERVICE);

        boolean hasBeenScheduled = false;
        for(JobInfo jobInfo : scheduler.getAllPendingJobs()){
            if(jobInfo.getId() == JOB_ID){
                hasBeenScheduled = true;
            }
        }

        return hasBeenScheduled;
    }

    @Override
    public boolean onStartJob(JobParameters params){

        //onStartJob is called when Android is ready to run your job
        //and start the JobService

        //JobService runs on the main thread, so we need to
        //move the work off the thread to a background thread
        mCurrentTask = new PollTask(this);
        mCurrentTask.execute(params);

        //returning true means that we're working on the job but that it's not done yet
        return true;
    }

    @Override
    public boolean onStopJob(JobParameters params){

        //onStopJob is called when our job needs to be interrupted and needs to be shut down

        //cancel our current task
        if(mCurrentTask != null){
            mCurrentTask.cancel(true);
        }

        //returning true means that our job should
        //be rescheduled to run again in the future
        return true;
    }

    private boolean isNetworkAvailableAndConnected(){
        ConnectivityManager cm = (ConnectivityManager)getSystemService(CONNECTIVITY_SERVICE);

        boolean isNetworkAvailable = cm.getActiveNetworkInfo() != null;
        boolean isNetworkConnected = isNetworkAvailable && cm.getActiveNetworkInfo().isConnected();

        return isNetworkConnected;
    }

    private void showBackgroundNotification(int requestCode, Notification notification){
        Intent i = new Intent(PollService.ACTION_SHOW_NOTIFICATION);
        i.putExtra(PollService.REQUEST_CODE, requestCode);
        i.putExtra(PollService.NOTIFICATION, notification);
        sendOrderedBroadcast(i, PollService.PERM_PRIVATE, null, null, Activity.RESULT_OK, null, null);
    }

    private class PollTask extends AsyncTask<JobParameters, Void, Void> {

        private Context mContext;
        public PollTask(Context context){
            mContext = context;
        }

        @Override
        protected Void doInBackground(JobParameters... params){
            JobParameters jobParams = params[0];

            //poll Flickr for new images
            if(!isNetworkAvailableAndConnected()){
                //we pass true here because we're not connected to the network
                //and should reschedule the job again at a later time
                jobFinished(jobParams, true);

                return null;
            }

            String query = QueryPreferences.getStoredQuery(mContext);
            String lastResultId = QueryPreferences.getLastResultId(mContext);
            int fetchedPage = QueryPreferences.getFetchedPage(mContext);

            Log.d(TAG, "fetchedPage in the Poll Service is: " + fetchedPage);

            List<GalleryItem> items;

            if(query == null){
                items = new FlickrFetchr().fetchRecentPhotos(fetchedPage);
            }
            else{
                items = new FlickrFetchr().searchPhotos(query, fetchedPage);
            }

            if(items.size() == 0){
                //we pass true here because we didn't get any results
                //and should reschedule the job again at a later time
                jobFinished(jobParams, true);

                return null;
            }

            String resultId = items.get(0).getId();
            if(resultId.equals(lastResultId)){
                Log.i(TAG, "Got an old result: " + resultId);
            }
            else{
                Log.i(TAG, "Got an new result: " + resultId);

                Resources resources = getResources();
                Intent i = PhotoGalleryActivity.newIntent(mContext);
                PendingIntent pi = PendingIntent.getActivity(mContext, 0, i, 0);

                Notification notification = new NotificationCompat.Builder(mContext)
                        .setTicker(resources.getString(R.string.new_pictures_title))
                        .setSmallIcon(android.R.drawable.ic_menu_report_image)
                        .setContentTitle(resources.getString(R.string.new_pictures_title))
                        .setContentText(resources.getString(R.string.new_pictures_text))
                        .setContentIntent(pi)
                        .setAutoCancel(true)
                        .build();

                showBackgroundNotification(0, notification);
            }

            QueryPreferences.setLastResultId(mContext, resultId);

            jobFinished(jobParams, false);

            return null;
        }
    }
}
