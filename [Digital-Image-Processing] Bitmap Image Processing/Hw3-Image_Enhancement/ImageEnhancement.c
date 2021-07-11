#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(1)

#define BM 0x4D42
#define BI_RGB 0
#define FILENAME "TestIn.bmp"
#define OUTPUTNAME "TestOut.bmp"
#define FUNC "p(r)=kr,k=1/256"

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
void RGB2YUV(pBYTE R,pBYTE G,pBYTE B,pBYTE Y,pBYTE U,pBYTE V);
void YUV2RGB(pBYTE Y,pBYTE U,pBYTE V,pBYTE R,pBYTE G,pBYTE B);
pBYTE getYUVMat(pBYTE buffer,int x,int y);
pBYTE YUVMat2Buff(pBYTE YUVMat,int x,int y);
pBYTE logVisEnhance(pBYTE buffer,int x,int y);
pBYTE hisEqualization(pBYTE buffer,int x,int y);

int main(void){
	bmh bmHeader;
	pBYTE bmData=readColorBMP(&bmHeader),newbmData;
	char input;
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
			printf("/*********************************************************************/\n\n");
			printf("1.v-Visibility enhancement using logarithmic operation\n\n");
			printf("2.h-Histogram equalization using transform function \"%s\"\n\n",FUNC);
			printf("3.q-Exit\n\n");
			printf("/*********************************************************************/\n\n");
			printf("Your Choice:");
			input=getchar();
			switch(input){
				case 'v':
					newbmData=logVisEnhance(bmData,bmHeader.ih.biWidth,bmHeader.ih.biHeight);
					if(writeColorBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\n\nAn error occurred during writing!");
					else printf("\n\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 'h':
					newbmData=hisEqualization(bmData,bmHeader.ih.biWidth,bmHeader.ih.biHeight);
					if(writeColorBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\n\nAn error occurred during writing!");
					else printf("\n\nWriting image complete!");
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

void RGB2YUV(pBYTE R,pBYTE G,pBYTE B,pBYTE Y,pBYTE U,pBYTE V){
	*Y=0.299*(*R)+0.587*(*G)+0.114*(*B);
	*U=-0.147*(*R)-0.289*(*G)+0.435*(*B)+128;
	*V=0.615*(*R)-0.515*(*G)-0.100*(*B)+128;
}

void YUV2RGB(pBYTE Y,pBYTE U,pBYTE V,pBYTE R,pBYTE G,pBYTE B){
	double fR,fG,fB;
	fR=(*Y)+1.140*(*V-128);
	fG=(*Y)-0.394*(*U-128)-0.581*(*V-128);
	fB=(*Y)+2.032*(*U-128);
	if(fR>255) *R=255;
	else if(fR<0) *R=0;
	else *R=(BYTE)fR;
	if(fG>255) *G=255;
	else if(fG<0) *G=0;
	else *G=(BYTE)fG;
	if(fB>255) *B=255;
	else if(fB<0) *B=0;
	else *B=(BYTE)fB;
}

pBYTE getYUVMat(pBYTE buffer,int x,int y){
	pBYTE resultMat;
	int rowSize,i,j;
	rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	resultMat=(pBYTE)malloc(x*y*3);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			RGB2YUV(buffer+i*rowSize+j*3,buffer+i*rowSize+j*3+1,buffer+i*rowSize+j*3+2,resultMat+i*3*x+j*3,resultMat+i*3*x+j*3+1,resultMat+i*3*x+j*3+2);
	return resultMat;
}

pBYTE YUVMat2Buff(pBYTE YUVMat,int x,int y){
	pBYTE resultBuffer;
	int rowSize,i,j;
	rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	resultBuffer=(pBYTE)malloc(rowSize*y*3);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			YUV2RGB(YUVMat+i*3*x+j*3,YUVMat+i*3*x+j*3+1,YUVMat+i*3*x+j*3+2,resultBuffer+i*rowSize+j*3,resultBuffer+i*rowSize+j*3+1,resultBuffer+i*rowSize+j*3+2);
	return resultBuffer;
}

pBYTE logVisEnhance(pBYTE buffer,int x,int y){
	pBYTE YUVMat,resultBuffer;
	int i,j;
	int maxY=0;
	double newY;
	YUVMat=getYUVMat(buffer,x,y);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			if(*(YUVMat+3*i*x+3*j)>maxY) maxY=*(YUVMat+3*i*x+3*j);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			newY=log(*(YUVMat+3*i*x+3*j)+1)/log(maxY+1)*255;
			*(YUVMat+3*i*x+3*j)=newY;
		}
	resultBuffer=YUVMat2Buff(YUVMat,x,y);
	free(YUVMat);
	return resultBuffer;
}

pBYTE hisEqualization(pBYTE buffer,int x,int y){
	pBYTE YUVMat,resultBuffer;
	double accuRatio[256];
	double accuProb[256];
	int mappingTable[256];
	int i,j,scale=x*y;
	YUVMat=getYUVMat(buffer,x,y);
	for(i=0;i<256;i++){
		if(i==0) accuRatio[i]=0;
		else accuRatio[i]=1.0/255*i;
	}
	for(i=0;i<256;i++)
		accuProb[i]=0;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			accuProb[*(YUVMat+3*i*x+3*j)]++;
	for(i=0;i<256;i++){
		accuProb[i]/=scale;
		if(i>0) accuProb[i]+=accuProb[i-1];
	}
	for(i=0;i<256;i++){
		for(j=1;j<256;j++){
			if(accuProb[i]>=accuRatio[j-1]&&accuProb[i]<=accuRatio[j]){
				if(accuProb[i]-accuRatio[j-1]<accuRatio[j]-accuProb[i])
					mappingTable[i]=j-1;
				else mappingTable[i]=j;
				break;
			}
		}
	}
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			*(YUVMat+3*i*x+3*j)=mappingTable[*(YUVMat+3*i*x+3*j)];
	resultBuffer=YUVMat2Buff(YUVMat,x,y);
	free(YUVMat);
	return resultBuffer;
}
