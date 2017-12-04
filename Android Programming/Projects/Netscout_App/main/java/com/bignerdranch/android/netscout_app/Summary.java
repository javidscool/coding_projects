package com.bignerdranch.android.netscout_app;

import java.util.UUID;

public class Summary {

    private UUID mID;
    private String title;
    private String body;

    public Summary(String title, String menu, String subMenu){
        //generate unique identifier
        mID = UUID.randomUUID();

        this.title = title;

        if(subMenu.isEmpty()) {
            this.body = "Detail card for " + title + " element\nFor " + menu + " list";
        }
        else{
            this.body = "Detail card for " + title + " element\nFor " + menu + "/" + subMenu + " list";
        }
    }

    public UUID getID() {
        return mID;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }
}
