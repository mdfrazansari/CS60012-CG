#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double* rgbToHSV(int color[3])
{
	double R = color[0]/255.0;
	double G = color[1]/255.0;
	double B = color[2]/255.0;
	double Cmax = fmax(R, G);
	Cmax = fmax(Cmax, B);

	double Cmin = fmin(R, G);
	Cmin = fmin(Cmin, B);

	double delta = Cmax - Cmin;
	double H, S, V;

	if(delta == 0)
	{
		H = 0;
	} 
	else if(Cmax == R)
	{
		H = 60*((int)((G-B)/delta)%6);
	}	
	else if(Cmax == G)
	{
		H = 60*((int)((B-R)/delta) + 2);
	}	
	else
	{
		H = 60*((int)((R-G)/delta)+4);
	}


	if(Cmax == 0)
	{
		S = 0;
	}	
	else
	{
		S = delta/Cmax;
	}


	V = Cmax;
	double *retVal = (double*)malloc(sizeof(double)*3);
	retVal[0] = H;
	retVal[1] = S;
	retVal[2] = V;

	return retVal;

}

int* hsvToRGB(double HSV[])
{
	double H = HSV[0];
	double S = HSV[1];
	double V = HSV[2];
	
	double C = V*S;

	int Hd = H/60;
	double X = C*(1 - abs((Hd%2) - 1));
	double m = V - C;
	float Rd, Gd, Bd;


	if(Hd < 1)
	{
		Rd = C;
		Gd = X;
		Bd = 0;
	}
	else if(Hd < 2)
	{
		Rd = X;
		Gd = C;
		Bd = 0;
	}
	else if(Hd < 3)
	{
		Rd = 0;
		Gd = C;
		Bd = X;
	}
	else if(Hd < 4)
	{
		Rd = 0;
		Gd = X;
		Bd = C;
	}
	else if(Hd < 5)
	{
		Rd = X;
		Gd = 0;
		Bd = C;
	}
	else
	{
		Rd = C;
		Gd = 0;
		Bd = X;
	}


	int R, G, B;
	
	R = (Rd+m)*255;
	G = (Gd+m)*255;
	B = (Bd+m)*255;
	int *retVal = (int*)malloc(sizeof(int)*3);
	retVal[0] = R;
	retVal[1] = G;
	retVal[2] = B;

	return retVal;

}

void printArray(int arr[][3], int N)
{
	int i;
	for (i = 0; i < N; i++)
	{
		printf("%d %d %d\n", arr[i][0], arr[i][1], arr[i][2]);
	}


}

void swap(int arr[][3], int i, int j)
{
	int x = arr[i][0];
	int y = arr[i][1];
	int z = arr[i][2];

	arr[i][0] = arr[j][0];
	arr[i][1] = arr[j][1];
	arr[i][2] = arr[j][2];

	arr[j][0] = x;
	arr[j][1] = y;
	arr[j][2] = z;
}


void sortByZ(int arr[][3], int N)
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (arr[i][2] < arr[j][2])
			{
				swap(arr, i, j);
			}
		}
	}

}

int findMinAt(int arr[][3], int index, int N)
{
	int i, min;
	min = arr[0][index];
	for (i = 1; i < N; i++)
	{
		if (arr[i][index] < min)
		{
			min = arr[i][index];
		}		

	}

return min;

}


int findMaxAt(int arr[][3], int index, int N)
{
	int i, max;
	max = arr[0][index];
	for (i = 1; i < N; i++)
	{
		if (arr[i][index] > max)
		{
			max = arr[i][index];
		}		

	}
return max;
}

void translate(int arr[][3], int N)
{
	int xMin = findMinAt(arr, 0, N);
	int yMin = findMinAt(arr, 1, N);
	int zMax = findMaxAt(arr, 2, N);
	if (xMin-- >= 0)
		xMin = 0;
	if (yMin-- >= 0)
		yMin = 0;
	if (zMax++ <= 0)
		zMax = 0;
		
	int i;
	for (i = 0; i < N; i++)
	{	
		arr[i][0] -= xMin;
		arr[i][1] -= yMin;
		arr[i][2] -= zMax;
	}

}

int scale(int arr[][3], int N)
{
	int i, j;
	
	int xMin = findMinAt(arr, 0, N);
	int xMax = findMaxAt(arr, 0, N);

	int yMin = findMinAt(arr, 1, N);
	int yMax = findMaxAt(arr, 1, N);

	int scaleX = 500/abs(xMax-xMin);			// 500 is maximum canvas size
	int scaleY = 500/abs(yMax-yMin);

	int scaleXY = scaleX;
	if(scaleY < scaleXY)
		scaleXY = scaleY;
	scaleXY--;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < 3; j++)
		{
			arr[i][j] *= scaleXY; 
		}

	}
return scaleXY;

}

void createSVG(int arr[][3], double* HSV, int N)
{
	translate(arr, N);						// translate x, y to svg co-ordinate
	sortByZ(arr, N);						// sort points by z
	int scaleXY = scale(arr, N);			// scale x, y according to canvas size
	
	int xMin = findMinAt(arr, 0, N);
	int xMax = findMaxAt(arr, 0, N);

	int yMin = findMinAt(arr, 1, N);
	int yMax = findMaxAt(arr, 1, N);
	
	int zMin = findMinAt(arr, 2, N);
	int zMax = findMaxAt(arr, 2, N);

	int width = xMax - xMin + 2*scaleXY;
	int height = yMax - yMin + 2*scaleXY;

	int R = 255;
	int G = 255;
	int B = 0;

	double H, S, V;
	H = HSV[0];
	S = HSV[1];
	V = HSV[2];
	double Vd = 1.0-V;
	

	// output svg file
	FILE *fp = fopen("output.svg", "w+");
	fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n", width, height);
	fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"black\" />\n", 0, 0, width, height);
	
	int i;
	for (i = 0; i < N; i++)
	{
		double transparency = (arr[i][2]-zMin)*1.0/(zMax-zMin);
		double opacity =  1 - transparency;
		double cV = V + opacity*Vd; // scale V according to depth
		double color[3] = {H, S, cV};
		int *rgb = hsvToRGB(color);
		
		fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"rgb(%d,%d,%d)\" opacity=\"%f\" />\n", arr[i][0] - scaleXY/2, arr[i][1] - scaleXY/2, scaleXY, scaleXY, rgb[0], rgb[1], rgb[2], opacity);
	}
	
	fprintf(fp, "</svg>");
	fclose(fp);
}

int main()
{
	FILE *fp = fopen("input.txt", "r+");
	
	int i, N;
	int x, y, z;	
	fscanf(fp, "%d\n", &N);
	int arr[N][3];
	// load points from file
	for (i = 0; i < N; i++)
	{
		fscanf(fp, "%d %d %d\n", &x, &y, &z);
		arr[i][0] = x;
		arr[i][1] = -y;
		arr[i][2] = z;

	}
	fclose(fp);
	int R, G, B;
	printf("Enter RGB in hex : ");
	scanf("%x %x %x", &R, &G, &B);
	int color[3] = {R, G, B};
	double* HSV = rgbToHSV(color);
	//printf("%f %f %f", HSV[0], HSV[1], HSV[2]);
	
	createSVG(arr, HSV, N);

return 0;
}
