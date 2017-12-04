package com.bignerdranch.android.photogallery;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SearchView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.inputmethod.InputMethodManager;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by javidscool on 4/17/2017.
 */

public class PhotoGalleryFragment extends VisibleFragment {

    private static final String TAG = "PhotoGalleryFragment";

    private static final String SAVED_CURRENT_PAGE = "curPage";
    private static final String SAVED_PREV_PAGE = "prevPage";
    private static final String SAVED_PREV_LAST_POS = "lastPos";
    private static final String SAVED_SET_IT_OFF = "setItOff";
    private static final String SAVED_LOCKDOWN = "lockdown";
    private static final String SAVED_LAYOUT_MANAGER = "layoutManager";

    private static final int columnWidth = 360;

    private RecyclerView mPhotoRecyclerView;
    private ProgressBar mProgressBar;
    private List<GalleryItem> mItems = new ArrayList<>();
    private ThumbnailDownloader<PhotoHolder> mThumbnailDownloader;

    //this keeps track of the page that we've currently fetched
    private int currentPage = 1;
    private int prevPage = 1;
    private int prevLastPosition = 0;
    private boolean setItOff = false;

    //doing this to use our bundle in inner classes
    private Bundle myInstanceState;

    //this keeps track of whether we're in an adapter update lockdown or not,
    //so no scroll events happen if we are and nothing gets improperly touched
    private boolean lockdown = false;

    public static PhotoGalleryFragment newInstance(){
        return new PhotoGalleryFragment();
    }

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setRetainInstance(true);
        setHasOptionsMenu(true);
        QueryPreferences.setFetchedPage(getActivity(), currentPage);
        Log.d(TAG, "OnCreate updateItems");

        if(savedInstanceState != null){
            currentPage = savedInstanceState.getInt(SAVED_CURRENT_PAGE, 1);
            prevPage = savedInstanceState.getInt(SAVED_PREV_PAGE, 1);
            prevLastPosition = savedInstanceState.getInt(SAVED_PREV_LAST_POS, 0);
            setItOff = savedInstanceState.getBoolean(SAVED_SET_IT_OFF, false);
            lockdown = savedInstanceState.getBoolean(SAVED_LOCKDOWN, false);
        }

        updateItems();

        Handler responseHandler = new Handler();
        mThumbnailDownloader = new ThumbnailDownloader<>(responseHandler);
        mThumbnailDownloader.setThumbnailDownloadListener(new ThumbnailDownloader.ThumbnailDownloadListener<PhotoHolder>(){

            @Override
            public void onThumbnailDownloaded(PhotoHolder photoHolder, Bitmap bitmap){
                //we run this code to handle the downloaded image once it's complete
                Drawable drawable = new BitmapDrawable(getResources(), bitmap);
                photoHolder.bindDrawable(drawable);
            }
        });

        mThumbnailDownloader.start();
        mThumbnailDownloader.getLooper();
        Log.i(TAG, "Background thread started");
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        outState.putInt(SAVED_CURRENT_PAGE, currentPage);
        outState.putInt(SAVED_PREV_PAGE, prevPage);
        outState.putInt(SAVED_PREV_LAST_POS, prevLastPosition);
        outState.putBoolean(SAVED_SET_IT_OFF, setItOff);
        outState.putBoolean(SAVED_LOCKDOWN, lockdown);

        outState.putParcelable(SAVED_LAYOUT_MANAGER, mPhotoRecyclerView.getLayoutManager().onSaveInstanceState());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View v = inflater.inflate(R.layout.fragment_photo_gallery, container, false);

        mProgressBar = (ProgressBar)v.findViewById(R.id.fragment_photo_gallery_progress_bar);
        mProgressBar.setIndeterminate(true);

