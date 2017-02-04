#include<iostream>
#include<fstream>

int* getVertex(double a[], int vid)
{
	int x, y, z;
	switch(vid)
	{
		case 1:
			x = a[0] - 0.5; y = a[1] - 0.5; z = a[2] + 0.5; 
			break;
		case 2:
			x = a[0] + 0.5; y = a[1] - 0.5; z = a[2] + 0.5; 
			break;
		case 3:
			x = a[0] + 0.5; y = a[1] + 0.5; z = a[2] + 0.5; 
			break;
		case 4:
			x = a[0] - 0.5; y = a[1] + 0.5; z = a[2] + 0.5; 
			break;
		case 5:
			x = a[0] + 0.5; y = a[1] - 0.5; z = a[2] -0.5; 
			break;
		case 6:
			x = a[0] + 0.5; y = a[1] + 0.5; z = a[2] -0.5; 
			break;
		case 7:
			x = a[0] - 0.5; y = a[1] + 0.5; z = a[2] -0.5; 
			break;
		case 8:
			x = a[0] - 0.5; y = a[1] - 0.5; z = a[2] -0.5; 
			break;

	}
	int *vertex = new int[3];
	vertex[0] = x;
	vertex[1] = y;
	vertex[2] = z;
return vertex;

}


int* getFace(int fid, int shift)
{
	int f1, f2, f3, f4;
	switch(fid)
	{
		case 1:
			f1 = 1; f2 = 2; f3 = 3; f4 = 4;
			break; 
		case 2:
			f1 = 8; f2 = 7; f3 = 6; f4 = 5;
			break; 
		case 3:
			f1 = 2; f2 = 5; f3 = 6; f4 = 3;
			break; 
		case 4:
			f1 = 1; f2 = 4; f3 = 7; f4 = 8;
			break; 
		case 5:
			f1 = 3; f2 = 6; f3 = 7; f4 = 4;
			break; 
		case 6:
			f1 = 1; f2 = 8; f3 = 5; f4 = 2;
			break; 

	}

	int *face = new int[3];
	face[0] = f1 + shift;
	face[1] = f2 + shift;
	face[2] = f3 + shift;
	face[3] = f4 + shift;
return face;
}


int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cout << "use: " << argv[0] << " inputfile outputfile" << std::endl;
	return -1;
	}
	
	std::ifstream infile;
	infile.open(argv[1]);
	
	std::ofstream outfile;
	outfile.open(argv[2]);

	int N;

	infile >> N;
	int coord[N][3];
	for(int i = 0; i < N; i++)
	{
		infile >> coord[i][0] >> coord[i][1] >> coord[i][2];
	}

	
	double *center = new double[3];

//	outfile << "mtllib color.mtl" << std::endl;

	for (int i = 0; i < N; i++)   // vertex
	{
		center[0] = coord[i][0] + 0.5;
		center[1] = coord[i][1] + 0.5;
		center[2] = coord[i][2] - 0.5;

		for (int j = 1; j <=8; j++)
		{
			int *v = getVertex(center, j);
			outfile <<"v " <<  v[0] << " " << v[1] << " " << v[2] << std::endl;
		}
	}

//	outfile << "usemtl xy" << std::endl;
	for (int i = 0; i < N; i++)
	{
		for (int k = 1; k <= 2; k++)  // xy
		{
			int *face = getFace(k, i*8);
			outfile << "f " << face[0] << " " << face[1] << " " << face[2] << " " << face[3] << std::endl; 
		}
	}

//	outfile << "usemtl yz" << std::endl;
	for (int i = 0; i < N; i++)
	{
		for (int k = 3; k <= 4; k++) // xz
		{
			int *face = getFace(k, i*8);
			outfile << "f " << face[0] << " " << face[1] << " " << face[2] << " " << face[3] << std::endl; 
		}
	}
	
//	outfile << "usemtl xz" << std::endl;
	for (int i = 0; i < N; i++)
	{
		for (int k = 5; k <= 6; k++) // yz
		{
			int *face = getFace(k, i*8);
			outfile << "f " << face[0] << " " << face[1] << " " << face[2] << " " << face[3] << std::endl; 
		}
	}


	infile.close();
	outfile.close();
	return 0;
}
