#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void readEchoImage(string inputfname, string echofname, int **&
	imagedata, int& head1, int& head2, int& head3, int& head4){
		int t1, t2;
		ifstream inputfile(inputfname);
		ofstream echofile(echofname);
		inputfile >> head1;
		echofile << head1 << " ";
		inputfile >> head2;
		echofile << head2 << " ";
		inputfile >> head3;
		echofile << head3 << " ";
		inputfile >> head4;
		echofile << head4 << "\n";

		imagedata = new int*[head1];

		for (int i = 0; i < head1; i++)
			imagedata[i] = new int[head2];

		for (t1 = 0; t1 < head1; t1++) {

			for (t2 = 0; t2 < head2; t2++) {
				inputfile >> imagedata[t1][t2];
				echofile << imagedata[t1][t2] << " ";
			}
			echofile << "\n";

		}

		inputfile.close(); echofile.close();
}

void prettyPrint(string ofile, int ** imgdata, int h1, int h2){
	int t1, t2;
	ofstream output(ofile);
	for (t1 = 1; t1<h1; t1++){
		for (t2 = 1; t2<h2; t2++){
			if (imgdata[t1][t2]>9){
				output << imgdata[t1][t2] << "";
			}
			else if (imgdata[t1][t2]>0){
				output << imgdata[t1][t2] << " ";
			}
			else
				output << "  ";
		}
		output << "\n";
	}

	output.close();
}

