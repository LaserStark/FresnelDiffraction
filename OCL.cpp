// OCL.cpp: 定义控制台应用程序的入口点。
#pragma once
#include "stdafx.h"
#include "field.h"
#include"space.h"
#include"iostream"
#include "math.h"
#include"iteration.h"

#define PI 3.141592653
using namespace std;

int main()
{
	float  p_wide=200,p_height=300,bwide =20,bheight=100,dim = 1, lambda = 1,distance=500,phi1=1,phi2=0;
	//RectangleField *field = new RectangleField( p_wide, p_height, bwide, bheight,dim, lambda, 1, 0);
	RectanglePlateIteration *iteration = new RectanglePlateIteration( p_wide, p_height,bwide,bheight, dim,distance, phi1, phi2);
	iteration->fresnelIteration(3);
	/*float wide =10, p_radius=10,m_radius = 5, dim = 1, lambda = 1, c = 1,e = 0,distance=1000,phi1=PI,phi2=0;
	int n = 0;
	cout << "Please input width:";
	cin >> wide ;
	cout << "Please input radius of phase plate:";
	cin >> p_radius;
	cout << "Please input radius of mirror:";
	cin >> m_radius;
	cout << "Please input dim:";
	cin >> dim;
	cout << "Please input distance of diffraction:";
	cin >> distance;
	cout << "Please input phi1:";
	cin >> phi1;
	cout << "Please input phi2:";
	cin >> phi2;
	cout << "Please input number of iteration:";
	cin >> n;
	CircleField *input = new CircleField(wide, m_radius, dim, lambda, c, e);
	//input->saveAmplitude(0);
	PlateIteration *iteration = new PlateIteration(input,p_radius,distance,phi1,phi2);
	//SimpleIteration *iteration = new SimpleIteration(input, distance);
	iteration->fresnelIteration(n);*/
	cout << " finished"<< endl;
    return 0;
}