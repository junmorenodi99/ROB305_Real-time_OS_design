/**
 * @file main_td2a.cpp
 * @author Nicolas Dimate (https://github.com/junmorenodi99)
 * @brief Main function for the creation of a incremental counter using pthread
 * @version 0.1
 * @date 2022-03-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <time.h>
#include <pthread.h>


#include "timerLib/timerLib.h"

using namespace std;

/**
 * @brief Data structure for thread use 
 * 
 */
struct Data 
{
    int nLoops;
    double pCounter;
};


/**
 * @brief This function must perform a loop incrementing by 1.0 the value of the counter pointed by pCounter
 * it must perform this loop nLoops times. 
 * 
 * @param nLoops 
 * @param pCounter 
 */
void incr(int nLoops, double* pCounter)
{
    for(int i = 0; i < nLoops; ++i)
    {
        *pCounter += 1.0;
    }
}

/**
 * @brief Function that calls the incr() function
 * 
 * @param threadData 
 * @return void* 
 */
void* call_incr(void* threadData)
{
	Data* pThreadData = (Data*) threadData;

	incr(pThreadData->nLoops, (double*) &pThreadData->pCounter);

	return threadData;
}


/**
 * @brief Implementation of the main function, using the created functions.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[])
{
    // To measure the execution time
   	timespec time_begin;
    timespec time_end; 

    // Exception handling
    if (argc < 2)
    {
        std::cerr << "Please enter the value of: <nLoops> <nTasks>" << endl;
        return 1;
    }

    int nLoops = stoi(argv[1]);
    int nTasks = stoi(argv[2]);

    // Initialization
    Data threadData = {nLoops, 0.0};

    time_begin = timespec_now();
    
    // Creation and joining of Threads
	pthread_t incrementThread[nTasks];

	for(int iThread = 0; iThread<nTasks; iThread++)
	{
		pthread_create(&incrementThread[iThread], NULL, call_incr, (void*) &threadData);
        cout << "Thread " << iThread << " created" << endl;
	}
	

	for(int iThread = 0; iThread<nTasks; iThread++)
	{
		pthread_join(incrementThread[iThread], NULL); 
        cout << "Thread " << iThread << " joined" << endl;
	}

    time_end = timespec_now();

    // Visualization of results
    cout << "Final counter value: " << threadData.pCounter << endl;    
    cout << "Execution time: " << (timespec_to_ms(time_end - time_begin)/1000) << " seconds" << endl; 
    
    return 0;
}

