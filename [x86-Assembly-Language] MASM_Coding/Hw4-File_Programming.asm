data segment
filename_buf db 16,0,16 dup(0) ;文件名输入缓冲区，含回车共16个字符（含‘.’共15个有效字符），支持DOS 8.3式文件名和部分长文件名
hex_to_search_buf db 100,0,100 dup(0) ;存放待搜索字符串，最大支持99个字符（含空格）
hex_to_search db 20 dup(0) ;经过转换的待搜索十六进制数，去除空格后以二进制密集存储（两个十六进制数占一个字节，含空格99个字符分配20个字节一定不会溢出）
fp dw 0 ;文件指针
f_len dw 0,0 ;文件未被载入过的长度，执行file_init初始化后为文件长度
f_len_total dw 0,0 ;文件总长度
f_buf db 400h dup(0) ;文件缓冲区，两个扇区的大小（2*512B=1KB)
f_wptr dw 0,0 ;文件工作指针，指向下一次需要读入文件缓冲区的文件位置
len_to_read dw 0 ;待读取的文件长度
l_hex dw 0 ;待搜索串转换成数后占用的字节数
result dw 0,0 ;结果（成功查找点到文件头部的距离）
massages_A db 'Input file name:',0Ah,0Dh,'$'
massages_B db 0Ah,0Dh,'Input a hex string, e.g. 41 42 43 0D 0A:',0Ah,0Dh,'$'
massages_C db 0Ah,0Dh,'Fail to open file!','$' ;提示信息以'$'结尾方便调用int 21h(09h)输出
massages_D db 0Ah,0Dh,'Not found!',0Ah,0Dh,'$'
massages_E db 0Ah,0Dh,'Found at ','$'
data ends

stk segment stack ;开辟堆栈段供子程序保存/恢复寄存器使用
s dw 14 dup(0)
stk ends

code segment
assume cs:code,ds:data,ss:stk

input proc ;输入函数，负责输入文件名和要搜索的十六进制值到对应变量
mov ah,09h
mov dx,offset massages_A
int 21h ;输出提示信息
mov ah,0Ah
mov dx,offset filename_buf
int 21h ;缓冲输入文件名称
mov ah,09h
mov dx,offset massages_B
int 21h ;输出提示信息
mov ah,0Ah
mov dx,offset hex_to_search_buf
int 21h ;缓冲输入待搜索字符串
mov bl,filename_buf[1]
xor bh,bh ;将bh清零
add bx,2 ;将bx定位到输入输入字符串的末尾后一个字节,即回车'0Dh'处
mov byte ptr filename_buf[bx],0 ;将'0Dh'改为0，使其满足ASCIZ filename的要求
ret
input endp


file_open proc ;打开文件函数，cx返回打开情况（为0则打开失败），成功后handle（16位）放在ds:(offset fp)中
mov ah,3Dh ;21h中断的3Dh号功能：打开已存在的文件并获取该文件的handle
mov al,0 ;al=0表示访问模式为只读
mov dx,offset filename_buf
add dx,2 ;ds:dx指向以'\0'结束的ASCIZ文件名字符串
int 21h
jc file_open_error ;文件打开错误会使CF标志置1，ax返回错误码；文件成功打开会通过ax返回文件指针
mov [fp],ax
mov cx,1
ret
file_open_error:
mov ah,09h
mov dx,offset massages_C
int 21h ;输出错误信息
mov cx,0 ;通知主程序文件打开错误
ret
file_open endp


file_close proc
mov ah,3Eh ;21h中断的3Eh号功能用于关闭文件
mov bx,[fp] ;bx中存放file handle
int 21h
mov word ptr [fp],0 ;关闭文件后清除文件指针
ret
file_close endp


file_init proc ;获取文件长度，放在ds:(offset f_len)中(小端规则，32位)，并定位到文件头
mov ah,42h ;21h中断的42h号功能用于定位文件指针文件
mov bx,[fp] ;bx中存放file handle
mov al,2 ;al=2，定位EOF
and cx,0
and dx,0 ;清空cx与dx，即使结果的偏移为0
int 21h
mov f_len[0],ax
mov f_len[2],dx ;dx:ax返回EOF到文件头部的字节数，即文件长度
mov f_len_total[0],ax
mov f_len_total[2],dx ;f_len_total在搜索中不作改变，并在结果计算时调用
mov ah,42h
mov al,0 ;al=0，定位文件头，方便文件操作
and cx,0
and dx,0
int 21h
ret
file_init endp


