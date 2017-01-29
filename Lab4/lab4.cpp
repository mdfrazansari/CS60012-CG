#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>


class Point
{
	public:
	int x, y, z, index;
	Point(int x, int y, int z, int index)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->index = index;
	}

	
	bool isEqual(const Point p2)
	{
		if (x == p2.x && y == p2.y && z == p2.z)
		{
			return true;
		}
		return false;
	}

	void print()
	{
		std::cout << x << " " << y << " " << z << " " << index;
	}
};


class Face
{
	public:
	int f1, f2, f3, f4, index;
	Face(int f1, int f2, int f3, int f4, int index)
	{
		this->f1 = f1;
		this->f2 = f2;
		this->f3 = f3;
		this->f4 = f4;
		this->index = index;
	}

	void print()
	{
		std::cout << f1 << " " << f2 << " " << f3 << " " << f4 << " " << index;
	}
};

bool isPointEqual(const Point p1, const Point p2)
{
	if ( p1.x == p2.x && p1.y == p2.y && p1.z == p2.z)
	{
		return true;
	}
	return false;
}

bool isFaceEqual(const Face face1, const Face face2)
{
	if ( face1.f1 == face2.f1 && face1.f2 == face2.f2 && face1.f3 == face2.f3 && face1.f4 == face2.f4)
	{
		return true;
	}
	return false;
}

bool compareX(const Point p1, const Point p2)
{
	return (p1.x < p2.x);
}
bool compareY(const Point p1, const Point p2)
{
	return (p1.y < p2.y);
}
bool compareZ(const Point p1, const Point p2)
{
	return (-p1.z < -p2.z);
}

bool comparePoint(const Point p1, const Point p2)
{
	if (p1.x < p2.x)
	{
		return true;
	}
	else if(p1.x == p2.x)
	{
		if (p1.y < p2.y)
		{
			return true;
		}
		else if(p1.y == p2.y)
		{
			if (p1.z > p2.z)
				return true;
		}
		else return false;
	}
	return false;
}

int main(int argc, char* argv[])
{


	int x, y, z;
	int f1, f2, f3, f4;
	std::vector<Point> vertices;	
	std::vector<Face> faces;	
	Point *prevPoint = new Point(-1,-1, -1, -1);
	int index = 1;
	while(true)
	{
		scanf("v %d %d %d\n", &x, &y, &z);
		Point *p = new Point(x, y, z, index++);
		if (isPointEqual(*p, *prevPoint))
		{
			break;
		}
		vertices.push_back(*p);
		prevPoint = p;
	}


	Face *prevFace = new Face(-1, -1, -1, -1, -1);
	index = 1;
	while(true)
	{
		scanf("f %d %d %d %d\n", &f1, &f2, &f3, &f4);
		Face *f = new Face(f1, f2, f3, f4, index++);
		if (isFaceEqual(*f, *prevFace))
		{
			break;
		}
		faces.push_back(*f);
		prevFace = f;
	}



	///   

	std::stable_sort(vertices.begin(), vertices.end(), comparePoint);
	std::vector<std::pair<int, int> > r;
	std::pair<int, int> pair;
	Point pp = vertices[0];
		
	for (int i = 1; i < vertices.size(); i++)
	{
		if(!isPointEqual(pp, vertices[i]))
		{
			pp = vertices[i];		
		}
		
		r.push_back(*(new std::pair<int, int>(vertices[i].index, pp.index)));
	}
	for (int i = 0; i < r.size(); i++)
	{
		if(r[i].first == r[i].second)
		{
			
		}
	}


	return 0;
}
