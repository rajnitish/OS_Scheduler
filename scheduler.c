/*
 ============================================================================
 Name        : schedule.c
 Author      : monika
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
// test msg jsajhsajjksds

// New changes ..after push we will see in browser
#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include"Queue.h"

#define M 100
#define N 100
int processes[M][7];
int ro[M][7];
int priorp[M][7];
#define LARGE_BURST 1000000

#define RUN_MY_TESTCASE

enum param{pid,arr,bst,wt,tat,ct,prior};

double avg_wt[10] ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, avg_tat[10] ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

void display(int processes[][7],int n)
{
	printf("\nPID\tArrival\tBurst\tWait \tTAT \tCT  \tPriority \n");
	printf("---\t-------\t-----\t---- \t--- \t--  \t-------- \n");

	for(int i= 0; i< n;i++)
	{
		for (int k=0; k<7; k++)
		{
			printf("%d \t", processes[i][k]);
		}
		printf("\n");
	}
	printf("---------------------------------------------------------\n\n");

}
// Calculate total waiting time and total turn around time
void calAvg(int processes[][7],int n, int a ){
	if(n<=0) return;
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

void performFCFS( int ps[][7], int n)
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
	printf("\n****************Scheduling Parameters after FCFS******************\n");

}

void change(int *a, int *b)
{
	*a = *a +*b;
	*b = *a - *b;
	*a = *a - *b;
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
}

void performSJF_NON_PREEMPT(int ps[][7], int n)
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

	printf("\n*********Scheduling Parameters after SJF Non Preemptive **********\n");

}

void performSJF_PREEMPT(int ps[][7], int n)
{
	int current_time = 0;
	int ps_completed = 0;
	int visited[100] = {0};
	int burstRemain[100]={0};

	for(int i = 0 ;i <n ;i++)
		burstRemain[i] = ps[i][bst];

	while(n != ps_completed)
	{

		int remainBurst = LARGE_BURST;
		int ps_id = -1;

		for(int i = 0; i < n; i++)
		{
			if(0 == visited[i] && ps[i][arr] <= current_time)
			{
				if(remainBurst > burstRemain[i])
				{
					ps_id = i;
					remainBurst = burstRemain[i];
				}
				if(remainBurst == burstRemain[i])
				{
					if(ps[i][arr] < ps[ps_id][arr])
					{
						ps_id = i;
						remainBurst = burstRemain[i];
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
				visited[ps_id] = 1;
				ps_completed++;
			}
		}
		else
		{
			current_time++;
		}
	}

	printf("\n*********Scheduling Parameters after SJF Preemptive **********\n");

}


void performRR(int ps[][7], int n,int quant){

	int currentTime = 0;
	int ps_completed = 0;
	int burstRemain[100];
	int visited[100]={0};

	for(int i = 0 ;i <n ;i++)
		burstRemain[i] = ps[i][bst];

	int ps_id;
	visited[0] = 1;
	struct Q readyQ;
	Q_init(&readyQ);
	Q_PUSH(&readyQ,0);

	while(n != ps_completed) {
		ps_id = Q_FRONT(&readyQ);
		Q_POP(&readyQ);

		if(burstRemain[ps_id] == ps[ps_id][bst]) {
			if(currentTime<ps[ps_id][arr])
				currentTime = ps[ps_id][arr];
		}

		int ps_remainTimeNow = burstRemain[ps_id]- quant;
		if(0<ps_remainTimeNow) {
			currentTime += quant;
			burstRemain[ps_id] = ps_remainTimeNow;
		}
		else {
			ps_completed++; // This process finished

			ps[ps_id][ct] = currentTime + burstRemain[ps_id];
			ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
			ps[ps_id][wt] = ps[ps_id][tat] - ps[ps_id][bst];

			currentTime = ps[ps_id][ct];
			burstRemain[ps_id] = 0;

		}

		for(int i = 1; i < n; i++) {
			if(0 == visited[i] && ps[i][arr] <= currentTime && burstRemain[i] > 0 ) {
				visited[i] = 1;
				Q_PUSH(&readyQ,i);

			}
		}
		if(burstRemain[ps_id] > 0) {
			Q_PUSH(&readyQ,ps_id);
		}

		if(Q_EMPTY(&readyQ)) {
			for(int i = 1; i < n; i++) {
				if(burstRemain[i] > 0) {
					visited[i] = 1;
					Q_PUSH(&readyQ,i);
					break;
				}
			}
		}


	}



	printf("\n*********Scheduling Parameters after Round Robin **********\n");

}

void performPRIORITY_NON_PREEMPT(int ps[][7], int n){
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

	printf("\n*********Process Parameters after Priority Non Preemptive ********\n");
	display(processes, n);
	calAvg(processes, n, 4);
}
// priority with preemption
void performPRIORITY_PREEMPT(int ps[][7],int n){
	int currentTime = 0;
	int ps_completed = 0;
	int visited[100] = {0};
	int burstRemain[100]={0};

	for(int i = 0 ;i <n ;i++)
		burstRemain[i] = ps[i][bst];



	while(n != ps_completed)
	{

		int remainBurst = LARGE_BURST;
		int ps_id = -1;

		for(int i = 0; i < n; i++)
		{
			if(0 == visited[i] && currentTime >= ps[i][arr])// to find the least priority amongst all the visited processes
			{
				if(ps[i][prior] < remainBurst)
				{
					remainBurst = ps[i][prior];
					ps_id = i;
				}
				if(ps[i][prior] == remainBurst)
				{
					if(ps[i][arr] < ps[ps_id][arr])
					{
						remainBurst = ps[i][prior];
						ps_id = i;
					}
				}
			}
		}
		if(ps_id != -1)
		{
			burstRemain[ps_id] -= 1;
			currentTime++;
			if(burstRemain[ps_id] == 0)

			{
				ps[ps_id][ct] = currentTime;
				ps[ps_id][tat] = ps[ps_id][ct] - ps[ps_id][arr];
				ps[ps_id][wt] = ps[ps_id][tat] - ps[ps_id][bst];
				visited[ps_id] = 1;
				ps_completed++;
			}
		}
		else
		{
			currentTime++;
		}
	}

	printf("\n*********Process Parameters after Priority Preemptive ********\n");
	display(processes, n);
	calAvg(processes, n, 5);
}
void performPRIORITY(int processes[][7],int n,int option){

	if(option == 0) // Internal Priority with Preemption
	{

#ifndef RUN_MY_TESTCASE
		for(int i=0; i<n;i++){
			processes[i][6]=(rand() % 4)+2;
		}
#endif

		performPRIORITY_PREEMPT(processes,n);
	}
	else if(option == 1) // Internal Priority without Preemption
	{

#ifndef RUN_MY_TESTCASE
		for(int i=0; i<n;i++){
			processes[i][6]=(rand() % 4)+2;
		}
#endif
		performPRIORITY_NON_PREEMPT(processes,n);
	}
	else if(option == 2) // External Priority with Preemption
	{
		printf("Kindly enter the priority of each process: 0 being considered as highest priority\n");
		for(int i=0; i<n;i++){
			printf("Enter priority for Process P%d\t",i+1);
			scanf("%d",&processes[i][6]);
		}
		performPRIORITY_PREEMPT(processes,n);
	}
	else if(option == 3) // External Priority without Preemption
	{
		printf("Kindly enter the priority of each process: 0 being considered as highest priority\n");
		for(int i=0; i<n;i++){
			printf("Enter priority for Process P%d\t",i+1);
			scanf("%d",&processes[i][6]);
		}
		performPRIORITY_NON_PREEMPT(processes,n);
	}

}

void performMULTI_LEVEL_QUEUE(int processes[][7], int n){
	printf("Dividing the processes in 3 levels of queue based upon priority \n");
	printf("[0 - 2] Upper Level (First to serve)  [3 - 5] Mid Level (Next to serve) [Above 6] Last Level\n ");

#ifndef RUN_MY_TESTCASE
	for(int i=0; i<n;i++){
		processes[i][6]=(rand() % 7); // randomly assigning the priorities
	}
#endif

	int q1[n][7], q2[n][7], q3[n][7];
	int x=0,y=0,z=0,q, quant=3;
	// switch case to assign different queues to each process as per it priority i.e 0-2  assign to q1 and so on..

	for(int i=0;i<n;i++)
	{
		q = processes[i][6];

		switch(q)
		{
		case 0 ... 2:
		for(int k=0;k<7;k++)
		{
			q1[x][k]=processes[i][k];
		}
		x++;
		break;
		case 3 ... 5:
		for(int k=0;k<7;k++)
		{
			q2[y][k]=processes[i][k];
		}
		y++;
		break;
		default :
			for(int k=0;k<7;k++)
			{
				q3[z][k]=processes[i][k];
			}
			z++;
			break;
		}
	}



	/******** Queues Displaying Levels********/
	printf("\nPID\tArrival\tBurst\tWait \tTAT \tCT  \tPriority \n");
	printf("---\t-------\t-----\t---- \t--- \t--  \t-------- \n");
	if(x == 0) printf("   NO PROCESS IN UPPER LEVEL QUEUE    \n");
	for(int i= 0; i< x;i++){
		for (int k=0; k<7; k++)
			printf("%d \t", q1[i][k]);
		printf("\n"); }
	printf("---------------------------------------------------------\n");

	if(y == 0) printf("    NO PROCESS IN MID LEVEL QUEUE     \n");
	for(int i= 0; i< y;i++){
		for (int k=0; k<7; k++)
			printf("%d \t", q2[i][k]);
		printf("\n");}
	printf("---------------------------------------------------------\n");
	if(z == 0) printf("   NO PROCESS IN LAST LEVEL QUEUE    \n");
	for(int i= 0; i< z;i++){
		for (int k=0; k<7; k++)
			printf("%d \t", q3[i][k]);
		printf("\n");}
	printf("---------------------------------------------------------\n");
	/***************************************/


	if(x !=0)// checking if the queue is not empty
	{
		printf("\nProcessing the first queue\n");
		performRR(q1,x,quant);
		display(q1, x);
		calAvg(q1, x, 6);
	}
	else
		printf("\nFirst queue is empty\n");

	if (y != 0)
	{
		printf("\nProcessing  the second queue\n");
		performSJF_NON_PREEMPT(q2,y);
		display(q2, y);
		calAvg(q2, y, 7);
	}
	else
		printf("\nSecond queue is empty\n");

	if(z!=0)
	{
		printf("\nProcessing  the third queue\n");
		performFCFS(q3,z);
		display(q3, z);
		calAvg(q3, z, 8);
	}
	else
		printf("\nThird queue is empty\n");

	int active_count = ((x>0)?1:0)+((y>0)?1:0)+((z>0)?1:0);
	avg_wt[6]=( avg_wt[6]+avg_wt[7]+avg_wt[8] )/active_count;
	avg_tat[6]=(avg_tat[6]+avg_tat[7]+avg_tat[8])/active_count;
	printf("\n\n");
	printf("Average waiting time and Turn around time after implementing multi-queues\n");
	printf("Average waiting time = %0.2f\n",avg_wt[6]);
	printf("Average turn around time = %0.2f \n",avg_tat[6]);

}