file_load proc ;将文件工作指针f_wptr指向的文件部分（长度为len_to_read）读入f_buf
push ax ;保护通用寄存器，在主程序中段调用时非常必要
push bx
push cx
push dx
mov ah,42h
mov bx,[fp]
mov al,0 ;以文件头为参考位置
mov cx,f_wptr[2]
mov dx,f_wptr[0] ;f_wptr遵循小端规则
int 21h ;将文件定位到文件工作指针f_wptr指向的位置
mov ah,3Fh ;21h中断的3Fh号功能用来读文件到文件缓冲区
mov bx,[fp] ;bx中存放文件指针
mov cx,[len_to_read] ;cx存放要读入文件缓冲区的文件字节数
mov dx,offset f_buf ;ds:dx指向文件缓冲区
int 21h ;读入指定长度到缓冲区
pop dx
pop cx
pop bx
pop ax ;恢复通用寄存器
ret
file_load endp


hex_atoi proc ;将输入十六进制串转为整数的函数，只接受大写字母，只被执行一次
mov al,hex_to_search_buf[1]
xor ah,ah
add ax,1
mov bx,3
div bx
mov dx,ax;得到待搜索串转换成数后占用的字节数，保存在dx中用作循环计数
mov [l_hex],ax ;同时存储该字节数以方便搜索
call string_ltou
mov di,0 ;di存放转换后数字的偏移
lea bx,hex_to_search_buf[2] ;bx保存字符所在内存的偏移地址
convert_a_byte:
mov ah,[bx]
cmp ah,'A'
jb ah_is_number
sub ah,'A'
add ah,10
jmp convert_al
ah_is_number:
sub ah,'0'
convert_al:
mov al,[bx+1]
cmp al,'A'
jb al_is_number ;字母与数字用各自的方法转换
sub al,'A'
add al,10
jmp finish
al_is_number:
sub al,'0'
finish:
mov cl,4
shl ah,cl
or al,ah ;将结果拼成一个字节放入al中
mov hex_to_search[di],al ;存放一组转换结果
inc di 
add bx,3 ;跳过空格
sub dx,1 ;完成一组转换
jnz convert_a_byte ;没有完成转换则继续
ret
hex_atoi endp


string_ltou proc ;将十六进制串可能存在的小写字母全部与转化为大写字母（lower_case to upper_case)
push ax
push bx
push cx
push dx
mov cl,hex_to_search_buf[1] ;cl用作计数
inc cl ;由于cl的初值未计入回车，而cl先自减1，判断之后才转换，故cx先要自增1
mov si,1 ;si用作偏移
next:
dec cl
jz done ;完成转换则返回主调函数
inc si ;si第一次自增后的值为2，指向第一个字符
mov al,hex_to_search_buf[si] ;al存储转换的中间值
cmp al,'a'
jb next
cmp al,'z'
ja next
sub al,'a'
add al,'A' ;是小写字母就转换为对应大写字母
mov hex_to_search_buf[si],al ;保存转换结果，替换原小写字母
jmp next
done:
pop dx
pop cx
pop bx
pop ax
ret
string_ltou endp


search_hex proc ;核心的查找函数,实现文件缓冲区载入逻辑和查找逻辑，只被执行一次
mov word ptr [f_wptr],0 ;文件工作指针的初始化
reload:
mov ax,[f_len]
cmp ax,400h
ja load_full
mov [len_to_read],ax ;若未读取的文件大小不超过缓冲区大小，读取长度为文件大小
jmp do_load
load_full:
mov word ptr [len_to_read],400h ;若未读取的文件大小大于缓冲区大小，则把缓冲区读满
do_load:
call file_load ;读文件到缓冲区
cmp [len_to_read],400h
jne last_find ;如果上一次读取的文件大小不足缓冲区大小，说明文件读完了，进行最后一次匹配
add word ptr [f_wptr],400h 
mov ax,[l_hex] ;如果文件没读完，向前移动工作指针到未读取部分的起始
sub ax,1
sub word ptr [f_wptr],ax ;缓冲区最后（l_hex-1）个字节需要放到缓冲区头进行重载，否则不能进行完整的匹配
mov ax,400h
sub ax,[l_hex]
add ax,1
sub f_len[0],ax
sbb f_len[2],0 ;未读取的文件长度减去本次读取的长度

