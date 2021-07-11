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

char A[0x100], B[0x100], PE[0x100];   // ecc曲线参数a, b, p
char GX[0x100], GY[0x100], NE[0x100]; // 基点G的x坐标, y坐标, G的阶(均为256位)
char DE[0x100];                       // ecc的私钥d(256位)
char N1[2][0x100], D1[2][0x100], X2[2][0x100]; 
                // N1存放用ecc公钥加密过的rsa参数N, 
                // 其中N1[0]存放第1部分密文, N1[1]存放第2部分密文;
                // D1存放用ecc公钥加密过的rsa私钥d;
                // X2存放用ecc公钥加密过的X1
char X1[0x100]; // X1存放用rsa公钥加密过的X                      
char X[0x100];  // X存放一个256位的随机数, X<N 且 X<NE
char N[0x100], D[0x100];       // rsa的N及私钥d(256位)
char RX[0x100], RY[0x100];     // ecc公钥R的x坐标及y坐标 
char C[2][0x100]; // 存放用ecc公钥加密过的X, C[0]是第1部分密文, C[1]是第2部分密文
char S[2][0x100]; // 存放用ecnr签名过的RSA_private_encrypt_PKCS1_type_2(SHA1(X), D)
                  // 其中SHA1是散列算法, RSA_private_encrypt_PKCS1_type_2()是用RSA的
                  // 私钥d对SHA1(X)进行加密(实际上是签名), 加密前必须对SHA1(X)按PKCS1_type_2
                  // 方式进行填充, 使得SHA1(X)的长度从20字节变成0x20字节(即256位);
                  // 针对N为256位的PKCS1_type_2填充格式如下:
                  // 0x00, 0x02, 9字节非零随机数, 0x00, 20字节明文
                  // 归纳起来讲, S是对SHA1(X)的两次签名, 第1次是用rsa的私钥签名, 第2次是用ecc的私钥签名
char TS[0x100]; //临时存放用RSA私钥签名(PKCS1_type_2填充)SHA1(X)后的结果

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
/********************准备工作********************/
	BIGNUM *a,*b,*p,*n,*gx,*gy; //椭圆曲线六要素之五
	BIGNUM *d; //ecc私钥
	BIGNUM *tx,*ty; //解密中间坐标大数变量
	BIGNUM *r,*s; //两部分密文
	BIGNUM *m; //解密后明文
	EC_POINT *G,*P; //基点G和第一部分密文对应点P
	EC_GROUP *group;
	BN_CTX *ctx;
	a=BN_new(); //创建大数
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
	BN_hex2bn(&a,A); //十六进制字符串转大数
	BN_hex2bn(&b,B);
	BN_hex2bn(&p,PE);
	BN_hex2bn(&n,NE);
	BN_hex2bn(&gx,GX);
	BN_hex2bn(&gy,GY);
	BN_hex2bn(&d,DE);
	ctx=BN_CTX_new(); //创建上下文变量
	group=EC_GROUP_new(EC_GFp_simple_method()); //创建群
	EC_GROUP_set_curve_GFp(group,p,a,b,ctx); //设置a,b,p
	G=EC_POINT_new(group); //创建基点G
	P=EC_POINT_new(group); //创建第一部分密文对应点P
	EC_POINT_set_affine_coordinates_GFp(group,G,gx,gy,ctx); //设置基点G
	EC_GROUP_set_generator(group,G,n,BN_value_one()); //完成椭圆曲线设置(余因子=1)
/*********************解密N1到N********************/
	BN_hex2bn(&r,N1[0]);
	BN_hex2bn(&s,N1[1]); //转换密文大数r,s
	EC_POINT_set_compressed_coordinates_GFp(group,P,r,0,ctx); //r(P的X坐标)->P(点)
	//解密:m=s/(d*P).x
	EC_POINT_mul(group,P,NULL,P,d,ctx); //P=dP
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //取上一步P的(X)坐标到临时大数(tx)
	BN_mod_inverse(tx,tx,n,ctx); //tx=tx在(mod n)下求逆(即1/(d*P).x)
	BN_clear(m); //清空明文大数
	BN_mod_mul(m,s,tx,n,ctx); //m=s*tx(mod n)
	memset(N,0,sizeof(N)); //清空明文十六进制字符串
	strcpy(N,BN_bn2hex(m)); //明文大数转明文十六进制字符串
