#ifndef THREADPOOL_H
#define THREADPOOL_H


#include<iostream>
#include<pthread.h>
#include<cstdlib>
#include<cassert>
#include<cstdio>
#include<deque>

using namespace std;

struct Job
{
		void* (*doJobFun)(void *arg);    //线程回调函数
		void *arg;                                //回调函数参数

};

class ThreadPool
{
		unsigned int threadNum;                   //线程池中开启线程的个数
		unsigned int maxJobNum;                //队列中最大job的个数
		deque<Job> jobQueue;				//任务队列
		pthread_t *pthreads;              //线程池中所有线程的pthread_t
		pthread_mutex_t mutex;            //互斥信号量
		pthread_cond_t condQueueEmpty;       //队列为空的条件变量
		pthread_cond_t condQueueFull;    //队列为满的条件变量
		bool isPoolClose;                   //线程池是否已经关闭

		public:
		ThreadPool(int threadNum=10, int maxJobNum=100);
		int addJob(Job job);
		int destroy();
		~ThreadPool();
		friend void* run(void* arg);
};
#endif
