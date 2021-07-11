#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/sha.h>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

char A[0x100], B[0x100], PE[0x100];   // ecc���߲���a, b, p
char GX[0x100], GY[0x100], NE[0x100]; // ����G��x����, y����, G�Ľ�(��Ϊ256λ)
char DE[0x100];                       // ecc��˽Կd(256λ)
char N1[2][0x100], D1[2][0x100], X2[2][0x100]; 
                // N1�����ecc��Կ���ܹ���rsa����N, 
                // ����N1[0]��ŵ�1��������, N1[1]��ŵ�2��������;
                // D1�����ecc��Կ���ܹ���rsa˽Կd;
                // X2�����ecc��Կ���ܹ���X1
char X1[0x100]; // X1�����rsa��Կ���ܹ���X                      
char X[0x100];  // X���һ��256λ�������, X<N �� X<NE
char N[0x100], D[0x100];       // rsa��N��˽Կd(256λ)
char RX[0x100], RY[0x100];     // ecc��ԿR��x���꼰y���� 
char C[2][0x100]; // �����ecc��Կ���ܹ���X, C[0]�ǵ�1��������, C[1]�ǵ�2��������
char S[2][0x100]; // �����ecnrǩ������RSA_private_encrypt_PKCS1_type_2(SHA1(X), D)
                  // ����SHA1��ɢ���㷨, RSA_private_encrypt_PKCS1_type_2()����RSA��
                  // ˽Կd��SHA1(X)���м���(ʵ������ǩ��), ����ǰ�����SHA1(X)��PKCS1_type_2
                  // ��ʽ�������, ʹ��SHA1(X)�ĳ��ȴ�20�ֽڱ��0x20�ֽ�(��256λ);
                  // ���NΪ256λ��PKCS1_type_2����ʽ����:
                  // 0x00, 0x02, 9�ֽڷ��������, 0x00, 20�ֽ�����
                  // ����������, S�Ƕ�SHA1(X)������ǩ��, ��1������rsa��˽Կǩ��, ��2������ecc��˽Կǩ��
char TS[0x100]; //��ʱ�����RSA˽Կǩ��(PKCS1_type_2���)SHA1(X)��Ľ��

void get_input(void){
	scanf("%s",A);
	scanf("%s",B);
	scanf("%s",PE);
	scanf("%s",GX);
	scanf("%s",GY);
	scanf("%s",NE);
	scanf("%s",DE);
	scanf("%s",N1[0]);
	scanf("%s",N1[1]);
	scanf("%s",D1[0]);
	scanf("%s",D1[1]);
	scanf("%s",X2[0]);
	scanf("%s",X2[1]);
	return;
}

void ecc_decrypt(void){
/********************׼������********************/
	BIGNUM *a,*b,*p,*n,*gx,*gy; //��Բ������Ҫ��֮��
	BIGNUM *d; //ecc˽Կ
	BIGNUM *tx,*ty; //�����м������������
	BIGNUM *r,*s; //����������
	BIGNUM *m; //���ܺ�����
	EC_POINT *G,*P; //����G�͵�һ�������Ķ�Ӧ��P
	EC_GROUP *group;
	BN_CTX *ctx;
	a=BN_new(); //��������
	b=BN_new();
	p=BN_new();
	n=BN_new();
	gx=BN_new();
	gy=BN_new();
	r=BN_new();
	s=BN_new();
	d=BN_new();
	tx=BN_new();
	ty=BN_new();
	m=BN_new();
	BN_hex2bn(&a,A); //ʮ�������ַ���ת����
	BN_hex2bn(&b,B);
	BN_hex2bn(&p,PE);
	BN_hex2bn(&n,NE);
	BN_hex2bn(&gx,GX);
	BN_hex2bn(&gy,GY);
	BN_hex2bn(&d,DE);
	ctx=BN_CTX_new(); //���������ı���
	group=EC_GROUP_new(EC_GFp_simple_method()); //����Ⱥ
	EC_GROUP_set_curve_GFp(group,p,a,b,ctx); //����a,b,p
	G=EC_POINT_new(group); //��������G
	P=EC_POINT_new(group); //������һ�������Ķ�Ӧ��P
	EC_POINT_set_affine_coordinates_GFp(group,G,gx,gy,ctx); //���û���G
	EC_GROUP_set_generator(group,G,n,BN_value_one()); //�����Բ��������(������=1)
/*********************����N1��N********************/
	BN_hex2bn(&r,N1[0]);
	BN_hex2bn(&s,N1[1]); //ת�����Ĵ���r,s
	EC_POINT_set_compressed_coordinates_GFp(group,P,r,0,ctx); //r(P��X����)->P(��)
	//����:m=s/(d*P).x
	EC_POINT_mul(group,P,NULL,P,d,ctx); //P=dP
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //ȡ��һ��P��(X)���굽��ʱ����(tx)
	BN_mod_inverse(tx,tx,n,ctx); //tx=tx��(mod n)������(��1/(d*P).x)
	BN_clear(m); //������Ĵ���
	BN_mod_mul(m,s,tx,n,ctx); //m=s*tx(mod n)
	memset(N,0,sizeof(N)); //�������ʮ�������ַ���
	strcpy(N,BN_bn2hex(m)); //���Ĵ���ת����ʮ�������ַ���
/********************����D1��D*********************/
	BN_hex2bn(&r,D1[0]);
	BN_hex2bn(&s,D1[1]);
	EC_POINT_set_compressed_coordinates_GFp(group,P,r,0,ctx);
	EC_POINT_mul(group,P,NULL,P,d,ctx);
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx);
	BN_mod_inverse(tx,tx,n,ctx);
	BN_clear(m);
	BN_mod_mul(m,s,tx,n,ctx);
	memset(D,0,sizeof(D));
	strcpy(D,BN_bn2hex(m));
