data segment
s db 100 dup(0)
t db 100 dup(0)
data ends

code segment
assume cs:code,ds:data
main: ;�������
mov ax,data
mov ds,ax ;��ʼ�����ݶμĴ���
mov bx,0 ;bx�����洢�����±꣬��ʼ��


s_buf: ;���������ӳ���
mov ah,1
int 21h ;����int 21h��1�Ź�������һ���ַ������浽al
mov s[bx],al ;�ȱ���������ַ����ڴ�
inc bx ;�����±�����1
cmp al,0Dh ;��⻻�з������̻س���
jne s_buf ;δ����س���ѭ��ִ��s_buf�ӳ���
mov bx,0 ;���³�ʼ��s���±������Ϊ���ݴ�����׼��
mov si,0 ;��ʼ��t���±����


s_proc: ;���ݴ����ӳ���
mov al,s[bx] ;ȡ�����ݷ���al
cmp al,0Dh ;����ַ���s��ȡ���
je t_out ;�������t
inc bx ;����s�±�����1

space: ;�жϿո�
cmp al,20h
je s_proc ;���洢��t

lower_case: ;�жϲ�����Сд��ĸ
cmp al,61h
jb is_other ;ASCIIС��'a'����Сд��ĸ
cmp al,7Ah ;ASCII����'z'����Сд��ĸ
ja is_other
sub al,32 ;ת��Ϊ��д��ĸ
mov t[si],al ;�洢������t
inc si ;t���±�����1
jmp s_proc ;������һ���ַ�

is_other: ;�����ַ��Ĵ���
mov t[si],al ;ԭ����������t
inc si ;t���±�����1
jmp s_proc ;������һ���ַ�

t_out: ;��������ӳ���
mov dl,0Ah
mov ah,2
int 21h 
mov dl,0Dh
mov ah,2
int 21h ;���һ���س���һ�����з�����ֹ����������غ���ʾ
mov t[si],0Dh ;������tĩ�����ӻس�
mov t[si+1],0Ah ;������tĩ�����ӻ��з�
mov t[si+2],'$' ;������tĩ������'$'
mov ah,9
mov dx,offset t
int 21h ;����int 21h��9�Ź��������'$'��β���ַ���t
mov ah,4Ch
mov al,0
int 21h ;����DOS����ϵͳ��������Ϊ1

code ends
end main