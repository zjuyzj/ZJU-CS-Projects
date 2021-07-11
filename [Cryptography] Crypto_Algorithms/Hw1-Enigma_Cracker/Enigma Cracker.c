#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char reflector[26]="YRUHQSLDPXNGOKMIEBFZCWVJAT"; 
const char rotor_table[5][27]= //start from 0
{
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
    "AJDKSIRUXBLHWTMCQGZNPYFVOE",
    "BDFHJLCPRTXVZNYEIWGAKMUSQO",
    "ESOVPZJAYQUIRHXLNFTGKDCMWB",
    "VZBRGITYUPSDNHLXAWMJQOFECK"
};
const char step_char[5]="RFWKA";    // Royal Flags Wave Kings Above

typedef struct{
    int sel_rotor;
    char ring_rotor;
    char stat_rotor;
}rotor;

typedef struct{
    rotor rotor_l;
    rotor rotor_c;
    rotor rotor_r;
}rotors;

typedef struct{
    rotors rotors;
    char plugboard[26];
}machine;

void machine_init(machine* enigma,char* plugboard_conf,char* ring_conf,int left_rotor_stat);
void machine_conf(machine* enigma,int c_rotor_sel,int r_rotor_sel,char l_rotor_stat,char c_rotor_stat,char r_rotor_stat);
char lookup_single_rotor(char input,int dir,int rotor_pos,const machine* enigma);
char char_conv(char input,machine* enigma);
char* str_conv(char* str,machine* enigma);
char* enigma_crack(machine* enigma,char* cipher,char* word);

int main(void){
    machine enigma;
    char plugboard_conf[32],ring_conf[5];
    char word[22],cipher[502];
    char* plain;
    int l_rotor_sel;
    fgets(plugboard_conf,32,stdin);
    fgets(ring_conf,5,stdin);
    fgets(cipher,502,stdin);
    fgets(word,22,stdin);
    scanf("%d",&l_rotor_sel);
    machine_init(&enigma,plugboard_conf,ring_conf,l_rotor_sel);
    
    //machine_conf(&enigma,3,2,'Z','V','E');
    //plain=str_conv(cipher,&enigma);
    //printf("%s\n",plain);

    plain=enigma_crack(&enigma,cipher,word);
    printf("MessageKey=%c%c%c\n",enigma.rotors.rotor_l.stat_rotor,enigma.rotors.rotor_c.stat_rotor,enigma.rotors.rotor_r.stat_rotor);
    printf("PlainText=%s\n",plain);;
    printf("RotorNum=%d%d%d\n",enigma.rotors.rotor_l.sel_rotor,enigma.rotors.rotor_c.sel_rotor,enigma.rotors.rotor_r.sel_rotor);
    return 0;
}

void machine_init(machine* enigma,char* plugboard_conf,char* ring_conf,int l_rotor_sel){
    int i;
    for(i=0;i<26;i++) enigma->plugboard[i]='\0';    //unused
    for(i=0;i<10;i++){
        enigma->plugboard[plugboard_conf[3*i+1]-'A']=plugboard_conf[3*i];
        enigma->plugboard[plugboard_conf[3*i]-'A']=plugboard_conf[3*i+1];
    }
    enigma->rotors.rotor_l.ring_rotor=ring_conf[0];
    enigma->rotors.rotor_c.ring_rotor=ring_conf[1];
    enigma->rotors.rotor_r.ring_rotor=ring_conf[2];
    enigma->rotors.rotor_l.sel_rotor=l_rotor_sel;
    enigma->rotors.rotor_c.sel_rotor=1;
    enigma->rotors.rotor_r.sel_rotor=1; //start from 1
    enigma->rotors.rotor_l.stat_rotor='A';
    enigma->rotors.rotor_c.stat_rotor='A';
    enigma->rotors.rotor_r.stat_rotor='A';
    return;
}

void machine_conf(machine* enigma,int c_rotor_sel,int r_rotor_sel,char l_rotor_stat,char c_rotor_stat,char r_rotor_stat){
    enigma->rotors.rotor_c.sel_rotor=c_rotor_sel;
    enigma->rotors.rotor_r.sel_rotor=r_rotor_sel;
    enigma->rotors.rotor_l.stat_rotor=l_rotor_stat;
    enigma->rotors.rotor_c.stat_rotor=c_rotor_stat;
    enigma->rotors.rotor_r.stat_rotor=r_rotor_stat;
    return;
}

