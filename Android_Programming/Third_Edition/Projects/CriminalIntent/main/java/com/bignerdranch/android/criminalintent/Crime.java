package com.bignerdranch.android.criminalintent;

import android.text.format.DateFormat;

import java.util.Date;
import java.util.Locale;
import java.util.UUID;

/**
 * Created by javidscool on 4/1/2017.
 */

public class Crime {

    private UUID mId;
    private String mTitle;
    private Date mDate;
    private Date mTime;
    private boolean mSolved;
    private boolean mRequiresPolice;
    private String mSuspect;
    private long contactID;

    public String getSuspect() {
        return mSuspect;
    }

    public void setSuspect(String suspect) {
        mSuspect = suspect;
    }

    public UUID getId() {
        return mId;
    }

    public String getTitle() {
        return mTitle;
    }

    public void setTitle(String title) {
        mTitle = title;
    }

    public Date getDate() {
        return mDate;
    }

    public void setDate(Date date) {
        mDate = date;
    }

    public Date getTime() {
        return mTime;
    }

    public void setTime(Date date) {
        mTime = date;
    }

    public String getDateFormatted(Locale locale){
        String dateFormat = new DateFormat().getBestDateTimePattern(locale, "EEEE, MMM dd, yyyy");
        DateFormat df = new DateFormat();
        return df.format(dateFormat, mDate).toString();
    }

    public String getTimeFormatted(Locale locale){
        String dateFormat = new DateFormat().getBestDateTimePattern(locale, "hh:mm a");
        DateFormat df = new DateFormat();
        return df.format(dateFormat, mTime).toString();
    }

    public boolean isSolved() {
        return mSolved;
    }

    public void setSolved(boolean solved){
        mSolved = solved;
    }

    public boolean isRequiresPolice(){
        return mRequiresPolice;
    }

    public void setRequiresPolice(boolean requiresPolice) {
        mRequiresPolice = requiresPolice;
    }

    public Crime(){
        this(UUID.randomUUID());
    }

    public Crime(UUID id){
        mId = id;
        mDate = new Date();
        mTime = new Date();
    }

    public String getPhotoFilename(){
        return "IMG_" + getId().toString() + ".jpg";
    }

    public long getContactID() {
        return contactID;
    }

    public void setContactID(long contactID) {
        this.contactID = contactID;
    }
}
