#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <cstdlib>
#include <ctime>
using namespace std;

#define random(x) (::rand()%(x))
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define PROB_MONSTER 0.15
#define PROB_UNSAFE 0.3
#define PROB_UNSAFE_MONSTER 0.2
#define CLS_COMMAND "clear"

typedef enum room_types{lobby,stable,unsafe} room_type;
typedef enum directions{north,west,upstairs,south,east,downstairs} direction;
string room_types_dict[3]={"Lobby","Normal","Dangerous"};
string directions_dict[6]={"north","west","upstairs","south","east","downstairs"};

bool prob_generator(double prob){
    if(prob>1) return true;
    else if(prob<=0) return false;
    else{
        if(random(100)+1>100*prob) return false;
        else return true;
    }
}

template <class T>
int get_index(T* array,T elem,int size){
    int index;
    for(index=0;index<size;index++)
        if(array[index]==elem) break;
    if(index>=size) return -1;
    else return index;
}

class Room{
private:
    room_types room_type;
    int room_level;
    map<direction,Room*> room_linker;
    bool has_monster;
    bool has_princess;
public:
    Room(int max_rooms,int room_level,bool gen_princess,directions entrance_dir,Room* entrance_room);
    inline string get_info(void);
    map<direction,Room*> get_exit(void){return room_linker;}
    void room_update(void);
    bool check_lobby(void) {return (room_type==lobby);}
    bool check_princess(void) {return has_princess;}
    bool check_monster(void) {return has_monster;}
};

class Game{
private:
    Room* entrance;
    static int min_level;
    static int max_level;
    bool found_princess;
public:
    Game(int max_rooms,int level_range);
    void start(void);
    friend class Room;
};

Room::Room(int max_rooms,int room_level,bool gen_princess,directions entrance_dir,Room* entrance_room){
    /********************Set basic information*******************/
    max_rooms--;
    this->room_level=room_level;
    room_linker[entrance_dir]=entrance_room;
    /******Allocate room type and generate princess/monster******/
    if(entrance_room==nullptr){
        room_type=lobby;
        if(max_rooms==0) has_princess=true;
        else has_princess=false;
        has_monster=false;
    }else{
        if(max_rooms==0){ //End of generation(deepest)
            room_type=stable;
            has_princess=gen_princess;
            if(prob_generator(PROB_MONSTER)&&!gen_princess) has_monster=true;
            else has_monster=false;
        }else{
            has_princess=false; //Princess always in the deepest room
            if(prob_generator(PROB_UNSAFE)){
                room_type=unsafe;
                has_monster=false; //No monster in unsafe room when first visited
            }else{
                room_type=stable;
                if(!gen_princess)
                    if(prob_generator(PROB_MONSTER)) has_monster=true;
                    else has_monster=false;
                //Stable room on the way to the princess' room is always safe
                else has_monster=false;
            }
        }
    }
    /****************Generate new rooms recursively**************/
    if(max_rooms!=0){
        int i,next_level,total_num=max_rooms,subroom_num,subroom_cap,subroom_princess;
        directions room_dir,opposite_dir;
        bool illegal_gen,subroom_gen_princess;
        stack<int> num_part;
        subroom_num=random(min(max_rooms,5))+1; //At least one room in one direction of subrooms
        subroom_princess=random(subroom_num)+1;
        for(i=0;i<subroom_num;i++){ //Generate a partition of the number of rooms on each direction
            if(i!=subroom_num-1){
                subroom_cap=random(total_num-(subroom_num-i-1))+1; //At least one room in one direction of subrooms
                num_part.push(subroom_cap);
                total_num-=num_part.top();
            }else num_part.push(total_num);
        }
        for(i=0;i<subroom_num;i++){
            room_dir=(directions)random(5);
            if(room_dir==upstairs) next_level=(room_level==-1)?(1):(room_level+1);
            else if(room_dir==downstairs) next_level=(room_level==1)?(-1):(room_level-1);
            else next_level=room_level;
            illegal_gen=next_level>(Game::max_level)||(next_level<Game::min_level);
            illegal_gen|=(room_dir==entrance_dir);
            if(illegal_gen){ //Out of range of level limit
                i--;
                continue;
            }
            opposite_dir=(directions)((room_dir+3)%6);
            subroom_cap=num_part.top();
            subroom_gen_princess=((i+1)==subroom_princess)?true:false; //Only one princess
            subroom_gen_princess=subroom_gen_princess&&gen_princess;
            room_linker[room_dir]=new Room(subroom_cap,next_level,subroom_gen_princess,opposite_dir,this);
            num_part.pop();
        }
    }
}

string Room::get_info(void){
    string temp="Current Position:"+room_types_dict[room_type]+" Room";
    temp=temp+",Floor:"+::to_string(room_level);
    return temp;
}

void Room::room_update(void){
    if(room_type==unsafe) //In unsafe room,generate monster randomly
        if(prob_generator(PROB_UNSAFE_MONSTER)) has_monster=true;
        else has_monster=false;
}

int Game::max_level=1;
int Game::min_level=0;
Game::Game(int max_rooms,int level_range){
    ::srand((int)time(NULL));
    int rand=random(level_range);
    Game::max_level=max(rand,level_range-rand);
    Game::min_level=-(level_range-max_level);
    found_princess=false;
    entrance=new Room(max_rooms,1,true,south,nullptr);
}

void Game::start(void){
    string input,exits_str;
    Room* current_room=entrance;
    map<directions,Room*> exits;
    map<directions,Room*>::iterator p;
    ::system(CLS_COMMAND);
    cout<<"Welcome to the game Adventure!"<<endl;
    cout<<"On the north of you lies the entrance"<<endl;
    cout<<"<Press Enter Key to enter the castle>"<<endl;
    ::getchar();
    ::system(CLS_COMMAND);
    while(1){
        cout<<current_room->get_info()<<endl;
        exits=current_room->get_exit();
        if(found_princess==true&&current_room->check_lobby()){
            cout<<"Congratulations!You rescued the princess!"<<endl;
            cout<<"The game is going to exit..."<<endl;
            return;
        }else if(current_room->check_monster()){
            cout<<"You are eatten by the monster!"<<endl;
            cout<<"Game Over!"<<endl;
            return;
        }else if(current_room->check_princess()){
            cout<<"You meet the princess!"<<endl;
            cout<<"Please find the way out!"<<endl;
            found_princess=true;
        }
        while(1){
            exits_str="";
            for(p=exits.begin();p!=exits.end();p++)
                if(p->second!=nullptr) exits_str=exits_str+directions_dict[p->first]+" ";
            cout<<"Directions:"<<exits_str<<endl;
            cout<<"Please enter command:"<<endl;
            getline(cin,input);
            ::system(CLS_COMMAND);
            if(input=="exit"){
                cout<<"Game Aborted!"<<endl;
                return;
            }else if(input.substr(0,3)!="go "){
                cout<<"Wrong Command!"<<endl;
                cout<<"Available Commands:go <direction>,exit"<<endl;
            }else{
                if(get_index(directions_dict,input.substr(3),6)==-1||exits_str.find(input.substr(3))==string::npos){
                    cout<<"Direction \""<<input.substr(3)<<"\" NOT usable!"<<endl;
                    continue;
                }
                current_room->room_update(); //Update the room when leaving
                current_room=exits[(directions)get_index(directions_dict,input.substr(3),6)];
                break;
            }
        }
    }
}

int main(void){
    Game adventure(10,2);
    adventure.start();
    return 0;
}
