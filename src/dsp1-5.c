#include <stdio.h>
#include <math.h>
#include <string.h>

#define NUM 500

typedef struct{
	double re;
	double im;
}comp;

void inputData(comp *data, char *filename, int N);
void outputData(comp *data, char *filename, int N);
void dft(comp *xn, int N, comp *Xk, int a, int b);
void ampSpectrum(comp *Xk, int N, double *spec);
void phaSpectrum(comp *Xk, int N, double *spec);

comp xn[NUM];
comp Xk[NUM];
char *filename = "txt/dsp1-5.txt";

int main(){
	
	int i = 0;
	int N = NUM;
	double buf[NUM]={0}, pha_buf[NUM]={0};
	comp a[NUM];
	FILE *fp;

	for(i=0;i<N;i++){
		Xk[i].re = 0;
		Xk[i].im = 0;
		xn[i].re = 0;
		xn[i].im = 0;
		a[i].re = 0;
		a[i].im = 0;
	}

	inputData(xn, filename, N);
	dft(xn, N, Xk, 1, 1);
	ampSpectrum(Xk, N, buf);
	phaSpectrum(Xk, N, pha_buf);
	dft(Xk, N, a, -1, N);

//	for(i=0;i<N;i++){
//		printf("xn[%d].re\t=%12lf\n", i, xn[i].re);
//		printf("xn[%d].im\t=%12lf\n", i, xn[i].im);
//		printf("a[%d].re\t=%12lf\n", i, a[i].re);
//		printf("a[%d].im\t=%12lf\n", i, a[i].im);
//		printf("Xk[%d].re\t=%12lf\n",i ,Xk[i].re);
//		printf("Xk[%d].im\t=%12lf\n",i ,Xk[i].im);
//		printf("amp[%d]=%12lf\n", i, buf[i]);
//	}

	outputData(xn, "xn", N);
	outputData(a, "a", N);
	outputData(Xk, "Xk", N);
	
	fp = fopen("amp.txt", "w");

	for(i = 0; i < N; i++){
		if(fprintf(fp, "%lf\n", buf[i]) < 0 ){
			printf("ERROR\n");
			return 0;
		}
	}

	fp = fopen("pha.txt", "w");

	for(i = 0; i < N; i++){
		if(fprintf(fp, "%lf\n", pha_buf[i]) < 0 ){
			printf("ERROR\n");
			return 0;
		}
	}

	return 0;
}

void inputData(comp *data, char *filename, int N){
	FILE *fp;
	int i=0;

	fp = fopen(filename, "r");
	for(i=0;i<N;i++){
		if(fscanf(fp, "%lf\n", &data[i].re)==EOF){
			printf("[ERROR] cant read file to the end.");
			return;
		}
	}
}

void outputData(comp *data, char *filename, int N){
	FILE *fp_re, *fp_im;
	int i = 0;
	char filename_re[256];
	char filename_im[256];

	strcpy(filename_re, filename);
	strcpy(filename_im, filename);
	strcat(filename_re, "_re.csv");
	strcat(filename_im, "_im.csv");

	fp_re = fopen(filename_re, "w");
	fp_im = fopen(filename_im, "w");
	if(fp_re == NULL){
		printf("cant open file.\n");
	}
	if(fp_im == NULL){
		printf("cant open file.\n");
	}

	for(i = 0; i < N; i++){
		if(fprintf(fp_re, "%lf,\n", data[i].re) < 0 ){
			printf("ERROR\n");
			return;
		}
		if(fprintf(fp_im, "%lf,\n", data[i].im) < 0){
			printf("[ERROR] cant write file to the end.");
			return;
		}
	}

	fclose(fp_re);
	fclose(fp_im);
}

void dft(comp *xn, int N, comp *Xk, int a, int b){
	int i,k;

	for(k=0;k<N;k++){
		for(i=0;i<N;i++){
			Xk[k].re += (xn[i].re * cos((2*M_PI/N)*i*k) + a * (xn[i].im * sin((2*M_PI/N)*i*k))) / b;
			Xk[k].im += (xn[i].im * cos((2*M_PI/N)*i*k) - a * (xn[i].re * sin((2*M_PI/N)*i*k))) / b;
		}
	}
}

void ampSpectrum(comp *Xk, int N, double *spec){
	int k=0;

	for(k=0;k<N;k++){
		spec[k] = sqrt(Xk[k].re*Xk[k].re + Xk[k].im*Xk[k].im);
	}
}

void phaSpectrum(comp *Xk, int N, double *spec){
	int k=0;
	for(k=0;k<N;k++){
		spec[k] = atan(Xk[k].im/Xk[k].re);
	
	}
}
