#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
using namespace std;

class Student{
private:
    string stu_name;
    string stu_id;
    map<string,double> stu_courses;
public:
    Student(string stu_id,string stu_name="");
    void set_name(string stu_name);
    void add_course(string course_name,double course_mark);
    string get_id(void);
    string get_name(void);
    double get_mark(string course);
    double get_average(void);
};

class Course{
private:
    string course_name;
    int selected_times;
    double total_mark;
public:
    Course(string course_name);
    void add_mark(double single_mark);
    double get_average(void);
};

class Database{
private:
    map<string,Student*> stu_relation;
    map<string,Course*> course_relation;
public:
    void insert_student_info(string student_id,string student_name);
    void insert_student_mark(string student_id,string course_name,double course_mark);
    double get_courses_average(void);
    void print_csv(void);
};

int judge_input_type(string s);
string slice_by_comma(string s,int key_selection);
double string_to_double(string s);

int main(void){
    Database data;
    string temp_str;
    while(getline(cin,temp_str)){
        if(judge_input_type(temp_str))
            data.insert_student_mark(slice_by_comma(temp_str,0),slice_by_comma(temp_str,1),string_to_double(slice_by_comma(temp_str,2)));
        else data.insert_student_info(slice_by_comma(temp_str,0),slice_by_comma(temp_str,1));
    }
    data.print_csv();
    return 0;
}

int judge_input_type(string s){
    int comma_count=0;
    while(s.find_first_of(",")!=string::npos){
        comma_count++;
        s=s.substr(s.find_first_of(",")+1);
    }
    if(comma_count==1) return 0;
    else return 1;
}

string slice_by_comma(string s,int key_selection){
    int comma_count=0;
    string result="";
    while(s.length()>0){
        if(comma_count==(key_selection+1)) break;
        else if(s[0]==','){
            s.erase(0,2);
            comma_count++;
        }else if(comma_count==key_selection){
            result+=s[0];
            s.erase(0,1);
        }else s.erase(0,1);
    }
    return result;
}

double string_to_double(string s){
    double result;
    istringstream temp_stream(s);
    temp_stream>>result;
    return result;
}

Student::Student(string stu_id,string stu_name){
    this->stu_name=stu_name;
    this->stu_id=stu_id;
    return;
}

void Student::set_name(string stu_name){
    this->stu_name=stu_name;
    return;
}

void Student::add_course(string course_name,double course_mark){
    stu_courses[course_name]=course_mark;
    return;
}

string Student::get_id(void){
    return stu_id;
}
string Student::get_name(void){
    return stu_name;
}

double Student::get_mark(string course){
    if(stu_courses.find(course)!=stu_courses.end())
        return stu_courses[course];
    else return -1;
}

double Student::get_average(void){
    map<string,double>::iterator p;
    double marks_sum=0;
    int courses_count=0;
    for(p=stu_courses.begin();p!=stu_courses.end();p++){
        marks_sum+=p->second;
        courses_count++;
    }
    if(courses_count==0) return -1;
    else return marks_sum/courses_count;
}

Course::Course(string course_name){
    this->course_name=course_name;
    this->selected_times=0;
    this->total_mark=0;
    return;
}

void Course::add_mark(double single_mark){
    this->selected_times++;
    this->total_mark+=single_mark;
    return;
}

double Course::get_average(void){
    if(selected_times==0) return -1;
    else return total_mark/selected_times;
}

void Database::insert_student_info(string student_id,string student_name){
    if(stu_relation.find(student_id)!=stu_relation.end()){
        stu_relation[student_id]->set_name(student_name);
        return;
    }else{
        stu_relation[student_id]=new Student(student_id,student_name);
        return;
    }
}

void Database::insert_student_mark(string student_id,string course_name,double course_mark){
    if(stu_relation.find(student_id)!=stu_relation.end())
        stu_relation[student_id]->add_course(course_name,course_mark);
    else{
        stu_relation[student_id]=new Student(student_id);
        stu_relation[student_id]->add_course(course_name,course_mark);
    }
    if(course_relation.find(course_name)!=course_relation.end()){
        course_relation[course_name]->add_mark(course_mark);
        return;
    }else{
        course_relation[course_name]=new Course(course_name);
        course_relation[course_name]->add_mark(course_mark);
        return;
    }
}

double Database::get_courses_average(void){
    map<string,Course*>::iterator p;
    int courses_count=0;
    double courses_average_sum=0;
    for(p=course_relation.begin();p!=course_relation.end();p++){
        courses_average_sum+=p->second->get_average();
        courses_count++;
    }
    if(courses_count==0) return -1;
    else return courses_average_sum/courses_count;
}

void Database::print_csv(void){
    map<string,Course*>::iterator p_course;
    map<string,Student*>::iterator p_student;
    cout<<"student id, name";
    for(p_course=course_relation.begin();p_course!=course_relation.end();p_course++)
        cout<<", "<<p_course->first;
    cout<<", average"<<endl;
    for(p_student=stu_relation.begin();p_student!=stu_relation.end();p_student++){
        cout<<p_student->second->get_id()<<", "<<p_student->second->get_name();
        for(p_course=course_relation.begin();p_course!=course_relation.end();p_course++){
            if(p_student->second->get_mark(p_course->first)!=-1)
                cout<<fixed<<setprecision(1)<<", "<<p_student->second->get_mark(p_course->first);
            else cout<<", ";
        }
        if(p_student->second->get_average()==-1) cout<<", "<<endl;
        else cout<<fixed<<setprecision(1)<<", "<<p_student->second->get_average()<<endl;
    }
    if(course_relation.begin()!=course_relation.end()){
        cout<<", ";
        for(p_course=course_relation.begin();p_course!=course_relation.end();p_course++)
            cout<<", "<<p_course->second->get_average()<<fixed<<setprecision(1);
        if(get_courses_average()==-1) cout<<", "<<endl;
        else cout<<fixed<<setprecision(1)<<", "<<get_courses_average()<<endl;
    }
}