char lookup_single_rotor(char input,int dir,int rotor_pos,const machine* enigma){
    char temp,result;
    int delta,sel_rotor,i;

    switch(rotor_pos){
        case 1:
            delta=enigma->rotors.rotor_r.stat_rotor-enigma->rotors.rotor_r.ring_rotor;
            sel_rotor=enigma->rotors.rotor_r.sel_rotor;
            break;
        case 2:
            delta=enigma->rotors.rotor_c.stat_rotor-enigma->rotors.rotor_c.ring_rotor;
            sel_rotor=enigma->rotors.rotor_c.sel_rotor;
            break;
        case 3:
            delta=enigma->rotors.rotor_l.stat_rotor-enigma->rotors.rotor_l.ring_rotor;
            sel_rotor=enigma->rotors.rotor_l.sel_rotor;
            break;
    }

    temp=input+delta;
    if(temp<'A') temp+=26;
    else if(temp>'Z') temp-=26;
    if(dir==1) temp=rotor_table[sel_rotor-1][temp-'A']; //lookup from right to left
    else{
        for(i=0;i<26;i++) if(rotor_table[sel_rotor-1][i]==temp) break; //lookup from left to right
        temp='A'+i;
    }
    result=temp-delta;
    if(result<'A') result+=26;
    else if(result>'Z') result-=26;
    
    return result;
}

char char_conv(char input,machine* enigma){   //both for encoding and decoding(symmetric)
    char result,temp;

    //rotor stepping logic
    enigma->rotors.rotor_r.stat_rotor++;
    if(enigma->rotors.rotor_r.stat_rotor>'Z') enigma->rotors.rotor_r.stat_rotor='A';
    if(enigma->rotors.rotor_r.stat_rotor==step_char[enigma->rotors.rotor_r.sel_rotor-1]){
        enigma->rotors.rotor_c.stat_rotor++;
        if(enigma->rotors.rotor_c.stat_rotor>'Z') enigma->rotors.rotor_c.stat_rotor='A';
        if(enigma->rotors.rotor_c.stat_rotor==step_char[enigma->rotors.rotor_c.sel_rotor-1]){
            enigma->rotors.rotor_l.stat_rotor++;
            if(enigma->rotors.rotor_l.stat_rotor>'Z') enigma->rotors.rotor_l.stat_rotor='A';
        }
    }else if(enigma->rotors.rotor_c.stat_rotor==(step_char[enigma->rotors.rotor_c.sel_rotor-1]-1)){
        enigma->rotors.rotor_c.stat_rotor++;
        if(enigma->rotors.rotor_c.stat_rotor>'Z') enigma->rotors.rotor_c.stat_rotor='A';
        enigma->rotors.rotor_l.stat_rotor++;
        if(enigma->rotors.rotor_l.stat_rotor>'Z') enigma->rotors.rotor_l.stat_rotor='A';
    }

    //plugboard input dir
    if(enigma->plugboard[input-'A']!='\0') temp=enigma->plugboard[input-'A'];
    else temp=input;

    temp=lookup_single_rotor(temp,1,1,enigma);  //rotor 1 input dir
    temp=lookup_single_rotor(temp,1,2,enigma);  //rotor 2 input dir
    temp=lookup_single_rotor(temp,1,3,enigma);  //rotor 3 input dir
    temp=reflector[temp-'A'];   //reflector
    temp=lookup_single_rotor(temp,2,3,enigma);  //rotor 3 output dir
    temp=lookup_single_rotor(temp,2,2,enigma);  //rotor 2 output dir
    temp=lookup_single_rotor(temp,2,1,enigma);  //rotor 1 output dir

    //plugboard output dir
    if(enigma->plugboard[temp-'A']!='\0') result=enigma->plugboard[temp-'A'];
    else result=temp;
 
    return result;
}

char* str_conv(char* str,machine* enigma){
    if(str==NULL) return NULL;
    int pos=0;
    char letter=str[pos];
    char* result=(char*)malloc(sizeof(char)*(strlen(str)+1));
    while(letter!='\n'&&letter!='\0'){  //no '\n' in result
        result[pos]=char_conv(letter,enigma);
        letter=str[++pos];
    }
    result[pos]='\0';
    return result;
}

char* enigma_crack(machine* enigma,char* cipher,char* word){
    char r_rotor_stat,l_rotor_stat,c_rotor_stat;
    int c_rotor_sel,r_rotor_sel;
    char* plain;
    word[strlen(word)-1]='\0';    //delete '\n'
    for(c_rotor_sel=1;c_rotor_sel<=5;c_rotor_sel++)
        for(r_rotor_sel=1;r_rotor_sel<=5;r_rotor_sel++)
            for(l_rotor_stat='A';l_rotor_stat<='Z';l_rotor_stat++)
                for(c_rotor_stat='A';c_rotor_stat<='Z';c_rotor_stat++)
                    for(r_rotor_stat='A';r_rotor_stat<='Z';r_rotor_stat++){
                        machine_conf(enigma,c_rotor_sel,r_rotor_sel,l_rotor_stat,c_rotor_stat,r_rotor_stat);
                        plain=str_conv(cipher,enigma);
                        if(strstr(plain,word)){
                            machine_conf(enigma,c_rotor_sel,r_rotor_sel,l_rotor_stat,c_rotor_stat,r_rotor_stat);
                            return plain;
                        }else free(plain);
                    }
    return NULL;
}
