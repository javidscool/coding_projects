package com.bignerdranch.android.criminalintent;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.provider.MediaStore;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.ShareCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;

import java.io.File;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.UUID;

import static android.widget.CompoundButton.*;

/**
 * Created by javidscool on 4/1/2017.
 */

public class CrimeFragment extends Fragment {

    private static final String[] CONTACTS_PERMISSIONS = new String[]{
            Manifest.permission.READ_CONTACTS
    };

    private static final String ARG_CRIME_ID = "crime_id";
    private static final String DIALOG_DATE = "DialogDate";
    private static final String DIALOG_TIME = "DialogTime";
    private static final String DIALOG_FULL_PIC = "DialogFullPic";

    private static final int REQUEST_DATE = 0;
    private static final int REQUEST_TIME = 1;
    private static final int REQUEST_CONTACT= 2;
    private static final int REQUEST_PHOTO = 3;
    private static final int REQUEST_CONTACTS_PERMISSIONS = 5;


    private Crime mCrime;
    private File mPhotoFile;
    private EditText mTitleField;
    private Button mDateButton;
    private Button mTimeButton;
    private CheckBox mSolvedCheckBox;
    private CheckBox mRequiresPolice;
    private Button mSuspectButton;
    private Button mReportButton;
    private ImageButton mPhotoButton;
    private ImageView mPhotoView;
    private Point mPhotoViewSize;
    private Callbacks mCallbacks;
    private Button mCrimeCall;

    /*
    //REQUIRED interface for hosting activities
    */
    public interface Callbacks{
        void onCrimeUpdated(Crime crime);
    }

    public static CrimeFragment newInstance(UUID crimeID){
        Bundle args = new Bundle();
        args.putSerializable(ARG_CRIME_ID, crimeID);

        CrimeFragment fragment = new CrimeFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onAttach(Context context){
        super.onAttach(context);
        mCallbacks = (Callbacks)context;
    }

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        UUID crimeID = (UUID)getArguments().getSerializable(ARG_CRIME_ID);
        mCrime = CrimeLab.get(getActivity()).getCrime(crimeID);
        mPhotoFile = CrimeLab.get(getActivity()).getPhotoFile(mCrime);

        setHasOptionsMenu(true);
    }

    @Override
    public void onPause(){
        super.onPause();
        CrimeLab.get(getActivity()).updateCrime(mCrime);
    }

    @Override
    public void onDetach(){
        super.onDetach();
        mCallbacks = null;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){

        View v = inflater.inflate(R.layout.fragment_crime, container, false);

        mTitleField = (EditText)v.findViewById(R.id.crime_title);
        mTitleField.setText(mCrime.getTitle());
        mTitleField.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                //intentionally left blank
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                mCrime.setTitle(s.toString());
                updateCrime();
            }

