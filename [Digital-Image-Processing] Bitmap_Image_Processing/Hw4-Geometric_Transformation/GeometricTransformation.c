#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(1)

#define BM 0x4D42
#define BI_RGB 0
#define FILENAME "TestIn.bmp"
#define OUTPUTNAME "TestOut.bmp"
#define ABS(x) ((x)>=0?(x):(-x))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define PI 3.1415926

enum bistate{false,true};
typedef enum bistate boolean;

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
pBYTE createCanvas(int x,int y,BYTE initR,BYTE initG,BYTE initB);
pBYTE geoTranslation(int x,int y,int newX,int newY,int x0,int y0,pBYTE buffer); //Center aligned
pBYTE geoShear(int x,int y,double dx,double dy,pBYTE buffer);
pBYTE geoMirror(int x,int y,boolean enableX,boolean enableY,pBYTE buffer);
pBYTE geoScale(int x,int y,double c,double d,pBYTE buffer);
pBYTE geoRotation(int x,int y,double angle,pBYTE buffer);

int main(void){
	bmh bmHeader;
	pBYTE bmData=readColorBMP(&bmHeader),newbmData;
	int newWidth,newHeight,x0,y0;
	boolean isMirrorX,isMirrorY;
	double slope,scaleX,scaleY,angle;
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
			printf("/***************************************/\n\n");
			printf("1.t-Do a translation on the image.\n\n");
			printf("2.m-Do a mirror on the image.\n\n");
			printf("3.s-Do a shear on the image.\n\n");
			printf("4.c-Scale the image.\n\n");
			printf("5.r-Rotate the image.\n\n");
			printf("6.q-Exit\n\n");
			printf("/***************************************/\n\n");
			printf("Your Choice:");
			input=getchar();
			switch(input){
				case 't':
					printf("\nInput Bitmap Scale:%d * %d\n\n",bmHeader.ih.biWidth,bmHeader.ih.biHeight);
					printf("Please input the width of new canvas:");
					scanf("%d",&newWidth);
					printf("Please input the height of new canvas:");
					scanf("%d",&newHeight);
					printf("Please input translation distance on X axis:");
					scanf("%d",&x0);
					printf("Please input translation distance on Y axis:");
					scanf("%d",&y0);
					newbmData=geoTranslation(bmHeader.ih.biWidth,bmHeader.ih.biHeight,newWidth,newHeight,x0,y0,bmData);
					if(writeColorBMP(newWidth,newHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\nAn error occurred during writing!");
					else printf("\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 'm':
					getchar();
					printf("\nDo you want to do a mirror on X axis?(y-Yes,n-No)");
					input=getchar();
					if(input=='y') isMirrorX=true;
					else isMirrorX=false;
					getchar();
					printf("Do you want to do a mirror on Y axis?(y-Yes,n-No)");
					input=getchar();
					if(input=='y') isMirrorY=true;
					else isMirrorY=false;
					newbmData=geoMirror(bmHeader.ih.biWidth,bmHeader.ih.biHeight,isMirrorX,isMirrorY,bmData);
					if(writeColorBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\nAn error occurred during writing!");
					else printf("\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 's':
					getchar();
					printf("\nShear the image on X or Y axis?(x-X axis.y-Y axis)");
					input=getchar();
					printf("Please input shear slope:");
					scanf("%lf",&slope);
					if(input=='x'){
						newbmData=geoShear(bmHeader.ih.biWidth,bmHeader.ih.biHeight,slope,0,bmData);
						newWidth=bmHeader.ih.biWidth+bmHeader.ih.biHeight*ABS(slope)+0.5;
						newHeight=bmHeader.ih.biHeight;
					}else{
						newbmData=geoShear(bmHeader.ih.biWidth,bmHeader.ih.biHeight,0,slope,bmData);
						newWidth=bmHeader.ih.biWidth;
						newHeight=bmHeader.ih.biHeight+bmHeader.ih.biWidth*ABS(slope)+0.5;
					}
					if(writeColorBMP(newWidth,newHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\nAn error occurred during writing!");
					else printf("\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 'c':
					printf("\nPlease input scale ratio on X axis(not 0):");
					scanf("%lf",&scaleX);
					printf("Please input scale ratio on Y axis(not 0):");
					scanf("%lf",&scaleY);
					newbmData=geoScale(bmHeader.ih.biWidth,bmHeader.ih.biHeight,scaleX,scaleY,bmData);
					newWidth=bmHeader.ih.biWidth*ABS(scaleX)+0.5;
					newHeight=bmHeader.ih.biHeight*ABS(scaleY)+0.5;
					if(writeColorBMP(newWidth,newHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
						printf("\nAn error occurred during writing or parameters error!");
					else printf("\nWriting image complete!");
					free(newbmData);
					sleep(1);
					system("cls");
					break;
				case 'r':
					printf("\nPlease input angle for rotation(unit:degree):");
					scanf("%lf",&angle);
					angle=angle/360*2*PI;
					newbmData=geoRotation(bmHeader.ih.biWidth,bmHeader.ih.biHeight,angle,bmData);
					newWidth=abs(bmHeader.ih.biWidth*sin(angle))+abs(bmHeader.ih.biHeight*cos(angle))+0.5;
					newHeight=abs(bmHeader.ih.biWidth*cos(angle))+abs(bmHeader.ih.biHeight*sin(angle))+0.5;
					if(writeColorBMP(newWidth,newHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData)!=0)
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

pBYTE createCanvas(int x,int y,BYTE initR,BYTE initG,BYTE initB){
	int rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	pBYTE resultBuffer=(pBYTE)malloc(rowSize*y);
	int i,j;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			*(resultBuffer+i*rowSize+3*j)=initR;
			*(resultBuffer+i*rowSize+3*j+1)=initG;
			*(resultBuffer+i*rowSize+3*j+2)=initB;
		}
	return resultBuffer;
}

pBYTE geoTranslation(int x,int y,int newX,int newY,int x0,int y0,pBYTE buffer){
	pBYTE resultBuffer=createCanvas(newX,newY,255,255,255);
	int marginX=(newX-x)/2,marginY=(newY-y)/2;
	int rowSize=3*x,newRowSize=3*newX;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	if(newRowSize%4!=0) newRowSize=newRowSize+(4-newRowSize%4);
	int i,j,corrX,corrY;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			corrX=j+x0+marginX;
			corrY=i+y0+marginY;
			if(corrX>=0&&corrX<newX&&corrY>=0&&corrY<newY){
				*(resultBuffer+corrY*newRowSize+3*corrX)=*(buffer+i*rowSize+3*j);
				*(resultBuffer+corrY*newRowSize+3*corrX+1)=*(buffer+i*rowSize+3*j+1);
				*(resultBuffer+corrY*newRowSize+3*corrX+2)=*(buffer+i*rowSize+3*j+2);
			}
		}
	return resultBuffer;
}

pBYTE geoShear(int x,int y,double dx,double dy,pBYTE buffer){
	int newX=x+y*ABS(dx)+0.5,newY=y+x*ABS(dy)+0.5;
	pBYTE resultBuffer=createCanvas(newX,newY,255,255,255);
	int rowSize=3*x,newRowSize=3*newX;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	if(newRowSize%4!=0) newRowSize=newRowSize+(4-newRowSize%4);
	int i,j,corrX,corrY,offsetX,offsetY;
	if(dx>=0) offsetX=0;
	else offsetX=newX-x;
	if(dy>=0) offsetY=0;
	else offsetY=newY-y;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			corrX=j+dx*i+offsetX;
			corrY=i+dy*j+offsetY;
			*(resultBuffer+corrY*newRowSize+3*corrX)=*(buffer+i*rowSize+3*j);
			*(resultBuffer+corrY*newRowSize+3*corrX+1)=*(buffer+i*rowSize+3*j+1);
			*(resultBuffer+corrY*newRowSize+3*corrX+2)=*(buffer+i*rowSize+3*j+2);
		}
	return resultBuffer;
}

pBYTE geoMirror(int x,int y,boolean enableX,boolean enableY,pBYTE buffer){
	pBYTE resultBuffer=createCanvas(x,y,255,255,255);
	int rowSize=3*x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	int i,j,corrX,corrY;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			if(enableX==true) corrX=x-j-1;
			else corrX=j;
			if(enableY==true) corrY=y-i-1;
			else corrY=i;
			*(resultBuffer+corrY*rowSize+3*corrX)=*(buffer+i*rowSize+3*j);
			*(resultBuffer+corrY*rowSize+3*corrX+1)=*(buffer+i*rowSize+3*j+1);
			*(resultBuffer+corrY*rowSize+3*corrX+2)=*(buffer+i*rowSize+3*j+2);
		}
	return resultBuffer;
}

pBYTE geoScale(int x,int y,double c,double d,pBYTE buffer){ //Using nearest interpolytion
	if(c==0||d==0) return NULL;
	int newX=x*ABS(c)+0.5,newY=y*ABS(d)+0.5;
	pBYTE resultBuffer=createCanvas(newX,newY,255,255,255);
	int rowSize=3*x,newRowSize=3*newX;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	if(newRowSize%4!=0) newRowSize=newRowSize+(4-newRowSize%4);
	int i,j,corrX,corrY;
	for(i=0;i<newY;i++)
		for(j=0;j<newX;j++){ //Upsampling
			corrX=j/ABS(c);
			corrY=i/ABS(d);
			*(resultBuffer+i*newRowSize+3*j)=*(buffer+corrY*rowSize+3*corrX);
			*(resultBuffer+i*newRowSize+3*j+1)=*(buffer+corrY*rowSize+3*corrX+1);
			*(resultBuffer+i*newRowSize+3*j+2)=*(buffer+corrY*rowSize+3*corrX+2);
		}
	return resultBuffer;
}

pBYTE geoRotation(int x,int y,double angle,pBYTE buffer){ //Using nearest interpolytion
	int newX=abs(x*sin(angle))+abs(y*cos(angle))+0.5;
	int newY=abs(x*cos(angle))+abs(y*sin(angle))+0.5;
	pBYTE resultBuffer=createCanvas(newX,newY,255,255,255);
	int rowSize=3*x,newRowSize=3*newX;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	if(newRowSize%4!=0) newRowSize=newRowSize+(4-newRowSize%4);
	int i,j,corrX,corrY,offsetX,offsetY;
	int angleInDegree=360*angle/(2*PI);
	angleInDegree%=360;
	if(angleInDegree>0&&angleInDegree<=90){
		offsetX=y*sin(angle);
		offsetY=0;
	}else if(angleInDegree>90&&angleInDegree<=180){
		offsetX=newX;
		offsetY=-y*cos(angle);
	}else if(angleInDegree>180&&angleInDegree<=270){
		offsetX=-x*cos(angle);
		offsetY=newY;
	}else{
		offsetX=0;
		offsetY=-x*sin(angle);
	}
	for(i=0;i<newY;i++)
		for(j=0;j<newX;j++){
			corrX=(j-offsetX)*cos(angle)+(i-offsetY)*sin(angle);
			corrY=(i-offsetY)*cos(angle)-(j-offsetX)*sin(angle);
			if(corrX>=0&&corrX<x&&corrY>=0&&corrY<y){
				*(resultBuffer+i*newRowSize+3*j)=*(buffer+corrY*rowSize+3*corrX);
				*(resultBuffer+i*newRowSize+3*j+1)=*(buffer+corrY*rowSize+3*corrX+1);
				*(resultBuffer+i*newRowSize+3*j+2)=*(buffer+corrY*rowSize+3*corrX+2);	
			}
		}
	return resultBuffer;
}
