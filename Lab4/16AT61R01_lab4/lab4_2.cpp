#include<iostream>
#include<stdio.h>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>

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
    vector<int>* direction();
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
	void removeDuplicateFaces2();
	void backfaceculling();
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
	//obj.removeDuplicateVertices();
	//obj.removeDuplicateFaces();

	//obj.reindexVertices();
	
	obj.backfaceculling();
	obj.outputVertices();
	obj.outputFaces();
	
	std::cerr << "after total vertices :" << obj.vertices.size() << endl;
	std::cerr << "after total faces :" << obj.faces.size() << endl;


return 0;
}

vector<int>* Face::direction()
{
	Point* p1 = vertices[0];
	Point* p2 = vertices[1];
	vector<int> *dir = new vector<int>;
	if(p2->x > p1->x)
	{		
		dir->push_back(0);
		dir->push_back(0);
		dir->push_back(-1);
	} 
	else if(p2->x < p1->x)
	{		
		dir->push_back(0);
		dir->push_back(0);
		dir->push_back(1);
	}else if(p2->y > p1->y)
	{		
		dir->push_back(1);
		dir->push_back(0);
		dir->push_back(0);
	} 
	else if(p2->y < p1->y)
	{		
		dir->push_back(-1);
		dir->push_back(0);
		dir->push_back(0);
	}if(p2->z > p1->z)
	{		
		dir->push_back(0);
		dir->push_back(1);
		dir->push_back(0);
	} 
	else if(p2->z < p1->z)
	{		
		dir->push_back(0);
		dir->push_back(-1);
		dir->push_back(0);
	}
	return dir;
}

void Obj::backfaceculling()
{
	int x, y, z;
	x = 1;
	y = 2;
	z = 0;
	double denum = sqrt(x*x + y*y + z*z);
	double dcx, dcy, dcz;
	dcx = x/denum;
	dcy = y/denum;
	dcz = z/denum;
	// std::cout << dcx << " " << dcy << "  " << dcz << endl;
	
	for (vector<Face*>::iterator it = faces.begin(); it != faces.end();)
	{
		vector<int> * dir = (*it)->direction();
		int x1 = (*dir)[0];
		int y1 = (*dir)[1];
		int z1 = (*dir)[2];
		double denum1 = sqrt(x1*x1 + y1*y1 + z1*z1);
		double dcx1, dcy1, dcz1;
		dcx1 = x1/denum1;
		dcy1 = y1/denum1;
		dcz1 = z1/denum1;

		double dot = dcx*dcx1 + dcy*dcy1 + dcz*dcz1;
	
		std::cerr << dot << endl;
		if(dot < 0)
		{
			faces.erase(it);
		}
		else ++it;
	}	

}


void Obj::reindexVertices()
{
	int index = 1;
	for (vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		(*it)->id = index++;
	}
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
			Point *removePoint = v2[i];
			Point *otherPoint = v2[i+1];
			toRemove.insert(removePoint->id);
			// update reference from faces
			for(vector<Face*>::iterator fit = removePoint->faces.begin(); fit != removePoint->faces.end(); ++fit)
			{
				for (vector<Point*>::iterator pit = (*fit)->vertices.begin(); pit != (*fit)->vertices.end(); ++pit)
				{
					if((*pit)->id == removePoint->id)
					{
						//cout << (*pit)->id << " -- " << v2[i+1]->id << endl;
						(*pit) = otherPoint;
						(*pit)->faces.push_back(*fit);
					}
				}	
			}
		}
	}
	// remove duplicate vertices
	for(vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); )
	{
		if((*it)->faces.size() == 0 || toRemove.find((*it)->id) != toRemove.end())
		{	
			vertices.erase(it);
		}
		else ++it;
	}
	
}

void Obj::removeDuplicateFaces()
{
	set<int> toRemove;
	int ii = 0;
	for (vector<Face*>::iterator fit1 = faces.begin(); fit1 != faces.end(); ++fit1)
	{
		cerr << ii++ << endl;
		for (vector<Face*>::iterator fit2 = fit1+1; fit2 != faces.end(); ++fit2)
		{
			if (*fit1 != *fit2 && (*fit1)->equalTo(*fit2))
			{
				for(vector<Point*>::iterator pit = (*fit2)->vertices.begin(); pit != (*fit2)->vertices.end(); ++pit)
				{
					for (vector<Face*>::iterator pfit = (*pit)->faces.begin(); pfit != (*pit)->faces.end();)
					{
						if((*pfit)->equalTo(*fit2))
						{
							(*pit)->faces.erase(pfit);
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


void Obj::removeDuplicateFaces2()
{
	set<int> toRemove;
	vector<set<int> > faceSet;
	for (vector<Face*>::iterator fit1 = faces.begin(); fit1 != faces.end(); ++fit1)
	{
		(*fit1)->print();
		set<int> s;
		s.insert((*fit1)->vertices[0]->id);
		s.insert((*fit1)->vertices[1]->id);
		s.insert((*fit1)->vertices[2]->id);
		s.insert((*fit1)->vertices[3]->id);
		faceSet.push_back(s);
	}
	int i = 0;
	for(vector<set<int> >::iterator fit = faceSet.begin(); fit != faceSet.end(); ++fit)
	{
		cout << i << endl;
		for(set<int>::iterator sit = (*fit).begin(); sit != (*fit).end();++sit)
		{
			std::cout << *sit << "--";
		}
		std::cout << endl;
		int j = 0;
		for(vector<set<int> >::iterator fit2 = fit+1; fit2 != faceSet.end(); ++fit2)
		{	
			for(set<int>::iterator sit = (*fit2).begin(); sit != (*fit2).end();++sit)
			{
				std::cout << *sit << "**";
			}
			std::cout << endl;
			if((*fit) == *fit2)
			{
				std::cout <<"ye*****************************************************" << endl;	
				break;
			}
			else {
					j++;
					
				}
		}	i++;
	}
	std::cout << "face almost done" << endl; 	
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
		p->print();
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
		//f->print();
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
}

bool Point::equalTo(Point *other)
{
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
