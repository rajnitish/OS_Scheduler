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

void gantt(int ps[][7],int n){
	printf("****************The Gantt Chart****************\n");
	printf(" ");
	for(int i=0;i<n; i++){
		for(int j=0; j<ps[i][bst]; j++){ //printing the top line
			printf("--");
		}
		printf(" ");
	}
	printf("\n");
	//for printing the middle line
	printf("|");
	for(int i=0;i<n; i++){
		for(int j=0; j<ps[i][bst]-1; j++){
			printf(" ");
		}
		printf("P%d",ps[i][pid]);
		for(int j=0; j<ps[i][bst]-1; j++){
			printf(" ");
		}
		printf("|");
	}
	//for printing bottom line
	printf("\n");
	printf(" ");
	for(int i=0;i<n; i++){
		for(int j=0; j<ps[i][bst]; j++){
			printf("--");
		}
		printf(" ");
	}
	//for printing the timeline
	printf("\n");
	printf("0");
	for(int i=0;i<n; i++){
		for(int j=0; j<ps[i][bst]; j++){
			printf("  ");
		}
		if(ps[i][ct]<10)
			printf("%d",ps[i][ct]);

		else{
			printf("%d",ps[i][ct]);

		}
	}
	printf("\n");
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
	printf("\n****************Scheduling Parameters after FIFO******************\n");
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
		int ps_id = -1;
		int mn = INT_MAX;
		for(int i = 0; i < n; i++)
		{
			if(ps[i][arr] <= current_time && is_completed[i] == 0)
			{
				if(ps[i][bst] < mn)
				{
					mn = ps[i][bst];
					ps_id = i;
				}
				if(ps[i][bst] == mn)
				{
					if(ps[i][arr] < ps[ps_id][arr])
					{
						mn = ps[i][bst];
						ps_id = i;
					}
				}
			}
		}
		if(ps_id != -1)
		{
			ps[ps_id][ct]  = current_time + ps[ps_id][bst];
			ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
			ps[ps_id][wt]  = ps[ps_id][tat] - ps[ps_id][bst];
			is_completed[ps_id] = 1;
			completed++;
			current_time = ps[ps_id][ct];

		}
		else
		{ current_time++; }
	}




	printf("\n******************Scheduling Parameters after SJF Non Preemptive ******************\n");
	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n----------------------------------------------\n");

}

void sjf_Preempt(int ps[][7], int n)
{
	int current_time = 0;
	int completed = 0;
	int is_completed[100] = {0};
	int burstRemain[100]={0};

	for(int i = 0 ;i <n ;i++)
		burstRemain[i] = ps[i][bst];



	while(completed != n)
	{
		int ps_id = -1;
		int mn = 10000000;
		for(int i = 0; i < n; i++)
		{
			if(ps[i][arr] <= current_time && is_completed[i] == 0)
			{
				if(burstRemain[i] < mn)
				{
					mn = burstRemain[i];
					ps_id = i;
				}
				if(burstRemain[i] == mn)
				{
					if(ps[i][arr] < ps[ps_id][arr])
					{
						mn = burstRemain[i];
						ps_id = i;
					}
				}
			}
		}
		if(ps_id != -1)
		{
			burstRemain[ps_id] -= 1;
			current_time++;
			if(burstRemain[ps_id] == 0)

			{
				ps[ps_id][ct] = current_time;
				ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
				ps[ps_id][wt] = ps[ps_id][tat] - ps[ps_id][bst];
				is_completed[ps_id] = 1;
				completed++;
			}
		}
		else
		{
			current_time++;
		}
	}


	printf("\n******************Scheduling Parameters after SJF Preemptive ******************\n");

	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n----------------------------------------------\n");

}


int queue[100],front=0,rear=0,size=100;

void Q_PUSH(int data)
{
	if(rear==size)
	{
		//printf("\n Queue is Full");

	}
	else
	{
		queue[rear++] = data;
	}
}

int Q_FRONT()
{
	return queue[front];
}

void Q_POP()
{
	if(front==rear)
	{

		//printf("\n Queue is Empty");
	}
	else
	{
		front++;
		size++;
	}
}

void Q_DISPLAY()
{
	printf("\n");
	for(int i = front;i<rear;i++)
		printf("%d ",queue[i]);
	printf("\n");
}

unsigned char Q_EMPTY()
{
	return (front==rear);
}

