#pragma once

#include"vector"
#include"math.h"
#include"fstream"
#include"iostream"
#include"string"

using namespace std;
#define PI 3.1415926

class Information {
private:
	float m_wide;
	float m_height;
	float m_radius;
	float m_dim;
	float m_lambda;
	int m_xnumber;
	int m_ynumber;
public:
	Information() {
		m_wide = 0;
		m_height = 0;
		m_dim = 0;
		m_radius = 0;
		m_lambda = 0;
	}
	Information(float wide, float height,float radius, float dim, float lambda) {
		m_wide = wide;
		m_height = height;
		m_radius = radius;
		m_dim = dim;
		m_lambda = lambda;
	}
	Information(float wide, float radius, float dim, float lambda) {
		m_wide = wide;
		m_height = wide;
		m_radius = radius;
		m_dim = dim;
		m_lambda = lambda;
	}
	Information(float wide, float radius, float dim) {
		m_wide = wide;
		m_height = wide;
		m_radius = radius;
		m_dim = dim;
		m_lambda = 1;
	}
	float getWide() {
		return m_wide;
	}
	float getHeight() {
		return m_height;
	}
	float getRadius() {
		return m_radius;
	}
	float getDim() {
		return m_dim;
	}
	float getLambda() {
		return m_lambda;
	}
	int getNumber() {
		return (int)(m_wide / m_dim);
	}
	int getXnumber() {
		return (int)(m_wide / m_dim);
	}
	int getYnumber() {
		return (int)(m_height / m_dim);
	}
	float getCenter() {
		float number = this->getNumber();
		float center=number*m_dim / 2;
		return center;
	}
	float getXcenter() {
		float xnumber = this->getXnumber();
		float xcenter = xnumber*m_dim / 2;
		return xcenter;
	}
	float getYcenter() {
		float ynumber = this->getYnumber();
		float ycenter = ynumber*m_dim / 2;
		return ycenter;
	}
	void setWide(float wide) {
		m_wide = wide;
	}
	void setHeight(float height) {
		m_height = height;
	}
	void setRadius(float radius) {
		m_radius = radius;
	}
	void setDim(float dim) {
		m_dim = dim;
	}
};

class Exp {
private:
	float m_c = 0;
	float m_e = 0;
public:
	Exp() {
		m_c = 0;
		m_e = 0;
	}
	Exp(float c, float e) {
		m_c = c;
		m_e = e;
	}
	void setE(float e) {
		m_e = e;
	}
	void setC(float c) {
		m_c = c;
	}
	float getE() {
		return m_e;
	}
	float getC() {
		return m_c;
	}
	void add(Exp *p) {
		float p_c = p->getC();
		float p_e = p->getE();
		float new_c= sqrt(m_c*m_c + p_c*p_c + 2 * m_c*p_c*cos(m_e - p_e));
		float temp1=m_c*sin(m_e) + p_c*sin(p_e);
		float temp2 = m_c*cos(m_e) + p_c*cos(p_e);
		float new_e = atan(temp1 / temp2);
		this->setC(new_c);
		this->setE(new_e);
	}
	void add(float p_c, float p_e) {
		float new_c = sqrt(m_c*m_c + p_c*p_c + 2 * m_c*p_c*cos(m_e - p_e));
		float temp1 = m_c*sin(m_e) + p_c*sin(p_e);
		float temp2 = m_c*cos(m_e) + p_c*cos(p_e);
		float new_e = atan(temp1 / temp2);
		this->setC(new_c);
		this->setE(new_e);
	}
	void simplify() {
		int temp = m_e / PI;
		float new_e = m_e - temp*PI;
		this->setE(new_e);
	}
	void multi(Exp *p) {
		float new_c = m_c*p->getC();
		float new_e = m_e + p->getE();
		this->setC(new_c);
		this->setE(new_e);
	}
	void phaseShift(float extra_phase) {
		float new_e = m_e + extra_phase;
		this->setE(new_e);
	}
};

