.386
data segment use16
;被乘数输入缓冲区
buf_1	db 6 ;第一个字节存放限制输入的最大字符数（含回车）
	db 0 ;第二个字节存放实际输入的字符数（系统运行时填充）
	db 6 dup(0) ;第三个字节存放输入的字符的ASCII码（含回车，系统运行时填充）
;乘数输入缓冲区
buf_2	db 6 ;16位无符号乘法，十进制表示最多五个字符，加上回车0Dh共六个字符
	db 0
	db 6 dup(0)
m_a dw 1 dup(0) ;换算后的被乘数（16位）
m_b dw 1 dup(0) ;换算后的乘数（16位）
r dd 1 dup(0) ;乘法运算结果（32位）
data ends

code segment use16
assume cs:code,ds:data
start:

init:
mov ax,data
mov ds,ax ;初始化数据段寄存器ds

input:
mov dx,offset buf_1 ;使DS:DX指向buf_1
mov ah,0Ah
int 21h ;调用int 21h的0Ah功能输入被乘数
mov ah,02h
mov dl,0Ah
int 21h ;换行
mov dx,offset buf_2
mov ah,0Ah
int 21h ;同理输入乘数

mov ah,02h ;准备调用int 21h的02h来输出字符
;输出被乘数
mov bx,2 ;bx用作数组下标，跳过缓冲区的前两个字节
display_equation_num1:
mov dl,buf_1[bx]
inc bx ;下标自增1
cmp dl,0Dh ;检测回车以判断是否完成输出
je display_equation_sign
int 21h ;输出一个字符
jmp display_equation_num1 ;循环输出

;输出乘号
display_equation_sign:
mov dl,'*'
int 21h

;同理，输出乘数
mov bx,2
display_equation_num2:
mov dl,buf_2[bx]
inc bx
cmp dl,0Dh
je display_equation_others
int 21h
jmp display_equation_num2

;输出等号并换到下一行首
display_equation_others:
mov dl,'='
int 21h
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h

;将输入的被乘数的字符转化为数
convert_input1:
mov ax,0 ;ax用作暂时存放结果
mov bx,2 ;bx存放数组下标
mov dh,0 ;保证dx高八位是空的，否则后面的加法会出错
atoi_1:
mov dl,buf_1[bx] ;dl存放单个字符（8位）
cmp dl,0Dh 
je convert_input2 ;被乘数转换完成后转换乘数
sub dl,'0' ;字符转数字
push dx
mov cx,10 ;用cx存放因子10
mul cx
pop dx ;恢复因乘法被破坏的dx
add ax,dx ;将原结果将乘10（十进制左移一位）并加上新的个位
inc bx ;下标自增1
jmp atoi_1 ;循环

;保存被乘数，将乘数的字符转化为数
convert_input2:
mov m_a,ax ;保存被乘数
mov ax,0
mov bx,2 ;dh已经清零
atoi_2:
mov dl,buf_2[bx]
cmp dl,0Dh 
je multiply
sub dl,'0'
push dx
mov cx,10
mul cx
pop dx
add ax,dx
inc bx
jmp atoi_2

;保存乘数，乘法运算并保存结果
multiply:
mov m_b,ax ;保存乘数
mov ax,m_a
mov bx,m_b
mul bx ;16位乘法结果的高16位存放在dx中，低16位存放在ax中
push dx
push ax
pop eax ;将结果拼接成32位存放在eax中，注意堆栈遵循FIFO和小端规则
mov r,eax ;保存结果到内存

;以十进制输出结果
mov eax,r ;由于商可能是32位的，要使用64位除法（32位的商）来防止溢出
mov ebx,10 ;ebx中存放固定的除数10
mov cx,0 ;cx用作入栈计数，实现堆栈的平衡
output_dec:
mov edx,0 ;被除数的高32位存放在edx中，而除法运算后存放余数，故每次除法前都要清零
div ebx
add dl,'0' ;edx存放余数，且余数一定不超过8位，所以取dl，将十进制数码转换为字符
push dx ;先得到十进制数最低位的ASCII码，放入堆栈以实现逆序输出（push至少是16位的）
inc cx ;堆栈计数加一
cmp eax,0
je print_dec_reverse ;各个位分离完成后开始逆序输出
jmp output_dec ;循环输出
print_dec_reverse:
pop dx ;将要输出的字符从堆栈取出并直接放入dl
mov ah,02h
int 21h
dec cx
jnz print_dec_reverse ;没有输出完成就继续输出
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h ;换行到下一行首

;以十六进制输出结果(思路类似以十进制输出，在此基础上添加判断字母/数字的逻辑)
mov eax,r
mov ebx,16
mov cx,16 ;计数变量，固定次数：8次入栈，8次出栈以实现补足8位输出（0/16=0，实现了补0）
output_hex:
mov edx,0
div ebx
cmp dl,9 ;判断是否应该表示为A-F
ja is_letter ;如果是，则采用转换为字母ASCII的方法
add dl,'0' ;否则按照转换为数字ASCII处理
jmp output_hex_next ;如果为数字，则要防止是字母时的转换代码被执行到
is_letter: ;是字母是的转换代码
sub dl,10
add dl,'A'
output_hex_next: ;将ASCII码入栈
push dx
dec cx
cmp cx,8 ;8次入栈完成就开始输出
je print_hex_reverse
jmp output_hex
print_hex_reverse:
pop dx
mov ah,02h
int 21h
dec cx
jnz print_hex_reverse
mov dl,'h' ;输出十六进制标志'h'
int 21h
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h ;换行

;以二进制输出结果
mov eax,r
mov si,8 ;si用作外循环计数
;外循环，输出四个一组的二进制数
output_bin:
mov cx,0 ;cx用作内循环计数，每输出4位就输出一个空格
;输出一位
print_a_bit:
shl eax,1 ;eax左移一位，使最高位进入CF
jc print_1
push eax ;显示字符时调用软件中断需要改变ah，注意保护eax
mov ah,02h
mov dl,'0'
int 21h ;CF中不是1就输出字符'0'
pop eax ;恢复eax
inc cx ;已输出一位，cx自增1
cmp cx,4
je print_space
jmp print_a_bit
;输出空格
print_space:
dec si ;已完成一组四位的输出，si自减1
jz print_B ;完成输出则输出二进制标志'B'并退出程序
push eax
mov ah,02h
mov dl,' '
int 21h
pop eax
jmp output_bin ;否则输出空格后继续输出下一组
;输出字符'1'
print_1:
push eax
mov ah,02h
mov dl,'1'
int 21h ;输出字符'1'
pop eax
inc cx ;已输出一位，cx自增1
cmp cx,4
je print_space ;完成一组输出后输出空格
jmp print_a_bit ;否则输出下一位
print_B:
mov ah,02h
mov dl,'B'
int 21h ;输出二进制标志'B'

;返回DOS系统
exit_to_DOS:
mov ah,4Ch
int 21h

code ends
end start