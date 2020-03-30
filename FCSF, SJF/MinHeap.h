typedef Process Data; 

typedef struct PriorityQueue{
	int capacity;
	int size;
	Data* arr;
}PriorityQueue;

typedef PriorityQueue* PQueue;

int isFull(PQueue Q){
	return Q -> size == Q -> capacity;
}

int isEmpty(PQueue Q){
	return Q -> size == 0;
}


PQueue createPQueue(const int maxsize){
	PQueue tmp = (PQueue)malloc(sizeof(PriorityQueue));

	tmp -> capacity = maxsize;
	tmp -> size = 0;
	tmp -> arr = (Data*)malloc(sizeof(Data) * maxsize);

	tmp -> arr[0].rem_t = -1; 
	return tmp;
}

void enqueue(PQueue q,const Data d){
	if(isFull(q)){
		//printf("Queue Full!\n");
		return;
	}	
	int i = ++q -> size;
	for(; q -> arr[i/2].rem_t > d.rem_t; i /= 2){
			q -> arr[i] = q -> arr[i/2];
	}
	
	q -> arr[i] = d; 

}

Data dequeue(PQueue q){
	if(isEmpty(q)){
		//printf("Queue Empty!\n");
		return q -> arr[0]; 
	}
	int i,child;
	Data min,last;

	min = q -> arr[1];
	last = q -> arr[q -> size--];

	for(i = 1; i * 2 <= q -> size ; i = child){
		child = i * 2;

		if(child != q -> size && q -> arr[child + 1].rem_t < q -> arr[child].rem_t)
			child ++;
		if(last.rem_t >= q -> arr[child].rem_t)
			q -> arr[i] = q -> arr[child];
		else
			break;
	}

	q -> arr[i] = last;
	return min;
}

void display(PQueue Q){
	for(int i = 1 ; i <= Q -> size ; i++)
		printf("PID :%d rem_t: %4.2f\n",Q -> arr[i].pid,Q -> arr[i].rem_t);
}