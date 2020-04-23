/*
 ============================================================================
 Name        : schedule.c
 Author      : monika
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include<limits.h>
#define M 100
#define N 100
int processes[M][7];
int fifo[M][7];
int sjff[M][7];
int ro[M][7];
//int prior[M][7];
int priorp[M][7];
int mlq[M][7];
int q1[M][7];
int q2[M][7];
int q3[M][7];

enum param{pid,arr,bst,wt,tat,ct,prior};

double avg_wt[8] ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, avg_tat[8] ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

void display(int processes[][7],int n)
{

	for(int i= 0; i< n;i++)
	{
		for (int k=0; k<7; k++)
		{
			printf("%d \t", processes[i][k]);
		}
		printf("\n");
	}
	printf("\n\n----------------------------------------------\n");


}
// Calculate total waiting time and total turn around time
void calAvg(int processes[][7],int n, int a ){
	int total_wt=0, total_tat=0;
	for (int i=0; i<n; i++)
	{
		total_wt  += processes[i][3];
		total_tat += processes[i][4];
	}
	avg_wt[a]=(float)total_wt / (float)n;// vector to hold the weighting time of all the scheduling at different index values
	avg_tat[a]=(float)total_tat / (float)n;// vector to hold the weighting time of all the scheduling at different index values
	printf("Average waiting time = %f",avg_wt[a]);
	printf("\n");
	printf("Average turn around time = %f \n",avg_tat[a]);
}

void ff( int ps[][7], int n)
{
	ps[0][wt]=0;
	ps[0][tat]=ps[0][bst];
	ps[0][ct]=ps[0][arr]+ps[0][bst]+ps[0][wt];

	for(int i=1; i<n;i++)
	{
		if(ps[i-1][ct]<ps[i][arr])
			ps[i][ct]=ps[i][arr]+ps[i][bst];
		else
			ps[i][ct]=ps[i-1][ct]+ps[i][bst];

		ps[i][tat]=ps[i][ct]-ps[i][arr];

		ps[i][wt]=ps[i][tat]-ps[i][bst];

		if(ps[i][3]<0)
		{
			printf("\nNOT CONVINCED\n");
			ps[i][3]=0;
		}
	}

	//Display processes along with all details
	printf("\n****************Gantt chart after FIFO******************\n");
	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \tPriority \n");
	printf("\n----------------------------------------------\n");


}

void change(int *a, int *b)
{
	int temp =*a;
	*a=*b;
	*b=temp;
}

void rearrange(int p[][7], int n)
{
	for(int i= 0; i<n-1; i++)
	{
		for (int j = i; j< n; j++ )
		{
			if(p[i][1]>p[j][1])
			{ for(int k=0; k<7; k++)
			{
				change(&p[i][k],&p[j][k]);
			}

			}
		}
	}
	for(int i=0;i<n;i++){
		processes[i][0]=i+1;
	}
	/*	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n\n----------------------------------------------\n");
	display(p,n);*/
}

void sjf_No_Preempt(int ps[][7], int n)
{
	int completed = 0;
	int current_time = 0;
	int  is_completed[100] = {0};

	while(completed != n)
	{
		int idx = -1;
		int mn = INT_MAX;
		for(int i = 0; i < n; i++)
		{
			if(ps[i][arr] <= current_time && is_completed[i] == 0)
			{
				if(ps[i][bst] < mn)
				{
					mn = ps[i][bst];
					idx = i;
				}
				if(ps[i][bst] == mn)
				{
					if(ps[i][arr] < ps[idx][arr])
					{
						mn = ps[i][bst];
						idx = i;
					}
				}
			}
		}
		if(idx != -1)
		{
			ps[idx][ct]  = current_time + ps[idx][bst];
			ps[idx][tat] = ps[idx][ct] - ps[idx][arr];
			ps[idx][wt]  = ps[idx][tat] - ps[idx][bst];
			is_completed[idx] = 1;
			completed++;
			current_time = ps[idx][ct];

		}
		else
		{ current_time++; }
	}




	printf("\n******************Gantt chart after SJF Non Preemptive ******************\n");
	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n----------------------------------------------\n");

}