class Cell {
private:
	int m_i;
	int m_j;
	float m_x;
	float m_y;
	Exp *m_intensity;
public:
	Cell() {
		m_i = 0;
		m_j = 0;
		m_x = 0;
		m_y = 0;
		m_intensity = new Exp();
	}
	Cell(int i, int j, float dim, float c, float e) {
		m_i = i;
		m_j = j;
		m_x = i*dim;
		m_y = j*dim;
		m_intensity = new Exp(c, e);
	}
	Cell(int i, int j, float x, float y, float c, float e) {
		m_i = i;
		m_y = y;
		m_x = x;
		m_y = y;
		m_intensity = new Exp(c, e);
	}
	~Cell() {
		delete m_intensity;
	}
	int getI() {
		return m_i;
	}
	int getJ() {
		return m_j;
	}
	float getX() {
		return m_x;
	}
	float getY() {
		return m_y;
	}
	float getAmplitude() {
		return m_intensity->getC();
	}
	float getPhase() {
		float phase=m_intensity->getE();
		return phase;
	}
	void setI(int i) {
		m_i = i;
	}
	void setJ(int j) {
		m_j = j;
	}
	void setX(float x) {
		m_x = x;
	}
	void setY(float y) {
		m_y = y;
	}
	void setAmplitude(float c) {
		m_intensity->setC(c);
	}
	void setPhase(float e) {
		m_intensity->setE(e);
	}
	void setIntensity(float c, float e) {
		m_intensity->setC(c);
		m_intensity->setE(e);
	}
	void setIntensity(Exp exp) {
		float c = exp.getC();
		float e = exp.getE();
		m_intensity->setC(c);
		m_intensity->setE(e);
	}
	void setIntensity(Exp *exp) {
		float c = exp->getC();
		float e = exp->getE();
		m_intensity->setC(c);
		m_intensity->setE(e);
	}
	Exp* getIntensity() {
		return m_intensity;
	}
};

class Field {
public:
	Information *m_info;
	vector<Cell*> *m_intensity;
	virtual void saveAmplitude(int n)=0;
	virtual void savePhase(int n)=0;
	virtual void normalize()=0;
	virtual Information* getInfo()=0;
	virtual vector<Cell*>* getIntensity()=0;
	virtual bool isAvailable(int i, int j) = 0;
};

