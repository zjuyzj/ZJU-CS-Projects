#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(1)

#define BM 0x4D42
#define BI_RGB 0
#define FILENAME "TestIn.bmp"
#define OUTPUTNAME "TestOut.bmp"

enum bistate{false,true};
typedef enum bistate boolean;

int* mFiltKernel; //Kernel for mean filtering,non-normalized(normalize:convolution then plus 1/(mx*my))
int LapOpKernel[9]={0,1,0,
					1,-4,1,
					0,1,0}; //Kernel for Laplacian Operator
int mx,my,mcx,mcy;
int lx=3,ly=3,lcx=1,lcy=1; //(lx,ly):Size of Laplacian Operator's kernel;(lcx,lcy):Center of Laplacian Operator's kernel. 

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
pBYTE convolutionOp(int x,int y,pBYTE buffer,pBYTE auxBuffer,int kX,int kY,int ckX,int ckY,int* kernel,boolean isNormalized);
pBYTE meanFiltering(int x,int y,pBYTE buffer);
pBYTE LaplacianEnh(int x,int y,pBYTE buffer);

int main(void){
	bmh bmHeader;
	pBYTE bmData=readColorBMP(&bmHeader),newbmData;
	char input;
	int i;
	if(bmData==NULL){
		printf("/************************************************************/\n");
		printf("Can not open your BMP file!\n");
		printf("File not found or file format error.\n");
		printf("File name:%s\n",FILENAME);
		printf("Format:Windows DIB,color,no compression,24 bits depth\n");
		printf("/************************************************************/\n");
		printf("(Press any key to exit.)\n");
		system("pause");
		exit(1);
	}
	else
		while(1){
			system("cls");
			printf("Please press a key to select a function in the list:\n\n");
			printf("/***************************************/\n\n");
			printf("1.m-Do a mean filtering.\n\n");
			printf("2.e-Do a Laplacian image enhancement.\n\n");
			printf("3.q-Exit\n\n");
			printf("/***************************************/\n\n");
			printf("Your Choice:");
			input=getchar();
			switch(input){
				case 'm':
					printf("\nInput Bitmap Scale:%d * %d\n",bmHeader.ih.biWidth,bmHeader.ih.biHeight);
					printf("\nPlease input window's width:");
					scanf("%d",&mx);
					printf("\nPlease input window's height:");
					scanf("%d",&my);
					mcx=mx/2;
					mcy=my/2;
					mFiltKernel=(int*)malloc(sizeof(int)*mx*my);
					for(i=0;i<mx*my;i++) *(mFiltKernel+i)=1;
					newbmData=meanFiltering(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmData);
					if(writeColorBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\nAn error occurred during writing!");
					else printf("\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 'e':
					newbmData=LaplacianEnh(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmData);
					if(writeColorBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\nAn error occurred during writing!");
					else printf("\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 'q':
					exit(0);
					break;
				defult:
					continue;
			}
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

pBYTE convolutionOp(int x,int y,pBYTE buffer,pBYTE auxBuffer,int kX,int kY,int ckX,int ckY,int* kernel,boolean isNormalized){
	int i,j,m,n,sumR,sumG,sumB,normFact=0,rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	pBYTE resultBuffer=(pBYTE)malloc(rowSize*y);
	for(i=0;i<rowSize*y;i++) *(resultBuffer+i)=0;
	for(i=0;i<kY;i++)
		for(j=0;j<kX;j++)
			normFact+=*(kernel+kX*i+j);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			if(j-ckX<0||i-ckY<0||j+ckX>=x||i+ckY>=y) continue;
			sumR=sumG=sumB=0;
			for(m=0;m<kY;m++)
				for(n=0;n<kX;n++){
					sumR+=(*(buffer+(i+m-ckY)*rowSize+(j+n-ckX)*3))*(*(kernel+m*kX+n));
					sumG+=(*(buffer+(i+m-ckY)*rowSize+(j+n-ckX)*3+1))*(*(kernel+m*kX+n));
					sumB+=(*(buffer+(i+m-ckY)*rowSize+(j+n-ckX)*3+2))*(*(kernel+m*kX+n));
				}
			if(isNormalized==true){
				sumR/=normFact;
				sumG/=normFact;
				sumB/=normFact;
			}
			if(auxBuffer!=NULL){
				sumR=-sumR;
				sumG=-sumG;
				sumB=-sumB;
				sumR+=*(auxBuffer+i*rowSize+j*3);
				sumG+=*(auxBuffer+i*rowSize+j*3+1);
				sumB+=*(auxBuffer+i*rowSize+j*3+2);
				if(sumR>255) sumR=255;
				if(sumR<0) sumR=0;
				if(sumG>255) sumG=255;
				if(sumG<0) sumG=0;
				if(sumB>255) sumB=255;
				if(sumB<0) sumB=0;
			} 
			*(resultBuffer+i*rowSize+j*3)=sumR;
			*(resultBuffer+i*rowSize+j*3+1)=sumG;
			*(resultBuffer+i*rowSize+j*3+2)=sumB;
		}
	return resultBuffer;
}

pBYTE meanFiltering(int x,int y,pBYTE buffer){
	pBYTE resultBuffer;
	resultBuffer=convolutionOp(x,y,buffer,NULL,mx,my,mcx,mcy,mFiltKernel,true);
	return resultBuffer;
}

pBYTE LaplacianEnh(int x,int y,pBYTE buffer){
	pBYTE resultBuffer;
	resultBuffer=convolutionOp(x,y,buffer,buffer,lx,ly,lcx,lcy,LapOpKernel,false);
	return resultBuffer;
}
