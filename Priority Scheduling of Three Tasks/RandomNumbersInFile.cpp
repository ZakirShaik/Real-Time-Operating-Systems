// RandomNumbersInFile.cpp : Defines the entry point for the console application.
//By Zakir Shaik
//Code to generate random coordinates in file.

#include "stdafx.h"
#include<stdlib.h>
#include<iostream>
int main()
{
	int fl_cntr = 0;
	FILE *fptr;
	char buffer[255];
	size_t size = 5;
	int data[1000];
	int x1, y1, x2, y2;
	size_t fl_cnt = 0;
	int abc = 0;
	fptr = fopen("dp_coords.txt", "w");
	if (fptr == NULL) {
		printf("Sorry file not created.");
		exit(1);
	}

	for(int i = 0;i<1000000;i++){
		int c = 0;
		//Random coordinates. Value generated between 0 to 32767(max value of rand).
		x1 = rand();
		y1 = rand();
		fprintf(fptr, "%d\t%d\n", x1, y1);
		c++;
	}

	fclose(fptr);
    return 0;
}

