package com.bignerdranch.android.netscout_app;

import android.content.Context;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.UUID;

public class SummaryLab {

    private static SummaryLab sSummaryLab;

    private List<List<List<Summary>>> mSummaries;

    public SummaryLab(HashMap<String, List<String>> menuOptions){

        TreeMap<String, List<String>> orderedMenuOptions = new TreeMap<String, List<String>>(menuOptions);
        mSummaries = new ArrayList<List<List<Summary>>>();

        for(Map.Entry<String, List<String>> entry : orderedMenuOptions.entrySet()){

            String key = entry.getKey();
            List<String> value = entry.getValue();

            //make our menuList
            List<List<Summary>> menuList = new ArrayList<List<Summary>>();

            //first add main menu items
            ArrayList<Summary> mainMenu = new ArrayList<Summary>();
            for(int i = 0; i < 30; i++){

                String title = "Summary" + (11 + i);
                mainMenu.add(new Summary(title, key, ""));
            }
            menuList.add(mainMenu);

            if(value.isEmpty()){
                mSummaries.add(menuList);

                continue;
            }
            else {
                //next add sub menu items
                for (int i = 0; i < value.size(); i++) {
                    ArrayList<Summary> subMenu = new ArrayList<Summary>();
                    for (int j = 0; j < 30; j++) {
                        String title = "Summary" + (11 + j);
                        subMenu.add(new Summary(title, key, value.get(i)));
                    }
                    menuList.add(subMenu);
                }

                mSummaries.add(menuList);
            }
        }
    }

    public List<Summary> getSummaries(int group, int child){
        return mSummaries.get(group).get(child);
    }

    public List<Summary> getSummaries(int group){
        return mSummaries.get(group).get(0);
    }

    public Summary getSummary(UUID id){
        for(int i = 0; i < mSummaries.size(); i++) {
            for (int j = 0; j < mSummaries.get(i).size(); j++) {
                for (Summary summary : mSummaries.get(i).get(j)) {
                    if (summary.getID().equals(id)) {
                        return summary;
                    }
                }
            }
        }
        return null;
    }
}
