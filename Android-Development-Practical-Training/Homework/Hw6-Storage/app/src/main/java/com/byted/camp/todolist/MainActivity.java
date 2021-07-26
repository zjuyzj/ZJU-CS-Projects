package com.byted.camp.todolist;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import com.byted.camp.todolist.beans.Note;
import com.byted.camp.todolist.beans.Priority;
import com.byted.camp.todolist.beans.State;
import com.byted.camp.todolist.db.TodoContract;
import com.byted.camp.todolist.db.TodoDbHelper;
import com.byted.camp.todolist.debug.DebugActivity;
import com.byted.camp.todolist.debug.FileDemoActivity;
import com.byted.camp.todolist.debug.SpDemoActivity;
import com.byted.camp.todolist.ui.NoteListAdapter;

import java.util.Collections;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_CODE_ADD = 1002;

    private RecyclerView recyclerView;
    private NoteListAdapter notesAdapter;

    private TodoDbHelper dbHelper;
    private SQLiteDatabase database;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivityForResult(
                        new Intent(MainActivity.this, NoteActivity.class),
                        REQUEST_CODE_ADD);
            }
        });

        dbHelper = new TodoDbHelper(this);
        database = dbHelper.getWritableDatabase();

        recyclerView = findViewById(R.id.list_todo);
        recyclerView.setLayoutManager(new LinearLayoutManager(this,
                LinearLayoutManager.VERTICAL, false));
        recyclerView.addItemDecoration(
                new DividerItemDecoration(this, DividerItemDecoration.VERTICAL));
        notesAdapter = new NoteListAdapter(new NoteOperator() {
            @Override
            public void deleteNote(Note note) {
                if(database==null) return;
                String sel= TodoContract.TodoNote._ID+" = ?";
                String[] selArgs={String.valueOf(note.id)};
                database.delete(TodoContract.TodoNote.TABLE_NAME, sel, selArgs);
                notesAdapter.refresh(loadNotesFromDatabase());
            }

            @Override
            public void updateNote(Note note) {
                if(database==null) return;
                String sel=TodoContract.TodoNote._ID+" = ?";
                String[] selArgs={String.valueOf(note.id)};
                ContentValues content=new ContentValues();
                content.put(TodoContract.TodoNote.COLUMN_NAME_STATE,note.getState().intValue);
                database.update(TodoContract.TodoNote.TABLE_NAME, content,sel, selArgs);
                notesAdapter.refresh(loadNotesFromDatabase());
            }
        });
        recyclerView.setAdapter(notesAdapter);

        notesAdapter.refresh(loadNotesFromDatabase());

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        database.close();
        database = null;
        dbHelper.close();
        dbHelper = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            case R.id.action_settings:
                return true;
            case R.id.action_debug:
                startActivity(new Intent(this, DebugActivity.class));
                return true;
            case R.id.action_file:
                startActivity(new Intent(this, FileDemoActivity.class));
                return true;
            case R.id.action_sp:
                startActivity(new Intent(this, SpDemoActivity.class));
                return true;
            default:
                break;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE_ADD
                && resultCode == Activity.RESULT_OK) {
            notesAdapter.refresh(loadNotesFromDatabase());
        }
    }

    private List<Note> loadNotesFromDatabase() {
        if (database == null) {
            return Collections.emptyList();
        }
        List<Note> result = new LinkedList<>();
        String order=TodoContract.TodoNote.COLUMN_NAME_PRIORITY+" DESC";
        Cursor cursor=database.query(TodoContract.TodoNote.TABLE_NAME, null,
                null, null,
                null, null, order);
        while(cursor.moveToNext()){
            Note todo_item=new Note(cursor.getLong(cursor.getColumnIndex(TodoContract.TodoNote._ID)));
            todo_item.setContent(cursor.getString(cursor.getColumnIndex(TodoContract.TodoNote.COLUMN_NAME_CONTENT)));
            todo_item.setDate(new Date(cursor.getLong(cursor.getColumnIndex(TodoContract.TodoNote.COLUMN_NAME_DATE))));
            todo_item.setState(State.from(cursor.getInt(cursor.getColumnIndex(TodoContract.TodoNote.COLUMN_NAME_STATE))));
            todo_item.setPriority(Priority.from(cursor.getInt(cursor.getColumnIndex(TodoContract.TodoNote.COLUMN_NAME_PRIORITY))));
            result.add(todo_item);
        }
        cursor.close();
        return result;
    }
}
