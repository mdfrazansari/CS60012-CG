#include<iostream>
#include<vector>

// generates unit cube given bottom right vertex


using namespace std;


class Cube
{
	public:
	int x, y, z;
	int id;
	Cube(int, int, int, int);
	void printVertices();
	void printFaces();


};

int main()
{
	int N; // No. of cubes
	int x, y, z; // vertices coordinate
	vector<Cube *> cubes;
	cin >> N;
	for (int i = 0; i < N; i++)
	{
		cin >> x >> y >> z;
		Cube *cube = new Cube(x, y, z, i);
		cubes.push_back(cube);
	}

	for(int i =0; i < N; i++)
	{
		cubes[i]->printVertices();	
	}
	for(int i =0; i < N; i++)
	{
		cubes[i]->printFaces();	
	}



return 0;
}


Cube::Cube(int x, int y, int z, int id)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->id = id;
}

void Cube::printVertices()
{
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				cout << x+k << "  "  << y+j << " " << z-i << endl;
			}
}

void Cube::printFaces()
{
	int offset = id*6;
	cout << "f " << offset + 1 << "  " << offset + 2 << "  " << offset + 4 << "  " << offset + 3 << endl;
	cout << "f " << offset + 2 << "  " << offset + 6 << "  " << offset + 8 << "  " << offset + 4 << endl;
	cout << "f " << offset + 5 << "  " << offset + 7 << "  " << offset + 8 << "  " << offset + 6 << endl;
	cout << "f " << offset + 1 << "  " << offset + 3 << "  " << offset + 7 << "  " << offset + 5 << endl;
	cout << "f " << offset + 3 << "  " << offset + 4 << "  " << offset + 8 << "  " << offset + 7 << endl;
	cout << "f " << offset + 1 << "  " << offset + 5 << "  " << offset + 6 << "  " << offset + 2 << endl;

}
