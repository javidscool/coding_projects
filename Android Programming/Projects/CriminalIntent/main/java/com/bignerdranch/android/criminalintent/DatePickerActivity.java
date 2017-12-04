package com.bignerdranch.android.criminalintent;

import android.content.Context;
import android.content.Intent;
import android.support.v4.app.Fragment;

import java.util.Date;

public class DatePickerActivity extends SingleFragmentActivity {

    private static final String EXTRA_DATE_ID = "com.bignerdranch.anroid.criminalintent.crime_id";

    public static Intent newIntent(Context packageContext, Date date) {
        Intent intent = new Intent(packageContext, DatePickerActivity.class);
        intent.putExtra(EXTRA_DATE_ID, date);
        return intent;
    }

    @Override
    protected Fragment createFragment(){
        Date date = (Date)getIntent().getSerializableExtra(EXTRA_DATE_ID);
        return DatePickerFragment.newInstance(date);
    }

}
