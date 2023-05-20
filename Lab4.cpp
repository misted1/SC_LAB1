#define HAVE_STRUCT_TIMESPEC
#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <ctime>

using namespace std;
const int n = 4;

struct param {
	int rank;
	const char* order;
};

/* Функция, которую будет исполнять созданный поток */
void* thread_job(void* arg)
{
	clock_t start = clock();
	param* pArguments = (param*)arg;
	int b = 5;
	for (int i = 1; i < 10e6; i++)
	{
		b = b + 1;
	}
	clock_t end = clock();
	double thread_time = (double)(end - start) / CLOCKS_PER_SEC;
	//cout << "Thread_job time: "  << thread_time << endl;
	//cout << "Thread is running... " << b << endl;
	std::cout << "Thread " << pArguments->rank << " write " << pArguments->order << endl;
	return ((void*)b);
}
int main()
{	
	

	// Определяем переменные: идентификатор потока и код ошибки
	
	pthread_t threads[n];
	pthread_attr_t thread_attr[n];
	int err;
	const char* messages[] = { "First masage", "Second masage", "Third masage", "Fourth masage" };
	param param[n];
	for (int i = 0; i < n; ++i) {
		param[i].rank = i + 1;
		param[i].order = messages[i];
	}
	for (int i = 0; i < n; i++) {
		err = pthread_attr_init(&thread_attr[i]);
		if (err != 0) {
			cout << "Cannot create thread attribute: " << strerror(err) << endl;
				exit(-1);
			}
		pthread_attr_setdetachstate(&thread_attr[i], PTHREAD_CREATE_JOINABLE);
		err = pthread_attr_setstacksize(&thread_attr[i], 1024 * 1024 * i);
		if (err != 0) {
			cout << "Setting stack size attribute failed: " << strerror(err)
				<< endl;
			exit(-1);
		}



		clock_t start = clock();
		err = pthread_create(&threads[i], &thread_attr[i], thread_job, (void*)&param[i]);

		if (err != 0) {
			cout << "Cannot create a thread: " << strerror(err) << endl;
		exit(-1);
		}
		clock_t end = clock();
		double thread_time = (double)(end - start) / CLOCKS_PER_SEC;
		cout << "Create thread time: "  << thread_time << endl;

	}
	for (auto& thread : threads) {
		pthread_join(thread, nullptr);
	}

	for (auto& thread_attr : thread_attr) {
		pthread_attr_destroy(&thread_attr);
	}

	pthread_exit(NULL);
}