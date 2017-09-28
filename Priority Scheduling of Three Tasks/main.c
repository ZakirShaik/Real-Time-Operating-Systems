/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/******************************************************************************
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.
 * The simply blinky demo is implemented and described in main_blinky.c.  The
 * more comprehensive test and demo application is implemented and described in
 * main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and FreeRTOS hook functions.
 *
 *******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *
 * This demo was created using Windows XP on a dual core laptop, and has since
 * been maintained using Windows 7 on a quad core laptop.
 *
 *******************************************************************************
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

//my includes
#include<math.h>
#include<time.h>
#include<string.h>

/* This project provides two demo applications.  A simple blinky style demo
application, and a more comprehensive test and demo application.  The
mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is 1 then the blinky demo will be built.
The blinky demo is implemented and described in main_blinky.c.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is not 1 then the comprehensive test and
demo application will be built.  The comprehensive test and demo application is
implemented and described in main_full.c. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE	7001
#define mainREGION_2_SIZE	18105
#define mainREGION_3_SIZE	2807

//Declaring global variables to store response times
double response_time_Task1;
double response_time_Task2;
double response_time_Task3;
int count_task1 = 0;
int count_task2 = 0;
int count_task3 = 0;
int data[10000];
int flag = 0;
int fl_cntr = 0;

/*-----------------------------------------------------------*/

/*
 * main_blinky() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1.
 * main_full() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 0.
 */
extern void main_blinky( void );
extern void main_full( void );

/*
 * Only the comprehensive demo uses application hook (callback) functions.  See
 * http://www.freertos.org/a00016.html for more information.
 */
void vFullDemoTickHookFunction( void );
void vFullDemoIdleFunction( void );

/*
 * This demo uses heap_5.c, so start by defining some heap regions.  It is not
 * necessary for this demo to use heap_5, as it could define one large heap
 * region.  Heap_5 is only used for test and example purposes.  See
 * http://www.freertos.org/a00111.html for an explanation.
 */
static void  prvInitialiseHeap( void );

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile( void );

/*-----------------------------------------------------------*/

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/* The user trace event posted to the trace recording on each tick interrupt.
Note:  This project runs under Windows, and Windows will not be executing the
RTOS threads continuously.  Therefore tick events will not appear with a regular
interval within the trace recording. */
traceLabel xTickTraceUserEvent;
static portBASE_TYPE xTraceRunning = pdTRUE;

void File_DP_ParkingLotMonitoring(void *pvParameters);
void DP_ParkingLotMonitoring(void *pvParameters);
void B190_Monitoring(void *pvParameters);
void DP_PE_Consumption_Recording(void *pvParameters);
/*-----------------------------------------------------------*/

int main( void )
{
	
	/* This demo uses heap_5.c, so start by defining some heap regions.  heap_5
	is only used for test and example reasons.  Heap_4 is more appropriate.  See
	http://www.freertos.org/a00111.html for an explanation. */
	prvInitialiseHeap();

	/* Initialise the trace recorder and create the label used to post user
	events to the trace recording on each tick interrupt.  Use of the trace
	recorder is optional.  See http://www.FreeRTOS.org/trace for more
	information. */
	vTraceInitTraceData();
	xTickTraceUserEvent = xTraceOpenLabel( "tick" );

	/* The mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is described at the top
	of this file. */
	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY == 1 )
	{
		////CREATING THREE TASKS
		//int val1 = xTaskCreate(File_DP_ParkingLotMonitoring, "File_DP_ParkingLotMonitoring", 1000, NULL, 3, NULL);
		int val1 = xTaskCreate(DP_ParkingLotMonitoring, "DP_ParkingLotMonitoring", 1000, NULL, 3, NULL);
		int val2 = xTaskCreate(B190_Monitoring, "B190_Monitoring", 1000, NULL, 2, NULL);
		int val3 = xTaskCreate(DP_PE_Consumption_Recording, "DP_PE_Consumption_Recording", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

		////IF THREE TASKS ARE SUCCESSFULLY CREATED THEN START SCHEDULER
		if(val1 == 1 && val2 == 1 && val3 == 1)
		vTaskStartScheduler();
		//For debugging purpose only.
		printf("Task scheduler stopped...");
		//main_blinky();
		
	}
	#else
	{
		/* Start the trace recording - the recording is written to a file if
		configASSERT() is called. */
		printf( "\r\nTrace started.\r\nThe trace will be dumped to disk if a call to configASSERT() fails.\r\n" );
		printf( "Uncomment the call to kbhit() in this file to also dump trace with a key press.\r\n" );
		uiTraceStart();

		main_full();
	}
	#endif

	return 0;
}
/*-----------------------------------------------------------*/
////START OF MY CODE..........................

