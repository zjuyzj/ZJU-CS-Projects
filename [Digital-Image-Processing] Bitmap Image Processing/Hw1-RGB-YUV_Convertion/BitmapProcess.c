#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)

#define BM 0x4D42
#define BI_RGB 0
#define FILENAME "lena512color.bmp"
#define OUTPUTNAME "lena512out.bmp"

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
int writeBMP(char type,int x,int y,int resX,int resY,pBYTE buffer);
void RGB2YUV(pBYTE R,pBYTE G,pBYTE B,pBYTE Y,pBYTE U,pBYTE V);
void YUV2RGB(pBYTE Y,pBYTE U,pBYTE V,pBYTE R,pBYTE G,pBYTE B);
pBYTE Color2Gray(pBYTE buffer,int x,int y);
pBYTE adjColorLuminance(pBYTE buffer,int x,int y,double percentage);

int main(void){
	bmh bmHeader;
	pBYTE bmData=readColorBMP(&bmHeader),newbmData;
	pBYTE bmGray;
	char input;
	double ratio;
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
			printf("1.w-Write %s into %s without processing\n\n",FILENAME,OUTPUTNAME);
			printf("2.g-Convert %s to a grayscale image %s\n\n",FILENAME,OUTPUTNAME);
			printf("3.a-Adjust luminance of %s,saved as %s\n\n",FILENAME,OUTPUTNAME);
			printf("4.q-Exit\n\n");
			printf("/*********************************************************************/\n\n");
			printf("Your Choice:");
			input=getchar();
			switch(input){
				case 'w':
					if(writeBMP('C',bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,bmData)!=0)
						printf("\n\nAn error occurred during writing!");
					else printf("\n\nWriting image complete!");
					sleep(1);
					system("cls");
					break;
				case 'g':
					bmGray=Color2Gray(bmData,bmHeader.ih.biWidth,bmHeader.ih.biHeight);
					if(writeBMP('G',bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,bmGray))
						printf("\n\nAn error occurred during writing!");
					else printf("\n\nWriting image complete!");
					sleep(1);
					system("cls");
					break;
				case 'a':
					printf("\n\nPlease input a ratio,such as 0.75(Recommanded:0.35-1.10):");
					scanf("%lf",&ratio);
					newbmData=adjColorLuminance(bmData,bmHeader.ih.biWidth,bmHeader.ih.biHeight,ratio);
					if(writeBMP('C',bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\n\nAn error occurred during writing!");
					else printf("\n\nWriting image complete!");
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

int writeBMP(char type,int x,int y,int resX,int resY,pBYTE buffer){
	FILE* fp;
	bmh newHeader;
	pBYTE newPalette;
	int rowSize;
	int totalSize;
	int i;
	if(type=='C'){ //Color image
		rowSize=3*x;
		newHeader.ih.biBitCount=24;
	}
	else if(type=='G'){ //Grayscale image
		rowSize=x;
		newHeader.ih.biBitCount=8;
		newPalette=(pBYTE)malloc(256*4);
		if(!newPalette) return 1;
		for(i=0;i<=255;i++){
			*(newPalette+4*i)=i;
			*(newPalette+4*i+1)=i;
			*(newPalette+4*i+2)=i;
			*(newPalette+4*i+3)=0;
		}
	}else return 1;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	totalSize=rowSize*y+sizeof(bmh);
	if(type=='G') totalSize+=256*4;
	newHeader.fh.bfSize=totalSize;
	newHeader.fh.bfType=BM;
	newHeader.fh.bfReserved1=0;
	newHeader.fh.bfReserved2=0;
	newHeader.fh.bfOffbits=sizeof(bmh);
	if(type=='G') newHeader.fh.bfOffbits+=256*4;
	newHeader.ih.biSize=sizeof(bmih);
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
	if(type=='G') fwrite(newPalette,256*4,1,fp);
	fwrite(buffer,rowSize*y,1,fp);
	fclose(fp);
	return 0;
}

void RGB2YUV(pBYTE R,pBYTE G,pBYTE B,pBYTE Y,pBYTE U,pBYTE V){
	*Y=0.298*(*R)+0.612*(*G)+0.117*(*B);
	*U=-0.168*(*R)-0.330*(*G)+0.498*(*B)+128;
	*V=0.449*(*R)-0.435*(*G)-0.083*(*B)+128;
}

void YUV2RGB(pBYTE Y,pBYTE U,pBYTE V,pBYTE R,pBYTE G,pBYTE B){
	double fR,fG,fB;
	fR=(*Y)+1.4075*(*V-128);
	fG=(*Y)-0.3455*(*U-128)-0.7169*(*V-128);
	fB=(*Y)+1.779*(*U-128);
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

pBYTE adjColorLuminance(pBYTE buffer,int x,int y,double percentage){
	BYTE tY,tU,tV;
	pBYTE newData;
	int rowSize,i,j,offset;
	rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	newData=(pBYTE)malloc(rowSize*y);
	if(!newData) return NULL;
	for(i=0;i<y;i++){
		for(j=0;j<x;j++){
			offset=i*rowSize+j*3;
			RGB2YUV(buffer+offset,buffer+offset+1,buffer+offset+2,&tY,&tU,&tV);
			tY=(BYTE)((double)tY*percentage);
			if(tY>255) tY=255;
			YUV2RGB(&tY,&tU,&tV,newData+offset,newData+offset+1,newData+offset+2);
		}
	}
	return newData;
}

pBYTE Color2Gray(pBYTE buffer,int x,int y){
	pBYTE grayData;
	int i,j,offsetA,offsetB;
	BYTE Y,U,V;
	int rowSizeA=3*x,rowSizeB=x;
	if(rowSizeA%4!=0) rowSizeA=rowSizeA+(4-rowSizeA%4);
	if(rowSizeB%4!=0) rowSizeB=rowSizeB+(4-rowSizeB%4);
	grayData=(pBYTE)malloc(rowSizeB*y);
	if(!grayData) return NULL;
	for(i=0;i<y;i++){
		for(j=0;j<x;j++){
			offsetA=i*rowSizeA+j*3;
			offsetB=i*rowSizeB+j;
			RGB2YUV(buffer+offsetA,buffer+offsetA+1,buffer+offsetA+2,&Y,&U,&V);
			*(grayData+offsetB)=Y;
		}
	}
	return grayData;
}