            @Override
            public void afterTextChanged(Editable s) {
                //intentionally left blank
            }
        });

        mDateButton = (Button)v.findViewById(R.id.crime_date);
        updateDate();
        mDateButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v){
                //if there's no detail view, we're on a phone so present this as an activity
                if(getActivity().findViewById(R.id.detail_fragment_container) == null) {
                    Intent i = DatePickerActivity.newIntent(getActivity(), mCrime.getDate());
                    startActivityForResult(i, REQUEST_DATE);
                }
                else{
                    FragmentManager manager = getFragmentManager();
                    DatePickerFragment dialog = DatePickerFragment.newInstance(mCrime.getDate());
                    dialog.setTargetFragment(CrimeFragment.this, REQUEST_DATE);
                    dialog.show(manager, DIALOG_DATE);
                }
            }
        });

        mTimeButton = (Button)v.findViewById(R.id.crime_time);
        updateTime();
        mTimeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v){
                FragmentManager manager = getFragmentManager();
                TimePickerFragment dialog = TimePickerFragment.newInstance(mCrime.getDate());
                dialog.setTargetFragment(CrimeFragment.this, REQUEST_TIME);
                dialog.show(manager, DIALOG_TIME);
            }
        });

        mSolvedCheckBox = (CheckBox)v.findViewById(R.id.crime_solved);
        mSolvedCheckBox.setChecked(mCrime.isSolved());
        mSolvedCheckBox.setOnCheckedChangeListener(new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                mCrime.setSolved(isChecked);
                updateCrime();
            }
        });

        mRequiresPolice = (CheckBox)v.findViewById(R.id.requires_police);
        mRequiresPolice.setChecked(mCrime.isRequiresPolice());
        mRequiresPolice.setOnCheckedChangeListener(new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                mCrime.setRequiresPolice(isChecked);
                updateCrime();
            }
        });

        mReportButton = (Button)v.findViewById(R.id.crime_report);
        mReportButton.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v){

                //using ShareCompat to build my implicit intent instead of building it by hand
                ShareCompat.IntentBuilder.from(getActivity())
                        .setType("text/plain")
                        .setSubject(getString(R.string.crime_report_subject))
                        .setText(getCrimeReport())
                        .setChooserTitle(getString(R.string.send_report))
                        .startChooser();
            }
        });


        final Intent pickContact = new Intent(Intent.ACTION_PICK, ContactsContract.Contacts.CONTENT_URI);
        mSuspectButton = (Button)v.findViewById(R.id.crime_suspect);
        mSuspectButton.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v){
                startActivityForResult(pickContact, REQUEST_CONTACT);
            }
        });

        mCrimeCall = (Button)v.findViewById(R.id.crime_call);
        mCrimeCall.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                //***************
                //what we're gonna do here is pick a phone number and call it
                //***************

                Uri contentUri = ContactsContract.CommonDataKinds.Phone.CONTENT_URI;

                //we're gonna use the contacts ID from our phone book to read the ID from the phone database
                String selectClause = ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = ?";

                String[] queryFields = new String[] {ContactsContract.CommonDataKinds.Phone.NUMBER};
                String[] selectParams = new String[] {Long.toString(mCrime.getContactID())};

                Cursor cursor = getActivity().getContentResolver().query(contentUri, queryFields, selectClause, selectParams, null);

                //now we actually call the number
                int count = cursor.getCount();
                if(cursor != null && cursor.getCount() > 0){
                    try{
                        cursor.moveToFirst();
                        String number = cursor.getString(0);
                        Uri phoneNum = Uri.parse("tel:" + number);

                        //start an intent to dial the number but let the user press call
                        Intent i = new Intent(Intent.ACTION_DIAL, phoneNum);
                        startActivity(i);
                    }
                    finally {
                        cursor.close();
                    }
                }
            }
        });

        if(mCrime.getSuspect() != null){
            mSuspectButton.setText(mCrime.getSuspect());
            updateCallSuspectButton();
        }

        //disable the suspect button if we can't find any outside apps that
        //can give us a suspect
        PackageManager packageManager = getActivity().getPackageManager();
        if(packageManager.resolveActivity(pickContact, PackageManager.MATCH_DEFAULT_ONLY) == null){
            mSuspectButton.setEnabled(false);
        }


        mPhotoButton = (ImageButton)v.findViewById(R.id.crime_camera);

        final Intent captureImage = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);

        boolean canTakePhoto = mPhotoFile != null && captureImage.resolveActivity(packageManager) != null;
        mPhotoButton.setEnabled(canTakePhoto);

        mPhotoButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Uri uri = FileProvider.getUriForFile(getActivity(),
                        "com.bignerdranch.android.criminalintent.fileprovider",
                        mPhotoFile);
                captureImage.putExtra(MediaStore.EXTRA_OUTPUT, uri);

                List<ResolveInfo> cameraActivities = getActivity().getPackageManager().queryIntentActivities(captureImage, PackageManager.MATCH_DEFAULT_ONLY);

                for(ResolveInfo activity : cameraActivities){
                    getActivity().grantUriPermission(activity.activityInfo.packageName, uri, Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
                }

                startActivityForResult(captureImage, REQUEST_PHOTO);
            }
        });

        //this will set up the size of our photo view once a layout pass happens
        mPhotoView = (ImageView)v.findViewById(R.id.crime_photo);
        ViewTreeObserver observer = mPhotoView.getViewTreeObserver();
        observer.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                mPhotoViewSize = new Point();
                mPhotoViewSize.set(mPhotoView.getWidth(), mPhotoView.getHeight());
                updatePhotoView();

                //once we've set up the photo view, we don't need
                //to call this listener anymore because our image view is created
                mPhotoView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
            }
        });

        mPhotoView.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                FragmentManager manager = getFragmentManager();
                FullImageFragment dialog = FullImageFragment.newInstance(mPhotoFile);
                dialog.show(manager, DIALOG_FULL_PIC);
            }
        });

        return v;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data){

        if(resultCode != Activity.RESULT_OK){
            return;
        }

        if(requestCode == REQUEST_DATE){
            Date date = (Date) data.getSerializableExtra(DatePickerFragment.EXTRA_DATE);

            mCrime.setDate(date);
            updateCrime();
            updateDate();
        }
        else if(requestCode == REQUEST_TIME){
            Date date = (Date) data.getSerializableExtra(TimePickerFragment.EXTRA_TIME);
            mCrime.setTime(date);
            updateCrime();
            updateTime();
        }
        else if(requestCode == REQUEST_CONTACT && data != null){

            Uri contactUri = data.getData();
            //specify which fields you want your query to
            //return values for
            String[] queryFields = new String[]{
                    ContactsContract.Contacts.DISPLAY_NAME,
                    ContactsContract.Contacts._ID
            };

            //perform your query - the contactUri is like
            //a "where" clause here
            Cursor c = getActivity().getContentResolver().query(contactUri, queryFields, null, null, null);

            try{
                //double-check that you actually got
                //results
                if(c.getCount() == 0){
                    return;
                }

                //pull out the first column of the first row of data
                //this is your suspect's name
                //the second row of data is the user's contact ID, which we need to get their phone number
                //there is only one row of data: the contact the user picked
                c.moveToFirst();
                String suspect = c.getString(0);
                long contactID = c.getLong(1);

                mCrime.setSuspect(suspect);
                mCrime.setContactID(contactID);
                mSuspectButton.setText(suspect);

                updateCrime();

                // Get the suspect's mobile phone (cell phone) number.
                if (hasContactPermission()) {
                    updateCallSuspectButton();
                } else {
                    // This will call onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults).
                    requestPermissions(CONTACTS_PERMISSIONS, REQUEST_CONTACTS_PERMISSIONS);
                }
            }
            finally{
                c.close();
            }
        }
        else if(requestCode == REQUEST_PHOTO){
            Uri uri = FileProvider.getUriForFile(getActivity(),
                                                    "com.bignerdranch.android.criminalintent.fileprovider",
                                                    mPhotoFile);

            getActivity().revokeUriPermission(uri, Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
            updateCrime();
            updatePhotoView();

            mPhotoView.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mPhotoView.announceForAccessibility("Crime Scene Photo set");
                }
            }, 100);
        }
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater){
        super.onCreateOptionsMenu(menu, inflater);
        inflater.inflate(R.menu.single_fragment_crime_list, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        switch(item.getItemId()){
            case R.id.menu_item_delete_crime:
                if(getActivity().findViewById(R.id.detail_fragment_container) == null) {
                    CrimeLab.get(getActivity()).deleteCrime(mCrime.getId());
                    getActivity().finish();
                }
                else{
                    CrimeListActivity activity = (CrimeListActivity)getActivity();
                    activity.onCrimeIdDelete(mCrime.getId());
                }
                return true;

            default:
                return super.onOptionsItemSelected(item);
        }
    }

    private void updateCrime(){
        CrimeLab.get(getActivity()).updateCrime(mCrime);
        mCallbacks.onCrimeUpdated(mCrime);
    }

    private void updateDate(){
        Locale currentLocale = getResources().getConfiguration().locale;
        mDateButton.setText(mCrime.getDateFormatted(currentLocale));
    }

    private void updateTime(){
        Locale currentLocale = getResources().getConfiguration().locale;
        mTimeButton.setText(mCrime.getTimeFormatted(currentLocale));
    }

    private String getCrimeReport(){
        String solvedString = null;

        if(mCrime.isSolved()){
            solvedString = getString(R.string.crime_report_solved);
        }
        else{
            solvedString = getString(R.string.crime_report_unsolved);
        }

        String dateFormat = "EEE, MMM, dd";
        String dateString = DateFormat.format(dateFormat, mCrime.getDate()).toString();

        String suspect = mCrime.getSuspect();
        if(suspect == null){
            suspect = getString(R.string.crime_report_no_suspect);
        }
        else{
            suspect = getString(R.string.crime_report_suspect, suspect);
        }

        String report = getString(R.string.crime_report, mCrime.getTitle(), dateString, solvedString, suspect);

        return report;
    }

    private void updatePhotoView(){
        if(mPhotoFile == null || !mPhotoFile.exists()){
            mPhotoView.setImageDrawable(null);
            mPhotoView.setContentDescription(getString(R.string.crime_photo_no_image_description));
        }
        else{
            if(mPhotoViewSize == null){
                Bitmap bitmap = PictureUtils.getScaledBitmap(mPhotoFile.getPath(), getActivity());
                mPhotoView.setImageBitmap(bitmap);
            }
            else{
                //if we already set up the photo size with out ViewTreeObserver, we can create the bitmap with the coordinates we got
                Bitmap bitmap = PictureUtils.getScaledBitmap(mPhotoFile.getPath(), mPhotoViewSize.x, mPhotoViewSize.y);
                mPhotoView.setImageBitmap(bitmap);
            }
            mPhotoView.setContentDescription(getString(R.string.crime_photo_image_description));
        }
    }

    private void updateCallSuspectButton(){
        mCrimeCall.setEnabled(true);
        mCrimeCall.setText("Call " + mCrime.getSuspect() + "!");
    }

    private boolean hasContactPermission() {
        int result = ContextCompat.checkSelfPermission(getActivity(), CONTACTS_PERMISSIONS[0]);
        return result == PackageManager.PERMISSION_GRANTED;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        switch (requestCode) {
            case REQUEST_CONTACTS_PERMISSIONS:
                if (hasContactPermission()) {
                    updateCallSuspectButton();
                }
        }
    }
}
