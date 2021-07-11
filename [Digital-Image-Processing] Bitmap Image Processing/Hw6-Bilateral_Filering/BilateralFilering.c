#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(1)

#define BM 0x4D42
#define BI_RGB 0
#define PI 3.14159
#define FILENAME "TestIn.bmp"
#define OUTPUTNAME "TestOut.bmp"

typedef unsigned char BYTE,*pBYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef struct bitmapFileHeader{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffbits; 
} bmfh;

typedef struct bitmapInfoHeader{
	DWORD biSize;
	DWORD biWidth;
	DWORD biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	DWORD biXPelsPerMeter;
	DWORD biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} bmih;

typedef struct bitmapHeader{
	bmfh fh;
	bmih ih;
} bmh;

pBYTE readColorBMP(bmh* header);
int writeColotBMP(int x,int y,int resX,int resY,pBYTE buffer);
double gaussFunc(int x,double sigma);
BYTE singleConvOp(pBYTE mat,double* kernel,int kx,int ky);
pBYTE biFiltering(int x,int y,pBYTE buffer,int win_size,double sigma_s,double sigma_r);

int main(void){
	bmh bmHeader;
	pBYTE bmData=readColorBMP(&bmHeader),newbmData;
	char input;
	int i;
	int winSize;
	double sigmaS,sigmaR;
	if(bmData==NULL){
		printf("/************************************************************/\n");
		printf("Can not open your BMP file!\n");
		printf("File not found or file format error.\n");
		printf("File name:%s\n",FILENAME);
		printf("Format:Windows DIB,color,no compression,24 bits depth\n");
		printf("/************************************************************/\n");
		printf("(Press any key to exit.)\n\n");
		system("pause");
		exit(1);
	}
	else{
		printf("Bilateral Filtering on %s\n",FILENAME);
		printf("\nInput Bitmap Scale:%d * %d\n",bmHeader.ih.biWidth,bmHeader.ih.biHeight);
		printf("\nPlease input window's size:");
		scanf("%d",&winSize);
		if(winSize>bmHeader.ih.biWidth||winSize>bmHeader.ih.biWidth){
			printf("\nIncorrect window's size!\n\n");
			printf("(Press any key to exit.)\n\n");
			system("pause");
			exit(1);
		}
		printf("\nPlease input parameter-sigma s:");
		scanf("%lf",&sigmaS);
		printf("\nPlease input parameter-sigma r:");
		scanf("%lf",&sigmaR);
		newbmData=biFiltering(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmData,winSize,sigmaS,sigmaR);
		if(writeColorBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
			printf("\nAn error occurred during writing!");
		else printf("\nWriting image complete!\n");
		free(newbmData);
		printf("\n(Press any key to exit.)\n\n");
		system("pause");
		exit(0);
	}
} 

pBYTE readColorBMP(bmh* header){
	FILE* fp;
	int datalen;
	fp=fopen(FILENAME,"rb");
	if(!fp) return NULL;
	fseek(fp,0,SEEK_SET);
	fread(header,sizeof(bmh),1,fp);
	if(header->fh.bfType!=BM||header->ih.biBitCount!=24||header->ih.biCompression!=BI_RGB) return NULL; 
	datalen=header->fh.bfSize-header->fh.bfOffbits;
	pBYTE buffer=(pBYTE)malloc(datalen);
	fseek(fp,header->fh.bfOffbits,SEEK_SET);
	fread(buffer,datalen,1,fp);
	fclose(fp);
	return buffer;
}

int writeColorBMP(int x,int y,int resX,int resY,pBYTE buffer){
	if(buffer==NULL) return 1;
	FILE* fp;
	bmh newHeader;
	int rowSize;
	int totalSize;
	int i;
	rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	totalSize=rowSize*y+sizeof(bmh);
	newHeader.fh.bfOffbits=sizeof(bmh);
	newHeader.fh.bfSize=totalSize;
	newHeader.fh.bfType=BM;
	newHeader.fh.bfReserved1=0;
	newHeader.fh.bfReserved2=0;
	newHeader.ih.biSize=sizeof(bmih);
	newHeader.ih.biBitCount=24;
	newHeader.ih.biWidth=x;
	newHeader.ih.biHeight=y;
	newHeader.ih.biPlanes=1;
	newHeader.ih.biCompression=BI_RGB;
	newHeader.ih.biSizeImage=0;
	newHeader.ih.biXPelsPerMeter=resX;
	newHeader.ih.biYPelsPerMeter=resY;
	newHeader.ih.biClrUsed=0;
	newHeader.ih.biClrImportant=0;
	fp=fopen(OUTPUTNAME,"wb+");
	if(!fp) return 1;
	fseek(fp,0,SEEK_SET);
	fwrite(&newHeader,sizeof(bmh),1,fp);
	fwrite(buffer,rowSize*y,1,fp);
	fclose(fp);
	return 0;
}

double gaussFunc(int x,double sigma){
	double k,t,result;
	k=1/(sigma*sqrt(2*PI));
	t=-pow(x,2)/(2*pow(sigma,2));
	result=k*exp(t);
	return result;
}

BYTE singleConvOp(pBYTE mat,double* kernel,int kx,int ky){
	int i,j;
	double normFact=0,result=0;
	for(i=0;i<ky;i++)
		for(j=0;j<kx;j++) normFact+=*(kernel+kx*i+j);
	for(i=0;i<ky;i++)
		for(j=0;j<kx;j++) result+=(*(mat+kx*i+j))*(*(kernel+kx*i+j));
	if(normFact!=0) result/=normFact;
	if(result>255) result=255;
	else if(result<0) result=0;
	return (BYTE)result;
}

pBYTE biFiltering(int x,int y,pBYTE buffer,int win_size,double sigma_s,double sigma_r){
	pBYTE resultBuffer,tMatR,tMatG,tMatB;
	double* kernelR;
	double* kernelG;
	double* kernelB;
	int i,j,m,n,diffR,diffG,diffB,rowSize=3*x;
	double dist;
	tMatR=(pBYTE)malloc(win_size*win_size);
	tMatG=(pBYTE)malloc(win_size*win_size);
	tMatB=(pBYTE)malloc(win_size*win_size);
	kernelR=(double*)malloc(sizeof(double)*win_size*win_size);
	kernelG=(double*)malloc(sizeof(double)*win_size*win_size);
	kernelB=(double*)malloc(sizeof(double)*win_size*win_size);
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	resultBuffer=(pBYTE)malloc(rowSize*y);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			*(resultBuffer+rowSize*i+3*j)=255;
			*(resultBuffer+rowSize*i+3*j+1)=255;
			*(resultBuffer+rowSize*i+3*j+2)=255;
		}
	for(i=0;i<y-win_size;i++)
		for(j=0;j<x-win_size;j++){
			for(m=0;m<win_size;m++)
				for(n=0;n<win_size;n++){
					dist=sqrt(pow(m-win_size/2,2)+pow(n-win_size/2,2));
					diffR=abs(*(buffer+(i+m)*rowSize+(j+n)*3)-*(buffer+(i+win_size/2)*rowSize+(j+win_size/2)*3));
					diffG=abs(*(buffer+(i+m)*rowSize+(j+n)*3+1)-*(buffer+(i+win_size/2)*rowSize+(j+win_size/2)*3+1));
					diffB=abs(*(buffer+(i+m)*rowSize+(j+n)*3+2)-*(buffer+(i+win_size/2)*rowSize+(j+win_size/2)*3+2));
					*(kernelR+m*win_size+n)=gaussFunc(dist,sigma_s)*gaussFunc(diffR,sigma_r);
					*(kernelG+m*win_size+n)=gaussFunc(dist,sigma_s)*gaussFunc(diffG,sigma_r);
					*(kernelB+m*win_size+n)=gaussFunc(dist,sigma_s)*gaussFunc(diffB,sigma_r);
					*(tMatR+win_size*m+n)=*(buffer+(i+m)*rowSize+(j+n)*3);
					*(tMatG+win_size*m+n)=*(buffer+(i+m)*rowSize+(j+n)*3+1);
					*(tMatB+win_size*m+n)=*(buffer+(i+m)*rowSize+(j+n)*3+2);
				}
			*(resultBuffer+(i+win_size/2)*rowSize+(j+win_size/2)*3)=singleConvOp(tMatR,kernelR,win_size,win_size);
			*(resultBuffer+(i+win_size/2)*rowSize+(j+win_size/2)*3+1)=singleConvOp(tMatG,kernelG,win_size,win_size);
			*(resultBuffer+(i+win_size/2)*rowSize+(j+win_size/2)*3+2)=singleConvOp(tMatB,kernelB,win_size,win_size);
		}
	free(tMatR);
	free(tMatG);
	free(tMatB);
	free(kernelR);
	free(kernelG);
	free(kernelB);
	return resultBuffer;
}
