
struct Q
{

	int queue[100];
	int front;
	int rear;
	int size;


};

void Q_init(struct Q* myQ)
{
	myQ->front = 0;
	myQ->rear = 0;
	myQ->size =100;
}


void Q_PUSH(struct Q* myQ,int data)
{
	if(myQ->rear==myQ->size)
	{
		//printf("\n Queue is Full");

	}
	else
	{
		myQ->queue[myQ->rear++] = data;
	}
}

int Q_FRONT(struct Q* myQ)
{
	return myQ->queue[myQ->front];
}

void Q_POP(struct Q* myQ)
{
	if(myQ->front==myQ->rear)
	{

		//printf("\n Queue is Empty");
	}
	else
	{
		myQ->front++;
		myQ->size++;
	}
}

void Q_DISPLAY(struct Q* myQ)
{
	printf("\n");
	for(int i = myQ->front;i<myQ->rear;i++)
		printf("%d ",myQ->queue[i]);
	printf("\n");
}

unsigned char Q_EMPTY(struct Q* myQ)
{
	return (myQ->front==myQ->rear);
}
