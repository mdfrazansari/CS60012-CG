#include<iostream>
#include<stdio.h>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;

class Face;
class Point;
bool comparePoint(const Point *p1, const Point *p2);
bool compareFace(const Face *f1, const Face *f2);
int sortFace(const Face *f1, const Face *f2);

class Point {
	public:
	int x, y, z, id, hashCode;
	vector<Face*> faces;
	
	Point(int x, int y, int z, int id);
	bool equalTo(Point *other);
	void print();
};

class Face {
	public:
	int id, hashCode;
	vector<Point*> vertices;
	
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
	void reindexVertices();
	void removeDuplicateFaces();
	void rebuildFacesHashCode();
	void outputVertices();
	void outputFaces();
};

int main() {
	
	Obj obj;
	obj.readObj();

	int vSize = obj.vertices.size();
	int fSize = obj.faces.size();
		
	std::cerr << "before total vertices :" << vSize << endl;
	std::cerr << "before total faces :" << fSize << endl;
	
	obj.removeDuplicateVertices();
	std::cerr << "duplicate vertices done" << endl;
	obj.removeDuplicateFaces();
	obj.reindexVertices();

	obj.outputVertices();
	obj.outputFaces();
	
	std::cerr << "after total vertices :" << obj.vertices.size() << endl;
	std::cerr << "after total faces :" << obj.faces.size() << endl;


return 0;
}

void Obj::reindexVertices()
{
	int index = 1;
	for (vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		(*it)->id = index++;
	}
}

void Obj::rebuildFacesHashCode()
{
	for (vector<Face*>::iterator fit = faces.begin(); fit != faces.end(); ++fit)
	{
		(*fit)->hashCode = (*fit)->vertices[0]->hashCode
							+ (*fit)->vertices[1]->hashCode
							+ (*fit)->vertices[2]->hashCode
							+ (*fit)->vertices[3]->hashCode;
	}	
}

void Obj::removeDuplicateVertices()
{
	vector<Point*> v2(vertices);
	sort(v2.begin(), v2.end(), comparePoint);
	vector<int> toRemove(vertices.size());

	for(int i = 0; i < v2.size() - 1; ++i)
	{
		if(v2[i]->equalTo(v2[i+1]))
		{
			Point *removePoint = v2[i];
			Point *otherPoint = v2[i+1];
			toRemove[removePoint->id] = otherPoint->id;
		}
	}
	for(vector<Face*>::iterator fit = faces.begin(); fit != faces.end(); ++fit)
    {
		for (vector<Point*>::iterator pit = (*fit)->vertices.begin(); pit != (*fit)->vertices.end(); ++pit)
		{
			if(toRemove[(*pit)->id] != 0)
			{
				int i = (*pit)->id;
				while(toRemove[i] !=  0)
				{
					i = toRemove[i];
				}
					
		//		vertices[i-1]->print();
                (*pit) = vertices[i-1];
				(*pit)->faces.push_back(*fit);
			}
		}
	}
	int index = 1;
	for(vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); )
	{
		if((*it)->faces.size() == 0 || toRemove[index] != 0)
		{
			 vertices.erase(it);
		}
		else ++it;
		index++;
	}
}

void Obj::removeDuplicateFaces()
{
	sort(faces.begin(), faces.end(), sortFace);
	set<int> toRemove;
	int index = 0;
	for (vector<Face*>::iterator fit1 = faces.begin(); fit1 != faces.end(); ++fit1)
	{
//		cerr << index++ << " : " << (*fit1)->hashCode << endl;
		for (vector<Face*>::iterator fit2 = fit1+1; fit2 != faces.end() && compareFace(*fit1, *fit2) ; ++fit2)
		{	
			if ((*fit1)->equalTo(*fit2))
			{
				for(vector<Point*>::iterator pit = (*fit2)->vertices.begin(); pit != (*fit2)->vertices.end(); ++pit)
				{
					for (vector<Face*>::iterator pfit = (*pit)->faces.begin(); pfit != (*pit)->faces.end();)
					{
						if((*pfit)->equalTo(*fit2))
						{
							(*pit)->faces.erase(pfit);
							break;
						}
						else ++pfit;
					}
				}
				toRemove.insert((*fit1)->id);
				toRemove.insert((*fit2)->id);
				break;
			}
		}
	}
	// remove duplicate faces
	for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); )
	{
		if(toRemove.find((*it)->id) != toRemove.end())
		{	
			faces.erase(it);
		}
		else ++it;
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
		//p->print();
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
	//	f->print();
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
		(*it)->print();
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
	this->hashCode = 31*x + 13*y + 37*z;
}

bool Point::equalTo(Point *other)
{
	if(this->hashCode != other->hashCode)
		return false;
	else
		return (this->x == other->x && this->y == other->y && this->z == other->z);
}

void Point::print()
{
	cout << "v "<< x << " " << y << " " << z << " "  << endl;
}

Face::Face(Point *p1, Point *p2, Point *p3, Point *p4, int id)
{
	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	this->id = id;
	this->hashCode = p1->hashCode+p2->hashCode+p3->hashCode+p4->hashCode;
}

bool Face::equalTo(Face *other)
{
	if(this->vertices[0]->equalTo(other->vertices[0])
		|| this->vertices[1]->equalTo(other->vertices[0])
		|| this->vertices[2]->equalTo(other->vertices[0])
		|| this->vertices[3]->equalTo(other->vertices[0])
		)
	{
	set<Point*> v1(this->vertices.begin(), this->vertices.end());	
	set<Point*> v2(other->vertices.begin(), other->vertices.end());	
	return v1 == v2;
	}
	else
		return false;
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

bool compareFace(const Face *f1, const Face *f2)
{
	return f1->hashCode == f2->hashCode;
}
int sortFace(const Face *f1, const Face *f2)
{
	return f2->hashCode > f1->hashCode;
}
bool comparePoint(const Point *p1, const Point *p2)
{
		if (p1->hashCode < p2->hashCode)
		return true;
		else if(p1->hashCode > p2->hashCode)
		{
			return false;
		}
		
        else if (p1->x < p2->x)
        {
                return true;
        }
        else if(p1->x == p2->x)
        {
                if (p1->y < p2->y)
                {
                        return true;
                }
        }
        return false;
}
