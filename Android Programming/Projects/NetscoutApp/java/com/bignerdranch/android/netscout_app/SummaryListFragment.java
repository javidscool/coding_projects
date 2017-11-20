package com.bignerdranch.android.netscout_app;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.MenuItemCompat;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;


public class SummaryListFragment extends Fragment {

    private static final String LOG_TAG = "SummaryListFragment";

    private RecyclerView mSummaryRecyclerView;
    private SummaryAdapter mAdapter;

    private SummaryFragment summaryFragment;


    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fragment_summary_list, container, false);

        mSummaryRecyclerView = (RecyclerView)view.findViewById(R.id.summary_recycler_view);
        mSummaryRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));

        updateUI(0, 0);

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater){
        super.onCreateOptionsMenu(menu, inflater);

        inflater.inflate(R.menu.application_menu, menu);

        MenuItem menuItem = menu.findItem(R.id.mySwitches);
        View view = MenuItemCompat.getActionView(menuItem);
        Switch switchA = (Switch) view.findViewById(R.id.switchAForActionBar);
        switchA.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Toast.makeText(getActivity(),
                        "Switch A toggled!", Toast.LENGTH_SHORT).show();
            }
        });
        Switch switchB = (Switch) view.findViewById(R.id.switchBForActionBar);
        switchB.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Toast.makeText(getActivity(),
                        "Switch B toggled!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        switch(item.getItemId()){
            case R.id.refresh:
                Toast.makeText(getActivity(),
                        "Refresh Button toggled!", Toast.LENGTH_SHORT).show();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public void updateUI(int groupPos, int childPos){

        List<Summary> summaries = ((MainActivity)getActivity()).getSummaries(groupPos, childPos);

        if(mAdapter == null) {
            mAdapter = new SummaryAdapter(summaries);
            mSummaryRecyclerView.setAdapter(mAdapter);
        }
        else{
            mAdapter.setSummaries(summaries);
        }

        Log.d( LOG_TAG, "newScaleX in updateUI = " + mAdapter.getCurrentScaleX());
        for(int i = 0; i < mSummaryRecyclerView.getChildCount(); i++) {
            mSummaryRecyclerView.getChildAt(i).setScaleX(mAdapter.getCurrentScaleX());
            mAdapter.notifyDataSetChanged();
        }

    }

    public void moveRecycler(boolean isDrawerClosed, float newScaleX){

        Log.d( LOG_TAG, "recyclerView child count = " + mSummaryRecyclerView.getChildCount());
        Log.d( LOG_TAG, "newScaleX in moveRecycler = " + newScaleX);
        for(int i = 0; i < mSummaryRecyclerView.getChildCount(); i++){
            if(isDrawerClosed) {
                mSummaryRecyclerView.getChildAt(i).setScaleX((1 + newScaleX));
                mAdapter.setCurrentScaleX((1 + newScaleX));
                mAdapter.notifyDataSetChanged();
            }
            else{
                mSummaryRecyclerView.getChildAt(i).setScaleX((newScaleX));
                mAdapter.setCurrentScaleX((newScaleX));
                mAdapter.notifyDataSetChanged();
            }
        }

        Log.d( LOG_TAG, "newScaleX in updateUI AFTER = " + mAdapter.getCurrentScaleX());
        //mSummaryRecyclerView.setTranslationX(moveFactor / 2);
        //mSummaryRecyclerView.setScaleX(newScaleX);
    }

    private class SummaryHolder extends RecyclerView.ViewHolder implements View.OnClickListener{

        private Summary mSummary;

        public TextView mTitleTextView;

        public SummaryHolder(View itemView){
            super(itemView);
            itemView.setOnClickListener(this);

            mTitleTextView = (TextView)itemView.findViewById(R.id.list_item_summary);
        }

        public void bindSummary(Summary summary){
            mSummary = summary;
            mTitleTextView.setText(mSummary.getTitle());
        }

        @Override
        public void onClick(View v){
            FragmentManager fm = getActivity().getSupportFragmentManager();
            summaryFragment = (SummaryFragment)fm.findFragmentById(R.id.detail_fragment_container);

            if(summaryFragment == null){
                summaryFragment = new SummaryFragment();
                summaryFragment.setSummary(mSummary);
                fm.beginTransaction().add(R.id.detail_fragment_container, summaryFragment).commit();
            }
            else{
                Fragment newDetail = SummaryFragment.newInstance(mSummary);

                getActivity().getSupportFragmentManager().beginTransaction()
                        .replace(R.id.detail_fragment_container, newDetail)
                        .commit();
            }
            /*
            Toast.makeText(getActivity(),
                    mSummary.getTitle() + " : " + mSummary.getBody(), Toast.LENGTH_SHORT).show();
                    */
        }
    }


    private class SummaryAdapter extends RecyclerView.Adapter<SummaryHolder>{
        private List<Summary> mSummaries;

        private float currentScaleX;

        public void setCurrentScaleX(float newScaleX){
            currentScaleX = newScaleX;
        }
        public float getCurrentScaleX(){
            return currentScaleX;
        }

        public SummaryAdapter(List<Summary> summaries){
            currentScaleX = 1.0f;
            mSummaries = summaries;
        }

        @Override
        public SummaryHolder onCreateViewHolder(ViewGroup parent, int viewType){
            LayoutInflater layoutInflater = LayoutInflater.from(getActivity());

            View view = layoutInflater.inflate(R.layout.list_item_summary, parent, false);
            view.setScaleX(currentScaleX);

            return new SummaryHolder(view);
        }

        @Override
        public void onBindViewHolder(SummaryHolder holder, int position){
            Summary summary = mSummaries.get(position);
            holder.itemView.setScaleX(currentScaleX);
            holder.bindSummary(summary);
        }

        @Override
        public int getItemCount(){
            return mSummaries.size();
        }

        public void setSummaries(List<Summary> summaries){
            mSummaries = summaries;
        }
    }
}
