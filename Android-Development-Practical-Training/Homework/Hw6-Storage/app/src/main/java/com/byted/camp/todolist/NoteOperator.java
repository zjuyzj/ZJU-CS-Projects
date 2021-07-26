package com.byted.camp.todolist;


import com.byted.camp.todolist.beans.Note;

public interface NoteOperator {

    void deleteNote(Note note);

    void updateNote(Note note);
}