/********************����X2��X1********************/
	BN_hex2bn(&r,X2[0]);
	BN_hex2bn(&s,X2[1]);
	EC_POINT_set_compressed_coordinates_GFp(group,P,r,0,ctx);
	EC_POINT_mul(group,P,NULL,P,d,ctx);
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx);
	BN_mod_inverse(tx,tx,n,ctx);
	BN_clear(m);
	BN_mod_mul(m,s,tx,n,ctx);
	memset(X1,0,sizeof(X1));
	strcpy(X1,BN_bn2hex(m));
/******************�ͷ���Դ(��β)******************/
	EC_POINT_free(G);
	EC_POINT_free(P);
	BN_free(a);
	BN_free(b);
	BN_free(p);
	BN_free(n);
	BN_free(gx);
	BN_free(gy);
	BN_free(r);
	BN_free(s);
	BN_free(d);
	BN_free(tx);
	BN_free(ty);
	BN_free(m);
	EC_GROUP_free(group);
	BN_CTX_free(ctx);
	return;
}

void rsa_private_decrypt(void){
/********************׼������********************/
	BIGNUM *n,*d; //RSAȡģ����n��˽Կd
	BIGNUM *c,*m; //RSA����c������m
	BN_CTX *ctx; //�����ı���
	ctx=BN_CTX_new();
	n=BN_new();
	d=BN_new();
	c=BN_new();
	m=BN_new();
	BN_hex2bn(&n,N);
	BN_hex2bn(&d,D);
	BN_hex2bn(&c,X1);
/********************����X1��X********************/
	BN_clear(m); //������Ĵ���
	BN_mod_exp(m,c,d,n,ctx); //RSA˽Կ����(m=c^d mod n)
	memset(X,0,sizeof(X)); //�������ʮ�������ַ���
	strcpy(X,BN_bn2hex(m)); //���Ĵ���ת����ʮ�������ַ���
/******************�ͷ���Դ(��β)******************/
	BN_free(n);
	BN_free(d);
	BN_free(c);
	BN_free(m);
	BN_CTX_free(ctx);
	return;
}

