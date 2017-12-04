package com.bignerdranch.android.criminalintent;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;
import java.util.Locale;
import java.util.UUID;

/**
 * Created by javidscool on 4/4/2017.
 */

public class CrimeListFragment extends Fragment {

    private static final String TAG = "CrimeListFragment";

    private static final String SAVED_SUBTITLE_VISIBLE = "subtitle";

    private RecyclerView mCrimeRecyclerView;
    private CrimeAdapter mAdapter;
    private boolean mSubtitleVisible;
    private Callbacks mCallBacks;
    private OnDeleteCrimeCallback mDeleteCallback;

    private Button mNoCrimes;
    private LinearLayout mNoCrimesSetup;

    /*
    //REQUIRED interface for hosting activities
    */
    public interface Callbacks{
        void onCrimeSelected(Crime crime);
    }
    public interface OnDeleteCrimeCallback{
        void onCrimeIdDelete(UUID crimeId);
    }

    @Override
    public void onAttach(Context context){
        super.onAttach(context);

        mCallBacks = (Callbacks)context;
        mDeleteCallback = (OnDeleteCrimeCallback)context;
    }

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fragment_crime_list, container, false);

        mCrimeRecyclerView = (RecyclerView)view.findViewById(R.id.crime_recycler_view);
        mCrimeRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));

        setCrimeRecyclerViewItemTouchListener();

        if(savedInstanceState != null){
            mSubtitleVisible = savedInstanceState.getBoolean(SAVED_SUBTITLE_VISIBLE);
        }

        mNoCrimesSetup = (LinearLayout) view.findViewById(R.id.empty_crime_list);

        updateUI();

        return view;
    }

    @Override
    public void onResume(){
        super.onResume();
        updateUI();
    }

    @Override
    public void onSaveInstanceState(Bundle outState){
        super.onSaveInstanceState(outState);
        outState.putBoolean(SAVED_SUBTITLE_VISIBLE, mSubtitleVisible);
    }

    @Override
    public void onDetach(){
        super.onDetach();
        mCallBacks = null;
        mDeleteCallback = null;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater){
        super.onCreateOptionsMenu(menu, inflater);
        inflater.inflate(R.menu.fragment_crime_list, menu);

        MenuItem subtitleItem = menu.findItem(R.id.show_subtitle);

        if(mSubtitleVisible){
            subtitleItem.setTitle(R.string.hide_subtitle);
        }
        else{
            subtitleItem.setTitle(R.string.show_subtitle);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        switch(item.getItemId()){
            case R.id.new_crime:
                Crime crime = new Crime();
                CrimeLab.get(getActivity()).addCrime(crime);

                updateUI();
                mCallBacks.onCrimeSelected(crime);

                return true;
            case R.id.show_subtitle:
                mSubtitleVisible = !mSubtitleVisible;
                getActivity().invalidateOptionsMenu();
                updateSubtitle();
                return true;

            default:
                return super.onOptionsItemSelected(item);
        }
    }

    private void updateSubtitle(){
        CrimeLab crimeLab = CrimeLab.get(getActivity());
        int crimeCount = crimeLab.getCrimes().size();
        String subtitle = getResources().getQuantityString(R.plurals.subtitle_plural, crimeCount, crimeCount);

        if(!mSubtitleVisible){
            subtitle = null;
        }

        AppCompatActivity activity = (AppCompatActivity) getActivity();
        activity.getSupportActionBar().setSubtitle(subtitle);
    }

    public void updateUI(){
        CrimeLab crimeLab = CrimeLab.get(getActivity());
        List<Crime> crimes = crimeLab.getCrimes();

        if(mAdapter == null) {
            mAdapter = new CrimeAdapter(crimes);
            mCrimeRecyclerView.setAdapter(mAdapter);
        }
        else{
            mAdapter.setCrimes(crimes);
            mAdapter.notifyDataSetChanged();
        }


        //this code is for when we have no crimes and an empty RecyclerView
        //this makes it so we have a message and a button to add a crime
        int numEntries = CrimeLab.get(getActivity()).numEntries();

        if(numEntries == 0) {
            mNoCrimesSetup.setVisibility(View.VISIBLE);

            mNoCrimes = (Button) mNoCrimesSetup.findViewById(R.id.first_crime);
            mNoCrimes.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    addingCrime();
                    mNoCrimesSetup.setVisibility(View.GONE);
                }
            });
        }
        else{
            mNoCrimesSetup.setVisibility(View.GONE);
        }

        updateSubtitle();
    }

    //method for manually adding a crime
    private void addingCrime(){
        Crime crime = new Crime();
        CrimeLab.get(getActivity()).addCrime(crime);

        updateUI();
        mCallBacks.onCrimeSelected(crime);
    }

    public void deleteCrime(UUID crimeId) {
        CrimeLab.get(getActivity()).deleteCrime(crimeId);
    }

    public void setCrimeRecyclerViewItemTouchListener(){

        //SimpleCallback's constructor says we need to swipe to the left to activate the callback
        ItemTouchHelper.SimpleCallback itemTouchCallback = new ItemTouchHelper.SimpleCallback(0, ItemTouchHelper.LEFT){

            @Override
            public boolean onMove(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder, RecyclerView.ViewHolder target) {
                return false;
            }

            @Override
            public void onSwiped(RecyclerView.ViewHolder viewHolder, int direction) {
                int position = viewHolder.getAdapterPosition();
                Crime crime= mAdapter.getCrimes().get(position);
                Log.d(TAG, "onSwiped: " + crime.getId());
                mDeleteCallback.onCrimeIdDelete(crime.getId());
            }
        };

        //set our itemTouchCallback to connect to our itemTouchHelper and attach our recycler view
        //to our itemTouchHelper so it can operate on the recyclerView
        ItemTouchHelper itemTouchHelper = new ItemTouchHelper(itemTouchCallback);
        itemTouchHelper.attachToRecyclerView(mCrimeRecyclerView);
    }

    private abstract class AbstractCrimeHolder extends RecyclerView.ViewHolder implements View.OnClickListener{
        private Crime mCrime;

        private TextView mTitleTextView;
        private TextView mDateTextView;

        public AbstractCrimeHolder(LayoutInflater inflater, ViewGroup parent, int layoutId) {
            super(inflater.inflate(layoutId, parent, false));
            itemView.setOnClickListener(this);

            mTitleTextView = (TextView) itemView.findViewById(R.id.crime_title);
            mDateTextView = (TextView) itemView.findViewById(R.id.crime_date);
        }

        public void bind(Crime crime){
            mCrime = crime;

            Locale currentLocale = getResources().getConfiguration().locale;
            mTitleTextView.setText(mCrime.getTitle());
            mTitleTextView.setContentDescription("Title is: " + mCrime.getTitle());
            mDateTextView.setText(mCrime.getDateFormatted(currentLocale));
            mDateTextView.setContentDescription("Date is: " + mCrime.getDate());
        }

        public Crime getCrime(){
            return mCrime;
        }

        @Override
        public void onClick(View view){
            mCallBacks.onCrimeSelected(mCrime);
        }

    }

    private class CrimeHolder extends AbstractCrimeHolder{

        private ImageView mSolvedImageView;

        public CrimeHolder(LayoutInflater inflater, ViewGroup parent){
            super(inflater, parent, R.layout.list_item_crime);
            itemView.setOnClickListener(this);

            mSolvedImageView = (ImageView)itemView.findViewById(R.id.crime_solved);
        }

        @Override
        public void bind(Crime crime){
            super.bind(crime);
            mSolvedImageView.setVisibility(getCrime().isSolved() ? View.VISIBLE : View.GONE);

            if(getCrime().isSolved()){
                mSolvedImageView.setContentDescription("Crime is solved!");
            }
            else{
                mSolvedImageView.setContentDescription("Crime is not solved!");
            }
        }
    }

    private class PoliceCrimeHolder extends AbstractCrimeHolder {

        private Button mContactPolice;

        public PoliceCrimeHolder(LayoutInflater inflater, ViewGroup parent){
            super(inflater, parent, R.layout.list_item_crime_police);

            mContactPolice = (Button)itemView.findViewById(R.id.contact_police_button);
            mContactPolice.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick(View v){
                    Toast.makeText(getActivity(), "Police have been called!", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    private class CrimeAdapter extends RecyclerView.Adapter<AbstractCrimeHolder>{

        private static final int LIST_ITEM_CRIME = 0;
        private static final int LIST_ITEM_CRIME_POLICE = 1;

        private List<Crime> mCrimes;

        public CrimeAdapter(List<Crime> crimes){
            mCrimes = crimes;
        }

        @Override
        public AbstractCrimeHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            LayoutInflater layoutInflater = LayoutInflater.from(getActivity());

            if (viewType == LIST_ITEM_CRIME) {
                return new CrimeHolder(layoutInflater, parent);
            } else if (viewType == LIST_ITEM_CRIME_POLICE) {
                return new PoliceCrimeHolder(layoutInflater, parent);
            } else {
                return null;
            }
        }

        @Override
        public void onBindViewHolder(AbstractCrimeHolder holder, int position) {
            Crime crime = mCrimes.get(position);
            if (holder instanceof CrimeHolder){
                ((CrimeHolder) holder).bind(crime);
            }
            else if (holder instanceof PoliceCrimeHolder){
                ((PoliceCrimeHolder) holder).bind(crime);
            }
        }

        @Override
        public int getItemCount() {
            return mCrimes.size();
        }

        @Override
        public int getItemViewType(int position) {
            boolean requiresPolice = mCrimes.get(position).isRequiresPolice();
            if(requiresPolice) {
                return LIST_ITEM_CRIME_POLICE;
            } else {
                return LIST_ITEM_CRIME;
            }
        }

        public void setCrimes(List<Crime> crimes){
            mCrimes = crimes;
        }

        public List<Crime> getCrimes(){
            return mCrimes;
        }
    }
}
