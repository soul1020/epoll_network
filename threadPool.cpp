#include"threadPool.h"

void* run(void* arg)
{
		ThreadPool *pool = (ThreadPool *)arg;
		struct Job job ;
		while (1)  //死循环
		{
				pthread_mutex_lock(&(pool->mutex));

				if (pool->isPoolClose)   //线程池关闭，线程就退出
				{
						pthread_mutex_unlock(&(pool->mutex));
						pthread_exit(NULL);
				}

				while ((pool->jobQueue.empty()))   //队列为空时，就等待队列非空
				{
						pthread_cond_wait(&(pool->condQueueEmpty), &(pool->mutex));
						if (pool->isPoolClose)   //线程池关闭，线程就退出
						{
								pthread_mutex_unlock(&(pool->mutex));
								pthread_exit(NULL);
						}
				}

				job=pool->jobQueue.front();
				pool->jobQueue.pop_front();
				if (pool->jobQueue.size() == (pool->maxJobNum-1))
				{
						pthread_cond_broadcast(&(pool->condQueueFull));  //队列非满，就可以通知threadpool_add_job函数，添加新任务
				}
				pthread_mutex_unlock(&(pool->mutex));

				(*(job.doJobFun))(job.arg);   //线程真正要做的工作，回调函数的调用

		}
}

ThreadPool::ThreadPool(int threadNum, int maxJobNum)
{


		this->threadNum = threadNum;
		this->maxJobNum = maxJobNum;

		if (pthread_mutex_init(&(this->mutex), NULL))
		{
				perror("pthread_mutex_init");
				exit(1);
		}

		if (pthread_cond_init(&(this->condQueueEmpty), NULL))
		{
				perror("pthread_cond_init");
				exit(1);
		}

		if (pthread_cond_init(&(this->condQueueFull), NULL))
		{
				perror("pthread_cond_init");
				exit(1);
		}

		this->pthreads = (pthread_t *)malloc(sizeof(pthread_t) * this->threadNum);
		if (NULL == this->pthreads)
		{
				perror("pthreads malloc");
				exit(1);
		}
		this->isPoolClose = false;
		unsigned int i;
		for (i = 0; i < this->threadNum; ++i)
		{
				pthread_create(&(this->pthreads[i]), NULL, run, (void *)this);
		}



}

int ThreadPool::addJob(Job job)
{
		assert(job.doJobFun != NULL);
		if (this->isPoolClose)   //线程池关闭，线程就退出
		{
				return -1;
		}
		pthread_mutex_lock(&(this->mutex));
		while ((this->jobQueue.size() == this->maxJobNum))
		{
				pthread_cond_wait(&(this->condQueueFull), &(this->mutex));   //队列满的时候就等待
		}
		if (this->isPoolClose)    //队列关闭或者线程池关闭就退出
		{
				pthread_mutex_unlock(&(this->mutex));
				return -1;
		}

		if (this->jobQueue.empty())   
		{
				this->jobQueue.push_back(job);
				pthread_cond_broadcast(&(this->condQueueEmpty));  //队列空的时候，有任务来时就通知线程池中的线程：队列非空
		}
		else
		{
				this->jobQueue.push_back(job);
		}
		pthread_mutex_unlock(&(this->mutex));

		return 0;
}

int ThreadPool::destroy()
{
		pthread_mutex_lock(&(this->mutex));
		this->isPoolClose = true;        //置队列关闭标志
		this->jobQueue.clear();

		pthread_mutex_unlock(&(this->mutex));
		pthread_cond_broadcast(&(this->condQueueEmpty));  //唤醒线程池中正在阻塞的线程
		pthread_cond_broadcast(&(this->condQueueFull));   //唤醒添加任务的threadpool_add_job函数
		unsigned int i;
		for (i = 0; i < this->threadNum; ++i)
		{
				pthread_join(this->pthreads[i], NULL);    //等待线程池的所有线程执行完毕
		}

		pthread_mutex_destroy(&(this->mutex));          //清理资源
		pthread_cond_destroy(&(this->condQueueEmpty));
		pthread_cond_destroy(&(this->condQueueFull));      
		free(this->pthreads);
		return 0;
}

ThreadPool::~ThreadPool()
{
		destroy();
}
