package com.bignerdranch.android.criminalintent;

import android.content.Intent;
import android.support.v4.app.Fragment;

import java.util.UUID;

/**
 * Created by javidscool on 4/4/2017.
 */

public class CrimeListActivity extends SingleFragmentActivity implements CrimeListFragment.Callbacks, CrimeListFragment.OnDeleteCrimeCallback, CrimeFragment.Callbacks {

    @Override
    protected Fragment createFragment(){
        return new CrimeListFragment();
    }

    @Override
    protected int getLayoutResId(){
        return R.layout.activity_masterdetail;
    }

    @Override
    public void onCrimeSelected(Crime crime){
        if(findViewById(R.id.detail_fragment_container) == null){
            Intent intent = CrimePagerActivity.newIntent(this, crime.getId());

            startActivity(intent);
        }

        else{
            Fragment newDetail = CrimeFragment.newInstance(crime.getId());

            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.detail_fragment_container, newDetail)
                    .commit();
        }
    }

    //we implement onCrimeUpdated and onCrimeIdDelete thru callbacks here
    //because on tablets we have list views and detail views shown together,
    //so we need to immediately update our list view (which is stored in fragment_container)
    public void onCrimeUpdated(Crime crime){
        CrimeListFragment listFragment = (CrimeListFragment)getSupportFragmentManager().findFragmentById(R.id.fragment_container);

        listFragment.updateUI();
    }

    @Override
    public void onCrimeIdDelete(UUID crimeId) {
        CrimeListFragment listFragment = (CrimeListFragment)getSupportFragmentManager().findFragmentById(R.id.fragment_container);
        listFragment.deleteCrime(crimeId);
        listFragment.updateUI();

        if(findViewById(R.id.detail_fragment_container) != null){
            getSupportFragmentManager().beginTransaction()
                    .remove(getSupportFragmentManager().findFragmentById(R.id.detail_fragment_container))
                    .commit();
        }
    }
}