void frame0(int nRows, int nCols, int ** imagedata, int ** expandIMGD){
	int t1, t2;
	for (t1 = 1; t1 <= nRows; t1++){
		for (t2 = 1; t2 <= nCols; t2++){
			expandIMGD[t1][t2] = imagedata[t1 - 1][t2 - 1];
		}
	}
	for (t1 = 0; t1 <= nCols + 1; t1++){
		expandIMGD[0][t1] = 0;
		expandIMGD[nRows + 1][t1] = 0;
	}
	for (t1 = 1; t1 <= nRows; t1++){
		expandIMGD[t1][0] = 0;
		expandIMGD[t1][nCols + 1] = 0;
	}
}
int minPos(int n1, int n2, int n3, int n4){
	int minpos;
	minpos = n1 + n2 + n3 + n4;
	if (minpos == 0)
		return -1;
	if (n1 > 0)
		minpos = n1;
	if (n2 > 0 && n2<minpos)
		minpos = n2;
	if (n3>0 && n3<minpos)
		minpos = n3;
	if (n4>0 && n4<minpos)
		minpos = n4;
	return minpos;
}
int minPos(int n1, int n2, int n3, int n4, int n5){
	int minpos;
	minpos = n1 + n2 + n3 + n4 + n5;
	if (minpos == 0)
		return -1;
	if (n1>0)
		minpos = n1;
	if (n2 > 0 && n2<minpos)
		minpos = n2;
	if (n3>0 && n3<minpos)
		minpos = n3;
	if (n4>0 && n4<minpos)
		minpos = n4;
	if (n5>0 && n5 < minpos)
		minpos = n5;
	return minpos;
}
void constructThreeCell(int ti, int tj, int **data, int ** threeCell){
	int t1, t2, ci, cj;
	ci = ti - 1;
	cj = tj - 1;
	for (t1 = 0; t1 < 3; t1++){
		for (t2 = 0; t2 < 3; t2++){
			threeCell[t1][t2] = data[ci + t1][cj + t2];
		}
	}
}
void constructFiveCell(int ti, int tj, int **data, int ** fiveCell){
	int t1, t2, ci, cj;
	ci = ti - 2;
	cj = tj - 2;
	for (t1 = 0; t1 < 5; t1++){
		for (t2 = 0; t2 < 5; t2++){
			fiveCell[t1][t2] = data[ci + t1][cj + t2];
		}
	}
}
int countComponent(int ** fimg){
	int t1,i, j, n1, n2, n3, n4, n6, n7, n8, n9,count=1,label = 1;
	//path 1
	for (i = 1; i <= 5; i++){
		for (j = 1; j <= 5; j++){
			if (fimg[i][j] == 1){
				n1 = fimg[i - 1][j - 1];
				n2 = fimg[i - 1][j];
				n3 = fimg[i - 1][j + 1];
				n4 = fimg[i][j - 1];
				if (minPos(n1, n2, n3, n4) == -1){
					fimg[i][j] = label;
					label++;
				}
				else{
					fimg[i][j] = minPos(n1, n2, n3, n4);
				}
			}
		}
	}

	int * eq=new int[label];
	for(t1=1;t1<label;t1++)
		eq[t1]=t1;
	int pre;
	//path 2
	for (i = 5; i >= 1; i--){
		for (j = 5; j >= 1; j--){
			if (fimg[i][j] > 0){
				n6 = fimg[i][j + 1];
				n7 = fimg[i + 1][j - 1];
				n8 = fimg[i + 1][j];
				n9 = fimg[i + 1][j + 1];
				pre=fimg[i][j];
				fimg[i][j] = minPos(fimg[i][j], n6, n7, n8, n9);
				eq[pre]=fimg[i][j];
			}
		}
	}
	for(i=1;i<=5;i++){
		for(j=1;j<=5;j++){
			if (fimg[i][j] > 0){
				fimg[i][j]=1;
			}
		}
	}
	for(t1=1;t1<label;t1++){
		if(eq[t1]!=1)
			count++;
	}
	return count;
}
bool componentTest(int ti, int tj, int** data){
	int t1;
	int ** fivefiveimg = new int *[5];
	int ** framedimg = new int*[7];
	for (t1 = 0; t1 < 5; t1++)
		fivefiveimg[t1] = new int[5];
	for (t1 = 0; t1 < 7; t1++)
		framedimg[t1] = new int[7];

	constructFiveCell(ti, tj, data, fivefiveimg);
	frame0(5, 5, fivefiveimg, framedimg);

	int c1,c2;
	c1=countComponent(framedimg);
	framedimg[3][3] = 0;
	c2=countComponent(framedimg);

	if(c1==c2)
		return true;
	else
		return false;
}
bool cornerTest(int ti, int tj, int** data, int dic){
	int t1;
	int ** threethreeimg = new int *[3];
	for (t1 = 0; t1 < 3; t1++)
		threethreeimg[t1] = new int[3];
	constructThreeCell(ti, tj, data, threethreeimg);
	//if (dic == 0){
	if (threethreeimg[0][0] == 0 && threethreeimg[0][1] == 0 &&
		threethreeimg[0][2] == 0 && threethreeimg[1][0] == 0 &&
		(threethreeimg[1][2] == 1 || threethreeimg[2][0] == 1 ||
		threethreeimg[2][1] == 1 || threethreeimg[2][2] == 1))
		return false;
	if (threethreeimg[0][0] == 0 && threethreeimg[0][1] == 0 &&
		threethreeimg[0][2] == 0 && threethreeimg[1][2] == 0 &&
		(threethreeimg[1][0] == 1 || threethreeimg[2][0] == 1 ||
		threethreeimg[2][1] == 1 || threethreeimg[2][2] == 1))
		return false;
	//}
	//else if (dic == 1){
	if (threethreeimg[1][0] == 0 && threethreeimg[2][0] == 0 &&
		threethreeimg[2][1] == 0 && threethreeimg[2][2] == 0 &&
		(threethreeimg[0][0] == 1 || threethreeimg[0][1] == 1 ||
		threethreeimg[0][2] == 1 || threethreeimg[1][2] == 1))
		return false;
	if (threethreeimg[1][2] == 0 && threethreeimg[2][0] == 0 &&
		threethreeimg[2][1] == 0 && threethreeimg[2][2] == 0 &&
		(threethreeimg[0][0] == 1 || threethreeimg[0][1] == 1 ||
		threethreeimg[0][2] == 1 || threethreeimg[1][0] == 1))
		return false;
	//}
	//else if (dic == 2){
	if (threethreeimg[0][0] == 0 && threethreeimg[1][0] == 0 &&
		threethreeimg[2][0] == 0 && threethreeimg[0][1] == 0 &&
		(threethreeimg[0][2] == 1 || threethreeimg[1][2] == 1 ||
		threethreeimg[2][2] == 1 || threethreeimg[2][1] == 1))
		return false;
	if (threethreeimg[0][0] == 0 && threethreeimg[1][0] == 0 &&
		threethreeimg[2][0] == 0 && threethreeimg[2][1] == 0 &&
		(threethreeimg[0][2] == 1 || threethreeimg[1][2] == 1 ||
		threethreeimg[2][2] == 1 || threethreeimg[0][1] == 1))
		return false;
	//}
	//else if (dic == 3){
	if (threethreeimg[0][2] == 0 && threethreeimg[1][2] == 0 &&
		threethreeimg[2][2] == 0 && threethreeimg[0][1] == 0 &&
		(threethreeimg[0][0] == 1 || threethreeimg[1][0] == 1 ||
		threethreeimg[2][0] == 1 || threethreeimg[2][1] == 1))
		return false;
	if (threethreeimg[0][2] == 0 && threethreeimg[1][2] == 0 &&
		threethreeimg[2][2] == 0 && threethreeimg[2][1] == 0 &&
		(threethreeimg[0][0] == 1 || threethreeimg[1][0] == 1 ||
		threethreeimg[2][0] == 1 || threethreeimg[0][1] == 1))
		return false;
	//}
	return true;
}
bool testThin(int i, int j, int ** oridata, int dic, int time){
	if (!componentTest(i, j, oridata))
		return false;
	if (time == 0){
		if (!cornerTest(i, j, oridata, dic))
			return false;
	}
	return true;
}