        mPhotoRecyclerView = (RecyclerView)v.findViewById(R.id.photo_recycler_view);
        mPhotoRecyclerView.addOnScrollListener(new RecyclerView.OnScrollListener() {
            @Override
            public void onScrollStateChanged(RecyclerView recyclerView, int newState) {
                PhotoAdapter adapter = (PhotoAdapter)recyclerView.getAdapter();

                GridLayoutManager layoutManager = (GridLayoutManager)recyclerView.getLayoutManager();
                Log.d(TAG, "span count is: " + layoutManager.getSpanCount());

                Log.d(TAG, "adapter item count is: " + adapter.getItemCount());
                int lastPosition = adapter.getLastBoundPosition();

                //if we're under adapter update lockdown, don't process any scroll events
                //this is because our list and adapter are being updated, and shouldn't be touched
                if (!lockdown) {
                    if (lastPosition > (adapter.getItemCount() - layoutManager.getSpanCount()) ) {

                        // we have to do this check because this scroll listener is hit many times when we trigger a new item load,
                        // but we only need to update once
                        if (Math.abs(lastPosition - prevLastPosition) > layoutManager.getSpanCount()) {
                            //here we're moving up a page
                            prevLastPosition = lastPosition;

                            prevPage = currentPage;
                            currentPage++;

                            QueryPreferences.setFetchedPage(getActivity(), currentPage);
                            Log.d(TAG, "Scroll bigger updateItems");
                            updateItems();
                        }

                    } else if ((lastPosition > ((adapter.getItemCount() / 2) - layoutManager.getSpanCount())) && (prevPage > currentPage)) {
                        //we're scrolling to the next page but not loading any items cause we haven't gotten far enough yet to do so
                        prevLastPosition = lastPosition;

                        prevPage = currentPage;
                        currentPage++;
                    } else if ((lastPosition < ((adapter.getItemCount() / 2) - layoutManager.getSpanCount())) && (prevPage < currentPage)) {
                        //we're scrolling to the previous page but not loading any items cause we haven't gotten far enough yet to do so
                        prevLastPosition = lastPosition;

                        prevPage = currentPage;
                        currentPage--;
                    } else if (lastPosition == 0) {
                        //this all happens if we've reached the beginning of our list
                        if (Math.abs(prevLastPosition - lastPosition) > 0) {
                            prevLastPosition = lastPosition;

                            if (currentPage > 1) {
                                prevPage = currentPage;
                                currentPage--;

                                QueryPreferences.setFetchedPage(getActivity(), currentPage);
                                Log.d(TAG, "scroll smaller updateItems");
                                updateItems();
                            }
                        }
                    }
                }
            }
        });

