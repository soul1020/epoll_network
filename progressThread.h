#ifndef PROGERESS_THREAD_H_
#define PROGERESS_THREAD_H_

class ProgressThread
{
		public:
				ProgressThread();
				~ProgressThread();
				void Run();
		public:
				static void *Runtine(void *arg);
};
#endif