class Point
{
public:
	Point();
	Point(int px, int py, int pvalue);
	Point(int px, int py);
	~Point();
	void setXY(int px, int py);
	int getX();
	int getY();
private:
	int x;
	int y;
	int value;
};
void Point::setXY(int px, int py){
	x = px;
	y = py;
}
int Point::getX(){
	return x;
}
int Point::getY(){
	return y;
}
Point::Point(){
}
Point::Point(int px, int py, int pvalue){
	x = px;
	y = py;
	value = pvalue;
}
Point::Point(int px, int py){
	x = px;
	y = py;
}
Point::~Point()
{
}
Point neibh(int i, int j, int dirn){
	switch (dirn)
	{
	case 0:
		return Point(i, j + 1);
		break;
	case 1:
		return Point(i - 1, j + 1);
		break;
	case 2:
		return Point(i - 1, j);
		break;
	case 3:
		return Point(i - 1, j - 1);
		break;
	case 4:
		return Point(i, j - 1);
		break;
	case 5:
		return Point(i + 1, j - 1);
		break;
	case 6:
		return Point(i + 1, j);
		break;
	case 7:
		return Point(i + 1, j + 1);
		break;
	default:
		return Point(-1, -1);
		break;
	}
}

bool checkPoints(Point pa, Point pb){
	if (pa.getX() == pb.getX() && pa.getY() == pb.getY())
		return true;
	else
		return false;
}

