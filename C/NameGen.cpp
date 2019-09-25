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

#include <random>
#include <fstream>
#include <vector>
#include <string>
#include <set>
//~ #include <algorithm>
//~ #include <locale>

//~ #include "boost/filesystem/operations.hpp"
//~ #include "boost/filesystem/path.hpp"
//~ #include "boost/progress.hpp"
//~ #include <iostream>
//~ namespace fs = boost::filesystem;



void PrintUsage(void)
{
	fprintf
	(stderr, "usage: NameGen request [sex] \n\n"
				
			"Request: set or culture.\n"
				"\tSet: request set of avaliable cultures.\n"
				"\tCulture: request random name of given culture type. Acceptable culture types are given by set command.\n\n"
				
			"Sex: only for culture name request. For a culture random name request a sex must be given.\n"
				"\tEnter either M or F. \n\n"
				
			"Examples: \n"
			"NameGen set\n"
			"NameGen English F"
			"NameGen French M"	
	);
}


//~ std::string UpperCase(std::string& Input)
std::string UpperCase(std::string& Input)
{
    //~ char ch = 0;
    //~ std::string Output(Input.length(), 'X');
    for(size_t i=0; i < Input.length(); i++)
    {
        if(Input[i]>=97 && Input[i]<=122)
        {
            //~ ch=Input[i]-32;
            Input[i]=Input[i]-32;
        }
        //~ else
        //~ {
            //~ ch = Input[i];
        //~ }
        //~ Output[i] = ch;
    }
	//~ printf("%s\n", Input.c_str());
	//~ Input =  Output;
	//~ return Output;
	return Input;
}

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
	
	int GetRandLine (std::string& FileName, std::string& ReturnName)
	{
		std::vector<std::string> Names;
		std::string LineTxt;
		// Open file
		std::ifstream FileH(FileName);
		if ( !(FileH.is_open()) )
		{
			ReturnName.append("ERROR! Failed to open file \"" + FileName + "\"");
			return 1; 
		}
		
		// get number of lines in first file
		while (getline(FileH, LineTxt))
		{
			//~ printf("Name: %s", LineTxt.c_str());
			Names.insert(Names.end(), LineTxt); 					// get all names
		}
		
		// generate random number between 1 and number of lines 
		// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, (int)Names.size());
		int RandomNum = dis(gen);
		//~ printf("num of lines %zd, random number %d\n", Names.size(), RandomNum);
		
		// get the line number corresponding to rand num
		ReturnName = Names[RandomNum];
		
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
		if (CultureSet.find(ReqCulture) == CultureSet.end())
		{
			ReturnName.append("ERROR! numbered errors: 0: ERROR! Cannot get random name. Requested culture not found. Found cultures are: ");
			for (auto it : CultureSet)
			{
				ReturnName.append(it + ", ");
			}
			ReturnName.pop_back();
			ReturnName.pop_back(); 								// remove final ", "
			ReturnName.append(". ");
			return 2;
		}
		
		
		// construct file names
		std::string FileNameFirst = "NameFile_" + ReqCulture + "_F" + UpperCase(ReqSex) + ".txt";
		std::string FileNameLast = "NameFile_" + ReqCulture + "_LN" + ".txt";
		//~ printf("First NameFile: %s\n", FileNameFirst.c_str());
		//~ printf("Last NameFile: %s\n", FileNameLast.c_str());
		
		// Get rand name first
		std::string NameFirst;
		res = GetRandLine(FileNameFirst, NameFirst);
		if (res)
		{
			ReturnName.append("ERROR! numbered errors: 0: ERROR! Cannot get random First name. 1: ");
			ReturnName.append(NameFirst);
			return res;
		}
		
		// Get rand name last
		std::string NameLast;
		res = GetRandLine(FileNameLast, NameLast);
		if (res)
		{
			ReturnName.append("ERROR! numbered errors: 0: ERROR! Cannot get random Last name. 1: ");
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


int main(int argc, char **argv)
{
	//~ printf("Recieved request. Number of inputs: %d\n", argc-1);
	
	if ( 2 > argc || argc > 3 ) 									// check for minimum input
	{
			//~ printf("Error! Request minimum of 1 input and maxium of 2 inputs. Inputs given: %d\n", argc-1);
			fprintf(stderr, "Error! Request minimum of 1 input and maxium of 2 inputs. Inputs given: %d\n", argc-1);
			PrintUsage();
			return 1;        
	}
	std::vector<std::string> ProgInputs(argc-1);
	for (int i = 1; i < argc; i++) 									// print input
	{
		//~ ProgInputs.insert(myvector.end(), argv[i]);
		ProgInputs[i-1] = argv[i];
		//~ std::transform(ProgInputs.begin(), ProgInputs.end(), ProgInputs.begin(), (int (*)(int))std::toupper); 
		//~ for (auto & c: ProgInputs) c = (char)::toupper(c);
		//~ UpperCase(ProgInputs[i-1]);
		//~ printf("Arg #%d of %d: %s\n", i, argc-1, argv[i]); 			// first input is filename, ignore 
		//~ printf("Arg #%d of %d: %s\n", i, argc-1, ProgInputs[i-1].c_str()); 	// first input is filename, ignore 
		
	}
	
	if ((ProgInputs.size() == 2) && ( ProgInputs[1].length() == 1 )) // 1 input = get set, 2 input = get name
	{
		printf("Getting random name\n");
		//~ if (strlen(argv[2]) > 1)
		//~ {
			//~ fprintf(stderr, "ERROR! Sex input can only be 1 char long.\n");
			//~ PrintUsage();
			//~ return 2;
		//~ }
		GenFileNames GFN;
		//~ std::string ReqCulture(ProgInputs[0]);
		//~ std::string ReqSex(ProgInputs[1]);
		std::string Name;
		
		//~ int res = GFN.GetRandName("English", 'M', OutputName);
		int res = GFN.GetRandName(ProgInputs[0], ProgInputs[1], Name);
		if (res)
		{
			fprintf(stderr, "ERROR! Failed to get random name. Code: %d. %s\n", res, Name.c_str());
		}
		else
		{
			printf("%s %s NAME: %s\n", ProgInputs[0].c_str(), ProgInputs[1].c_str(), Name.c_str());
		}
	}
	else if((ProgInputs.size() == 1) && ( ProgInputs[0] == "set" )) // 1 input = get set, 2 input = get name
	{
		printf("Getting Culture set list\n");
		std::set<std::string> CultureSet;
		GenFileNames GFN;
		int res = GFN.GetCultures(CultureSet);
		
		if (!res)
		{
			printf("\nCultures: \n\n");
			for (auto i : CultureSet)
			{
				fprintf(stderr, "%s \n", i.c_str());
			}
			fprintf(stderr, " \n");
		}
		else
		{
			fprintf(stderr, "ERROR! Could not read Cultures. Error Code %d. Errors: \n", res);
			for (auto i : CultureSet)
			{
				fprintf(stderr, "%s \n", i.c_str());
			}
			fprintf(stderr, " \n");
			return 3;
		}
	}
	else
	{
		fprintf(stderr, "ERROR! input error. Use proper input commands. \n"); // likey not using proper key words
		PrintUsage();
		return 4;
	}
	
	
    return 0; // everything went OK
	
}
