#pragma once

#include "time.h"

using namespace std;

class CirclePlateIteration {
private:
	CircleField *input;
	float m_phi1,m_phi2,m_distance,p_radius,m_radius;
public:
	CirclePlateIteration() {
		input = new CircleField();
		p_radius = 0;
		m_phi1 = 0;
		m_phi2 = 0;
		m_distance = 0;
		cerr << "New Iteration error"<<endl;
	}
	CirclePlateIteration(CircleField *inputField,float phase_radius,float distance,float phi1,float phi2) {
		input = inputField;
		p_radius = phase_radius;
		m_phi1 = phi1;
		m_phi2 = phi2;
		m_distance = distance;
	}
	void huygensIteration(int time) {
		CircleField *output, *plate_field;
		Information *new_info=input->getInfo();
		FreeSpace *space = new FreeSpace(m_distance);
		PhasePlate *plate = new PhasePlate(m_phi1, m_phi2);
		Information *temp_info = new Information(new_info->getWide(),p_radius,new_info->getDim());
		cout << "start" << endl;
		time_t start, finish;
		start = clock();
		for (int i = 0; i < time; i++) {
			cout << i + 1 << " ";
			new_info = input->getInfo();
			plate_field = new CircleField(temp_info);
			output = new CircleField(new_info);
			space->huygensDiffraction(input, plate_field);
			plate_field->saveAmplitude(6 * i);
			plate_field->savePhase(6 * i + 1);
			plate->transfer(plate_field);
			plate_field->saveAmplitude(6 * i + 2);
			plate_field->savePhase(6 * i + 3);
			space->huygensDiffraction(plate_field, output);
			output->normalize();
			output->saveAmplitude(6*i+4);
			output->savePhase(6 * i + 5);
			delete input;
			delete plate_field;
			input = output;
		}
		finish = clock();
		int spent = finish - start;
		cout << spent;
	}
	void fresnelIteration(int time) {
		CircleField *output, *plate_field;
		Information *new_info = input->getInfo();
		FreeSpace *space = new FreeSpace(m_distance);
		PhasePlate *plate = new PhasePlate(m_phi1, m_phi2);
		Information *temp_info = new Information(new_info->getWide(), p_radius, new_info->getDim());
		cout << "start" << endl;
		time_t start, finish;
		start = clock();
		for (int i = 0; i < time; i++) {
			cout << i + 1 << " ";
			new_info = input->getInfo();
			plate_field = new CircleField(temp_info);
			output = new CircleField(new_info);
			space->fresnelDiffraction(input, plate_field);
			plate_field->saveAmplitude(6 * i);
			plate_field->savePhase(6 * i + 1);
			plate->transfer(plate_field);
			plate_field->saveAmplitude(6 * i + 2);
			plate_field->savePhase(6 * i + 3);
			space->fresnelDiffraction(plate_field, output);
			output->normalize();
			output->saveAmplitude(6 * i + 4);
			output->savePhase(6 * i + 5);
			delete input;
			delete plate_field;
			input = output;
		}
		finish = clock();
		int spent = finish - start;
		cout << spent<<"ms";
	}
	double getAngle() {
		double deltaPhi = m_phi1 - m_phi2;
		double deltaNumber = 0.5*deltaPhi / PI;
		double lambda = 1;
		double refractionIndex = 1.55;
		double x = deltaNumber*lambda / (refractionIndex-1);
		double z = p_radius * 2;
		double c = sqrt(x*x + z*z);
		double alpha = atan(x / z);
		double tempAngle = acos(x*refractionIndex / z);
		double beta = 0.5*3.141592653 - alpha - tempAngle;
		return beta;
	}
};

