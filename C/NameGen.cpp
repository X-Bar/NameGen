/***********************************************************************************************************************
FILENAME:   NameGen.cpp
AUTHORS:    Cody L. Lundberg

DESCRIPTION:
Name generator that can read names from files to create different random cultural names
* name parts
* sex (male, female)
* type (first last) 
all held in 3 different files (male first, female first, last)
user selects sex and culture and receives names

Convertion from python code

REVISION HISTORY:
01.07.2014   CDM     original file creation
Y2019 M09 D03        Updating file with features and STL
***********************************************************************************************************************/

#include <dirent.h>
#include <stdio.h>
//~ #include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <vector>
#include <string>
#include <set>

//~ #include "boost/filesystem/operations.hpp"
//~ #include "boost/filesystem/path.hpp"
//~ #include "boost/progress.hpp"
//~ #include <iostream>
//~ namespace fs = boost::filesystem;





class GenFileNames
{
private: // private data
	DIR *pdir;
	//~ std::set<std::string> Cultures; 
	
	
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
	
	std::string ConstructErrorStringFromSet (const std::set<std::string> ErrorSet)
	{
		std::string ErrorString; 
		int i = 0;
		for (auto it : ErrorSet)
		{
			// Errors will be strings inside of ErrorSet, now we append them to our error string
			ErrorString.append(std::to_string(++i) + " " + it + ". ");
			ErrorString.append(it.c_str());
		}
		return ErrorString;
	}
	
	int GetRandLine (std::string FileName, std::string ReturnName)
	{
		// Open first file
		
		// get number of lines in first file
		
		// generate random number between 1 and number of lines
		
		// get the line number corresponding to rand num
		
		return 0;
	}
	
public: // public methods
	//~ GenFileNames(void) 									// constructor
	//~ {
		
	//~ }
	
	int GetCultures(std::set<std::string>& OutputSet)
	{
		// http://www.dreamincode.net/forums/topic/60036-accessing-directories-in-cc-part-ii/
		pdir = NULL;
		struct dirent *pent = NULL;
		int ChdirError = 0;
		
		ChdirError = chdir("../Names");
		if ( ChdirError != 0 )
		{
			//~ perror("chdir() to /Names failed\n");
			//~ printf("ERROR! chdir() to /Names failed\n");
			std::string Error ("ERROR! chdir() to /Names failed. chdir error number: " + std::to_string(ChdirError));
			OutputSet.insert(Error);
			return 1;
		}
		
		pdir = opendir ("."); 										// "." will refer to the current directory
		if (pdir == NULL) 											// if pdir wasn't initialised correctly
		{ 															// print an error message and exit the program
			//~ printf ("ERROR! pdir could not be initialised correctly\n");
			//~ exit (2);	 											// exit the program, using 2 as the status (most common for a failed execution)
			OutputSet.insert("ERROR! pdir could not be initialised correctly");
			return 2;
		} // end if

		while ( (pent = readdir (pdir) ) ) 							// while there is still something in the directory to list
		{
			if (pent == NULL) 										// if pent has not been initialised correctly
			{ 														// print an error message, and exit the program
				//~ printf ("ERROR! \"struct dirent *pent\" could not be initialised correctly\n");
				//~ exit (3);
				OutputSet.insert("ERROR! \"struct dirent *pent\" could not be initialised correctly");
				return 3;
			}
			//~ printf ("Found File: %s\n", pent->d_name);				// otherwise, it was initialised correctly. let's print it on the console:
			std::string FileName(pent->d_name);
			std::vector<std::string> NameSplit = SplitString(FileName, '_');
			if ( (NameSplit.size() < 3) )
			{	
				fprintf(stderr, "File \"%s\" not a NameFile. Not dilimited correctly. Ignoring.\n", FileName.c_str());
			}
			else if (NameSplit[0] != "NameFile")
			{
				fprintf(stderr, "File \"%s\"not a NameFile. Incorrect prefix. Ignoring.\n", FileName.c_str());
			}
			else
			{
				//~ printf("Culture %s\n", NameSplit[1].c_str());
				// use std::set to track culture types. this will automatically remove duplicates and sort
				OutputSet.insert(NameSplit[1]);
			}
			
		}
		//~ for (auto it = Cultures.begin(); it < Cultures.end(); it++)
		//~ printf(" \n");
		//~ for (auto i : Cultures)
		//~ {
			//~ printf("%s \n", i.c_str());
		//~ }
		//~ printf(" \n");
		closedir (pdir);
		pdir = NULL;
		
		
		
		return 0;
	}
	
	
	int GetRandName(std::string& ReqCulture, std::string& ReqSex, std::string& ReturnName)
	{
		std::set<std::string> CultureSet;
		int res = GetCultures(CultureSet); 						// call internal method to get culture list
		if (res) 												// GetCultures reported error
		{
			ReturnName.append("ERROR! numbered errors: 0: ERROR! Cannot get random name. Failed to get culture set. "); 
			// Return string will be a string of errors
			ReturnName.append(ConstructErrorStringFromSet(CultureSet));
			return res;
		}
		// construct file names
		std::string FileNameFirst = "NameFile_" + ReqCulture + "F" + ReqSex;
		std::string FileNameLast = "NameFile_" + ReqCulture + "LN";
		
		// Get rand name first
		std::string NameFirst;
		res = GetRandLine(FileNameFirst, NameFirst);
		if (!res)
		{
			ReturnName.append("ERROR! numbered errors: 0: ERROR! Cannot get random First name. 1:");
			ReturnName.append(NameFirst);
			return res;
		}
		
		// Get rand name last
		std::string NameLast;
		res = GetRandLine(FileNameLast, NameLast);
		if (!res)
		{
			ReturnName.append("ERROR! numbered errors: 0: ERROR! Cannot get random Last name. 1:");
			ReturnName.append(NameFirst);
			return res;
		}
		
		ReturnName = NameFirst + " " + NameLast;
		return res;
	}
		
