#include<iostream>
#include<stdio.h>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;

class Face;
class Point;
bool comparePoint(const Point *p1, const Point *p2);

class Point {
	public:
	 int x, y, z, id;
	vector<Face*> faces;
	
	Point(int x, int y, int z, int id);
	bool equalTo(Point *other);
	void print();
};

class Face {
	public:
	int id;
	vector<Point*> vertices;
	Face(){}	// constructor
	Face(Point*, Point*, Point*, Point*, int);	// constructor
	bool equalTo(Face *other);
	void print();
};

class Obj {
	public:
	vector<Point*> vertices;
	vector<Face*> faces;

	void readObj();
	Point* getPointByIndex(int);
	void removeDuplicateVertices();
	void removeDuplicateFaces();
	void outputVertices();
	void outputFaces();
};

int main() {
	
	Obj obj;
	obj.readObj();

	set<Point*> sp;
	set<Point*> sp2;

	sp.insert(obj.vertices[1]);
	sp.insert(obj.vertices[2]);
	sp2.insert(obj.vertices[2]);
	sp2.insert(obj.vertices[1]);
		
	obj.removeDuplicateVertices();
	obj.removeDuplicateFaces();
	obj.outputVertices();
	obj.outputFaces();
//	Point * p1 = obj.vertices[1];
//	Face * f1 = obj.faces[6];
//	f1->print();
	for (vector<Point*>::iterator pit = obj.vertices.begin(); pit != obj.vertices.end(); ++pit)
	{
		cout << (*pit)-> id << " : " << (*pit)->faces.size() << endl;
	}



return 0;
}


void Obj::removeDuplicateVertices()
{
	vector<Point*> v2(vertices);	
	sort(v2.begin(), v2.end(), comparePoint);
	set<int> toRemove;

	for(int i = 0; i < v2.size() - 1; ++i)
	{
		if(v2[i]->equalTo(v2[i+1]))
		{
			toRemove.insert(v2[i+1]->id);
			// update reference from faces
			for(vector<Face*>::iterator fit = v2[i+1]->faces.begin(); fit != v2[i+1]->faces.end(); ++fit)
			{
				for (vector<Point*>::iterator pit = (*fit)->vertices.begin(); pit != (*fit)->vertices.end(); ++pit)
				{
					if((*pit)->id == v2[i+1]->id)
					{
						(*pit) = v2[i];
						(*pit)->faces.push_back(*fit);
						break;
					}
				}	
			}
		}
	}
	// remove duplicate vertices
	for(vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); )
	{
		if(toRemove.find((*it)->id) != toRemove.end())
		{	
			vertices.erase(it);
		}
		else ++it;
	}	
}

void Obj::removeDuplicateFaces()
{
	for (vector<Face*>::iterator fit1 = faces.begin(); fit1 != faces.end(); ++fit1)
	{
		for (vector<Face*>::iterator fit2 = faces.begin(); fit2 != faces.end();)
		{
			if (*fit1 != *fit2 && (*fit1)->equalTo(*fit2))
			{
				for(vector<Point*>::iterator pit = (*fit2)->vertices.begin(); pit != (*fit2)->vertices.end(); ++pit)
				{
					for (vector<Face*>::iterator pfit = (*pit)->faces.begin(); pfit != (*pit)->faces.end(); ++pfit)
					{
						if((*pfit)->equalTo(*fit2))
						{
							(*pit)->faces.erase(pfit);
						}
					}
				}
				faces.erase(fit2);
				faces.erase(fit1);
				break;
			}
			else ++fit2;
		}
	}	
}

void Obj::readObj()
{
	int x, y, z, id;
	// read points
	Point *prevPoint = new Point(-1, -1, -1, -1);
	id = 1;
	while(true)
	{
		scanf("v %d %d %d ", &x, &y, &z);
		Point *p = new Point(x, y, z, id++);
		if (prevPoint->equalTo(p))
			break;
		else
		{
			vertices.push_back(p);
			prevPoint = p;
		}
	}

	// read Faces
	int p1, p2, p3, p4;
	Face *prevFace = new Face(getPointByIndex(1), getPointByIndex(1), getPointByIndex(1), getPointByIndex(1), -1);
	Point *fp1, *fp2, *fp3, *fp4;
	id = 1;
	while(true)
	{
		scanf("f %d %d %d %d\n", &p1, &p2, &p3, &p4);
		fp1 = getPointByIndex(p1);
		fp2 = getPointByIndex(p2);
		fp3 = getPointByIndex(p3);
		fp4 = getPointByIndex(p4);
		Face *f = new Face(fp1, fp2, fp3, fp4, id++);
		if (prevFace->equalTo(f))
			break;
		else
		{
			faces.push_back(f);
			fp1->faces.push_back(f);
			fp2->faces.push_back(f);
			fp3->faces.push_back(f);
			fp4->faces.push_back(f);
			prevFace = f;
		}	
	}
}


void Obj::outputVertices()
{
	for(vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		Point *p = *it;
		cout << "v " << p->x << " " << p->y << " " << p->z << endl;
	}	
}
void Obj::outputFaces()
{
	for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		Face *f = *it;
		f->print();
	}	
}

Point* Obj::getPointByIndex(int index)
{
	if (vertices[index-1]->id == index)
	{
		return vertices[index - 1];	
	}
	cout << "ERROR" << endl;
	return NULL;

}

Point::Point(int x, int y, int z, int id)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->id = id;
}

bool Point::equalTo(Point *other)
{
	return (this->x == other->x && this->y == other->y && this->z == other->z);
}

void Point::print()
{
	cout << x << " " << y << " " << z << " " << id << endl;
}

Face::Face(Point *p1, Point *p2, Point *p3, Point *p4, int id)
{
	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	this->id = id;
}

bool Face::equalTo(Face *other)
{
	set<Point*> v1(this->vertices.begin(), this->vertices.end());	
	set<Point*> v2(other->vertices.begin(), other->vertices.end());	

	return v1 == v2;
}


void Face::print()
{
	cout << "f";
	for(vector<Point*>::iterator vit = vertices.begin(); vit != vertices.end(); ++vit)
		{
			Point *p = *vit;
			cout << " " << p->id;	
		}
		cout << endl;

}

bool comparePoint(const Point *p1, const Point *p2)
{
        if (p1->x < p2->x)
        {
                return true;
        }
        else if(p1->x == p2->x)
        {
                if (p1->y < p2->y)
                {
                        return true;
                }
                else if(p1->y == p2->y)
                {
                        if (p1->z > p2->z)
                                return true;
                }
                else return false;
        }
        return false;
}
