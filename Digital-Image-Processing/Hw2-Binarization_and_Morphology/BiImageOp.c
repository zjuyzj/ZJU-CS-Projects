#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(1)

#define BM 0x4D42
#define BI_RGB 0
#define FILENAME "SampleIn.bmp"
#define OUTPUTNAME "SampleOut.bmp"

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

int morpTemplete[9]={0,1,0,
					 1,1,1,
					 0,1,0};
int tX=3,tY=3,ctX=1,ctY=1; //(tX,tY):Size of templete;(ctX,ctY):Center of templete. 

pBYTE readGrayBMP(bmh* header,char* filename);
int writeGrayBMP(int x,int y,int resX,int resY,pBYTE buffer);
int getThreshold(int x,int y,pBYTE buffer); //Get the best global threshold using OTSU algorithm.
pBYTE getBinaryImg(int x,int y,pBYTE buffer);
pBYTE getLocBinaryImg(int x,int y,int windowX,int windowY,pBYTE buffer); //Get a binary image by get each local binary patch with a silding window,no overlapping.
pBYTE invertBW(int x,int y,pBYTE buffer);
pBYTE morpErosion(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete);
pBYTE morpDelation(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete);
pBYTE morpOpening(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete);
pBYTE morpClosing(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete);

int main(void){
	bmh bmHeader;
	pBYTE bmData=readGrayBMP(&bmHeader,FILENAME),newbmData;
	char input;
	int winWidth,winHeight;
	int i,j;
	if(bmData==NULL){
		printf("/************************************************************/\n");
		printf("Can not open your BMP file!\n");
		printf("File not found or file format error.\n");
		printf("File name:%s\n",FILENAME);
		printf("Format:Windows DIB,grayscale,no compression,8 bits depth\n");
		printf("/************************************************************/\n");
		printf("(Press any key to exit.)\n");
		system("pause");
		exit(1);
	}else if(tY>bmHeader.ih.biHeight||tX>bmHeader.ih.biWidth){
		printf("/*****************************************************************/\n");
		printf("Templete format error!\n");
		printf("Size of templete can't be larger than image to process'\n");
		printf("/*****************************************************************/\n");
		printf("(Press any key to exit.)\n");
		system("pause");
		exit(1);
	}else
		while(1){
			system("cls");
			printf("Input filename: %s    Output filename: %s\n\n",FILENAME,OUTPUTNAME);
			printf("Morphological operation templete(*:white pixel;0:black pixel):\n\n");
			for(i=0;i<tY;i++){
				for(j=0;j<tX;j++)
					if(morpTemplete[i*tX+j]==0) printf("0 ");
					else printf("* ");
				printf("\n");
			}
			printf("\nPlease press a key to select a function in the list:\n\n");
			printf("/****************************************************************************************************/\n\n");
			printf("1.b-Get a binary image using global OSTU algorithm\n\n");
			printf("2.w-Get a binary image using local OSTU algorithm (Sliding Window Mathod without Overlap)\n\n");
			printf("3.r-Reverse black and white pixels in a binary image then output\n\n");
			printf("4.d-Do a dilation on a binary image with templete then output\n\n");
			printf("5.e-Do an erosion on a binary image with templete then output\n\n");
			printf("6.o-Do an opening operation on a binary image with templete then output\n\n");
			printf("7.c-Do a closing operation on a binary image with templete then output\n\n");
			printf("8.q-Exit\n\n");
			printf("/****************************************************************************************************/\n\n");
			printf("Your Choice:");
			input=getchar();
			switch(input){
				case 'b':
					newbmData=getBinaryImg(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmData);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\n\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'w':
					printf("\nInput Bitmap Scale:%d * %d\n\n",bmHeader.ih.biWidth,bmHeader.ih.biHeight);
					printf("Please input sliding window's width:");
					scanf("%d",&winWidth);
					printf("Please input sliding window's height:");
					scanf("%d",&winHeight);
					newbmData=getLocBinaryImg(bmHeader.ih.biWidth,bmHeader.ih.biHeight,winWidth,winHeight,bmData);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'r':
					newbmData=invertBW(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmData);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'd':
					newbmData=morpDelation(bmHeader.ih.biWidth,bmHeader.ih.biHeight,tX,tY,ctX,ctY,bmData,morpTemplete);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\n\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'e':
					newbmData=morpErosion(bmHeader.ih.biWidth,bmHeader.ih.biHeight,tX,tY,ctX,ctY,bmData,morpTemplete);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\n\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'o':
					newbmData=morpOpening(bmHeader.ih.biWidth,bmHeader.ih.biHeight,tX,tY,ctX,ctY,bmData,morpTemplete);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\n\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'c':
					newbmData=morpClosing(bmHeader.ih.biWidth,bmHeader.ih.biHeight,tX,tY,ctX,ctY,bmData,morpTemplete);
					if(writeGrayBMP(bmHeader.ih.biWidth,bmHeader.ih.biHeight,bmHeader.ih.biXPelsPerMeter,bmHeader.ih.biYPelsPerMeter,newbmData))
						printf("\n\nAn error occurred during writing!");
					else{
						printf("\n\nWriting image complete!");
						free(newbmData);
					}
					sleep(1);
					system("cls");
					break;
				case 'q':
					free(bmData);
					exit(0);
					break;
				defult:
					continue;
			}
		}
} 

