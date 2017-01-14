#include<stdio.h>
#include<math.h>

int* rgbToHSV(int color[3])
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

	printf("HSV= %f, %f, %f", H, S, V);



}

void hsvToRGB(double HSV[])
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

	printf("RGB = %d, %d, %d", R, G, B);

}


int main()
{

	double color[] ={0,0,0.75};

	hsvToRGB(color);
		
	//rgbToHSV(color);


return 0;
}
