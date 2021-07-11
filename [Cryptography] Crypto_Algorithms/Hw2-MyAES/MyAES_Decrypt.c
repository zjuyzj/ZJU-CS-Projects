void build_sbox_inverse(void){
    int i;
    for(i=0;i<256;i++) sbox_inverse[sbox[i]]=i;
    return;
}

void ByteSubInverse(unsigned char *p, int n){
    int i;
    for(i=0;i<n;i++) p[i]=sbox_inverse[p[i]];
    return;
}

void ShiftRowInverse(unsigned char *p){
    int i;
    for(i=1;i<4;i++) ror_a_row(p+i*4,i);
    return;
}

void MixColumnInverse(unsigned char *p, unsigned char a[4], int do_mul){
    int i,j;
    unsigned char t[4][4];
    unsigned char b[4];
    for(i=0;i<4;i++){
        for(j=0;j<4;j++) b[j]=p[i*4+j];
        if(do_mul) aes_polynomial_mul(a,b,b);
        for(j=0;j<4;j++) t[j][i]=b[j];
    }
    for(i=0;i<4;i++)
        for(j=0;j<4;j++) p[4*i+j]=t[i][j];
    return;
}

void aes_polynomial_mul(unsigned char x[4], unsigned char y[4], unsigned char z[4]){
    int i,j;
    unsigned char temp,result[4];
    for(i=0;i<4;i++){
        result[i]=0;
        for(j=0;j<4;j++){
            temp=(unsigned char)aes_8bit_mul_mod_0x11B(x[(i+j+1)%4],y[j]);
            result[i]^=temp;
        }
    }
    temp=result[0];
    result[0]=result[2];
    result[2]=temp;
    for(i=0;i<4;i++) z[i]=result[i];
    return;
}

void aes_decrypt(unsigned char *bufin, unsigned char *bufout, unsigned char *key){
    int i,j;
    unsigned char a[4]={0x0B,0x0D,0x09,0x0E};
    unsigned char matrix[4][4];
    for(i=0;i<4;i++)
        for(j=0;j<4;j++) matrix[i][j]=bufin[i*4+j];
    for(i=key_rounds;i>=1;i--){
        AddRoundKey((unsigned char*)matrix,key+i*(4*4));
        if(i!=key_rounds) MixColumnInverse((unsigned char*)matrix,a,1);
        else MixColumnInverse((unsigned char*)matrix,a,0);
        ShiftRowInverse((unsigned char*)matrix);
        MixColumnInverse((unsigned char *)matrix,a,0);
        ByteSubInverse((unsigned char*)matrix,16);
    }
    AddRoundKey((unsigned char *)matrix,key);
    for(i=0;i<4;i++)
        for(j=0;j<4;j++) bufout[i*4+j]=matrix[i][j];
    return;
}