class SimpleIteration {
private:
	CircleField *input;
	float m_distance;
public:
	SimpleIteration() {
		input = new CircleField();
		m_distance = 0;
		cerr << "New Iteration error" << endl;
	}
	SimpleIteration(CircleField *inputField, float distance) {
		input = inputField;
		m_distance = distance;
	}
	/*void iteration(int time) {
	CircleField *output;
	Information *new_info=input->getInfo();
	FreeSpace *space = new FreeSpace(m_distance);
	PhasePlate *plate = new PhasePlate(m_phi1, m_phi2);
	cout << "start"<<endl;
	time_t start, finish;
	start = clock();
	for (int i = 0; i < time; i++) {
	new_info = input->getInfo();
	output = new CircleField(new_info);
	space->diffractionHuygens(input, output);
	output->saveAmplitude(i);
	delete input;
	input = output;
	cout << i+1 <<" time calculation"<< endl;
	}
	finish = clock();
	int dim = finish - start;
	cout << "spend "<<dim<<endl;
	}*/
	void huygensIteration(int time) {
		CircleField *output;
		Information *new_info;
		FreeSpace *space = new FreeSpace(m_distance);
		cout << "start" << endl;
		time_t start, finish;
		start = clock();
		for (int i = 0; i < time; i++) {
			new_info = input->getInfo();
			output = new CircleField(new_info);
			cout << i + 1 << " ";
			space->huygensDiffraction(input, output);
			output->normalize();
			delete input;
			input = output;
			output->saveAmplitude(2 * i);
			output->savePhase(2 * i + 1);
		}
		finish = clock();
		int spent = finish - start;
		cout << spent;
	}
	void fresnelIteration(int time) {
		CircleField *output;
		Information *new_info;
		FreeSpace *space = new FreeSpace(m_distance);
		cout << "start" << endl;
		time_t start, finish;
		start = clock();
		for (int i = 0; i < time; i++) {
			new_info = input->getInfo();
			output = new CircleField(new_info);
			cout << i + 1 << " ";
			space->fresnelDiffraction(input, output);
			output->normalize();
			delete input;
			input = output;
			output->saveAmplitude(2 * i);
			output->savePhase(2 * i + 1);
		}
		finish = clock();
		int spent = finish - start;
		cout << spent;
	}
};

class RectanglePlateIteration {
public:
	Field *input;
	float wide,height,m_phi1, m_phi2, m_distance, p_wide,p_height,m_dim;
public:
	RectanglePlateIteration() {
		input = new RectangleField();
		wide = 0;
		height = 0;
		p_wide = 0;
		p_height = 0;
		m_phi1 = 0;
		m_phi2 = 0;
		m_distance = 0;
		m_dim = 0;
		cerr << "New Iteration error" << endl;
	}
	RectanglePlateIteration(float phase_wide,float phase_height,float beam_wide,float beam_height,  float d,float distance, float phi1, float phi2) {
		wide = beam_wide;
		height = beam_height;
		p_wide = phase_wide;
		p_height = phase_height;
		m_dim = d;
		m_phi1 = phi1;
		m_phi2 = phi2;
		m_distance = distance;
		input = new RectangleField(p_wide, p_height, wide, height, m_dim, 1, 1, 0);
		input->saveAmplitude(0);
	}
	void fresnelIteration(int time) {
		Field *output, *plate_field;
		FreeSpace *space = new FreeSpace(m_distance);
		PhasePlate *plate = new PhasePlate(m_phi1, m_phi2);
		time_t start, finish;
		start = clock();
		cout << "start" << endl;
		for (int i = 0; i < time; i++) {
			cout << i + 1 << " ";
			plate_field = new RectangleField(p_wide,p_height,m_dim,1,0,0);
			output = new RectangleField(p_wide,p_height,wide,height,m_dim,1,0,0);
			space->fresnelDiffraction(input, plate_field);
			plate_field->saveAmplitude(6 * i+1);
			plate_field->savePhase(6 * i + 2);
			plate->transfer(plate_field);
			plate_field->saveAmplitude(6 * i + 3);
			plate_field->savePhase(6 * i + 4);
			space->fresnelDiffraction(plate_field, output);
			output->normalize();
			output->saveAmplitude(6 * i + 5);
			output->savePhase(6 * i + 6);
			delete input;
			delete plate_field;
			input = output;
		}
		/*input->saveAmplitude(0);
		for (int i = 1; i < 30;i++) {
			space = new FreeSpace(i);
			output = new RectangleField(p_wide, p_height, wide, height, m_dim, 1, 0, 0);
			space->fresnelDiffraction(input, output);
			output->saveAmplitude(i);
			delete output;
			delete space;
		}*/
		finish = clock();
		int spent = finish - start;
		cout << spent << "ms";
	}
};