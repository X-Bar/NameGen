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

#include <vector>
#include <string>

//~ #include "boost/filesystem/operations.hpp"
//~ #include "boost/filesystem/path.hpp"
//~ #include "boost/progress.hpp"
//~ #include <iostream>
//~ namespace fs = boost::filesystem;

#include <dirent.h>



class GenFileNames
{
private: // private data
	DIR *pdir;
	
	
	
public: // public data

private: // private methods
	
	
	const std::vector<std::string> SplitString(const std::string& s, const char& c)
	{
		// http://www.cplusplus.com/articles/2wA0RXSz/
		std::string buff{""};
		std::vector<std::string> v;
		
		for(auto n:s)
		{
			if(n != c) buff+=n; else
			if(n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if(buff != "") v.push_back(buff);
		
		return v;
	}
	
public: // public methods
	GenFileNames(void) 									// constructor
	{
		// http://www.dreamincode.net/forums/topic/60036-accessing-directories-in-cc-part-ii/
		pdir = NULL;
		struct dirent *pent = NULL;

		if (chdir("../Names") != 0)
			perror("chdir() to /Names failed");
		
		pdir = opendir ("."); 										// "." will refer to the current directory
		if (pdir == NULL) 											// if pdir wasn't initialised correctly
		{ 															// print an error message and exit the program
			printf ("\nERROR! pdir could not be initialised correctly");
			exit (1);	 											// exit the program, using 1 as the status (most common for a failed execution)
		} // end if

		while ( (pent = readdir (pdir) ) ) 							// while there is still something in the directory to list
		{
			if (pent == NULL) 										// if pent has not been initialised correctly
			{ 														// print an error message, and exit the program
				printf ("ERROR! pent could not be initialised correctly");
				exit (2);
			}
			printf ("Found File: %s\n", pent->d_name);				// otherwise, it was initialised correctly. let's print it on the console:
			std::string FileName(pent->d_name);
			std::vector<std::string> NameSplit = SplitString(FileName, '_');
			if ( (NameSplit.size() < 3) )
			{	
				printf("File not NameFile. Not dilimited.\n");
			}
			else if (NameSplit[0] != "NameFile")
			{
				printf("File not NameFile. Incorrect prefix.\n");
			}
			else
			{
				printf("Culture %s\n", NameSplit[1].c_str());
				// use std::set to track culture types. this will automaticly remove duplicates and sort
			}
		}
	}
		
	~GenFileNames(void) 											// deconstuctor
	{
		if (pdir != NULL)
		{
			closedir (pdir);
		}
		
	}
	
}; // end class GenFileNames


int main(void)
{
	GenFileNames GFN;
	//~ GFN
	
    return 0; // everything went OK
	
}