void updateTransposedProcess(int *q1,int x,int *q2,int y)
{
	for(int i=0;i<x;i++){
		for(int j=0;j<y;j++){
			if(*(q1+i*7 +pid) ==*(q2+j*7 +pid)){
				*(q2+j*7 +arr)= *(q1+i*7 +ct);
			}
		}
	}
}
void performMULTI_LEVEL_FEEDBACK_QUEUE(int processes[][7], int n){
	printf("Dividing the processes in 3 levels of queue based upon priority \n");
	printf("[0 - 2] Upper Level (First to serve)  [3 - 5] Mid Level (Next to serve) [Above 6] Last Level\n ");

#ifndef RUN_MY_TESTCASE
	for(int i=0; i<n;i++){
		processes[i][6]=(rand() % 7); // randomly assigning the priorities
	}
#endif

	int q1[n][7], q2[n][7], q3[n][7];
	int x=0,y=0,z=0,q;
	// switch case to assign different queues to each process as per it priority i.e 0-2  assign to q1 and so on..

	for(int i=0;i<n;i++)
	{
		q = processes[i][6];

		switch(q)
		{
		case 0 ... 2:
		if(processes[i][bst]<=4)
			for(int k=0;k<7;k++)
			{
				q1[x][k]=processes[i][k];
			}
		else{
			processes[i][bst]=processes[i][bst]-4;
			for(int k=0;k<7;k++)
			{
				q1[x][k]=processes[i][k];
			}
			q1[x][bst]=4;
			processes[i][prior]=3;
			for(int k=0;k<7;k++)
			{
				processes[n][k]=processes[i][k];
			}
			n=n+1;
		}
		x++;
		break;
		case 3 ... 5:
		if(processes[i][bst]<=8)
			for(int k=0;k<7;k++)
			{
				q2[y][k]=processes[i][k];
			}
		else{
			processes[i][bst]=processes[i][bst]-8;
			for(int k=0;k<7;k++)
			{
				q2[y][k]=processes[i][k];
			}
			q2[y][bst]=8;
			processes[i][prior]=6;
			for(int k=0;k<7;k++)
			{
				processes[n][k]=processes[i][k];
			}
			n=n+1;
		}
		y++;
		break;
		default :
			for(int k=0;k<7;k++)
			{
				q3[z][k]=processes[i][k];
			}
			z++;
			break;
		}
	}



	/******** Queues Displaying Levels********/
	printf("\nPID\tArrival\tBurst\tWait \tTAT \tCT  \tPriority \n");
	printf("---\t-------\t-----\t---- \t--- \t--  \t-------- \n");
	if(x == 0) printf("   NO PROCESS IN UPPER LEVEL QUEUE    \n");
	for(int i= 0; i< x;i++){
		for (int k=0; k<7; k++)
			printf("%d \t", q1[i][k]);
		printf("\n"); }
	printf("---------------------------------------------------------\n");

	if(y == 0) printf("    NO PROCESS IN MID LEVEL QUEUE     \n");
	for(int i= 0; i< y;i++){
		for (int k=0; k<7; k++)
			printf("%d \t", q2[i][k]);
		printf("\n");}
	printf("---------------------------------------------------------\n");
	if(z == 0) printf("   NO PROCESS IN LAST LEVEL QUEUE    \n");
	for(int i= 0; i< z;i++){
		for (int k=0; k<7; k++)
			printf("%d \t", q3[i][k]);
		printf("\n");}
	printf("---------------------------------------------------------\n");

	if(x !=0)// checking if the queue is not empty
	{
		printf("\nProcessing the first queue\n");
		performRR(q1,x,4);
		display(q1, x);
		calAvg(q1, x, 7);
	}
	else
		printf("\nFirst queue is empty\n");

	if (y != 0)
	{
		printf("\nProcessing  the second queue\n");
		updateTransposedProcess((int *)q1,x,(int *)q2,y);
		performRR(q2,y,8);
		display(q2, y);
		calAvg(q2, y, 8);
	}
	else
		printf("\nSecond queue is empty\n");

	if(z!=0)
	{
		printf("\nProcessing  the third queue\n");
		updateTransposedProcess((int *)q2,y,(int *)q3,z);
		performFCFS(q3,z);
		display(q3, z);
		calAvg(q3, z, 9);
	}
	else
		printf("\nThird queue is empty\n");

	int active_count = ((x>0)?1:0)+((y>0)?1:0)+((z>0)?1:0);

	printf("\n\n");
	printf("Average waiting time and Turn around time after implementing multi-queues\n");
	avg_wt[7]=( avg_wt[9]+avg_wt[7]+avg_wt[8] )/active_count;
	avg_tat[7]=(avg_tat[9]+avg_tat[7]+avg_tat[8])/active_count;
	printf("Average waiting time = %0.2f\n",avg_wt[7]);
	printf("Average turn around time = %0.2f \n",avg_tat[7]);
}
void PrintStats(){
	printf("---------------------------------------------------------------------------------------\n");
	printf("-------------------------------------Statistics----------------------------------------\n");
	printf("---------------------------------------------------------------------------------------\n");
	printf("Scheduling\t\t Average Waiting\t\t Average turn Around Time\n");
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 0 : FCFS \t\t\t %lf \t\t\t %lf \t\t|\n",avg_wt[0],avg_tat[0]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 1 : SJF \t\t\t %lf \t\t\t %lf \t\t|\n",avg_wt[1],avg_tat[1]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 2 : SRTF \t\t\t %lf \t\t\t %lf \t\t|\n",avg_wt[2],avg_tat[2]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 3 : RR  \t\t\t %lf \t\t\t %lf \t\t|\n",avg_wt[3],avg_tat[3]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 4 : PRIORITY PREEMPTIVE \t %lf \t\t\t %lf \t\t|\n",avg_wt[4],avg_tat[4]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 5 : PRIORITY NON-PREEMPT\t %lf \t\t\t %lf \t\t|\n",avg_wt[5],avg_tat[5]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 6 : MULTI LEVEL Q \t\t %lf \t\t\t %lf \t\t|\n",avg_wt[6],avg_tat[6]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 7 : MULTI LEVEL Q FEEDBACK\t %lf \t\t\t %lf \t\t|\n",avg_wt[7],avg_tat[7]);
	printf("---------------------------------------------------------------------------------------\n");
	printf("| 8 : LINUX SCHEDULER\t\t %lf \t\t\t %lf \t\t|\n",avg_wt[8],avg_tat[8]);
	printf("---------------------------------------------------------------------------------------\n");
	int m_wt=0,m_tat=0;
	int sd_wt=0,sd_tat=0;
	for(int i=0;i<9;i++)
	{
		m_wt   += avg_wt[i];
		m_tat  += m_tat+avg_tat[i];
	}
	m_wt=m_wt/9;
	m_tat=m_tat/9;
	int x = pow(m_wt,2);
	for(int i=0;i<9;i++){

		sd_wt += pow((avg_wt[i]-m_wt),2);
		sd_tat +=pow((avg_tat[i]-m_tat),2);
	}
	sd_wt=sqrt(sd_wt/9);
	sd_tat=sqrt(sd_tat/9);
	printf("Mean waiting time = %0.2f\n",m_wt);
	printf("Mean turn around time = %0.2f \n",m_tat);
	printf("Standard Deviation of waiting time = %0.2f\n",sd_wt);
	printf("standard Deviation of turn around time = %0.2f \n",sd_tat);
}
void runMyTestCase(int *n)
{
	//*n = 5;
	//int test[5][7] = { {0,0,5,0,0,0,0},{0,1,3,0,0,0,0},{0,2,1,0,0,0,0},{0,3,2,0,0,0,0},{0,4,3,0,0,0,0} };


	//*n = 6;
	//int test[6][7] = { {0,0,4,0,0,0,10},{0,1,5,0,0,0,8},{0,2,2,0,0,0,6},{0,3,1,0,0,0,2},
	//	{0,4,6,0,0,0,4},{0,6,3,0,0,0,0} };


	*n = 7;
	int test[7][7] = { {0,0,4,0,0,0,10},{0,1,2,0,0,0,3},{0,2,3,0,0,0,6},{0,3,5,0,0,0,2},
			{0,4,1,0,0,0,4},{0,5,4,0,0,0,0},{0,6,10,0,0,0,3}};


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

#ifdef RUN_MY_TESTCASE
	runMyTestCase(&n);
#endif

	rearrange(processes,n); // arranging data as per the arrival time

	char ch = 'Y';
	while('Y' == ch || 'y' == ch){

		fflush(stdin);


		printf("\nThe Process List are as follows :-\n");
		display(processes,n);

		printf("\nEnter 0 : FCFS 						Scheduling\n");
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
		printf("Enter 11 : Print Statistics\n");



		int cmdfound=-1;
		scanf("%d",&cmdfound);

		switch(cmdfound){
		case 0:
		{
			performFCFS(processes, n);
			display(processes, n);
			gantt(processes,n);
			calAvg(processes, n, 0);
		}
		break;
		case 1:
		{
			performSJF_NON_PREEMPT(processes, n);
			display(processes, n);
			calAvg(processes, n, 1);
		}
		break;
		case 2:
		{
			performSJF_PREEMPT(processes, n);
			display(processes, n);
			calAvg(processes, n, 2);
		}
		break;
		case 3:
		{

			int quant= 0;
			printf("Enter the quantum\n");
			scanf("%d", &quant);
			if(quant<=0)
			{
				printf("Heyaaa!!!! Dont play with time slice..Ohkay I will assign instead a better choice ..Assigned quant = 2\n\n");
				quant = 2;
			}

			performRR(processes,n,quant);
			display(processes, n);
			calAvg(processes, n, 3);
		}
		break;
		case 4 ... 7:
		{
			performPRIORITY(processes ,n,cmdfound-4);
		}
		break;
		case 8:
		{
			performMULTI_LEVEL_QUEUE(processes, n);
		}
		break;
		case 9:
		{
			performMULTI_LEVEL_FEEDBACK_QUEUE(processes, n);
		}
		break;
		case 10 :{

		}
		break;
		case 11:{
			PrintStats();
		}
		}





		printf("\n Want to see another schdeduling algo.... Press Y/y else N/n for quit\n");

		char x;
		scanf("\n%s",&x);
		ch = x;

		printf("\033[2J\033[0;0H");
	}
	return 0;
}