class RectangleField:public Field {
private:
	float b_wide = 0;
	float b_height = 0;
	vector<int> *m_position;
public:
	RectangleField() {
		m_info = new Information();
		cout << "New null Circle Field" << endl;
	}
	RectangleField(float pwide, float pheight,float bwide,float bheight, float dim, float lambda, float c, float e) {
		m_info = new Information(pwide, pheight,0, dim, lambda);
		b_wide = bwide;
		b_height = bheight;
		int xnumber = m_info->getXnumber(), ynumber = m_info->getYnumber();
		int count = 0;
		m_position = new vector<int>();
		m_intensity = new vector<Cell*>();
		for (int i = 0; i < xnumber; i++)
			for (int j = 0; j < ynumber; j++) {
				if (this->isAvailable(i, j)) {
					//cout << i << " " << j << endl;
					Cell *temp = new Cell(i, j, dim, c, e);
					m_intensity->push_back(temp);
					m_position->push_back(count);
					count++;
				}
				else
					m_position->push_back(-1);
			
			}
	}
	RectangleField(float pwide, float pheight, float dim, float lambda, float c, float e) {
		this->RectangleField::RectangleField(pwide, pheight, pwide, pheight, dim, lambda, c, e);
	}
	RectangleField(float wide, float height, float dim) {
		this->RectangleField::RectangleField(wide, height, dim, 1, 0, 0);
	}
	RectangleField(Information *info,float bwide,float bheight) {
		float c = 0, e = 0;
		float wide = info->getWide();
		float height = info->getHeight();
		float dim = info->getDim();
		this->RectangleField::RectangleField(wide, height, bwide, bheight, dim, 1, c, e);
	}
	RectangleField(Information *info) {
		float wide = info->getWide();
		float height = info->getHeight();
		this->RectangleField::RectangleField(info, wide, height);
	}
	~RectangleField() {
		delete m_info;
		delete m_intensity;
		delete m_position;
	}
	bool isAvailable(int i, int j) {
		float x = i*m_info->getDim();
		float y = j*m_info->getDim();
		float xcenter = m_info->getXcenter(), ycenter = m_info->getYcenter();
		float xdistance = abs(x - xcenter);
		float ydistance = abs(y - ycenter);
		float xr = b_wide / 2, yr = b_height / 2;
		//cout << xdistance << " " << ydistance<< endl;
		if ((xdistance <= xr) & (ydistance <= yr)) {
			//cout << xdistance << " " << ydistance << " " << 1 << endl;
			return 1;
		}	
		else {
			//cout << xdistance << " " << ydistance << " " << 0 << endl;
			return 0;
		}
			
	}
	int getAvailableNumber() {
		int number = 0, xnumber = m_info->getNumber();
		for (int i = 0; i < xnumber; i++)
			for (int j = 0; j < xnumber; j++)
				if (this->isAvailable(i, j))
					number++;
		return number;
	}
	int getPosition(int i, int j) {
		int ynumber = m_info->getYnumber();
		int number = ynumber*i + j;
		int position = m_position->at(number);
		return position;
	}
	float getBeamWide() {
		return b_wide;
	}
	float getBeamHeight() {
		return b_height;
	}
	float getAmplitude(int i, int j) {
		int position = this->getPosition(i, j);
		float amplitude;
		if (position != -1)
			amplitude = m_intensity->at(position)->getAmplitude();
		else
			amplitude = 0;
		return amplitude;
	}
	float getPhase(int i, int j) {
		int position = this->getPosition(i, j);
		float phase;
		if (position != -1)
			phase = m_intensity->at(position)->getPhase();
		else
			phase = 0;
		return phase;
	}
	Exp* getIntensity(int i, int j) {
		int xnumber = m_info->getXnumber();
		int position = i*xnumber + j;
		Exp *exp = m_intensity->at(position)->getIntensity();
		return exp;
	}
	vector<Cell*>* Field::getIntensity() {
		return m_intensity;
	}
	Information* Field::getInfo() {
		return m_info;
	}
	void printAmplitude() {
		int xnumber = m_info->getXnumber(), ynumber = m_info->getYnumber();
		for (int i = 0; i < xnumber; i++) {
			for (int j = 0; j < ynumber; j++) {
				float amplitude = this->getAmplitude(i, j);
					cout << amplitude<< " ";
			}
			cout << endl;
		}
	}
	void printPhase() {
		int xnumber = m_info->getXnumber(), ynumber = m_info->getYnumber();
		for (int i = 0; i < xnumber; i++) {
			for (int j = 0; j < ynumber; j++) {
				float phase = this->getPhase(i, j);
				cout << phase << " ";
			}
			cout << endl;
		}
	}
	void Field::saveAmplitude(int n) {
		string ss = to_string(n);
		string s ="data/"+ ss + ".txt";
		ofstream outfile(s, ios::out);
		if (!outfile){
			cerr << "open error!" << endl;
			exit(1);
		}
		int xnumber = m_info->getXnumber(), ynumber = m_info->getYnumber();
		float temp=0;
		for (int i = 0; i < xnumber; i++) {
			for (int j = 0; j < ynumber; j++) {
				temp = this->getAmplitude(i, j);
				outfile << temp << " ";
			}
			outfile << endl;
		}
		outfile.close();
	}
	void Field::savePhase(int n) {
		string ss = to_string(n);
		string s = "data/" + ss + ".txt";
		ofstream outfile(s, ios::out);
		if (!outfile) {
			cerr << "open error!" << endl;
			exit(1);
		}
		int xnumber = m_info->getXnumber(), ynumber = m_info->getYnumber();
		float temp=0;
		for (int i = 0; i < xnumber; i++) {
			for (int j = 0; j < ynumber; j++) {
				temp = this->getPhase(i, j);
				outfile << temp << " ";
			}
			outfile << endl;
		}
		outfile.close();
	}
	void Field::normalize() {
		float max = 0;
		int size = m_intensity->size();
		float temp;
		for (int i = 0; i < size; i++) {
			temp = m_intensity->at(i)->getAmplitude();
			if (temp > max)
				max = temp;
		}
		if(max!=0)
			for (int i = 0; i < size; i++) {
				temp = m_intensity->at(i)->getAmplitude();
				temp = temp / max;
				m_intensity->at(i)->setAmplitude(temp);
			}
	}
};