/********************解密D1到D*********************/
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
/********************解密X2到X1********************/
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
/******************释放资源(收尾)******************/
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
/********************准备工作********************/
	BIGNUM *n,*d; //RSA取模参数n和私钥d
	BIGNUM *c,*m; //RSA密文c和明文m
	BN_CTX *ctx; //上下文变量
	ctx=BN_CTX_new();
	n=BN_new();
	d=BN_new();
	c=BN_new();
	m=BN_new();
	BN_hex2bn(&n,N);
	BN_hex2bn(&d,D);
	BN_hex2bn(&c,X1);
/********************解密X1到X********************/
	BN_clear(m); //清空明文大数
	BN_mod_exp(m,c,d,n,ctx); //RSA私钥解密(m=c^d mod n)
	memset(X,0,sizeof(X)); //清空明文十六进制字符串
	strcpy(X,BN_bn2hex(m)); //明文大数转明文十六进制字符串
/******************释放资源(收尾)******************/
	BN_free(n);
	BN_free(d);
	BN_free(c);
	BN_free(m);
	BN_CTX_free(ctx);
	return;
}

void ecc_encrypt(void){
/********************准备工作********************/
	BIGNUM *a,*b,*p,*n,*gx,*gy; //椭圆曲线六要素之五
	BIGNUM *d; //ecc私钥
	BIGNUM *tx,*ty; //计算中的临时坐标大数
	BIGNUM *m; //明文大数m
	BIGNUM *k; //加密随机大数k
	EC_POINT *G,*R,*P; //基点G,公钥点R和临时点P
	EC_GROUP *group;
	BN_CTX *ctx;
	time_t c_time;
	a=BN_new(); //创建大数
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
	BN_hex2bn(&a,A); //十六进制字符串转大数
	BN_hex2bn(&b,B);
	BN_hex2bn(&p,PE);
	BN_hex2bn(&n,NE);
	BN_hex2bn(&gx,GX);
	BN_hex2bn(&gy,GY);
	BN_hex2bn(&d,DE);
	BN_hex2bn(&m,X);
	ctx=BN_CTX_new(); //创建上下文变量
	group=EC_GROUP_new(EC_GFp_simple_method()); //创建群
	EC_GROUP_set_curve_GFp(group,p,a,b,ctx); //设置a,b,p
	G=EC_POINT_new(group); //创建基点G
	R=EC_POINT_new(group); //创建公钥点R
	P=EC_POINT_new(group); //创建临时点P
	EC_POINT_set_affine_coordinates_GFp(group,G,gx,gy,ctx); //设置基点G
	EC_GROUP_set_generator(group,G,n,BN_value_one()); //完成椭圆曲线设置(余因子=1)
/***************计算ECC公钥点(RX,RY)***************/
	EC_POINT_mul(group,R,d,NULL,NULL,ctx); //R=d(私钥大数)*G
	EC_POINT_get_affine_coordinates_GFp(group,R,tx,ty,ctx); //取出公钥点R的坐标
	memset(RX,0,sizeof(RX));
	strcpy(RX,BN_bn2hex(tx));
	memset(RY,0,sizeof(RY));
	strcpy(RY,BN_bn2hex(ty)); //将大数坐标转化为十六进制字符串
/*****************生成加密随机大数k****************/
	c_time=(time_t)time(NULL);
	RAND_add(&c_time,sizeof(c_time),1); //设置时间相关随机数种子
	BN_rand(k,BN_num_bits(n),0,0); //产生n位随机大数k
/*********************加密X到C*********************/
	//加密:r=(k*G).x,s=m*(k*R).x mod n
	//计算第一部分(r)
	EC_POINT_mul(group,P,k,NULL,NULL,ctx); //P=k*G
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //tx=P.x(丢弃ty)
	memset(C[0],0,sizeof(C[0]));
	strcpy(C[0],BN_bn2hex(tx)); //将第一部分密文r转化为十六进制字符串保存
	//计算第二部分(s)
	EC_POINT_mul(group,P,NULL,R,k,ctx); //P=k*R
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //tx=P.x(丢弃ty)
	BN_mod_mul(ty,m,tx,n,ctx); //ty=m*tx mod n
	memset(C[1],0,sizeof(C[1]));
	strcpy(C[1],BN_bn2hex(ty)); //将第二部分密文s转化为十六进制字符串保存
/******************释放资源(收尾)******************/
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
	BIGNUM *n,*d; //RSA取模参数n和私钥d
	BIGNUM *m,*c; //RSA明文m和密文c
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
	for(i=2;i<11;i++) //随机填充九个字节
		RSA_plaintext[i]=(unsigned char)(rand()%256);
	RSA_plaintext[11]=0x00; //有效部分与填充部分的分隔
	for(i=12;i<32;i++)
		RSA_plaintext[i]=SHA1_digest[i-12];
	BN_bin2bn(RSA_plaintext,32,m); //设置明文大数
	BN_clear(c); //清空密文大数
	BN_mod_exp(c,m,d,n,ctx); //RSA私钥加密(签名)(c=m^d mod n)
	memset(TS,0,sizeof(TS)); //清空密文十六进制字符串
	strcpy(TS,BN_bn2hex(c)); //密文大数转明文十六进制字符串
	BN_free(n);
	BN_free(d);
	BN_free(m);
	BN_free(c);
	BN_CTX_free(ctx);
	return;
}