pBYTE readGrayBMP(bmh* header,char* filename){
	FILE* fp;
	int datalen;
	fp=fopen(filename,"rb");
	if(!fp) return NULL;
	fseek(fp,0,SEEK_SET);
	fread(header,sizeof(bmh),1,fp);
	if(header->fh.bfType!=BM||header->ih.biBitCount!=8||header->ih.biCompression!=BI_RGB) return NULL; 
	datalen=header->fh.bfSize-header->fh.bfOffbits;
	pBYTE buffer=(pBYTE)malloc(datalen);
	fseek(fp,header->fh.bfOffbits,SEEK_SET);
	fread(buffer,datalen,1,fp);
	fclose(fp);
	return buffer;
}

int writeGrayBMP(int x,int y,int resX,int resY,pBYTE buffer){
	FILE* fp;
	bmh newHeader;
	pBYTE newPalette;
	int rowSize;
	int totalSize;
	int i;
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
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	totalSize=rowSize*y+sizeof(bmh)+256*4;
	newHeader.fh.bfSize=totalSize;
	newHeader.fh.bfType=BM;
	newHeader.fh.bfReserved1=0;
	newHeader.fh.bfReserved2=0;
	newHeader.fh.bfOffbits=sizeof(bmh)+256*4;
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
	fwrite(newPalette,256*4,1,fp);
	fwrite(buffer,rowSize*y,1,fp);
	fclose(fp);
	return 0;
}

int getThreshold(int x,int y,pBYTE buffer){
	int tmpT,bestT;
	int N0=0,N1,N=x*y,tL0=0,tL1=0; //N0:Number of pixels in background(white pixels);tL:Total luminance.
	double u0,u1,newG,G=-1; //u:Average luminance;G:Variance between.
	int i,j,rowSize=x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	for(tmpT=bestT=0;tmpT<=255;tmpT++){
		N0=0,tL0=0,tL1=0;
		for(i=0;i<y;i++)
			for(j=0;j<x;j++){
				if(*(buffer+i*rowSize+j)<tmpT){
					N0++;
					tL0+=*(buffer+i*rowSize+j);
				}
				else tL1+=*(buffer+i*rowSize+j);
			}
			
		N1=N-N0;
		if(N0!=0) u0=(double)tL0/N0;
		else u0=0;
		if(N1!=0) u1=(double)tL1/N1;
		else u1=0;
		newG=((double)N0/N)*((double)N1/N)*pow(u0-u1,2);
		if(newG>G){
			G=newG;
			bestT=tmpT;
		}
	}
	return bestT;
}

pBYTE getBinaryImg(int x,int y,pBYTE buffer){
	int bufferSize,rowSize=x;
	int i,j;
	int T=getThreshold(x,y,buffer);
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	bufferSize=rowSize*y;
	pBYTE biImg=(pBYTE)malloc(bufferSize);
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			if(*(buffer+i*rowSize+j)>=T)
				*(biImg+i*rowSize+j)=255;
			else *(biImg+i*rowSize+j)=0;
		}
	return biImg;
}

