data segment
filename_buf db 16,0,16 dup(0) ;�ļ������뻺���������س���16���ַ�������.����15����Ч�ַ�����֧��DOS 8.3ʽ�ļ����Ͳ��ֳ��ļ���
hex_to_search_buf db 100,0,100 dup(0) ;��Ŵ������ַ��������֧��99���ַ������ո�
hex_to_search db 20 dup(0) ;����ת���Ĵ�����ʮ����������ȥ���ո���Զ������ܼ��洢������ʮ��������ռһ���ֽڣ����ո�99���ַ�����20���ֽ�һ�����������
fp dw 0 ;�ļ�ָ��
f_len dw 0,0 ;�ļ�δ��������ĳ��ȣ�ִ��file_init��ʼ����Ϊ�ļ�����
f_len_total dw 0,0 ;�ļ��ܳ���
f_buf db 400h dup(0) ;�ļ������������������Ĵ�С��2*512B=1KB)
f_wptr dw 0,0 ;�ļ�����ָ�룬ָ����һ����Ҫ�����ļ����������ļ�λ��
len_to_read dw 0 ;����ȡ���ļ�����
l_hex dw 0 ;��������ת��������ռ�õ��ֽ���
result dw 0,0 ;������ɹ����ҵ㵽�ļ�ͷ���ľ��룩
massages_A db 'Input file name:',0Ah,0Dh,'$'
massages_B db 0Ah,0Dh,'Input a hex string, e.g. 41 42 43 0D 0A:',0Ah,0Dh,'$'
massages_C db 0Ah,0Dh,'Fail to open file!','$' ;��ʾ��Ϣ��'$'��β�������int 21h(09h)���
massages_D db 0Ah,0Dh,'Not found!',0Ah,0Dh,'$'
massages_E db 0Ah,0Dh,'Found at ','$'
data ends

stk segment stack ;���ٶ�ջ�ι��ӳ��򱣴�/�ָ��Ĵ���ʹ��
s dw 14 dup(0)
stk ends

code segment
assume cs:code,ds:data,ss:stk

input proc ;���뺯�������������ļ�����Ҫ������ʮ������ֵ����Ӧ����
mov ah,09h
mov dx,offset massages_A
int 21h ;�����ʾ��Ϣ
mov ah,0Ah
mov dx,offset filename_buf
int 21h ;���������ļ�����
mov ah,09h
mov dx,offset massages_B
int 21h ;�����ʾ��Ϣ
mov ah,0Ah
mov dx,offset hex_to_search_buf
int 21h ;��������������ַ���
mov bl,filename_buf[1]
xor bh,bh ;��bh����
add bx,2 ;��bx��λ�����������ַ�����ĩβ��һ���ֽ�,���س�'0Dh'��
mov byte ptr filename_buf[bx],0 ;��'0Dh'��Ϊ0��ʹ������ASCIZ filename��Ҫ��
ret
input endp


file_open proc ;���ļ�������cx���ش������Ϊ0���ʧ�ܣ����ɹ���handle��16λ������ds:(offset fp)��
mov ah,3Dh ;21h�жϵ�3Dh�Ź��ܣ����Ѵ��ڵ��ļ�����ȡ���ļ���handle
mov al,0 ;al=0��ʾ����ģʽΪֻ��
mov dx,offset filename_buf
add dx,2 ;ds:dxָ����'\0'������ASCIZ�ļ����ַ���
int 21h
jc file_open_error ;�ļ��򿪴����ʹCF��־��1��ax���ش����룻�ļ��ɹ��򿪻�ͨ��ax�����ļ�ָ��
mov [fp],ax
mov cx,1
ret
file_open_error:
mov ah,09h
mov dx,offset massages_C
int 21h ;���������Ϣ
mov cx,0 ;֪ͨ�������ļ��򿪴���
ret
file_open endp


file_close proc
mov ah,3Eh ;21h�жϵ�3Eh�Ź������ڹر��ļ�
mov bx,[fp] ;bx�д��file handle
int 21h
mov word ptr [fp],0 ;�ر��ļ�������ļ�ָ��
ret
file_close endp


file_init proc ;��ȡ�ļ����ȣ�����ds:(offset f_len)��(С�˹���32λ)������λ���ļ�ͷ
mov ah,42h ;21h�жϵ�42h�Ź������ڶ�λ�ļ�ָ���ļ�
mov bx,[fp] ;bx�д��file handle
mov al,2 ;al=2����λEOF
and cx,0
and dx,0 ;���cx��dx����ʹ�����ƫ��Ϊ0
int 21h
mov f_len[0],ax
mov f_len[2],dx ;dx:ax����EOF���ļ�ͷ�����ֽ��������ļ�����
mov f_len_total[0],ax
mov f_len_total[2],dx ;f_len_total�������в����ı䣬���ڽ������ʱ����
mov ah,42h
mov al,0 ;al=0����λ�ļ�ͷ�������ļ�����
and cx,0
and dx,0
int 21h
ret
file_init endp