void ecc_ecnr_signing(void){
/********************准备工作********************/
	BIGNUM *a,*b,*p,*n,*gx,*gy; //椭圆曲线六要素之五
	BIGNUM *d; //ecc私钥
	BIGNUM *m; //明文大数m
	BIGNUM *k; //签名随机大数k
	BIGNUM *tx,*ty; //计算中的临时大数
	EC_POINT *G,*P; //基点G和临时点P
	EC_GROUP *group;
	BN_CTX *ctx;
	time_t c_time;
	a=BN_new(); //创建大数
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
	BN_hex2bn(&a,A); //十六进制字符串转大数
	BN_hex2bn(&b,B);
	BN_hex2bn(&p,PE);
	BN_hex2bn(&n,NE);
	BN_hex2bn(&gx,GX);
	BN_hex2bn(&gy,GY);
	BN_hex2bn(&d,DE);
	BN_hex2bn(&m,TS);
	ctx=BN_CTX_new(); //创建上下文变量
	group=EC_GROUP_new(EC_GFp_simple_method()); //创建群
	EC_GROUP_set_curve_GFp(group,p,a,b,ctx); //设置a,b,p
	G=EC_POINT_new(group); //创建基点G
	P=EC_POINT_new(group); //创建临时点P
	EC_POINT_set_affine_coordinates_GFp(group,G,gx,gy,ctx); //设置基点G
	EC_GROUP_set_generator(group,G,n,BN_value_one()); //完成椭圆曲线设置(余因子=1)
/*****************生成签名随机大数k****************/
	c_time=(time_t)time(NULL);
	RAND_add(&c_time,sizeof(c_time),1); //设置时间相关随机数种子
	BN_rand(k,BN_num_bits(n),0,0); //产生n位随机大数k
/*******************ECNR签名TS到S******************/
	//ECNR签名:r=(k*G).x+m,s=k-r*d
	//计算第一部分(r)
	EC_POINT_mul(group,P,k,NULL,NULL,ctx); //P=k*G
	EC_POINT_get_affine_coordinates_GFp(group,P,tx,ty,ctx); //tx=P.x(丢弃ty)
	BN_mod(ty,tx,n,ctx); //ty=tx mod n
	BN_add(tx,ty,m); //tx=ty+m
	BN_mod(ty,tx,n,ctx); //ty=tx mod n
	memset(S[0],0,sizeof(S[0]));
	strcpy(S[0],BN_bn2hex(ty)); //将第一部分密文r转化为十六进制字符串保存
	//计算第二部分(s)
	BN_mod_mul(tx,ty,d,n,ctx); //tx=ty*d mod n=r*d mod n
	BN_set_negative(tx,1); //tx=-tx=-r*d
	BN_add(ty,n,tx); //ty=n+tx=n-r*d
	BN_mod_add(tx,k,ty,n,ctx); //tx=(k+(n-r*d)) mod n=k-r*d
	memset(S[1],0,sizeof(S[1]));
	strcpy(S[1],BN_bn2hex(tx)); //将第二部分密文s转化为十六进制字符串保存
/******************释放资源(收尾)******************/
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
