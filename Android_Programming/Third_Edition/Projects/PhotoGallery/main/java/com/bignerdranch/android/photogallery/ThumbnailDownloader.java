package com.bignerdranch.android.photogallery;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.support.v4.util.LruCache;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import static android.provider.ContactsContract.CommonDataKinds.Website.URL;

/**
 * Created by javidscool on 4/17/2017.
 */

public class ThumbnailDownloader<T> extends HandlerThread {

    private static final String TAG = "ThumbnailDownloader";
    private static final int MESSAGE_DOWNLOAD = 0;
    private static final int PRELOAD_CACHE = 1;

    private Handler mRequestHandler;
    private ConcurrentMap<T, String> mRequestMap = new ConcurrentHashMap<>();
    private LruCache<String, Bitmap> mPhotoCache;
    private List<String> preloadedURLs = new ArrayList<>();
    private Handler mResponseHandler;
    private ThumbnailDownloadListener<T> mThumbnailDownloadListener;
    private boolean preloadingCache = false;

    public interface ThumbnailDownloadListener<T>{
        void onThumbnailDownloaded(T target, Bitmap bitmap);
    }

    public void setThumbnailDownloadListener(ThumbnailDownloadListener<T> listener){
        mThumbnailDownloadListener = listener;
    }


    public ThumbnailDownloader(Handler responseHandler){
        super(TAG);
        mResponseHandler = responseHandler;
    }

    @Override
    protected void onLooperPrepared(){
        mRequestHandler = new Handler(){
            @Override
            public void handleMessage(Message msg){
                //this takes care of what the handler will do when downloaded
                //messages are pulled off the queue and passed to it
                if(msg.what == MESSAGE_DOWNLOAD){
                    T target = (T)msg.obj;
                    Log.i(TAG, "Got a request for URL: " + mRequestMap.get(target));
                    handleRequest(target);
                }
                else if(msg.what == PRELOAD_CACHE){
                    //this takes care of what we need to do to preload the cache
                    Log.i(TAG, "Preloading cache");
                    if(!preloadingCache){
                        preloadingCache = true;
                        handlePreloadingOfCache(preloadedURLs);
                    }
                }
            }
        };

        // get max available VM memory, exceeding this amount will throw an
        // OutOfMemory exception
        // we store info in this cache in kilobytes as LruCache takes an
        // int in its constructor
        final int maxMemory = (int) (Runtime.getRuntime().maxMemory() / 1024);

        // Use 1/8th of the available memory for this memory cache.
        final int cacheSize = maxMemory / 8;

        mPhotoCache = new LruCache<String, Bitmap>(cacheSize) {
            @Override
            protected int sizeOf(String key, Bitmap bitmap) {
                // the cache size will be measured in kilobytes rather than
                // number of items
                return bitmap.getByteCount() / 1024;
            }
        };
    }

    //gets a bitmap/photo from the cache
    public Bitmap getBitmapFromMemCache(String key) {
        return mPhotoCache.get(key);
    }

    //adds a bitmap/photo to the cache
    private void addBitmapToMemoryCache(String key, Bitmap bitmap) {
        if (getBitmapFromMemCache(key) == null) {
            mPhotoCache.put(key, bitmap);
        }
    }

    public void queueThumbnail(T target, String url){
        Log.i(TAG, "Got a URL: " + url);

        if(url == null){
            mRequestMap.remove(target);
        }
        else{
            mRequestMap.put(target, url);
            mRequestHandler.obtainMessage(MESSAGE_DOWNLOAD, target).sendToTarget();
        }
    }

    //this function takes in a list of urls and preloads a cache with them
    public void preloadCache(T target, List<String> urls){
        preloadedURLs = urls;
        mRequestHandler.obtainMessage(PRELOAD_CACHE, target).sendToTarget();
    }

    private void handlePreloadingOfCache(List<String> urls){

        for(int i = 0; i < urls.size(); i++){

            //checking if the bitmap this url is associated with already exists in the cache
            //if not, we add it
            if(getBitmapFromMemCache(urls.get(i)) == null){
                try {
                    byte[] bitmapBytes = new FlickrFetchr().getUrlBytes(urls.get(i));
                    Bitmap bitmap = BitmapFactory.decodeByteArray(bitmapBytes, 0, bitmapBytes.length);
                    addBitmapToMemoryCache(urls.get(i), bitmap);
                    Log.i(TAG, "Bitmap created and preloaded into cache");
                }
                catch(IOException ioe){
                    Log.e(TAG, "Error downloading image", ioe);
                }
            }
        }

        preloadingCache = false;
    }

    public void clearQueue(){
        mRequestHandler.removeMessages(MESSAGE_DOWNLOAD);
    }

    private void handleRequest(final T target){
        try{
            //first we get our URL using our PhotoHolder target
            final String url = mRequestMap.get(target);

            if(url == null){
                return;
            }

            final Bitmap bitmap;

            //trying to get a bitmap from the cache
            //if we don't have a bitmap for this key in the cache, load the bitmap and
            //store it in the cache
            if(getBitmapFromMemCache(url) == null){
                byte[] bitmapBytes = new FlickrFetchr().getUrlBytes(url);
                bitmap = BitmapFactory.decodeByteArray(bitmapBytes, 0, bitmapBytes.length);
                addBitmapToMemoryCache(url, bitmap);
                Log.i(TAG, "Bitmap created and added to cache");
            }

            else {
                bitmap = getBitmapFromMemCache(url);
                //Log.i(TAG, "Bitmap taken out of cache");
            }

            //this handles sending a message back to the main queue to update
            //its UI (the PhotoHolder in our case)
            mResponseHandler.post(new Runnable() {
                @Override
                public void run() {
                    if(mRequestMap.get(target) != url){
                        return;
                    }

                    mRequestMap.remove(target);
                    mThumbnailDownloadListener.onThumbnailDownloaded(target, bitmap);
                }
            });
        }
        catch(IOException ioe){
            Log.e(TAG, "Error downloading image", ioe);
        }
    }
}
