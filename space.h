#pragma once

#include"parellell.h"

using namespace std;

#define PI 3.1415926

class FreeSpace {
private:
	float m_distance;
public:
	FreeSpace() {
		m_distance = 0;
	}
	FreeSpace(float distance) {
		m_distance = distance;
	}
	void diffractionHuygens(CircleField *input, CircleField *output) {
		vector<Cell*> *in_intensity = input->getIntensity();
		vector<Cell*> *out_intensity = output->getIntensity();
		float lambda = input->getInfo()->getLambda();
		float dim = input->getInfo()->getDim();
		this->diffractionHuygens(in_intensity, out_intensity, lambda, dim);
	}
	void diffractionFresnel(CircleField *input, CircleField *output) {
		vector<Cell*> *in_intensity = input->getIntensity();
		vector<Cell*> *out_intensity = output->getIntensity();
		float lambda = input->getInfo()->getLambda();
		float dim = input->getInfo()->getDim();
		this->diffractionFresnel(in_intensity, out_intensity, lambda, dim);
	}
	void huygensDiffraction(Field *input, Field *output) {
		huygensCalculate(input, output,m_distance);
	}
	void fresnelDiffraction(Field *input, Field *output) {
		fresnelCalculate(input, output, m_distance);
	}
private:
	void diffractionHuygens(vector<Cell*> *input, vector<Cell*> *output,float lambda,float dim) {
		int inputSize = input->size();
		int outputSize = output->size();
		Cell *inCell, *outCell;
		for (int i = 0; i < inputSize; i++) {
			inCell = input->at(i);
			for (int j = 0; j < outputSize; j++) {
				outCell = output->at(j);
				this->Huygens(inCell, outCell, m_distance, lambda, dim);
			}
		}
	}
	void diffractionFresnel(vector<Cell*> *input, vector<Cell*> *output, float lambda, float dim) {
		int inputSize = input->size();
		int outputSize = output->size();
		for (int i = 0; i < inputSize; i++) {
			Cell *inCell = input->at(i);
			for (int j = 0; j < outputSize; j++) {
				Cell *outCell = output->at(j);
				this->Fresnel(inCell, outCell, m_distance, lambda, dim);
			}
		}
	}
	static void Huygens(Cell *source, Cell *destination, float distance, float lambda,float dim) {
		float s_x = source->getX();
		float s_y = source->getY();
		float d_x = destination->getX();
		float d_y = destination->getY();
		float c = source->getAmplitude();
		float e = source->getPhase();
		float k = 2 * PI / lambda;
		float r = (s_x - d_x)*(s_x - d_x) + (s_y - d_y)*(s_y - d_y) + distance*distance;
		float new_c = distance*c*dim*dim / (lambda*r);
		r = sqrt(r);
		float temp1 = k*r;
		float temp2 = PI / 2;
		float new_e = e + temp1 + temp2;
		destination->getIntensity()->add(new_c, new_e);
	}
	static void Fresnel(Cell *source, Cell *destination, float distance,float lambda,float dim) {
		float s_x = source->getX();
		float s_y = source->getY();
		float d_x = destination->getX();
		float d_y = destination->getY();
		float c = source->getAmplitude();
		float e = source->getPhase();
		float k = 2*PI / lambda;
		float new_c = dim*dim*c / (lambda*distance);
		float temp0 = (s_x - d_x)*(s_x - d_x) + (s_y - d_y)*(s_y - d_y);
		float temp1 = 0.5*k*temp0 / distance;
		float temp2 = k*distance;
		float temp3 = -PI / 2;
		float new_e = temp1 + temp2 + temp3 + e;
		destination->getIntensity()->add(new_c, new_e);
	}
};

class PhasePlate {
private:
	float m_phi1, m_phi2;
public:
	PhasePlate() {
		m_phi1 = 0;
		m_phi2 = 0;
	}
	PhasePlate(float phi1, float phi2) {
		m_phi1 = phi1;
		m_phi2 = phi2;
	}
	void transfer(CircleField *input) {
		vector<Cell*> *in = input->getIntensity();
		int in_number = in->size();
		float radius = input->getInfo()->getRadius();
		float wide = 2 * radius;
		for (int i = 0; i < in_number; i++) {
			float temp_x = in->at(i)->getX();
			float extra_phase = m_phi2 + (m_phi1 - m_phi2)*(wide-temp_x) / wide;
			in->at(i)->getIntensity()->phaseShift(extra_phase);
		}
	}
	void transfer(Field *input) {
		vector<Cell*> *in = input->getIntensity();
		int in_number = in->size();
		float height = input->getInfo()->getHeight();
		float temp_y = 0, extra_phase = 0;
		for (int i = 0; i < in_number; i++) {
			temp_y= in->at(i)->getY();
			extra_phase = m_phi2 + (m_phi1 - m_phi2)*(height - temp_y) / height;
			in->at(i)->getIntensity()->phaseShift(extra_phase);
		}
	}
};