void sjf_Preempt(int ps[][7], int n)
{
	int current_time = 0;
	int completed = 0;
	int is_completed[100] = {0};
	int burst_remaining[100]={0};

	for(int i = 0 ;i <n ;i++)
		burst_remaining[i] = ps[i][bst];



	while(completed != n)
	{
		int idx = -1;
		int mn = 10000000;
		for(int i = 0; i < n; i++)
		{
			if(ps[i][arr] <= current_time && is_completed[i] == 0)
			{
				if(burst_remaining[i] < mn)
				{
					mn = burst_remaining[i];
					idx = i;
				}
				if(burst_remaining[i] == mn)
				{
					if(ps[i][arr] < ps[idx][arr])
					{
						mn = burst_remaining[i];
						idx = i;
					}
				}
			}
		}
		if(idx != -1)
		{
			burst_remaining[idx] -= 1;
			current_time++;
			if(burst_remaining[idx] == 0)

			{
				ps[idx][ct] = current_time;
				ps[idx][tat] = ps[idx][ct] - ps[idx][arr];
				ps[idx][wt] = ps[idx][tat] - ps[idx][bst];
				is_completed[idx] = 1;
				completed++;
			}
		}
		else
		{
			current_time++;
		}
	}


	printf("\n******************Gantt chart after SJF Preemptive ******************\n");

	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n----------------------------------------------\n");

}

void sjf(int processes[][7], int n)
{// arranging data as per the burst time
	for(int i=0; i<n;i++)
	{
		for(int j=i+1; j<n; j++)
		{
			if(processes[i][1]==processes[j][1])
			{
				if(processes[i][2]>processes[j][2])
				{
					for(int k=0; k<7; k++)
					{
						change(&processes[i][k],&processes[j][k]);
					}
				}
			}
		}

		if(i==0)
		{
			processes[i][3]=0;//wt
			processes[i][4]=processes[i][2];//tat
			processes[i][5]=processes[i][1]+processes[i][2]+processes[i][3];//ct
		}
		else
		{
			if(processes[i-1][5]<processes[i][1])
				processes[i][5]=processes[i][1]+processes[i][2];
			else
				processes[i][5]=processes[i-1][5]+processes[i][2];

			processes[i][4]=processes[i][5]-processes[i][1];

			processes[i][3]=processes[i][4]-processes[i][2];//wt

			if(processes[i][3]<0)
				processes[i][3]=0;
		}
	}
	printf("\n**********************************Gantt chart after SJF********************************\n");

	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n\n----------------------------------------------\n");

}


void rr(int processes[][7], int n){
	int quant= 0, sum=0;
	int times[n];// array of burst time to keep a track
	int x=0;
	printf("Enter the quantum\n");
	scanf("%d", &quant);
	for(int i=0; i<n; i++){
		sum += processes[i][2];
		times[i]= processes[i][2]; // maintaining an array of burst time
		processes[i][1]=0;
	}

	while(sum !=x ){
		for(int i= 0; i<n;i++){
			if(times[i]> quant){ //burst time is greater then quantum
				times[i]-=quant;
				processes[i][5]=x+ quant;
				x +=quant;

			}
			else if (times[i]==0){
				printf("\n");
			}

			else{
				processes[i][5]= x + times[i];
				x +=times[i];
				times[i]=0;

			}
		}
	}
	for(int i=0; i<n;i++){
		processes[i][4]= processes[i][5];
		processes[i][3]=processes[i][5]- processes[i][2];
	}
	printf("\n**************************Gantt chart after Round-Robin************************\n");

	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n\n----------------------------------------------\n");


}
void pnp(int processes[][7], int n){
	//arranging Data as per its priority
	for(int i=0; i<n;i++)
	{
		for(int j=i+1; j<n; j++)
		{
			if(processes[i][1]==processes[j][1])
			{
				if(processes[i][6]>processes[j][6])
				{
					for(int k=0; k<7; k++)
					{
						change(&processes[i][k],&processes[j][k]);
					}
				}
			}
		}
		if(i==0)// computing stats for the first process in the ready queue
		{
			processes[i][3]=0;//wt
			processes[i][4]=processes[i][2];//tat
			processes[i][5]=processes[i][1]+processes[i][2]+processes[i][3];//ct
		}
		else
		{
			if(processes[i-1][5]<processes[i][1]) // checking whether next processes arrives after the completion of the previous process
				processes[i][5]=processes[i][1]+processes[i][2];
			else
				processes[i][5]=processes[i-1][5]+processes[i][2];

			processes[i][4]=processes[i][5]-processes[i][1];

			processes[i][3]=processes[i][4]-processes[i][2];//wt

			if(processes[i][3]<0)
				processes[i][3]=0;
		}
	}
	printf("\n**********************************Gantt chart after Priority Scheduling without Preemption********************************\n");
	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n\n----------------------------------------------\n");
	display(prior, n);
	calAvg(prior, n, 3);
}
// priority with preemption
/*void pp(int processes[][7],int n){
	int timer =1;
	for(int i=0; i<n-1;i++)
	{for(int j=i+1; j<n;j++){
		if(processes[i][1]==processes[j][1]){ // if arrival time is same then arrange as per the priority
			if(processes[i][6]==processes[j][6]){
				for(int k=0; k<7; k++)
				{
					change(&processes[i][k],&processes[j][k]);
				}
			}
		}
	}
	}
	int turn[n];
	for(int i=0;i<n;i++){
		turn[i]= processes[i][1];
	}
	while(turn!=0)
	for(int i=0; i<n; i++)
	{ int low= processes[0][6];
			if(processes[i][1] <= timer && low> processes[i][6]){
			processes[turn[i]][2]-=timer;
			= i;
			timer++;
		}
		else{
			timer++;
		}
	}
display(priorp, n);
calAvg(priorp, n, 4);
}*/
void priority(int processes[][7],int procp[][7],int n){

	char a,z;
	printf("Press Y for Internal Priority else N for External Priority");
	scanf("%c",&a);
	if(a=='Y' || a=='y'){
		for(int i=0; i<n;i++){
			processes[i][6]=(rand() % 4)+2;
		}
	}
	else if(a=='N' || a=='n'){
		printf("Kindly enter the priority of each process: 0 is the highest priority");
		for(int i=0; i<n;i++){
			scanf("%d",&processes[i][6]);
		}
	}
	else{
		printf("Wrong Entry");}

	printf("Press Y for Priority with Preemption else N for Non preemption");
	scanf(" %c", &z);
	printf("input: %c",z);
	if(z =='Y' || z =='y'){
		printf("Priority with Preemption Started");
		//pp(procp,n);
	}
	else if(z =='N' || z=='n'){
		printf("Priority without Preemption Started");
		pnp(processes,n);}
	else{
		printf("Wrong Entry");}

	display(processes,n);
	calAvg(processes, n, 3);
}

