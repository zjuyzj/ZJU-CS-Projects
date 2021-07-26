package com.byted.camp.todolist.db;

import android.provider.BaseColumns;

public final class TodoContract {

    private TodoContract() {}

    public static final String CREATE_TABLE="create table "+TodoNote.TABLE_NAME+"("
            +TodoNote._ID+" integer primary key autoincrement,"
            +TodoNote.COLUMN_NAME_CONTENT+" text,"
            +TodoNote.COLUMN_NAME_DATE+" integer,"
            +TodoNote.COLUMN_NAME_STATE+" integer,"
            +TodoNote.COLUMN_NAME_PRIORITY+" integer)";

    public static final String DROP_TABLE="drop table "+TodoNote.TABLE_NAME+" if exists";

    public static class TodoNote implements BaseColumns {
        public static final String TABLE_NAME="todos";
        public static final String COLUMN_NAME_CONTENT="content";
        public static final String COLUMN_NAME_DATE="date";
        public static final String COLUMN_NAME_STATE="state";
        public static final String COLUMN_NAME_PRIORITY="priority";
    }

}