void rr(int ps[][7], int n){

	int quant= 0;
	printf("Enter the quantum\n");
	scanf("%d", &quant);


	int current_time = 0;
	int completed = 0;
	int is_completed[100] = {0};
	int burstRemain[100];

	for(int i = 0 ;i <n ;i++)
		burstRemain[i] = ps[i][bst];

	int ps_id;
	Q_PUSH(0);
	int mark[100];
	memset(mark,0,sizeof(mark));
	mark[0] = 1;

	while(completed != n) {
		ps_id = Q_FRONT();
		Q_POP();

		if(burstRemain[ps_id] == ps[ps_id][bst]) {
			if(current_time<ps[ps_id][arr])
				current_time = ps[ps_id][arr];
		}

		if(burstRemain[ps_id]-quant > 0) {
			burstRemain[ps_id] -= quant;
			current_time += quant;
		}
		else {
			current_time += burstRemain[ps_id];
			burstRemain[ps_id] = 0;
			completed++;

			ps[ps_id][ct] = current_time;
			ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
			ps[ps_id][wt] = ps[ps_id][tat] - ps[ps_id][bst];

		}

		for(int i = 1; i < n; i++) {
			if(burstRemain[i] > 0 && ps[i][arr] <= current_time && mark[i] == 0) {
				Q_PUSH(i);
				mark[i] = 1;
			}
		}
		if(burstRemain[ps_id] > 0) {
			Q_PUSH(ps_id);
		}

		if(Q_EMPTY()) {
			for(int i = 1; i < n; i++) {
				if(burstRemain[i] > 0) {
					Q_PUSH(i);
					mark[i] = 1;
					break;
				}
			}
		}


	}


	printf("\n******************Gantt chart after Round-Robin*****************\n");

	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n\n----------------------------------------------\n");


}

/*
int queue[100],front=0,rear=0,size=100;

void Q_PUSH(int data)
{
	if(rear==size)
	{
		//printf("\n Queue is Full");

	}
	else
	{
		queue[rear++] = data;
	}
}

int Q_FRONT()
{
	return queue[front];
}

void Q_POP()
{
	if(front==rear)
	{

		//printf("\n Queue is Empty");
	}
	else
	{
		front++;
		size++;
	}
}

void Q_DISPLAY()
{
	printf("\n");
	for(int i = front;i<rear;i++)
		printf("%d ",queue[i]);
	printf("\n");
}

unsigned char Q_EMPTY()
{
	return (front==rear);
}
 */