void multi(int processes[][7], int n){
	printf("Dividing the processes in 3 levels of queue\n");
	for(int i=0; i<n;i++){
		processes[i][6]=(rand() % 7); // randomly assigning the priorities
	}
	int q1[n][7], q2[n][7], q3[n][7], times[n], ttimes[n];
	int x=0,y=0,z=0,a=0,b=0, tsum=0, sum=0,avg_w,avg_t,q, quant=3;
	// switch case to assign different queues to each process as per it priority i.e 0-2  assign to q1 and so on..
	for(int i=0;i<n;i++){
		q=processes[i][6];
		switch(q){
		case 0 ... 2:	for(int k=0;k<7;k++){
			q1[x][k]=processes[i][k];
		}
		x++;
		break;
		case 3 ... 5: for(int k=0;k<7;k++){
			q2[y][k]=processes[i][k];}
		y++;
		break;
		default : for(int k=0;k<7;k++){
			q3[z][k]=processes[i][k];}
		z++;
		break;
		}
	}
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	display(mlq, n);
	for(int i=0; i<n; i++){
		tsum += processes[i][2];
		ttimes[i]= processes[i][2];
		processes[i][1]=0;
	}

	// The three queues are getting implemented using round robin
	//while(tsum != b){
	//processing the first queue
	for(int i=0;i<x;i++){
		sum+= q1[i][2];
		times[i]= q1[i][2];// copying the burst time
		q1[i][1]=0;}
	if(x !=0)// checking if the queue is not empty
	{
		printf("Processing the first queue\n");
		while(sum !=a){
			for(int i= 0; i<x;i++){
				if(times[i]> quant){
					times[i]-=quant;
					q1[i][5]= a + quant;
					a +=quant;

				}
				else if (times[i]==0){
					printf("\n");
				}

				else{
					q1[i][5]= a + times[i];
					a +=times[i];
					times[i]=0;

				}
			}
		}
		for(int i=0; i<n;i++){
			q1[i][4]= q1[i][5];
			q1[i][3]=q1[i][5]- q1[i][2];
		}

		printf("\n*********************Gantt chart after Round-Robin  with MLQ*******************\n");

		printf("\n\n----------------------------------------------\n");
		printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
		printf("\n\n----------------------------------------------\n");

		display(q1, x);
		calAvg(q1, x, 5);
	}
	else
		printf("First queue is empty\n");
	if (y != 0){
		printf("Processing  the second queue\n");
		sjf(q2,y);
		display(q2, y);
		calAvg(q2, y, 6);}
	else
		printf("Second queue is empty\n");
	if(z!=0){
		printf("Processing  the third queue\n");
		ff(q3,z);
		display(q3, z);
		calAvg(q3, z, 7);}
	else
		printf("Third queue is empty\n");
	avg_w=avg_wt[5]+avg_wt[6]+avg_wt[7];
	avg_t=avg_tat[5]+avg_tat[6]+avg_tat[7];
	avg_wt[5]=(float)avg_w / (float)3;// vector to hold the weighting time of all the scheduling at different index values
	avg_tat[5]=(float)avg_t / (float)3;// vector to hold the weighting time of all the scheduling at different index values

	printf("\n\n");
	printf("Average waiting time and Turn around time after implementing multi-queues\n");
	printf("Average waiting time = %d\n",avg_wt[5]);
	printf("Average turn around time = %d \n",avg_tat[5]);

}