pBYTE getLocBinaryImg(int x,int y,int windowX,int windowY,pBYTE buffer){
	int rowSize=x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	int locRowSize,locX,locY;
	if(windowX>x) windowX=x;
	if(windowY>y) windowY=y;
	int windowNumX=ceil((double)x/windowX),windowNumY=ceil((double)y/windowY);
	int i,j,m,n;
	pBYTE locBuffer,tmpBuffer;
	pBYTE resultBuffer=(pBYTE)malloc(rowSize*y);
	for(i=0;i<windowNumY;i++)
		for(j=0;j<windowNumX;j++){
			if((j+1)*windowX>x) locX=x-j*windowX;
			else locX=windowX;
			locRowSize=locX;
			if(locRowSize%4!=0) locRowSize=locRowSize+(4-locRowSize%4);
			if((i+1)*windowY>y) locY=y-i*windowY;
			else locY=windowY;
			locBuffer=(pBYTE)malloc(locRowSize*locY);
			for(m=0;m<locY;m++)
				for(n=0;n<locX;n++)
					*(locBuffer+locRowSize*m+n)=*(buffer+i*windowY*rowSize+j*windowX+m*rowSize+n);
			tmpBuffer=getBinaryImg(locX,locY,locBuffer);
			for(m=0;m<locY;m++)
				for(n=0;n<locX;n++)
					*(resultBuffer+i*windowY*rowSize+j*windowX+m*rowSize+n)=*(tmpBuffer+locRowSize*m+n);
			free(tmpBuffer);
			free(locBuffer);
		}
	return resultBuffer;
}

pBYTE morpDelation(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete){
	int rowSize=x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	int i,j,m,n,flag;
	pBYTE resultBuffer=(pBYTE)malloc(rowSize*y);
	for(i=0;i<rowSize*y;i++) *(resultBuffer+i)=0;
	for(i=0;i<y-ty;i++)
		for(j=0;j<x-tx;j++){
			flag=0;
			for(m=0;m<ty;m++)
				for(n=0;n<tx;n++)
					if((*(buffer+rowSize*i+j+rowSize*m+n)&*(templete+m*tx+n))!=0) flag=1;
			if(flag) *(resultBuffer+rowSize*i+j+rowSize*cty+ctx)=255;
		}
	return resultBuffer;
}

pBYTE morpErosion(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete){
	int rowSize=x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	int i,j,m,n,flag;
	pBYTE resultBuffer=(pBYTE)malloc(rowSize*y);
	for(i=0;i<rowSize*y;i++) *(resultBuffer+i)=0;
	for(i=0;i<y-ty;i++)
		for(j=0;j<x-tx;j++){
			flag=1;
			for(m=0;m<ty;m++)
				for(n=0;n<tx;n++)
					if((*(buffer+rowSize*i+j+rowSize*m+n)&*(templete+m*tx+n))==0&&*(templete+m*tx+n)!=0) flag=0;
			if(flag) *(resultBuffer+rowSize*i+j+rowSize*cty+ctx)=255;
		}
	return resultBuffer;
}

pBYTE morpOpening(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete){
	pBYTE tmpBuffer=morpErosion(x,y,tx,ty,ctx,cty,buffer,templete);
	pBYTE resultBuffer=morpDelation(x,y,tx,ty,ctx,cty,tmpBuffer,templete);
	free(tmpBuffer);
	return resultBuffer;
}

pBYTE morpClosing(int x,int y,int tx,int ty,int ctx,int cty,pBYTE buffer,int* templete){
	pBYTE tmpBuffer=morpDelation(x,y,tx,ty,ctx,cty,buffer,templete);
	pBYTE resultBuffer=morpErosion(x,y,tx,ty,ctx,cty,tmpBuffer,templete);
	free(tmpBuffer);
	return resultBuffer;
}

pBYTE invertBW(int x,int y,pBYTE buffer){
	int rowSize=x;
	if(rowSize%4!=0) rowSize=rowSize+(4-rowSize%4);
	pBYTE resultBuffer=(pBYTE)malloc(rowSize*y);
	int i,j;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++){
			if(*(buffer+i*rowSize+j)==0)
				*(resultBuffer+i*rowSize+j)=255;
			else *(resultBuffer+i*rowSize+j)=0;
		}
	return resultBuffer;
}