file_load proc ;���ļ�����ָ��f_wptrָ����ļ����֣�����Ϊlen_to_read������f_buf
push ax ;����ͨ�üĴ��������������жε���ʱ�ǳ���Ҫ
push bx
push cx
push dx
mov ah,42h
mov bx,[fp]
mov al,0 ;���ļ�ͷΪ�ο�λ��
mov cx,f_wptr[2]
mov dx,f_wptr[0] ;f_wptr��ѭС�˹���
int 21h ;���ļ���λ���ļ�����ָ��f_wptrָ���λ��
mov ah,3Fh ;21h�жϵ�3Fh�Ź����������ļ����ļ�������
mov bx,[fp] ;bx�д���ļ�ָ��
mov cx,[len_to_read] ;cx���Ҫ�����ļ����������ļ��ֽ���
mov dx,offset f_buf ;ds:dxָ���ļ�������
int 21h ;����ָ�����ȵ�������
pop dx
pop cx
pop bx
pop ax ;�ָ�ͨ�üĴ���
ret
file_load endp


hex_atoi proc ;������ʮ�����ƴ�תΪ�����ĺ�����ֻ���ܴ�д��ĸ��ֻ��ִ��һ��
mov al,hex_to_search_buf[1]
xor ah,ah
add ax,1
mov bx,3
div bx
mov dx,ax;�õ���������ת��������ռ�õ��ֽ�����������dx������ѭ������
mov [l_hex],ax ;ͬʱ�洢���ֽ����Է�������
call string_ltou
mov di,0 ;di���ת�������ֵ�ƫ��
lea bx,hex_to_search_buf[2] ;bx�����ַ������ڴ��ƫ�Ƶ�ַ
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
jb al_is_number ;��ĸ�������ø��Եķ���ת��
sub al,'A'
add al,10
jmp finish
al_is_number:
sub al,'0'
finish:
mov cl,4
shl ah,cl
or al,ah ;�����ƴ��һ���ֽڷ���al��
mov hex_to_search[di],al ;���һ��ת�����
inc di 
add bx,3 ;�����ո�
sub dx,1 ;���һ��ת��
jnz convert_a_byte ;û�����ת�������
ret
hex_atoi endp


string_ltou proc ;��ʮ�����ƴ����ܴ��ڵ�Сд��ĸȫ����ת��Ϊ��д��ĸ��lower_case to upper_case)
push ax
push bx
push cx
push dx
mov cl,hex_to_search_buf[1] ;cl��������
inc cl ;����cl�ĳ�ֵδ����س�����cl���Լ�1���ж�֮���ת������cx��Ҫ����1
mov si,1 ;si����ƫ��
next:
dec cl
jz done ;���ת���򷵻���������
inc si ;si��һ���������ֵΪ2��ָ���һ���ַ�
mov al,hex_to_search_buf[si] ;al�洢ת�����м�ֵ
cmp al,'a'
jb next
cmp al,'z'
ja next
sub al,'a'
add al,'A' ;��Сд��ĸ��ת��Ϊ��Ӧ��д��ĸ
mov hex_to_search_buf[si],al ;����ת��������滻ԭСд��ĸ
jmp next
done:
pop dx
pop cx
pop bx
pop ax
ret
string_ltou endp


search_hex proc ;���ĵĲ��Һ���,ʵ���ļ������������߼��Ͳ����߼���ֻ��ִ��һ��
mov word ptr [f_wptr],0 ;�ļ�����ָ��ĳ�ʼ��
reload:
mov ax,[f_len]
cmp ax,400h
ja load_full
mov [len_to_read],ax ;��δ��ȡ���ļ���С��������������С����ȡ����Ϊ�ļ���С
jmp do_load
load_full:
mov word ptr [len_to_read],400h ;��δ��ȡ���ļ���С���ڻ�������С����ѻ���������
do_load:
call file_load ;���ļ���������
cmp [len_to_read],400h
jne last_find ;�����һ�ζ�ȡ���ļ���С���㻺������С��˵���ļ������ˣ��������һ��ƥ��
add word ptr [f_wptr],400h 
mov ax,[l_hex] ;����ļ�û���꣬��ǰ�ƶ�����ָ�뵽δ��ȡ���ֵ���ʼ
sub ax,1
sub word ptr [f_wptr],ax ;���������l_hex-1�����ֽ���Ҫ�ŵ�������ͷ�������أ������ܽ���������ƥ��
mov ax,400h
sub ax,[l_hex]
add ax,1
sub f_len[0],ax
sbb f_len[2],0 ;δ��ȡ���ļ����ȼ�ȥ���ζ�ȡ�ĳ���

