__kernel void test(__global float *InAm,__global float *InPh,__global float *InX,__global float *InY,__global float *OutAm,__global float *OutPh,__global float *OutX,__global float *OutY,__global float *distance,__global int *length,__global float *Dim){
int index = get_global_id(0);
int l=*length;
float d=*distance;
float dim=*Dim;
float k=2*3.1415926;
float x1=OutX[index];
float y1=OutY[index];
float resultAm=OutAm[index];
float resultPh=OutPh[index];
float a0,p0,x0,y0,r2,r,c2,e2,temp,tempAm,tempPh;
float sin_e,sin_e2,cos_e,cos_e2,temp1,temp2;
for(int i=0;i<l;i++){
a0=InAm[i];
p0=InPh[i];
x0=InX[i];
y0=InY[i];
r2=(x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)+d*d;
c2=dim*dim*d*a0/r2;
r=sqrt(r2);
e2=p0+k*r+k*0.25;
temp=cos(resultPh-e2);
tempAm=sqrt(resultAm*resultAm+c2*c2+2*resultAm*c2*temp);
sin_e=sin(resultPh);
sin_e2=sin(e2);
cos_e=cos(resultPh);
cos_e2=cos(e2);
temp1=resultAm*sin_e+c2*sin_e2;
temp2=resultAm*cos_e+c2*cos_e2;
tempPh=atan(temp1/temp2);
resultPh=fmod(tempPh,k);
resultAm=tempAm;
if(i==(l-1)){
OutAm[index]=resultAm;
OutPh[index]=resultPh;
}
}
}