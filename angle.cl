__kernel void test(__global float *InAm,__global float *InPh,__global float *InX,__global float *InY,__global float *OutAm,__global float *OutPh,__global float *OutX,__global float *OutY,__global float *distance,__global int *length,__global float *Dim,__global float *Angle){
int index = get_global_id(0);
int l=*length;
float d=*distance;
float dim=*Dim;
float beta=*Angle;
float k=2*3.1415926;
float x1=OutX[index];
float y1=OutY[index];
float resultAm=OutAm[index];
float resultPh=OutPh[index];
float sin_beta=sin(beta);
float tan_beta=tan(beta);
float basic=x1*sin_beta+d;
x1=basic*tan_beta-x1;
float am0,ph0,x0,y0,r2,am2,ph2,temp,tempAm,tempPh;
float sin_ph,sin_ph2,cos_ph,cos_ph2,temp1,temp2,x2;
for(int i=0;i<l;i++){
am0=InAm[i];
ph0=InPh[i];
x0=InX[i];
y0=InY[i];
x2=(x1+x0)*(x1+x0);
r2=x2+(y1-y0)*(y1-y0);
d=sqrt(basic*basic+x2);
am2=dim*dim*am0/d;
ph2=ph0+k*d+0.5*k*r2/d-k*0.25;
temp=cos(resultPh-ph2);
tempAm=sqrt(resultAm*resultAm+am2*am2+2*resultAm*am2*temp);
sin_ph=sin(resultPh);
sin_ph2=sin(ph2);
cos_ph=cos(resultPh);
cos_ph2=cos(ph2);
temp1=resultAm*sin_ph+am2*sin_ph2;
temp2=resultAm*cos_ph+am2*cos_ph2;
tempPh=atan(temp1/temp2);
resultPh=fmod(tempPh,k);
resultAm=tempAm;
if(i==(l-1)){
OutAm[index]=resultAm;
OutPh[index]=resultPh;
}
}
}