void File_DP_ParkingLotMonitoring(void *pvParameters) {
	int x1, y1, x2, y2;
	float euclidean_distance, period, speed, mile = 1609;
	double avg_resp_time_t1, cur_res_time;
	clock_t start_time, end_time;
	FILE *fptr;
	char buffer[255];

	for (;; ) {
		start_time = clock();
		printf("Arrival time of task 1 is %d\n", start_time);

		//Reading coordinates from file.
		if (flag == 0) {
			fptr = fopen("dp_coords.txt", "r");
			if (fptr == NULL) {
				printf("Sorry there is no file in the current directory. Please place file with coordinates.");
				exit(1);
			}
			else {
				//Read x1, y1 from file.
				int abc = 0;
				while (abc<10000)
				{
					char *next = buffer;
					fgets(buffer, sizeof buffer, fptr);
					while (*next && *next != '\n')
					{
						data[abc] = strtol(next, &next, 0);
						abc++;
					}
				}
				flag = 9393;
			}
			fclose(fptr);
		}

		//Read x1,y1 data
		x1 = data[fl_cntr];
		y1 = data[fl_cntr + 1];

		//Calculating period in minutes. Maximum minutes = 180.
		period = (rand() % 180) / 60.0;
		//printf("Peroid in minutes = %.2f\n",period);

		//Read x2,y2 data.
		x2 = data[fl_cntr + 2];
		y2 = data[fl_cntr + 3];
		fl_cntr += 4;

		//Euclidean distance in miles. Maximum miles is 20 approx.
		euclidean_distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
		//printf("Euclidean distance in miles(max generated 22 miles) = %.2f\n",euclidean_distance);

		//Converting euclidean distance in miles and Calculating speed.
		speed = (euclidean_distance / mile) / period;
		printf("Speed in miles per hour %.2f\n", speed);

		//Generating alert.
		if (speed > 10.0)
			printf("Alert: The car is speeding up and is travelling with %.2f mph.\n", speed);

		end_time = clock();

		//Calculating average response time of task 1.
		cur_res_time = (double)end_time - (double)start_time / CLOCKS_PER_SEC * 1000;
		printf("Response time of task 1 for execution %d is %.2f seconds.\n", count_task1 + 1, cur_res_time);
		response_time_Task1 += cur_res_time;
		printf("Sum of response times till %d executions is %.2f seconds.\n", count_task1 + 1, response_time_Task1);
		count_task1++;

		avg_resp_time_t1 = response_time_Task1 / count_task1;
		printf("Average response time for task 1 after %d executions = %.2f seconds.\n\n", count_task1, avg_resp_time_t1);


		vTaskDelay(3000);

	}
}

