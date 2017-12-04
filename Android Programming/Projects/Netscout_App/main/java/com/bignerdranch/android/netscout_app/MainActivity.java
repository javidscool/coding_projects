package com.bignerdranch.android.netscout_app;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ExpandableListView;
import android.widget.FrameLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.Toast;
import com.bignerdranch.android.netscout_app.SummaryListFragment;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static final String LOG_TAG = "MainActivity";

    private ExpandableListAdapter listAdapter;
    private ExpandableListView mDrawerList;
    private List<String> listDataHeader;
    private HashMap<String, List<String>> listDataChild;

    private ActionBarDrawerToggle mDrawerToggle;
    private DrawerLayout mDrawerLayout;
    private String mActivityTitle;

    private FrameLayout fragmentContainer;
    private int fragContainerWidth;
    private FrameLayout detailFragmentContainer;

    private SummaryLab mSummaryLab;

    private SummaryListFragment summaryListFragment;
    private SummaryFragment summaryFragment;
    private boolean isDrawerClosed;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        fragmentContainer = (FrameLayout)findViewById(R.id.fragment_container);
        detailFragmentContainer = (FrameLayout)findViewById(R.id.detail_fragment_container);

        //setting up our drawer layout and title
        mDrawerLayout = (DrawerLayout)findViewById(R.id.drawer_layout);
        mDrawerList = (ExpandableListView)findViewById(R.id.navList);
        mActivityTitle = getTitle().toString();

        addMenuItems();
        setupDrawer();

        listAdapter = new ExpandableListAdapter(this, listDataHeader, listDataChild);
        mDrawerList.setAdapter(listAdapter);

        //setting up toggle switch
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setHomeButtonEnabled(true);

        //listview Group click listener
        mDrawerList.setOnGroupClickListener(new ExpandableListView.OnGroupClickListener() {

            @Override
            public boolean onGroupClick(ExpandableListView parent, View v,
                                        int groupPosition, long id) {
                Log.d( LOG_TAG, "groupPosition on groupClick = " + groupPosition);
                summaryListFragment.updateUI(groupPosition, 0);

                // Toast.makeText(getApplicationContext(),
                // "Group Clicked " + listDataHeader.get(groupPosition),
                // Toast.LENGTH_SHORT).show();
                return false;
            }
        });

        //listview Group expanded listener
        mDrawerList.setOnGroupExpandListener(new ExpandableListView.OnGroupExpandListener() {

            @Override
            public void onGroupExpand(int groupPosition) {
                Toast.makeText(getApplicationContext(),
                        listDataHeader.get(groupPosition) + " Expanded",
                        Toast.LENGTH_SHORT).show();
            }
        });

        //listview Group collasped listener
        mDrawerList.setOnGroupCollapseListener(new ExpandableListView.OnGroupCollapseListener() {

            @Override
            public void onGroupCollapse(int groupPosition) {
                Toast.makeText(getApplicationContext(),
                        listDataHeader.get(groupPosition) + " Collapsed",
                        Toast.LENGTH_SHORT).show();

            }
        });

        //listview on child click listener
        mDrawerList.setOnChildClickListener(new ExpandableListView.OnChildClickListener() {

            @Override
            public boolean onChildClick(ExpandableListView parent, View v,
                                        int groupPosition, int childPosition, long id) {

                Log.d( LOG_TAG, "groupPosition = " + groupPosition);
                Log.d( LOG_TAG, "childPosition = " + childPosition);
                summaryListFragment.updateUI(groupPosition, (childPosition + 1));


                /*
                Toast.makeText(
                        getApplicationContext(),
                        listDataHeader.get(groupPosition)
                                + " : "
                                + listDataChild.get(
                                listDataHeader.get(groupPosition)).get(
                                childPosition), Toast.LENGTH_SHORT)
                        .show();
                        */
                return false;
            }
        });


        //creating our list
        FragmentManager fm = getSupportFragmentManager();
        summaryListFragment = (SummaryListFragment)fm.findFragmentById(R.id.fragment_container);

        if(summaryListFragment == null){
            summaryListFragment = new SummaryListFragment();
            fm.beginTransaction().add(R.id.fragment_container, summaryListFragment).commit();
        }

        isDrawerClosed = true;
    }

    //called to add our menu items
    private void addMenuItems(){
        String[] menuArray = { "Menu Item 1", "Menu Item 2", "Menu Item 3", "Menu Item 4", "Menu Item 5", "Menu Item 6" };

        String[] subMenu1 = { "Sub Item 1", "Sub Item 2" };
        String[] subMenu2 = { "Sub Item 1" };
        String[] subMenu5 = { "Sub Item 1", "Sub Item 2", "Sub Item 3", "Sub Item 4" };
        String[] subMenu6 = { "Sub Item 1", "Sub Item 2", "Sub Item 3", "Sub Item 4", "Sub Item 5", "Sub Item 6"};

        listDataHeader = new ArrayList<String>(Arrays.asList(menuArray));
        listDataChild = new HashMap<String, List<String>>();

        listDataChild.put(listDataHeader.get(0), Arrays.asList(subMenu1));
        listDataChild.put(listDataHeader.get(1), Arrays.asList(subMenu2));
        listDataChild.put(listDataHeader.get(2), Collections.<String>emptyList());
        listDataChild.put(listDataHeader.get(3), Collections.<String>emptyList());
        listDataChild.put(listDataHeader.get(4), Arrays.asList(subMenu5));
        listDataChild.put(listDataHeader.get(5), Arrays.asList(subMenu6));

        //create the summaryLab
        mSummaryLab = new SummaryLab(listDataChild);

        mDrawerList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Toast.makeText(MainActivity.this, "Time for an upgrade!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    public List<Summary> getSummaries(int groupPos, int childPos){
        return mSummaryLab.getSummaries(groupPos, childPos);
    }

    //called after drawer methods have been set up
    private void setupDrawer(){

        //setting up our toggle
        mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout,
                R.string.drawer_open, R.string.drawer_close) {

            //called when a drawer has settled in a completely open state
            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                //set title for action bar and recreate options menu
                getSupportActionBar().setTitle("Navigation!");

                isDrawerClosed = false;

                //this creates call to onPrepareOptionsMenu()
                invalidateOptionsMenu();
            }

            //called when a drawer has settled in a completely closed state
            public void onDrawerClosed(View view) {
                super.onDrawerClosed(view);

                isDrawerClosed = true;

                //revert title and recreate options menu
                getSupportActionBar().setTitle(mActivityTitle);

                //this creates call to onPrepareOptionsMenu()
                invalidateOptionsMenu();
            }

            public void onDrawerSlide(View drawerView, float slideOffset) {

                float moveFactor = (mDrawerList.getWidth() * slideOffset);
                float newWidthX = fragContainerWidth - moveFactor;
                float newScaleX = newWidthX / fragContainerWidth;

                fragmentContainer.setTranslationX(moveFactor / 2);
                fragmentContainer.setScaleX(newScaleX);

                summaryListFragment.moveRecycler(isDrawerClosed, newScaleX);

                mDrawerLayout.bringChildToFront(drawerView);
                mDrawerLayout.requestLayout();
            }
        };

        //enable the stack menu and attach it to our drawer layout
        mDrawerToggle.setDrawerIndicatorEnabled(true);
        mDrawerLayout.setDrawerListener(mDrawerToggle);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        if (mDrawerToggle.onOptionsItemSelected(item)) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        //syncs the toggle state after onRestoreInstanceState has occurred.
        mDrawerToggle.syncState();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {

        super.onWindowFocusChanged(hasFocus);

        fragContainerWidth = fragmentContainer.getWidth();
        Log.d( LOG_TAG, "fragContainerWidth in onWindowFocusChanged = " + fragContainerWidth);
    }

}