void ecc_encrypt(void){
/********************׼������********************/
	BIGNUM *a,*b,*p,*n,*gx,*gy; //��Բ������Ҫ��֮��
	BIGNUM *d; //ecc˽Կ
	BIGNUM *tx,*ty; //�����е���ʱ�������
	BIGNUM *m; //���Ĵ���m
	BIGNUM *k; //�����������k
	EC_POINT *G,*R,*P; //����G,��Կ��R����ʱ��P
	EC_GROUP *group;
	BN_CTX *ctx;
	time_t c_time;
	a=BN_new(); //��������
	b=BN_new();
	p=BN_new();
	n=BN_new();
	gx=BN_new();
	gy=BN_new();
	d=BN_new();
	tx=BN_new();
	ty=BN_new();
	m=BN_new();
	k=BN_new();
	BN_hex2bn(&a,A); //ʮ�������ַ���ת����
	BN_hex2bn(&b,B);
	BN_hex2bn(&p,PE);
	BN_hex2bn(&n,NE);
	BN_hex2bn(&gx,GX);
	BN_hex2bn(&gy,GY);
	BN_hex2bn(&d,DE);
	BN_hex2bn(&m,X);
	ctx=BN_CTX_new(); //���������ı���
	group=EC_GROUP_new(EC_GFp_simple_method()); //����Ⱥ
	EC_GROUP_set_curve_GFp(group,p,a,b,ctx); //����a,b,p
	G=EC_POINT_new(group); //��������G
	R=EC_POINT_new(group); //������Կ��R
	P=EC_POINT_new(group); //������ʱ��P
	EC_POINT_set_affine_coordinates_GFp(group,G,gx,gy,ctx); //���û���G
	EC_GROUP_set_generator(group,G,n,BN_value_one()); //�����Բ��������(������=1)
/***************����ECC��Կ��(RX,RY)***************/
	EC_POINT_mul(group,R,d,NULL,NULL,ctx); //R=d(˽Կ����)*G
	EC_POINT_get_affine_coordinates_GFp(group,R,tx,ty,ctx); //ȡ����Կ��R������
	memset(RX,0,sizeof(RX));
	strcpy(RX,BN_bn2hex(tx));
	memset(RY,0,sizeof(RY));
	strcpy(RY,BN_bn2hex(ty)); //����������ת��Ϊʮ�������ַ���
/*****************���ɼ����������k****************/
	c_time=(time_t)time(NULL);
	RAND_add(&c_time,sizeof(c_time),1); //����ʱ��������������
	BN_rand(k,BN_num_bits(n),0,0); //����nλ�������k
/*********************����X��C*********************/
	//����:r=(k*G).x,s=m*(k*R).x mod n
	//�����һ����(r)
	EC_POINT_mul(group,P,k,NULL,NULL,ctx); //P=k*G
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //tx=P.x(����ty)
	memset(C[0],0,sizeof(C[0]));
	strcpy(C[0],BN_bn2hex(tx)); //����һ��������rת��Ϊʮ�������ַ�������
	//����ڶ�����(s)
	EC_POINT_mul(group,P,NULL,R,k,ctx); //P=k*R
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //tx=P.x(����ty)
	BN_mod_mul(ty,m,tx,n,ctx); //ty=m*tx mod n
	memset(C[1],0,sizeof(C[1]));
	strcpy(C[1],BN_bn2hex(ty)); //���ڶ���������sת��Ϊʮ�������ַ�������
/******************�ͷ���Դ(��β)******************/
	BN_free(a);
	BN_free(b);
	BN_free(p);
	BN_free(n);
	BN_free(gx);
	BN_free(gy);
	BN_free(d);
	BN_free(tx);
	BN_free(ty);
	BN_free(m);
	BN_free(k);
	EC_POINT_free(G);
	EC_POINT_free(R);
	EC_POINT_free(P);
	EC_GROUP_free(group);
	BN_CTX_free(ctx);
	return;
}

void rsa_private_signing_PKCS1_type_2(void){
	int i;
	BIGNUM *n,*d; //RSAȡģ����n��˽Կd
	BIGNUM *m,*c; //RSA����m������c
	BN_CTX *ctx;
	unsigned char X_dump[0x100];
	unsigned char SHA1_digest[0x100];
	unsigned char RSA_plaintext[0x100];
	ctx=BN_CTX_new();
	n=BN_new();
	d=BN_new();
	m=BN_new();
	c=BN_new();
	BN_hex2bn(&n,N);
	BN_hex2bn(&d,D);
	memset(X_dump,0,0x100);
	memset(SHA1_digest,0,0x100);
	memset(RSA_plaintext,0,0x100);
	for(i=0;i<32;i++)
		sscanf((char *)&X[i*2],"%02X",&X_dump[i]);
	SHA1(X_dump,32,SHA1_digest);
	//RSA_padding_add_PKCS1_type_2(RSA_plaintext,32,SHA1_digest,20);
	RSA_plaintext[0]=0x00;
	RSA_plaintext[1]=0x02;
	srand((time_t)time(NULL));
	for(i=2;i<11;i++) //������Ÿ��ֽ�
		RSA_plaintext[i]=(unsigned char)(rand()%256);
	RSA_plaintext[11]=0x00; //��Ч��������䲿�ֵķָ�
	for(i=12;i<32;i++)
		RSA_plaintext[i]=SHA1_digest[i-12];
	BN_bin2bn(RSA_plaintext,32,m); //�������Ĵ���
	BN_clear(c); //������Ĵ���
	BN_mod_exp(c,m,d,n,ctx); //RSA˽Կ����(ǩ��)(c=m^d mod n)
	memset(TS,0,sizeof(TS)); //�������ʮ�������ַ���
	strcpy(TS,BN_bn2hex(c)); //���Ĵ���ת����ʮ�������ַ���
	BN_free(n);
	BN_free(d);
	BN_free(m);
	BN_free(c);
	BN_CTX_free(ctx);
	return;
}

