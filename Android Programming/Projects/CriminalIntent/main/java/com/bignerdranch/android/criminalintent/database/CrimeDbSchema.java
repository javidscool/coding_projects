package com.bignerdranch.android.criminalintent.database;

/**
 * Created by javidscool on 4/8/2017.
 */

public class CrimeDbSchema {
    public static final class CrimeTable {
        public static final String NAME = "crimes";

        public static final class Cols {
            public static final String UUID = "uuid";
            public static final String TITLE = "title";
            public static final String DATE = "date";
            public static final String TIME = "time";
            public static final String SOLVED = "solved";
            public static final String POLICE = "police";
            public static final String SUSPECT = "suspect";
            public static final String CONTACT_ID = "contact_id";
        }
    }
}