class CircleField:public Field {
private:
	Information *m_info;
	vector<Cell*> *m_intensity;
	vector<int> *m_position;
public:
	CircleField() {
		m_info = new Information();
		cout << "New null Circle Field" << endl;
	}
	CircleField(float wide, float radius, float dim, float lambda, float c, float e) {
		m_info = new Information(wide, radius, dim, lambda);
		int number = m_info->getNumber(), availableNumber = this->getAvailableNumber();
		int count = 0;
		m_position = new vector<int>();
		m_intensity = new vector<Cell*>();
		for (int i = 0; i < number; i++)
			for (int j = 0; j < number; j++)
				if (this->isAvailable(i, j)) {
					Cell *temp = new Cell(i, j, dim, c, e);
					m_intensity->push_back(temp);
					m_position->push_back(count);
					count++;
				}
				else
					m_position->push_back(-1);
	}
	CircleField(float wide, float radius, float dim) {
		float c = 0, e = 0;
		m_info = new Information(wide, radius, dim, 1);
		int number = m_info->getNumber(), availableNumber = this->getAvailableNumber();
		int count = 0;
		m_position = new vector<int>();
		m_intensity = new vector<Cell*>();
		for (int i = 0; i < number; i++)
			for (int j = 0; j < number; j++)
				if (this->isAvailable(i, j)) {
					Cell *temp = new Cell(i, j, dim, c, e);
					m_intensity->push_back(temp);
					m_position->push_back(count);
					count++;
				}
				else
					m_position->push_back(-1);
	}
	CircleField(Information *info) {
		float c = 0, e = 0;
		float wide = info->getWide();
		float radius = info->getRadius();
		float dim = info->getDim();
		m_info = new Information(wide, radius, dim, 1);
		int number = m_info->getNumber(), availableNumber = this->getAvailableNumber();
		int count = 0;
		m_position = new vector<int>();
		m_intensity = new vector<Cell*>();
		for (int i = 0; i < number; i++)
			for (int j = 0; j < number; j++)
				if (this->isAvailable(i, j)) {
					Cell *temp = new Cell(i, j, dim, c, e);
					m_intensity->push_back(temp);
					m_position->push_back(count);
					count++;
				}
				else
					m_position->push_back(-1);
	}
	~CircleField() {
		delete m_info;
		delete m_intensity;
		delete m_position;
	}
	float getAmplitude(int i, int j) {
		int position = this->getPosition(i, j);
		float amplitude;
		if (position != -1)
			amplitude = m_intensity->at(position)->getAmplitude();
		else
			amplitude = 0;
		return amplitude;
	}
	float getPhase(int i, int j) {
		int position = this->getPosition(i, j);
		float phase;
		if (position != -1)
			phase = m_intensity->at(position)->getPhase();
		else
			phase = 0;
		return phase;
	}
	Exp* getIntensity(int i, int j) {
		int position = this->getPosition(i, j);
		Exp *exp;
		if (position != -1)
			exp = m_intensity->at(position)->getIntensity();
		else
			exp = new Exp();
		return exp;
	}
	bool isAvailable(int i, int j) {
		float x = i*m_info->getDim();
		float y = j*m_info->getDim();
		float center = m_info->getCenter();
		float distance = sqrt((x - center)*(x - center) + (y - center)*(y - center));
		float r = m_info->getRadius();
		if (distance < r) {
			return 1;
		}
		else {
			return 0;
		}
	}
	int getAvailableNumber() {
		int number = 0, xnumber = m_info->getNumber();
		for (int i = 0; i < xnumber; i++)
			for (int j = 0; j < xnumber; j++)
				if (this->isAvailable(i, j))
					number++;
		return number;
	}
	int getPosition(int i, int j) {
		int xnumber = m_info->getNumber();
		int number = xnumber*i + j;
		int position = m_position->at(number);
		return position;
	}
	vector<Cell*>* Field::getIntensity() {
		return m_intensity;
	}
	Information* Field::getInfo() {
		return m_info;
	}
	void printAmplitude() {
		int number = m_info->getNumber();
		for (int i = 0; i < number; i++) {
			for (int j = 0; j < number; j++) {
				float amplitude = this->getAmplitude(i, j);
				cout << amplitude << " ";
			}
			cout << endl;
		}
	}
	void printPhase() {
		int number = m_info->getNumber();
		for (int i = 0; i < number; i++) {
			for (int j = 0; j < number; j++) {
				float phase = this->getPhase(i, j);
				cout << phase << " ";
			}
			cout << endl;
		}
	}
	void Field::saveAmplitude(int n) {
		string ss = to_string(n);
		string s = "data/" + ss + ".txt";
		ofstream outfile(s, ios::out);
		if (!outfile) {
			cerr << "open error!" << endl;
			exit(1);
		}
		int number = m_info->getNumber();
		for (int i = 0; i < number; i++) {
			for (int j = 0; j < number; j++) {
				float temp = this->getAmplitude(i, j);
				outfile << temp << " ";
			}
			outfile << endl;
		}
		outfile.close();
	}
	void Field::savePhase(int n) {
		string ss = to_string(n);
		string s = "data/" + ss + ".txt";
		ofstream outfile(s, ios::out);
		if (!outfile) {
			cerr << "open error!" << endl;
			exit(1);
		}
		int number = m_info->getNumber();
		for (int i = 0; i < number; i++) {
			for (int j = 0; j < number; j++) {
				float temp = this->getPhase(i, j);
				outfile << temp << " ";
			}
			outfile << endl;
		}
		outfile.close();
	}
	void Field::normalize() {
		float max = 0;
		int size = m_intensity->size();
		float temp;
		for (int i = 0; i < size; i++) {
			temp = m_intensity->at(i)->getAmplitude();
			if (temp > max)
				max = temp;
		}
		if (max != 0)
			for (int i = 0; i < size; i++) {
				temp = m_intensity->at(i)->getAmplitude();
				temp = temp / max;
				m_intensity->at(i)->setAmplitude(temp);
			}
	}
};