find:
clc ;CF=0����������д�����
mov bx,0 ;bxΪ���ҵ�Ļ�����ƫ��
jmp search_a_point
search_next_point:
inc bx ;���ҵ��ƫ������1��ָ����һ�����ҵ�
search_a_point:
lea si,f_buf[bx]
mov di,offset hex_to_search ;cmpsb�൱��[ds:si]-[es:di]��������ֵ����FL��־�Ĵ�����ֵ
mov cx,l_hex ;������Ϊl_hex
repe cmpsb ;�������ֽڵĴ��Ƚϲ�������ֹ������cx=0���ҵ��ˣ�����ֲ�ƥ�䣨��ǰ���ҵ㴦�Ҳ�����
cmp cx,0 ;�ж��Ƿ��ҵ�
jne may_need_reload ;ֻҪ���������һ�αȽϣ��������һ���Ƿ�ƥ�䣬cx��Ϊ0
may_succeed: ;��cx=0���ж�ds:[di-1]��es:[si-1]�����ֽڵ������Ƿ�������
mov cl,ds:[di-1]
mov dl,es:[si-1]
cmp cl,dl
je succeed ;�����ȣ���ɹ��ҵ�������ǰ���ҵ�û�ҵ�
jmp may_need_reload
may_need_reload:
mov dx,400h
sub dx,[l_hex]
add dx,1
cmp bx,dx
jb search_next_point ;���û�ҵ��Ҳ���Ҫ���ػ������ʹ���һ�����ҵ㿪ʼ����
jmp reload ;��Ҫ���ػ�����

last_find:
mov dx,[len_to_read] ;��ȡ���һ�ζ����ֽ�����dx
sub f_len[0],dx
sbb f_len[2],0 ;δ��ȡ���ļ����ȼ�ȥ���ζ�ȡ�ĳ���
cmp dx,[l_hex]
jb failed ;���δ�����ĳ��ȱȴ����������ݳ��ȶ̣���һ���Ҳ���
sub dx,[l_hex] ;�����dxΪ������ƫ�Ƶ����ֵ
mov bx,0 ;bxΪ���ҵ�Ļ�����ƫ��
jmp last_search_a_point
last_search_next_point:
inc bx
last_search_a_point:
lea si,f_buf[bx]
mov di,offset hex_to_search
mov cx,l_hex ;������Ϊl_hex
repe cmpsb
cmp cx,0 ;�ж��Ƿ��ҵ�
je last_may_succeed
may_last_search_next_point: ;�ж��Ƿ���Ҫ�ƶ����ҵ����ƥ��
cmp bx,dx
jb last_search_next_point
jmp failed
last_may_succeed:
mov cl,ds:[di-1]
mov dl,es:[si-1]
cmp cl,dl
je succeed ;�����ȣ���ɹ��ҵ�������û�ҵ�
jmp may_last_search_next_point

failed:
mov ah,09h
mov dx,offset massages_D
int 21h ;���δ�ҵ�����ʾ
jmp exit

succeed:
mov ah,09h
mov dx,offset massages_E
int 21h ;��ʾ'Found at '
mov ax,[len_to_read] ;���ϴ����뻺�����ĳ��ȷ���ax
sub ax,bx ;bx�д�ŵ��ǳɹ�ʱ�Ĳ��ҵ�ƫ��,�õ��û������н��֮��ĳ���
add f_len[0],ax
adc f_len[2],0 ;�ô���λ�ӷ����ϵ�ǰ�������н��֮��ĳ��ȣ���f_len���ϻ���Ϊ���֮����ļ��ܳ�
mov ax,f_len[0]
mov bx,f_len_total[0]
sub bx,ax
mov cx,f_len[2]
mov dx,f_len_total[2]
sbb dx,cx ;����λ����������ҳɹ��㵽�ļ�ͷ���ľ���
mov result[0],bx
mov result[2],dx ;�������ڱ���result�У�С�˹���
call display_hex ;�����ת��Ϊʮ�����Ʋ���ʾ

exit:
ret
search_hex endp


display_hex proc ;�����ת��Ϊʮ�����Ʋ���ʾ
mov ax,result[2] ;����ʾ��λ
mov di,4 ;di����ѭ������
high_display_a_bit:
mov cl,4 ;cl��Ź̶���ѭ�����ƴ�����4��
rol ax,cl
mov bx,0Fh
and bx,ax ;bxֻ�е���λ��ʹ��
cmp bx,10
jae high_convert_to_letter
add bx,'0'
jmp high_display
high_convert_to_letter:
sub bx,10
add bx,'A'
high_display:
push ax ;��������жϻ��ƻ�ax����axѹ��ջ����
mov ah,02h
mov dl,bl
int 21h
pop ax ;�ָ�ax
dec di
jnz high_display_a_bit

mov ax,result[0] ;����ʾ��λ
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
int 21h ;������������ʹ�������
ret
display_hex endp


main:
mov ax,data
mov ds,ax ;��ʼ�����ݶμĴ���
mov es,ax ;��ʼ�����ӶμĴ���������Ҫʹ�ô�����������ǳ���Ҫ��
call input ;һ��ʼ�͵����ҽ�����һ�Σ����Բ������Ĵ���
call file_open
jcxz exit_to_DOS ;�ļ��򿪳������˳�����
call file_init
call hex_atoi
call search_hex
call file_close
exit_to_DOS:
mov ah,01h
int 21h ;���ַ����롰��ס����ʾ�Է���۲����
mov ah,4Ch ;����DOSϵͳ
int 21h
code ends
end main