void ecc_ecnr_signing(void){
/********************׼������********************/
	BIGNUM *a,*b,*p,*n,*gx,*gy; //��Բ������Ҫ��֮��
	BIGNUM *d; //ecc˽Կ
	BIGNUM *m; //���Ĵ���m
	BIGNUM *k; //ǩ���������k
	BIGNUM *tx,*ty; //�����е���ʱ����
	EC_POINT *G,*P; //����G����ʱ��P
	EC_GROUP *group;
	BN_CTX *ctx;
	time_t c_time;
	a=BN_new(); //��������
	b=BN_new();
	p=BN_new();
	n=BN_new();
	gx=BN_new();
	gy=BN_new();
	d=BN_new();
	m=BN_new();
	k=BN_new();
	tx=BN_new();
	ty=BN_new();
	BN_hex2bn(&a,A); //ʮ�������ַ���ת����
	BN_hex2bn(&b,B);
	BN_hex2bn(&p,PE);
	BN_hex2bn(&n,NE);
	BN_hex2bn(&gx,GX);
	BN_hex2bn(&gy,GY);
	BN_hex2bn(&d,DE);
	BN_hex2bn(&m,TS);
	ctx=BN_CTX_new(); //���������ı���
	group=EC_GROUP_new(EC_GFp_simple_method()); //����Ⱥ
	EC_GROUP_set_curve_GFp(group,p,a,b,ctx); //����a,b,p
	G=EC_POINT_new(group); //��������G
	P=EC_POINT_new(group); //������ʱ��P
	EC_POINT_set_affine_coordinates_GFp(group,G,gx,gy,ctx); //���û���G
	EC_GROUP_set_generator(group,G,n,BN_value_one()); //�����Բ��������(������=1)
/*****************����ǩ���������k****************/
	c_time=(time_t)time(NULL);
	RAND_add(&c_time,sizeof(c_time),1); //����ʱ��������������
	BN_rand(k,BN_num_bits(n),0,0); //����nλ�������k
/*******************ECNRǩ��TS��S******************/
	//ECNRǩ��:r=(k*G).x+m,s=k-r*d
	//�����һ����(r)
	EC_POINT_mul(group,P,k,NULL,NULL,ctx); //P=k*G
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //tx=P.x(����ty)
	BN_mod(ty,tx,n,ctx); //ty=tx mod n
	BN_add(tx,ty,m); //tx=ty+m
	BN_mod(ty,tx,n,ctx); //ty=tx mod n
	memset(S[0],0,sizeof(S[0]));
	strcpy(S[0],BN_bn2hex(ty)); //����һ��������rת��Ϊʮ�������ַ�������
	//����ڶ�����(s)
	BN_mod_mul(tx,ty,d,n,ctx); //tx=ty*d mod n=r*d mod n
	BN_set_negative(tx,1); //tx=-tx=-r*d
	BN_add(ty,n,tx); //ty=n+tx=n-r*d
	BN_mod_add(tx,k,ty,n,ctx); //tx=(k+(n-r*d)) mod n=k-r*d
	memset(S[1],0,sizeof(S[1]));
	strcpy(S[1],BN_bn2hex(tx)); //���ڶ���������sת��Ϊʮ�������ַ�������
/******************�ͷ���Դ(��β)******************/
	BN_free(a);
	BN_free(b);
	BN_free(p);
	BN_free(n);
	BN_free(gx);
	BN_free(gy);
	BN_free(d);
	BN_free(m);
	BN_free(k);
	BN_free(tx);
	BN_free(ty);
	EC_POINT_free(G);
	EC_POINT_free(P);
	EC_GROUP_free(group);
	BN_CTX_free(ctx);
	return;
}

int main()
{
	get_input();
    ecc_decrypt();
	rsa_private_decrypt();
	ecc_encrypt();
	rsa_private_signing_PKCS1_type_2();
	ecc_ecnr_signing();
	//puts(X);
    puts(C[0]);
    puts(C[1]);
    puts(S[0]);
    puts(S[1]);
    return 0;
}