find:
clc ;CF=0，正方向进行串操作
mov bx,0 ;bx为查找点的缓冲区偏移
jmp search_a_point
search_next_point:
inc bx ;查找点的偏移自增1，指向下一个查找点
search_a_point:
lea si,f_buf[bx]
mov di,offset hex_to_search ;cmpsb相当于[ds:si]-[es:di]后舍弃差值保留FL标志寄存器的值
mov cx,l_hex ;串长度为l_hex
repe cmpsb ;进行逐字节的串比较操作，终止条件：cx=0（找到了）或出现不匹配（当前查找点处找不到）
cmp cx,0 ;判断是否找到
jne may_need_reload ;只要进行了最后一次比较，无论最后一次是否匹配，cx都为0
may_succeed: ;若cx=0，判断ds:[di-1]与es:[si-1]两个字节的数据是否真的相等
mov cl,ds:[di-1]
mov dl,es:[si-1]
cmp cl,dl
je succeed ;如果相等，则成功找到，否则当前查找点没找到
jmp may_need_reload
may_need_reload:
mov dx,400h
sub dx,[l_hex]
add dx,1
cmp bx,dx
jb search_next_point ;如果没找到且不需要重载缓冲区就从下一个查找点开始查找
jmp reload ;需要重载缓冲区

last_find:
mov dx,[len_to_read] ;获取最后一次读的字节数到dx
sub f_len[0],dx
sbb f_len[2],0 ;未读取的文件长度减去本次读取的长度
cmp dx,[l_hex]
jb failed ;最后未搜索的长度比待搜索的数据长度短，则一定找不到
sub dx,[l_hex] ;作差后dx为搜索点偏移的最大值
mov bx,0 ;bx为查找点的缓冲区偏移
jmp last_search_a_point
last_search_next_point:
inc bx
last_search_a_point:
lea si,f_buf[bx]
mov di,offset hex_to_search
mov cx,l_hex ;串长度为l_hex
repe cmpsb
cmp cx,0 ;判断是否找到
je last_may_succeed
may_last_search_next_point: ;判断是否需要移动查找点继续匹配
cmp bx,dx
jb last_search_next_point
jmp failed
last_may_succeed:
mov cl,ds:[di-1]
mov dl,es:[si-1]
cmp cl,dl
je succeed ;如果相等，则成功找到，否则没找到
jmp may_last_search_next_point

failed:
mov ah,09h
mov dx,offset massages_D
int 21h ;输出未找到的提示
jmp exit

succeed:
mov ah,09h
mov dx,offset massages_E
int 21h ;显示'Found at '
mov ax,[len_to_read] ;将上次载入缓冲区的长度放入ax
sub ax,bx ;bx中存放的是成功时的查找点偏移,得到该缓冲区中结果之后的长度
add f_len[0],ax
adc f_len[2],0 ;用带进位加法加上当前缓冲区中结果之后的长度，将f_len加上换算为结果之后的文件总长
mov ax,f_len[0]
mov bx,f_len_total[0]
sub bx,ax
mov cx,f_len[2]
mov dx,f_len_total[2]
sbb dx,cx ;带借位减法计算查找成功点到文件头部的距离
mov result[0],bx
mov result[2],dx ;结果存放在变量result中，小端规则
call display_hex ;将结果转换为十六进制并显示

exit:
ret
search_hex endp


display_hex proc ;将结果转换为十六进制并显示
mov ax,result[2] ;先显示高位
mov di,4 ;di用作循环计数
high_display_a_bit:
mov cl,4 ;cl存放固定的循环左移次数（4）
rol ax,cl
mov bx,0Fh
and bx,ax ;bx只有低四位被使用
cmp bx,10
jae high_convert_to_letter
add bx,'0'
jmp high_display
high_convert_to_letter:
sub bx,10
add bx,'A'
high_display:
push ax ;这里调用中断会破坏ax，将ax压入栈保护
mov ah,02h
mov dl,bl
int 21h
pop ax ;恢复ax
dec di
jnz high_display_a_bit

mov ax,result[0] ;后显示低位
mov di,4
low_display_a_bit:
mov cl,4
rol ax,cl
mov bx,0Fh
and bx,ax
cmp bx,10
jae low_convert_to_letter
add bx,'0'
jmp low_display
low_convert_to_letter:
sub bx,10
add bx,'A'
low_display:
push ax
mov ah,02h
mov dl,bl
int 21h
pop ax
dec di
jnz low_display_a_bit

mov ah,02h
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h ;完成输出，换行使结果明显
ret
display_hex endp


main:
mov ax,data
mov ds,ax ;初始化数据段寄存器
mov es,ax ;初始化附加段寄存器，由于要使用串操作，这里非常重要！
call input ;一开始就调用且仅调用一次，可以不保护寄存器
call file_open
jcxz exit_to_DOS ;文件打开出错则退出程序
call file_init
call hex_atoi
call search_hex
call file_close
exit_to_DOS:
mov ah,01h
int 21h ;用字符输入“卡住”显示以方便观察输出
mov ah,4Ch ;返回DOS系统
int 21h
code ends
end main