void pnp(int ps[][7], int n){
	int completed = 0;
	int current_time = 0;
	int  is_completed[100] = {0};

	while(completed != n)
	{
		int ps_id = -1;
		int mn = INT_MAX;
		for(int i = 0; i < n; i++)
		{
			if(ps[i][arr] <= current_time && is_completed[i] == 0)
			{
				if(ps[i][prior] < mn)
				{
					mn = ps[i][prior];
					ps_id = i;
				}
				else if(ps[i][prior] == mn)
				{
					if(ps[i][arr] < ps[ps_id][arr])
					{
						mn = ps[i][prior];
						ps_id = i;
					}
				}
			}
		}
		if(ps_id != -1)
		{
			ps[ps_id][ct]  = current_time + ps[ps_id][bst];
			ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
			ps[ps_id][wt]  = ps[ps_id][tat] - ps[ps_id][bst];
			is_completed[ps_id] = 1;
			completed++;
			current_time = ps[ps_id][ct];

		}
		else
		{ current_time++; }
	}




	printf("\n******************Process Parameters after Priority Non Preemptive ******************\n");
	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n----------------------------------------------\n");

	display(processes, n);
	calAvg(processes, n, 3);
}
// priority with preemption
void pp(int ps[][7],int n){
	int current_time = 0;
	int completed = 0;
	int is_completed[100] = {0};
	int burstRemain[100]={0};

	for(int i = 0 ;i <n ;i++)
		burstRemain[i] = ps[i][bst];



	while(completed != n)
	{
		int ps_id = -1;
		int mn = 10000000;
		for(int i = 0; i < n; i++)
		{
			if(ps[i][arr] <= current_time && is_completed[i] == 0)// to find the least priority amongst all the visited processes
			{
				if(ps[i][prior] < mn)
				{
					mn = ps[i][prior];
					ps_id = i;
				}
				if(ps[i][prior] == mn)
				{
					if(ps[i][arr] < ps[ps_id][arr])
					{
						mn = ps[i][prior];
						ps_id = i;
					}
				}
			}
		}
		if(ps_id != -1)
		{
			burstRemain[ps_id] -= 1;
			current_time++;
			if(burstRemain[ps_id] == 0)

			{
				ps[ps_id][ct] = current_time;
				ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
				ps[ps_id][wt] = ps[ps_id][tat] - ps[ps_id][bst];
				is_completed[ps_id] = 1;
				completed++;
			}
		}
		else
		{
			current_time++;
		}
	}
	printf("\n******************Process Parameters after Priority with Preemption ******************\n");
	printf("\n\n----------------------------------------------\n");
	printf("PID\tArrival\tBurst\t Wait\t TAT\t CT \t Priority\n");
	printf("\n----------------------------------------------\n");
	display(processes, n);
	calAvg(processes, n, 4);
}
void priority(int processes[][7],int n,int option){

	if(option == 0) // Internal Priority with Preemption
	{
		for(int i=0; i<n;i++){
			processes[i][6]=(rand() % 4)+2;
		}

		pp(processes,n);
	}
	else if(option == 1) // Internal Priority without Preemption
	{
		for(int i=0; i<n;i++){
			processes[i][6]=(rand() % 4)+2;
		}
		pnp(processes,n);
	}
	else if(option == 2) // External Priority with Preemption
	{
		printf("Kindly enter the priority of each process: 0 is the highest priority");
		for(int i=0; i<n;i++){
			scanf("%d",&processes[i][6]);
		}
		pp(processes,n);
	}
	else if(option == 3) // External Priority without Preemption
	{
		printf("Kindly enter the priority of each process: 0 is the highest priority");
		for(int i=0; i<n;i++){
			scanf("%d",&processes[i][6]);
		}
		pnp(processes,n);
	}

	//display(processes,n);
	//calAvg(processes, n, 3);
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
		sjf_No_Preempt(q2,y);
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
	//int test[5][7] = { {0,0,5,0,0,0,0},{0,1,3,0,0,0,0},{0,2,1,0,0,0,0},{0,3,2,0,0,0,0},{0,4,3,0,0,0,0} };


	*n = 6;
	int test[6][7] = { {0,0,4,0,0,0,0},{0,1,5,0,0,0,0},{0,2,2,0,0,0,0},{0,3,1,0,0,0,0},{0,4,6,0,0,0,0},{0,6,3,0,0,0,0} };


	for(int i = 0;i<*n;i++)
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

	runMyTestCase(&n);
	rearrange(processes,n); // arranging data as per the arrival time

	char ch = 'Y';
	while('Y' == ch || 'y' == ch){

		fflush(stdin);


		printf("\nThe Process List are as follows :-\n");
		printf("PID\tArrival\tBurst\tWait\tTAT\tCT \tPriority \n");
		display(processes,n);

		printf("\nEnter 0 : FIFO 						Scheduling\n");
		printf("Enter 1 : SJF NON-PREEMPT				Scheduling\n");
		printf("Enter 2 : SJF PREEMPTIVE				Scheduling\n");
		printf("Enter 3 : RR   						Scheduling\n");
		printf("Enter 4 : PRIORITY BASED INTERNAL PREEMPTIVE		Scheduling\n");
		printf("Enter 5 : PRIORITY BASED INTERNAL NON-PREEMPT		Scheduling\n");
		printf("Enter 6 : PRIORITY BASED EXTERNAL PREEMPTIVE		Scheduling\n");
		printf("Enter 7 : PRIORITY BASED EXTERNAL NON-PREEMPT		Scheduling\n");
		printf("Enter 8 : MULTI LEVEL Q 				Scheduling\n");
		printf("Enter 9 : MULTI LEVEL Q FEEDBACK 			Scheduling\n");
		printf("Enter 10 : LINUX SCHEDULER 				Scheduling\n");



		int cmdfound=-1;
		scanf("%d",&cmdfound);

		switch(cmdfound){
		case 0:
		{
			ff(processes, n);
			display(processes, n);
			gantt(processes,n);
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
		case 5:
		case 6:
		case 7:
		{
			priority(processes ,n,cmdfound-4);
		}
		break;
		case 8:
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
