#include<stdio.h>
#include<math.h>

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

void createSVG(int arr[][3], int N)
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

	// output svg file
	FILE *fp = fopen("output.svg", "w+");
	fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n", width, height);
	fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"black\" />\n", 0, 0, width, height);
	
	int i;
	for (i = 0; i < N; i++)
	{
		double opacity = (arr[i][2]-zMin)*1.0/(zMax-zMin);
		fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" stroke=\"white\" stroke-width=\"1\" fill=\"yellow\" opacity=\"%f\" />\n", arr[i][0] - scaleXY/2, arr[i][1] - scaleXY/2, scaleXY, scaleXY, 1-opacity);
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
		arr[i][1] = y;
		arr[i][2] = z;

	}
	fclose(fp);
	
	createSVG(arr, N);

return 0;
}