void runMyTestCase(int *n)
{
	//*n = 5;
	//int test[5][7] = { {0,1,7,0,0,0,0},{0,2,5,0,0,0,0},{0,3,1,0,0,0,0},{0,4,2,0,0,0,0},{0,5,8,0,0,0,0} };


	*n = 6;
	int test[6][7] = { {0,0,7,0,0,0,0},{0,1,5,0,0,0,0},{0,2,3,0,0,0,0},{0,3,1,0,0,0,0},{0,4,2,0,0,0,0},{0,5,1,0,0,0,0} };


	for(int i = 0;i<6;i++)
		for(int j = 0 ;j <7;j++)
			processes[i][j] = test[i][j];

}

int main(int argc, char *argv[])
{

	int randomNo;
	srand(time(0));
	int n=(rand() % 4)+2;// no. of processes
	printf("\033[2J\033[0;0H");

	for (int i = 0; i < n; i++) {
		randomNo = (rand() % 10) + 1;
		processes[i][pid] =0 ;
		processes[i][arr]=(rand() % 10);
		processes[i][bst]= randomNo;
		processes[i][wt]=0;
		processes[i][tat]=0;
		processes[i][ct]=0;
		processes[i][prior]=0;
	}

	//runMyTestCase(&n);  // This is custome driver commented by monika

	// This is my comment
	rearrange(processes,n); // arranging data as per the arrival time

	char ch = 'Y';
	while('Y' == ch || 'y' == ch){

		fflush(stdin);


		printf("\nThe Process List are as follows :-\n");
		printf("PID\tArrival\tBurst\tWait\tTAT\tCT \tPriority \n");
		display(processes,n);

		printf("\nEnter 0 : FIFO 				Scheduling\n");
		printf("Enter 1 : SJF NON-PREEMPT		Scheduling\n");
		printf("Enter 2 : SJF PREEMPTIVE		Scheduling\n");
		printf("Enter 3 : RR   				Scheduling\n");
		printf("Enter 4 : PRIORITY BASED		Scheduling\n");
		printf("Enter 5 : MULTI LEVEL Q 		Scheduling\n");
		printf("Enter 6 : MULTI LEVEL Q FEEDBACK 	Scheduling\n");
		printf("Enter 7 : LINUX SCHEDULER 		Scheduling\n");



		int cmdfound=-1;
		scanf("%d",&cmdfound);

		switch(cmdfound){
		case 0:
		{
			ff(processes, n);
			display(processes, n);
			calAvg(processes, n, 0);
		}
		break;
		case 1:
		{
			sjf_No_Preempt(processes, n);
			display(processes, n);
			calAvg(processes, n, 1);
		}
		break;
		case 2:
		{
			sjf_Preempt(processes, n);
			display(processes, n);
			calAvg(processes, n, 1);
		}
		break;
		case 3:
		{
			rr(processes,n);
			display(processes, n);
			calAvg(processes, n, 2);
		}
		break;
		case 4:
		{
			priority(processes,priorp ,n);
		}
		break;
		case 5:
		{
			multi(mlq, n);
		}

		break;
		}





		printf("\n Want to see another schdeduling algo.... Press Y/y else N/n for quit\n");

		char x;
		scanf("\n%s",&x);
		ch = x;

		printf("\033[2J\033[0;0H");
	}
	return 0;
}
