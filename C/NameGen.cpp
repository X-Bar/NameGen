/***********************************************************************************************************************
FILENAME:   NameGen.cpp
AUTHORS:    Cody L. Lundberg

DESCRIPTION:
Name generator that can read names from files to create different random cultural names
* name parts
* gender (male, female)
* type (first last) 
all held in 3 different files (male first, female first, last)
user selects gender and culture and receives names

Convertion from python code

REVISION HISTORY:
01.07.2014   CDM     Cody original file creation
***********************************************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//~ #include "boost/filesystem/operations.hpp"
//~ #include "boost/filesystem/path.hpp"
//~ #include "boost/progress.hpp"
//~ #include <iostream>
//~ namespace fs = boost::filesystem;

#include <dirent.h>




int main(void)
{		
	// http://www.dreamincode.net/forums/topic/60036-accessing-directories-in-cc-part-ii/
	DIR *pdir = NULL;
	struct dirent *pent = NULL;

	if (chdir("../Names") != 0)
		perror("chdir() to /Names failed");
	
	pdir = opendir ("."); 										// "." will refer to the current directory
	if (pdir == NULL) 											// if pdir wasn't initialised correctly
	{ 															// print an error message and exit the program
		printf ("\nERROR! pdir could not be initialised correctly");
		exit (1);	 											// exit the program, using 1 as the status (most common for a failed execution)
	} // end if

	while (pent = readdir (pdir)) 								// while there is still something in the directory to list
	{
		if (pent == NULL) 										// if pent has not been initialised correctly
		{ 														// print an error message, and exit the program
			printf ("ERROR! pent could not be initialised correctly");
			exit (3);
		}
																// otherwise, it was initialised correctly. let's print it on the console:
		printf ("%s\n", pent->d_name);
	}
	
	

	
	
	
	
	
	
	closedir (pdir);
    return 0; // everything went OK
	
}
