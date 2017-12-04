package com.bignerdranch.android.netscout_app;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.UUID;

public class SummaryFragment extends Fragment{

    private Summary mSummary;
    private TextView mFragSummary;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        //mSummary = new Summary();
    }

    public static SummaryFragment newInstance(Summary summary){

        SummaryFragment fragment = new SummaryFragment();
        fragment.setSummary(summary);
        return fragment;
    }

    public void setSummary(Summary summary){
        mSummary = summary;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View v = inflater.inflate(R.layout.fragment_summary, container, false);

        mFragSummary = (TextView)v.findViewById(R.id.fragment_summary);
        mFragSummary.setText(mSummary.getBody());

        return v;

    }
}
