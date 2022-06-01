#include  <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <linux/sem.h>
void P(int semid,int index)
{
	struct sembuf sem;//信号量操作数组
	sem.sem_num = index;//信号量编号
	sem.sem_op = -1;//信号量操作，-1为P操作
	sem.sem_flg = 0;			//操作标记：0或IPC_NOWAIT等
	semop(semid,&sem,1);		//1:表示执行命令的个数
	return;
}
void V(int semid,int index)
{
	struct sembuf sem;//信号量操作数组
	sem.sem_num = index;//信号量编号
	sem.sem_op =  1;//信号量操作，1为V操作
	sem.sem_flg = 0; //操作标记
        semop(semid,&sem,1);//1:表示执行命令的个数
	return;
}
int semid;
int a=0;
#define  LOOPS	100//定义累加次数
pthread_t p1,p2;
void *subp1();
void *subp2();
main() 
{
	union semun  semopts; 
	int res;
	semid = semget(300,2,IPC_CREAT|0666);//创建两个信号量
	if (semid<0) {//判断是否创建成功
		printf("error");//创建失败，打印错误
	       	return;
	}
	semopts.val = 1;//设定SETVAL的值为1
	res=semctl(semid,0,SETVAL,semopts);//初始化信号量，信号量编号为0
	semopts.val = 0;//设定SETVAL的值为0
	res=semctl(semid,1,SETVAL,semopts);//初始化信号量，信号量编号为1
	if (res < 0) return;//判断是否初始化成功
	pthread_create(&p1,NULL,subp1,NULL);//创建线程p1（确定调用该线程函数的入口点）
	pthread_create(&p2,NULL,subp2,NULL);//创建线程p2（确定调用该线程函数的入口点）
	pthread_join(p1,NULL);//等待线程p1的终止
	pthread_join(p2,NULL);//等待线程p2的终止
	res=semctl(semid,0,IPC_RMID,0);//从系统中删除信号量
	if (res < 0) {//判断是否删除成功
		puts("error IPC_RMID");//信号量删除失败，输出信息
		return;
	}

}
void *subp1()
{
	int i,j;
	for (i=0;  i<= LOOPS;i++) {//不断循环，输出LOOPS+1次
		P(semid,0);//执行P操作
		printf("subp1:a=%d\n",a);//进入临界区，打印输出a的值
		V(semid,1);//执行V操作
	}
	return;//退出当前线程
}
void *subp2()
{
	int i,j;
	int k=1;//初始化累加的数字
	 for (i=0;i<=LOOPS;k++,i++) {
		P(semid,1);//执行P操作
		a+=k;      //进入临界区，1-100累加计算
		V(semid,0);//执行V操作
	}
	return//退出当前线程
}