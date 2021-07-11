data segment
s db 100 dup(0)
t db 100 dup(0)
data ends

code segment
assume cs:code,ds:data
main: ;程序入口
mov ax,data
mov ds,ax ;初始化数据段寄存器
mov bx,0 ;bx用作存储数组下标，初始化


s_buf: ;数据输入子程序
mov ah,1
int 21h ;调用int 21h的1号功能输入一个字符并保存到al
mov s[bx],al ;先保存输入的字符到内存
inc bx ;数组下标自增1
cmp al,0Dh ;检测换行符（键盘回车）
jne s_buf ;未输入回车则循环执行s_buf子程序
mov bx,0 ;重新初始化s的下标计数，为数据处理作准备
mov si,0 ;初始化t的下标计数


s_proc: ;数据处理子程序
mov al,s[bx] ;取出数据放入al
cmp al,0Dh ;如果字符串s读取完成
je t_out ;输出数组t
inc bx ;数组s下标自增1

space: ;判断空格
cmp al,20h
je s_proc ;不存储进t

lower_case: ;判断并处理小写字母
cmp al,61h
jb is_other ;ASCII小于'a'则不是小写字母
cmp al,7Ah ;ASCII大于'z'则不是小写字母
ja is_other
sub al,32 ;转换为大写字母
mov t[si],al ;存储进数组t
inc si ;t的下标自增1
jmp s_proc ;处理下一个字符

is_other: ;其他字符的处理
mov t[si],al ;原样存入数组t
inc si ;t的下标自增1
jmp s_proc ;处理下一个字符

t_out: ;数据输出子程序
mov dl,0Ah
mov ah,2
int 21h 
mov dl,0Dh
mov ah,2
int 21h ;输出一个回车和一个换行符，防止输入与输出重合显示
mov t[si],0Dh ;在数组t末端增加回车
mov t[si+1],0Ah ;在数组t末端增加换行符
mov t[si+2],'$' ;在数组t末端增加'$'
mov ah,9
mov dx,offset t
int 21h ;调用int 21h的9号功能输出以'$'结尾的字符串t
mov ah,4Ch
mov al,0
int 21h ;返回DOS操作系统，返回码为1

code ends
end main