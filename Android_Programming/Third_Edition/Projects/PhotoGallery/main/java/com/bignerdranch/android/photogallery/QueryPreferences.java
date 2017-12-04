package com.bignerdranch.android.photogallery;

import android.content.Context;
import android.preference.PreferenceManager;

/**
 * Created by javidscool on 4/17/2017.
 */

public class QueryPreferences {

    private static final String PREF_SEARCH_QUERY = "searchQuery";
    private static final String PREF_LAST_RESULT_ID = "lastResult";
    private static final String PREF_IS_ALARM_ON = "isAlarmOn";
    private static final String PREF_FETCHED_PAGE = "fetchedPage";

    public static String getStoredQuery(Context context){

        return PreferenceManager.getDefaultSharedPreferences(context)
                                .getString(PREF_SEARCH_QUERY, null);
    }

    public static void setStoredQuery(Context context, String query){

        PreferenceManager.getDefaultSharedPreferences(context)
                            .edit()
                            .putString(PREF_SEARCH_QUERY, query)
                            .apply();
    }

    public static String getLastResultId(Context context){

        return PreferenceManager.getDefaultSharedPreferences(context)
                                .getString(PREF_LAST_RESULT_ID, null);
    }

    public static void setLastResultId(Context context, String query){

        PreferenceManager.getDefaultSharedPreferences(context)
                            .edit()
                            .putString(PREF_LAST_RESULT_ID, query)
                            .apply();
    }

    public static boolean isAlarmOn(Context context){
        return PreferenceManager.getDefaultSharedPreferences(context)
                                .getBoolean(PREF_IS_ALARM_ON, false);
    }

    public static void setAlarmOn(Context context, boolean isOn){
        PreferenceManager.getDefaultSharedPreferences(context)
                            .edit()
                            .putBoolean(PREF_IS_ALARM_ON, isOn)
                            .apply();
    }

    public static int getFetchedPage(Context context){
        return PreferenceManager.getDefaultSharedPreferences(context)
                                .getInt(PREF_FETCHED_PAGE, 1);
    }

    public static void setFetchedPage(Context context, int fetchedPage){
        PreferenceManager.getDefaultSharedPreferences(context)
                            .edit()
                            .putInt(PREF_FETCHED_PAGE, fetchedPage)
                            .apply();
    }

}
