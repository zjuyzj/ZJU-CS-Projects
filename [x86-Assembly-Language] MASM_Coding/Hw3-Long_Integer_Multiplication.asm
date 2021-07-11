.386
data segment use16
;���������뻺����
buf_1	db 6 ;��һ���ֽڴ���������������ַ��������س���
	db 0 ;�ڶ����ֽڴ��ʵ��������ַ�����ϵͳ����ʱ��䣩
	db 6 dup(0) ;�������ֽڴ��������ַ���ASCII�루���س���ϵͳ����ʱ��䣩
;�������뻺����
buf_2	db 6 ;16λ�޷��ų˷���ʮ���Ʊ�ʾ�������ַ������ϻس�0Dh�������ַ�
	db 0
	db 6 dup(0)
m_a dw 1 dup(0) ;�����ı�������16λ��
m_b dw 1 dup(0) ;�����ĳ�����16λ��
r dd 1 dup(0) ;�˷���������32λ��
data ends

code segment use16
assume cs:code,ds:data
start:

init:
mov ax,data
mov ds,ax ;��ʼ�����ݶμĴ���ds

input:
mov dx,offset buf_1 ;ʹDS:DXָ��buf_1
mov ah,0Ah
int 21h ;����int 21h��0Ah�������뱻����
mov ah,02h
mov dl,0Ah
int 21h ;����
mov dx,offset buf_2
mov ah,0Ah
int 21h ;ͬ���������

mov ah,02h ;׼������int 21h��02h������ַ�
;���������
mov bx,2 ;bx���������±꣬������������ǰ�����ֽ�
display_equation_num1:
mov dl,buf_1[bx]
inc bx ;�±�����1
cmp dl,0Dh ;���س����ж��Ƿ�������
je display_equation_sign
int 21h ;���һ���ַ�
jmp display_equation_num1 ;ѭ�����

;����˺�
display_equation_sign:
mov dl,'*'
int 21h

;ͬ���������
mov bx,2
display_equation_num2:
mov dl,buf_2[bx]
inc bx
cmp dl,0Dh
je display_equation_others
int 21h
jmp display_equation_num2

;����ȺŲ�������һ����
display_equation_others:
mov dl,'='
int 21h
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h

;������ı��������ַ�ת��Ϊ��
convert_input1:
mov ax,0 ;ax������ʱ��Ž��
mov bx,2 ;bx��������±�
mov dh,0 ;��֤dx�߰�λ�ǿյģ��������ļӷ������
atoi_1:
mov dl,buf_1[bx] ;dl��ŵ����ַ���8λ��
cmp dl,0Dh 
je convert_input2 ;������ת����ɺ�ת������
sub dl,'0' ;�ַ�ת����
push dx
mov cx,10 ;��cx�������10
mul cx
pop dx ;�ָ���˷����ƻ���dx
add ax,dx ;��ԭ�������10��ʮ��������һλ���������µĸ�λ
inc bx ;�±�����1
jmp atoi_1 ;ѭ��

;���汻���������������ַ�ת��Ϊ��
convert_input2:
mov m_a,ax ;���汻����
mov ax,0
mov bx,2 ;dh�Ѿ�����
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

;����������˷����㲢������
multiply:
mov m_b,ax ;�������
mov ax,m_a
mov bx,m_b
mul bx ;16λ�˷�����ĸ�16λ�����dx�У���16λ�����ax��
push dx
push ax
pop eax ;�����ƴ�ӳ�32λ�����eax�У�ע���ջ��ѭFIFO��С�˹���
mov r,eax ;���������ڴ�

;��ʮ����������
mov eax,r ;�����̿�����32λ�ģ�Ҫʹ��64λ������32λ���̣�����ֹ���
mov ebx,10 ;ebx�д�Ź̶��ĳ���10
mov cx,0 ;cx������ջ������ʵ�ֶ�ջ��ƽ��
output_dec:
mov edx,0 ;�������ĸ�32λ�����edx�У����������������������ÿ�γ���ǰ��Ҫ����
div ebx
add dl,'0' ;edx���������������һ��������8λ������ȡdl����ʮ��������ת��Ϊ�ַ�
push dx ;�ȵõ�ʮ���������λ��ASCII�룬�����ջ��ʵ�����������push������16λ�ģ�
inc cx ;��ջ������һ
cmp eax,0
je print_dec_reverse ;����λ������ɺ�ʼ�������
jmp output_dec ;ѭ�����
print_dec_reverse:
pop dx ;��Ҫ������ַ��Ӷ�ջȡ����ֱ�ӷ���dl
mov ah,02h
int 21h
dec cx
jnz print_dec_reverse ;û�������ɾͼ������
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h ;���е���һ����

;��ʮ������������(˼·������ʮ����������ڴ˻���������ж���ĸ/���ֵ��߼�)
mov eax,r
mov ebx,16
mov cx,16 ;�����������̶�������8����ջ��8�γ�ջ��ʵ�ֲ���8λ�����0/16=0��ʵ���˲�0��
output_hex:
mov edx,0
div ebx
cmp dl,9 ;�ж��Ƿ�Ӧ�ñ�ʾΪA-F
ja is_letter ;����ǣ������ת��Ϊ��ĸASCII�ķ���
add dl,'0' ;������ת��Ϊ����ASCII����
jmp output_hex_next ;���Ϊ���֣���Ҫ��ֹ����ĸʱ��ת�����뱻ִ�е�
is_letter: ;����ĸ�ǵ�ת������
sub dl,10
add dl,'A'
output_hex_next: ;��ASCII����ջ
push dx
dec cx
cmp cx,8 ;8����ջ��ɾͿ�ʼ���
je print_hex_reverse
jmp output_hex
print_hex_reverse:
pop dx
mov ah,02h
int 21h
dec cx
jnz print_hex_reverse
mov dl,'h' ;���ʮ�����Ʊ�־'h'
int 21h
mov dl,0Ah
int 21h
mov dl,0Dh
int 21h ;����

;�Զ�����������
mov eax,r
mov si,8 ;si������ѭ������
;��ѭ��������ĸ�һ��Ķ�������
output_bin:
mov cx,0 ;cx������ѭ��������ÿ���4λ�����һ���ո�
;���һλ
print_a_bit:
shl eax,1 ;eax����һλ��ʹ���λ����CF
jc print_1
push eax ;��ʾ�ַ�ʱ��������ж���Ҫ�ı�ah��ע�Ᵽ��eax
mov ah,02h
mov dl,'0'
int 21h ;CF�в���1������ַ�'0'
pop eax ;�ָ�eax
inc cx ;�����һλ��cx����1
cmp cx,4
je print_space
jmp print_a_bit
;����ո�
print_space:
dec si ;�����һ����λ�������si�Լ�1
jz print_B ;����������������Ʊ�־'B'���˳�����
push eax
mov ah,02h
mov dl,' '
int 21h
pop eax
jmp output_bin ;��������ո����������һ��
;����ַ�'1'
print_1:
push eax
mov ah,02h
mov dl,'1'
int 21h ;����ַ�'1'
pop eax
inc cx ;�����һλ��cx����1
cmp cx,4
je print_space ;���һ�����������ո�
jmp print_a_bit ;���������һλ
print_B:
mov ah,02h
mov dl,'B'
int 21h ;��������Ʊ�־'B'

;����DOSϵͳ
exit_to_DOS:
mov ah,4Ch
int 21h

code ends
end start