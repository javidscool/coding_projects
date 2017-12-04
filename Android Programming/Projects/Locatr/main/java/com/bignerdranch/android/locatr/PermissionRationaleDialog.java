package com.bignerdranch.android.locatr;

import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AlertDialog;

/**
 * Created by javidscool on 12/3/2017.
 */

public class PermissionRationaleDialog extends DialogFragment {

    public static PermissionRationaleDialog newInstance(){
        PermissionRationaleDialog fragment = new PermissionRationaleDialog();
        return fragment;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        return new AlertDialog.Builder(getActivity())
                .setMessage("Locatr uses location data to find images near you on Flickr")
                .setPositiveButton("Got it", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        dismiss();
                    }
                })
                .create();
    }

    @Override
    public void onDismiss(DialogInterface dialog) {
        super.onDismiss(dialog);
        requestPermissions(LocatrFragment.LOCATION_PERMISSIONS, LocatrFragment.REQUEST_LOCATION_PERMISSIONS);
    }
}