int ** chainCode(int **cimg, int h1,int h2,int h3,int h4){

	int ** dirimg;
	int ** framedimg;

	int t1, t2,t3=0;

	framedimg = new int*[h1 + 2];
	for (t1 = 0; t1 < h1 + 2; t1++){
		framedimg[t1] = new int[h2 + 2];
	}
	dirimg = new int *[h1];
	for (t1 = 0; t1 < h1; t1++){
		dirimg[t1] = new int[h2];
	}
	for (t1 = 0; t1 < h1; t1++){
		for (t2 = 0; t2 < h2; t2++){
			dirimg[t1][t2] = -1;
		}
	}
	frame0(h1, h2, cimg, framedimg);

	//initial DIR table;
	int * DIRT = new int[8];
	for (t1 = 0; t1 < 8; t1++){
		DIRT[t1] = (t1 - 1) / 2 * 2;
	}
	DIRT[0] = 6;

	int i, j, sDIR, oriSD, pqDIR, chainnum = 0;
	Point Pstart, P, Q;

	//look for first none-zero pixel
	for (i = 1; i <= h1; i++){
		for (j = 1; j <= h2; j++){
			if (framedimg[i][j] > 0){
				Pstart = Point(i, j, framedimg[i][j]);
				t1 = Pstart.getX() - 1;
				t2 = Pstart.getY() - 1;
				i = h1 + 10;
				break;
			}
		}
	}

	P = Point(Pstart.getX(), Pstart.getY());
	sDIR = 4;

	bool goback = false;
	//get direction
	while (!goback){
		oriSD = sDIR;
		sDIR = (sDIR + 1) % 8;
		while (sDIR != oriSD){
			t1 = neibh(P.getX(), P.getY(), sDIR).getX();
			t2 = neibh(P.getX(), P.getY(), sDIR).getY();
			if (framedimg[t1][t2] > 0){
				pqDIR = sDIR;
				dirimg[P.getX() - 1][P.getY() - 1] = pqDIR;
				sDIR = DIRT[(pqDIR + 7) % 8];
				P.setXY(t1, t2);
				goback = checkPoints(P, Pstart);
				chainnum++;
				break;
			}
			else
				sDIR = (sDIR + 1) % 8;
		}
	}

	prettyPrint("chaincode.txt", dirimg, h1, h2);

	int ** chaincode=new int * [chainnum+1];
	for(t1=0;t1<chainnum+1;t1++)
		chaincode[t1]=new int[2];
	for(t1=0;t1<h1;t1++){
		for(t2=0;t2<h2;t2++){
			if(dirimg[t1][t2]>=0){
				chaincode[t3][0]=t1;
				chaincode[t3][1]=t2;
				t3++;
			}
		}
	}
	chaincode[t3][0]=-1;
	return chaincode;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int ** bimg;
	int h1, h2, h3, h4, label = 1, t1, t2, td,north, south, west, east, time = 0;
	bool change=true;
	readEchoImage("thin_data10.txt", "echo1.txt", bimg, h1, h2, h3, h4);
	int * tmpX=new int[h1];
	int * tmpY=new int[h1];
	int tmpNum=0;
	north = 0, south = h1 - 1, west = 0, east = h2 - 1;

	int ** candi;
	while(change){
		change=false;
		//north
		candi=chainCode(bimg,h1,h2,h3,h4);
		td=0;
		while(candi[td][0]!=-1){
			t2=candi[td][0];
			t1=candi[td][1];
			if((bimg[t2][t1]==1 && t2==0) || (bimg[t2][t1]==1 && bimg[t2-1][t1]==0)){
				if (testThin(t2, t1, bimg, 0, time)){
					tmpX[tmpNum]=t2;
					tmpY[tmpNum]=t1;
					tmpNum++;
					change=true;
				}
			}
			td++;
		}
		for(t1=0;t1<tmpNum;t1++){
			bimg[tmpX[t1]][tmpY[t1]]=0;
		}
		tmpNum=0;
		prettyPrint("output.txt", bimg, h1, h2);
		//south
		candi=chainCode(bimg,h1,h2,h3,h4);
		td=0;
		while(candi[td][0]!=-1){
			t2=candi[td][0];
			t1=candi[td][1];
			if((bimg[t2][t1]==1 && t2==h1-1)||(bimg[t2][t1]==1 && bimg[t2+1][t1]==0)){
				if(testThin(t2,t1,bimg,1,time)){
					tmpX[tmpNum]=t2;
					tmpY[tmpNum]=t1;
					tmpNum++;
					change=true;
				}
			}
			td++;
		}
		for(t1=0;t1<tmpNum;t1++){
			bimg[tmpX[t1]][tmpY[t1]]=0;
		}
		tmpNum=0;
		prettyPrint("output.txt", bimg, h1, h2);
		//west
		candi=chainCode(bimg,h1,h2,h3,h4);
		td=0;
		while(candi[td][0]!=-1){
			t1=candi[td][0];
			t2=candi[td][1];
			if((bimg[t1][t2]==1 && t2==0)||(bimg[t1][t2]==1 && bimg[t1][t2-1]==0)){
				if(testThin(t1,t2,bimg,2,time)){
					tmpX[tmpNum]=t1;
					tmpY[tmpNum]=t2;
					tmpNum++;
					change=true;
				}
			}
			td++;
		}
		for(t1=0;t1<tmpNum;t1++){
			bimg[tmpX[t1]][tmpY[t1]]=0;
		}
		tmpNum=0;
		prettyPrint("output.txt", bimg, h1, h2);
		//east
		candi=chainCode(bimg,h1,h2,h3,h4);
		td=0;
		while(candi[td][0]!=-1){
			t1=candi[td][0];
			t2=candi[td][1];
			if((bimg[t1][t2]==1 && t2==h2-1)||(bimg[t1][t2]==1 && bimg[t1][t2+1]==0)){
				if(testThin(t1,t2,bimg,3,time)){
					tmpX[tmpNum]=t1;
					tmpY[tmpNum]=t2;
					tmpNum++;
					change=true;
				}
			}
			td++;
		}
		for(t1=0;t1<tmpNum;t1++){
			bimg[tmpX[t1]][tmpY[t1]]=0;
		}
		tmpNum=0;
		prettyPrint("output.txt", bimg, h1, h2);
	}

	prettyPrint("output.txt", bimg, h1, h2);
	return 0;
}