        //using a ViewTreeObserver to set number of columns
        //this fires onGlobalLayout when a layout pass happens
        ViewTreeObserver observer = mPhotoRecyclerView.getViewTreeObserver();
        myInstanceState = savedInstanceState;
        observer.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {

                int width = mPhotoRecyclerView.getWidth();
                Log.d(TAG, "RecyclerView's width: " + mPhotoRecyclerView.getWidth());

                int numOfColsInRecycler = Math.round((width / columnWidth));

                if(myInstanceState != null){
                    mPhotoRecyclerView.setLayoutManager(new GridLayoutManager(getActivity(), numOfColsInRecycler));
                    //this reloads our scroll position, configuration, etc. so we don't jump back to the top again
                    mPhotoRecyclerView.getLayoutManager().onRestoreInstanceState(myInstanceState.getParcelable(SAVED_LAYOUT_MANAGER));
                }

                else {
                    //setting the number of columns based on the width of the RecyclerView
                    mPhotoRecyclerView.setLayoutManager(new GridLayoutManager(getActivity(), numOfColsInRecycler));
                }

                //once we've set up the recycler view, we don't need
                //to call this listener anymore because our recycler view is created
                mPhotoRecyclerView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
            }
        });

        setupAdapter();

        return v;
    }

    @Override
    public void onDestroyView(){
        super.onDestroyView();
        mThumbnailDownloader.clearQueue();
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        mThumbnailDownloader.quit();
        Log.i(TAG, "Background thread destroyed");
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater menuInflater){
        super.onCreateOptionsMenu(menu, menuInflater);
        menuInflater.inflate(R.menu.fragment_photo_gallery, menu);

        MenuItem searchItem = menu.findItem(R.id.menu_item_search);
        final SearchView searchView = (SearchView)searchItem.getActionView();

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String s) {
                Log.d(TAG, "QueryTextSubmit: " + s);
                QueryPreferences.setStoredQuery(getActivity(), s);
                currentPage = 1;
                prevPage = 1;
                prevLastPosition = 0;
                setItOff = false;
                lockdown = false;
                Log.d(TAG, "query submit updateItems");

                //hiding the soft keyboard once a search is submitted
                //first, check if no view has focus
                View view = getActivity().getCurrentFocus();
                if (view != null) {
                    InputMethodManager imm = (InputMethodManager)getActivity().getSystemService(Context.INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
                }

                //now we collapse the searchview
                searchView.onActionViewCollapsed();

                //clear recycler view contents and put in progress bar
                mPhotoRecyclerView.setVisibility(View.GONE);
                mProgressBar.setVisibility(View.VISIBLE);

                updateItems();
                return true;
            }

            @Override
            public boolean onQueryTextChange(String s) {
                Log.d(TAG, "QueryTextChange: " + s);
                return false;
            }
        });

        searchView.setOnSearchClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v){
                String query = QueryPreferences.getStoredQuery(getActivity());
                searchView.setQuery(query, false);
            }
        });

        MenuItem toggleItem = menu.findItem(R.id.menu_item_toggle_polling);

        boolean isAlarmOn = false;
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP){
            isAlarmOn = PollJobService.isJobServiceAlarmOn(getActivity());
        }
        else{
            isAlarmOn = PollService.isServiceAlarmOn(getActivity());
        }
        if(isAlarmOn){
            toggleItem.setTitle(R.string.stop_polling);
        }
        else{
            toggleItem.setTitle(R.string.start_polling);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        switch(item.getItemId()){
            case R.id.menu_item_clear:
                QueryPreferences.setStoredQuery(getActivity(), null);
                currentPage = 1;
                prevPage = 1;
                prevLastPosition = 0;
                setItOff = false;
                lockdown = false;
                Log.d(TAG, "Menu clear updateItems");
                updateItems();
                return true;
            case R.id.menu_item_toggle_polling:
                //toggling the alarm
                boolean shouldStartAlarm = false;
                if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP){
                    shouldStartAlarm = !PollJobService.isJobServiceAlarmOn(getActivity());
                }
                else{
                    shouldStartAlarm = !PollService.isServiceAlarmOn(getActivity());
                }

                //running a JobService if we're on Lollipop or above,
                //and an alarm service if we're below
                if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP){
                    PollJobService.setJobServiceAlarm(getActivity(), shouldStartAlarm);
                }
                else{
                    PollService.setServiceAlarm(getActivity(), shouldStartAlarm);
                }

                getActivity().invalidateOptionsMenu();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    private void updateItems(){
        String query = QueryPreferences.getStoredQuery(getActivity());
        new FetchItemsTask(query).execute(currentPage);
    }

    private void setupAdapter(){
        if(isAdded()){
            mPhotoRecyclerView.setAdapter(new PhotoAdapter(mItems));
        }
    }


    private class PhotoHolder extends RecyclerView.ViewHolder implements View.OnClickListener{

        private ImageView mItemImageView;
        private GalleryItem mGalleryItem;

        public PhotoHolder(View itemView){
            super(itemView);

            mItemImageView = (ImageView)itemView.findViewById(R.id.item_image_view);
            itemView.setOnClickListener(this);
        }

        public String getURLCheck(){
            return mGalleryItem.getUrl();
        }

        public void bindDrawable(Drawable drawable){
            mItemImageView.setImageDrawable(drawable);
        }

        public void bindGalleryItem(GalleryItem galleryItem){
            mGalleryItem = galleryItem;
        }

        @Override
        public void onClick(View v){
            Intent i = PhotoPageActivity.newIntent(getActivity(), mGalleryItem.getPhotoPageUri());
            startActivity(i);
        }
    }

    private class PhotoAdapter extends RecyclerView.Adapter<PhotoHolder>{
        private List<GalleryItem> mGalleryItems;

        //this variable tracks the last position we loaded in for a specific page
        //keep in mind the position index starts at zero!
        private int lastBoundPosition;

        public int getLastBoundPosition(){
            return lastBoundPosition;
        }

        public PhotoAdapter(List<GalleryItem> galleryItems){
            mGalleryItems = galleryItems;
        }

        @Override
        public PhotoHolder onCreateViewHolder(ViewGroup viewGroup, int viewType){
            LayoutInflater inflater = LayoutInflater.from(getActivity());
            View view = inflater.inflate(R.layout.list_item_gallery, viewGroup, false);
            return new PhotoHolder(view);
        }

        @Override
        public void onBindViewHolder(PhotoHolder photoHolder, int position){
            GalleryItem galleryItem = mGalleryItems.get(position);
            photoHolder.bindGalleryItem(galleryItem);

            //first check our cache to see if we have an image saved there already
            if(mThumbnailDownloader.getBitmapFromMemCache(photoHolder.getURLCheck()) != null) {
                Log.i(TAG, "we've already bound the image");
                Drawable drawable = new BitmapDrawable(getResources(), mThumbnailDownloader.getBitmapFromMemCache(photoHolder.getURLCheck()));
                photoHolder.bindDrawable(drawable);
            }
            else {
                Drawable placeHolder = getResources().getDrawable(R.drawable.bill_up_close);
                photoHolder.bindDrawable(placeHolder);

                //first we create a list of URLs to preload into our cache
                //we preload 20 items in either direction from where we're at right now
                preloadCache(photoHolder, position, 20);

                //then we queue the thumbnail to be loaded from our handler thread
                mThumbnailDownloader.queueThumbnail(photoHolder, galleryItem.getUrl());
            }
            lastBoundPosition = position;
            Log.i(TAG, "Last bound position is " + Integer.toString(lastBoundPosition));

            //we lift the adapter lockdown after we update the adapter and set a new last position
            if(lockdown) lockdown = false;
        }

        @Override
        public int getItemCount(){
            return mGalleryItems.size();
        }

        //custom method we use in our adapter to preload our bitmap cache
        //in our handler thread
        private void preloadCache(PhotoHolder photoHolder, int seedPosition, int preloadRadius){

            //we first have to mark our beginning and end indices
            int lowIndex, highIndex;

            //start with beginning index
            if( (seedPosition - preloadRadius) < 0){
                lowIndex = 0;
            }
            else{
                lowIndex = seedPosition - preloadRadius;
            }

            //mark high position
            if( (seedPosition + preloadRadius) > (mPhotoRecyclerView.getAdapter().getItemCount() - 1) ){
                highIndex = mPhotoRecyclerView.getAdapter().getItemCount() - 1;
            }
            else{
                highIndex = seedPosition + preloadRadius;
            }

            //now we go through our item gallery list and get all the URLs to send off to the handler to be preloaded
            List<String> urls = new ArrayList<>();
            for(int i = lowIndex; i <= highIndex; i++){
                if(i == seedPosition) continue;

                String url = mGalleryItems.get(i).getUrl();
                urls.add(url);
            }

            mThumbnailDownloader.preloadCache(photoHolder, urls);
        }
    }

    private class FetchItemsTask extends AsyncTask<Integer, Void, List<GalleryItem>>{

        private String mQuery;

        public FetchItemsTask(String query){
            mQuery = query;
        }

        @Override
        protected List<GalleryItem> doInBackground(Integer... params){
            Log.d(TAG, "I'm on page: " + params[0]);
            if(mQuery == null){
                return new FlickrFetchr().fetchRecentPhotos(params[0]);
            }
            else{
                return new FlickrFetchr().searchPhotos(mQuery, params[0]);
            }
        }

        @Override
        protected void onPostExecute(List<GalleryItem> items){
            //put recycler view up and clear progress bar
            mPhotoRecyclerView.setVisibility(View.VISIBLE);
            mProgressBar.setVisibility(View.GONE);

            //if we've already started
            if((currentPage >= 1) && setItOff){

                //when we are updating our adapter and the list that it uses, we lock shit
                //down so that no scroll events are processed that would touch a list that's
                //being worked on
                lockdown = true;

                if( (currentPage > prevPage) && (currentPage > 2)){
                    mItems.addAll(items);

                    //first we insert the items we got from our result
                    int insertedIndex = (mItems.size() - items.size() );
                    mPhotoRecyclerView.getAdapter().notifyItemRangeInserted(insertedIndex, items.size());

                    //next we delete the items from the beginning of the list
                    //subList does NOT include end index
                    mItems.subList(0, items.size()).clear();
                    mPhotoRecyclerView.getAdapter().notifyItemRangeRemoved(0, items.size());

                    //update our previous position to take these changes into effect
                    prevLastPosition -= items.size();
                }
                else if((currentPage > prevPage) && (currentPage == 2) ){

                    //taking into account if we go super up and then come super back
                    //we would have 200 items in this case and wouldn't need to add shit
                    //however, if this was our first time running the program, we would need to add shit
                    //because we only have 100 items to start
                    if( !(mItems.size() > 100)) {
                        //we insert our items we got as a result
                        mItems.addAll(items);
                        int insertedIndex = (mItems.size() - items.size());
                        mPhotoRecyclerView.getAdapter().notifyItemRangeInserted(insertedIndex, items.size());

                        //update our previous position
                        prevLastPosition = items.size();
                    }
                    else {
                        //update our previous position
                        //this is plus because we go super UP and then come super back
                        prevLastPosition += items.size();
                    }
                }
                else if(currentPage < prevPage){
                    //first we insert the items into our list and adapter
                    mItems.addAll(0, items);

                    mPhotoRecyclerView.getAdapter().notifyItemRangeInserted(0, items.size());

                    //now we remove the items at the end of our list and adapter
                    int removedIndex = (mPhotoRecyclerView.getAdapter().getItemCount() - items.size() );
                    //subList does NOT include end index
                    mItems.subList( removedIndex, mPhotoRecyclerView.getAdapter().getItemCount() ).clear();
                    mPhotoRecyclerView.getAdapter().notifyItemRangeRemoved(removedIndex, items.size());

                    //update our previous position to take these changes into effect
                    prevLastPosition += items.size();
                }
            }
            else {
                //check if we need to start all this off
                if(!setItOff) {
                    mItems.clear();
                    mItems = items;
                    setupAdapter();

                    setItOff = true;
                }
                else{
                    //update our previous position
                    prevLastPosition = 0;
                }
            }
        }
    }
}