void DP_ParkingLotMonitoring(void *pvParameters) {
	int x1, y1, x2, y2;
	float euclidean_distance,period,speed, mile = 1609;
	double avg_resp_time_t1, cur_res_time;
	clock_t start_time, end_time;

	for (;; ) {         
		start_time = clock();
		printf("Arrival time of task 1 is %d\n", start_time);

		//Random coordinates. Value generated between 0 to 32767(max value of rand).
		x1 = rand();
		y1 = rand();


		//Calculating period in minutes. Maximum minutes = 180.
		period = (rand()%180)/60.0;
		//printf("Peroid in minutes = %.2f\n",period);

		//After period the new coordinates generated. Value generated between 0 to 32767(max value of rand)
		x2 = rand();
		y2 = rand();

		//Euclidean distance in miles. Maximum miles is 20 approx.
		euclidean_distance = sqrt(pow(x2-x1,2)+pow(y2-y1,2));
		//printf("Euclidean distance in miles(max generated 22 miles) = %.2f\n",euclidean_distance);

		//Converting euclidean distance in miles and Calculating speed.
		speed = (euclidean_distance/mile) / period;
		printf("Speed in miles per hour %.2f\n",speed);

		//Generating alert.
		if (speed > 10.0)
			printf("Alert: The car is speeding up and is travelling with %.2f mph.\n",speed);

		end_time = clock();

		//Calculating average response time of task 1 when random coordinates are directly used.
		cur_res_time = (double)end_time - (double)start_time / CLOCKS_PER_SEC * 1000;
		printf("Response time of task 1 for execution %d is %.2f seconds.\n", count_task1 + 1, cur_res_time);
		response_time_Task1 += cur_res_time;
		printf("Sum of response times till %d executions is %.2f seconds.\n", count_task1 + 1, response_time_Task1);
		count_task1++;

		avg_resp_time_t1 = response_time_Task1 / count_task1;
		printf("Average response time for task 1 after %d executions = %.2f seconds.\n\n", count_task1, avg_resp_time_t1);


		vTaskDelay(3000);

	}
}
void B190_Monitoring(void *pvParameters) {
	int num_students, sum=0, avg=0, max=0, min=60;
	double avg_resp_time_t2, cur_res_time;
	clock_t start_time, end_time;
	for (;; ) {
		start_time = clock();
		printf("Arrival time of task 2 is %d\n", start_time);
		//Generating random student values for each hour in a day.
		for (int i = 1; i < 26; i++) {
			if (i == 25) {
				break;
			}
			num_students = 20 + rand() % 30;
			sum += num_students;
			if (num_students > max)
				max = num_students;
			if (num_students < min)
				min = num_students;

		}
		avg = sum / 24;
		printf("Sum of students = %d\nAverage number of students = %d\nMaximum number of students = %d\nMinimum number of students = %d\n",sum, avg, max, min);
		sum = 0;
		avg = 0;
		max = 0;
		min = 60;
		num_students = 0;

		if (configCHECK_FOR_STACK_OVERFLOW)
			printf("Yes......................Stack is overflowing in task 2.\n");
		end_time = clock();

		//Calculating average response time of task 2.
		cur_res_time = (double)end_time - (double)start_time / CLOCKS_PER_SEC * 1000;
		printf("Response time of task 2 for execution %d is %.2f seconds.\n", count_task2 + 1, cur_res_time);
		response_time_Task2 += cur_res_time;
		printf("Sum of response times till %d executions is %.2f seconds.\n", count_task2 + 1, response_time_Task2);
		count_task2++;

		avg_resp_time_t2 = response_time_Task2 / count_task2;
		printf("Average response time for task 2 after %d executions = %.2f seconds.\n\n", count_task2, avg_resp_time_t2);

		vTaskDelay(2500);

	}
}
void DP_PE_Consumption_Recording(void *pvParameters) {
	int power = 0, month = 30, count = 0;
	float avg_power_use = 0.0, tot_power = 0.0;
	double avg_resp_time_t3, cur_res_time;
	clock_t start_time, end_time;
	
	for (;; ) {
		start_time = clock();
		printf("Arrival time of task 3 is %d\n",start_time);
		int minutes = month * 24 * 60;
		//t1 = clock();
		for (int i = 1; i <= minutes; ++i) {
			//Power reading in KW.
			power = 10 + rand() % 190;
			tot_power += power;
			avg_power_use = tot_power / i;
			//Checking for power spikes.
			if ((float)power >= 5 * avg_power_use)
				printf("There is %d spike in power use at minute %d", power, i);

		}
		printf("The total energy consumption in this month = %.2f KW per month.\n",tot_power);
		
		printf("Average power use = %.2f kw per minute.\n",avg_power_use);

			if (month == 30)
				month = 31;
			else if (month == 31)
				month = 30;
			tot_power = 0;
			if (configCHECK_FOR_STACK_OVERFLOW)
				printf("Yes......................Stack is overflowing in task 3.\n");
			end_time = clock();

			//Calculating average response time of task 3.
			cur_res_time = (double)end_time - (double)start_time / CLOCKS_PER_SEC * 1000;
			printf("Response time of task 3 for execution %d is %.2f seconds.\n", count_task3 + 1, cur_res_time);
			response_time_Task3 += cur_res_time;
			printf("Sum of response times till %d executions is %.2f seconds.\n", count_task3 + 1, response_time_Task3);
			count_task3++;
			
			avg_resp_time_t3 = response_time_Task3 / count_task3;
			printf("Average response time for task 3 after %d executions = %.2f seconds.\n\n", count_task3, avg_resp_time_t3);

			
		vTaskDelay(2000);
	}

}
///////END OF MY CODE.............................
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
	size of the	heap available to pvPortMalloc() is defined by
	configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
	API function can be used to query the size of free heap space that remains
	(although it does not provide information on how the remaining heap might be
	fragmented).  See http://www.freertos.org/a00111.html for more
	information. */
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If application tasks make use of the
	vTaskDelete() API function to delete themselves then it is also important
	that vApplicationIdleHook() is permitted to return to its calling function,
	because it is the responsibility of the idle task to clean up memory
	allocated by the kernel to any task that has since deleted itself. */

	/* Uncomment the following code to allow the trace to be stopped with any
	key press.  The code is commented out by default as the kbhit() function
	interferes with the run time behaviour. */
	/*
		if( _kbhit() != pdFALSE )
		{
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
				xTraceRunning = pdFALSE;
			}
		}
	*/

	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		/* Call the idle task processing used by the full demo.  The simple
		blinky demo does not use the idle task hook. */
		vFullDemoIdleFunction();
	}
	#endif
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  This function is
	provided as an example only as stack overflow checking does not function
	when running the FreeRTOS Windows port. */
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */

	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		vFullDemoTickHookFunction();
	}
	#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */

	/* Write a user event to the trace log.  Note:  This project runs under
	Windows, and Windows will not be executing the RTOS threads continuously.
	Therefore tick events will not appear with a regular interval within the the
	trace recording. */
	vTraceUserEvent( xTickTraceUserEvent );
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
	/* This function will be called once only, when the daemon task starts to
	execute	(sometimes called the timer task).  This is useful if the
	application includes initialisation code that would benefit from executing
	after the scheduler has been started. */
}
/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static portBASE_TYPE xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */

	/* Parameters are not used. */
	( void ) ulLine;
	( void ) pcFileName;

	printf( "ASSERT! Line %d, file %s, GetLastError() %d\r\n", ulLine, pcFileName, GetLastError() );

 	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if( xPrinted == pdFALSE )
		{
			xPrinted = pdTRUE;
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			__asm{ NOP };
			__asm{ NOP };
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

static void prvSaveTraceFile( void )
{
FILE* pxOutputFile;

	fopen_s( &pxOutputFile, "Trace.dump", "wb");

	if( pxOutputFile != NULL )
	{
		fwrite( RecorderDataPtr, sizeof( RecorderDataType ), 1, pxOutputFile );
		fclose( pxOutputFile );
		printf( "\r\nTrace output saved to Trace.dump\r\n" );
	}
	else
	{
		printf( "\r\nFailed to create trace dump file\r\n" );
	}
}
/*-----------------------------------------------------------*/

static void  prvInitialiseHeap( void )
{
/* The Windows demo could create one large heap region, in which case it would
be appropriate to use heap_4.  However, purely for demonstration purposes,
heap_5 is used instead, so start by defining some heap regions.  No
initialisation is required when any other heap implementation is used.  See
http://www.freertos.org/a00111.html for more information.

The xHeapRegions structure requires the regions to be defined in start address
order, so this just creates one big array, then populates the structure with
offsets into the array - with gaps in between and messy alignment just for test
purposes. */
static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
const HeapRegion_t xHeapRegions[] =
{
	/* Start address with dummy offsets						Size */
	{ ucHeap + 1,											mainREGION_1_SIZE },
	{ ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
	{ ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
	{ NULL, 0 }
};

	/* Sanity check that the sizes and offsets defined actually fit into the
	array. */
	configASSERT( ( ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE ) < configTOTAL_HEAP_SIZE );

	/* Prevent compiler warnings when configASSERT() is not defined. */
	( void ) ulAdditionalOffset;

	vPortDefineHeapRegions( xHeapRegions );
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