	~GenFileNames(void) 											// deconstuctor
	{
		if (pdir != NULL)
		{
			closedir (pdir);
		}
		
	}
	
}; // end class GenFileNames

void PrintUsage(void)
{
	fprintf(stderr, "Usage: \n");
}


int main(int argc, char **argv)
{
	printf("Recieved request. Number of inputs: %d\n", argc-1);
	
	if ( 2 > argc || argc > 3 ) 									// check for minimum input
	{
			//~ printf("Error! Request minimum of 1 input and maxium of 2 inputs. Inputs given: %d\n", argc-1);
			fprintf(stderr, "Error! Request minimum of 1 input and maxium of 2 inputs. Inputs given: %d\n", argc-1);
			PrintUsage();
			return 1;        
	}
	for (int i = 1; i < argc; i++) 									// print input
	{
		printf("Arg #%d of %d: %s\n", i, argc-1, argv[i]); 			// first input is filename, ignore 
	}
	
	if (argc-1 > 1) 												// 1 input = get set, 2 input = get name
	{
		printf("Getting random name\n");
		
		if (strlen(argv[2]) > 1)
		{
			fprintf(stderr, "ERROR! Sex input can only be 1 char long.\n");
			PrintUsage();
			return 2;
		}
		
		
		
		GenFileNames GFN;
		std::string ReqCulture(argv[1]);
		std::string ReqSex("M");
		std::string Name;
		//~ int res = GFN.GetRandName("English", 'M', Name);
		int res = GFN.GetRandName(ReqCulture, ReqSex, Name);
		printf("NAME: %d %s\n", res, Name.c_str());
	}
	else
	{
		printf("Getting Culture set list\n");
		std::set<std::string> CultureSet;
		GenFileNames GFN;
		int res = GFN.GetCultures(CultureSet);
		
		if (!res)
		{
			printf("Cultures: \n");
			for (auto i : CultureSet)
			{
				fprintf(stderr, "%s \n", i.c_str());
			}
			fprintf(stderr, " \n");
		}
		else
		{
			//~ std::set<std::string>::iterator Error = CultureList.begin();
			//~ printf("Could not read Cultures. Error Code %d. Error: %s\n", res, (*Error).c_str() );
			fprintf(stderr, "ERROR! Could not read Cultures. Error Code %d. Errors: \n", res);
			for (auto i : CultureSet)
			{
				fprintf(stderr, "%s \n", i.c_str());
			}
			fprintf(stderr, " \n");
			return 3;
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
    return 0; // everything went OK
	
}
