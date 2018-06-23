//＊＊＊H30年度・DSP1-4・番号04＊＊＊
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM 2048
#define CROSS_NUM 701
#define AUTO_NUM 71


double correlation(double *data1,double *data2,int num);
double aveMatrix(double *data,int num);
void removeDCComponent(double *data,int num,double *ans);
int inputData(char *filename,double *data);
void productMatrix(double *a,double *b,int num,double *ans);
double inSumMatrix(double *a,int num);
double normalizationMatrix(double *a,int num);
double crossCorrelation(double *data1,double *data2,int num,double *array);

int whe = 0;

int main(){

	char *txt1="txt/txt1.txt";
	char *txt2="txt/txt2.txt";
	char *txt3="txt/txt3.txt";
	int crossNum = 701,autoNum = 71,i=0;
	double buf=0;
	double crossCorre[CROSS_NUM] = {0};
	double autoCorre[AUTO_NUM] = {0};
	double data1[NUM] = {0},data2[NUM] = {0},data3[NUM] = {0};
	FILE *fp1,*fp2;

	inputData(txt1,data1);
	inputData(txt2,data2);
	inputData(txt3,data3);

	buf=crossCorrelation(data1,data2,crossNum,crossCorre);
	printf("相互相関係数：\n");
	for(i=0;i<8;i++){
		printf("n = %5d : txt1 * txt2 = %lf\n",i*100,crossCorre[i*100]);	
	}

	buf=crossCorrelation(data1,data1,autoNum,autoCorre);
	printf("自己相関係数：\n");
	for(i=0;i<8;i++){
		printf("n = %5d : txt3 * txt3 = %lf\n",i*10,autoCorre[i*10]);	
	}
	
	fp1=fopen("output1.txt","w");
	for(i=0;i<701;i++){
		fprintf(fp1,"%lf\n",crossCorre[i]);
	}
	fp2=fopen("output2.txt","w");
	for(i=0;i<71;i++){
		fprintf(fp2,"%lf\n",autoCorre[i]);
	}

	fclose(fp1);
	fclose(fp2);	

	return 0;

}

double crossCorrelation(double *data1,double *data2,int num,double *array){	
/*	int bufNum = num;
	double buf = 0;
	double ans = -1;

	for(int i=0;i<num;i++){
		buf = 
		if(ans < buf){
			ans = buf;
		}
	}
*/
	int i=0,j=0;
	double ans = -1,sum =0;

	for(;i<num;i++){
		sum = 0;
		for(j=0;j<num;j++){
			sum+=data1[j]*data2[j+i];
		}
		sum = (double)(sum/(double)num);
		array[i]=sum;
		if(ans < sum){
			ans = sum;
			whe = i;
		}
	}
	return ans;
}

double correlation(double *data1,double *data2,int num){
	double buf1=0,buf2=0,buf3=0,ans[512];

	buf1 = normalizationMatrix(data1,num);
	buf2 = normalizationMatrix(data2,num);
	productMatrix(data1,data2,num,ans);
	buf3 = inSumMatrix(ans,num);

	return (double)((double)buf3/(buf1*buf2));
}

double aveMatrix(double *data,int num){
	return (double)(inSumMatrix(data,num)/num);
}

void removeDCComponent(double *data,int num,double *ans){
	int cnt=0;
	double buf=aveMatrix(data,num);
	for(;num>0;num--){
		ans[cnt] = data[cnt]-buf;
		cnt++;
	}
	return;
}

int inputData(char *filename,double *data){
	FILE *fp;
	int i=0;
	int num=0;
	int ref=0;
	fp=fopen(filename,"r");
	if(fp==NULL){
		printf("ERROR : cant open files.\n");
		exit(1);
	}
	while((ref=fscanf(fp,"%lf",&data[i]))!=EOF){
		i++;
		num+=ref;
	}
	data[i]=-1.0;
	return num;
}

void productMatrix(double *a,double *b,int num,double *ans){
	int cnt=0;
	for(;num>0;num--){
		ans[cnt]=a[cnt]*b[cnt];
		cnt++;
	}
	return;
}

double normalizationMatrix(double *a,int num){
	int cnt=0;
	double sum=0;
	double ans=0;
	for(;num>0;num--){
		sum+=a[cnt]*a[cnt];
		cnt++;
	}
	ans=sqrt(sum);
	return ans;
}

double inSumMatrix(double *a,int num){
	int cnt=0;
	double sum=0;
	for(;num>0;num--){
		sum+=a[cnt];
		cnt++;
	}
	return sum;
}
