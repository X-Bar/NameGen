#!/usr/bin/python

from __future__ import with_statement
import time
import mmap
import random
from collections import defaultdict

import glob
import re
import os
try:
    # Python2
    import Tkinter
except ImportError:
    # Python3
    import tkinter

IgnoreFileList = ['NameGen.py', 'NameGen.pyc', 'NameGen.exe', 'dist', 'build' 'dis\\', 'buid\\', '\dis', '\buid', '/dis', '/buid', 'dis/', 'buid/', 'NameGen.exe - Shortcut', 'NameGen.exe - Shortcut.lnk', 'NameGen.lnk', 'setup.py']

#~ http://stackoverflow.com/questions/845058/how-to-get-line-count-cheaply-in-python
# function counts number of lines in file and returns it
def mapcount(f):
    #~ f = open(filename, "r+")
    buf = mmap.mmap(f.fileno(), 0)
    lines = 0
    readline = buf.readline
    while readline():
        lines += 1
    return lines
# end function mapcount

def PrintRandomName(UserNameType, UserGenderType):
	success = 0													# success int flag (2 is success)
	i = 0														# reset counter
	for FileName in dirList:
		if FileName != ('NameGen.py' or 'NameGen.pyc'):
			#~ print FileName
			NameSplit = FileName.split('_');
			if NameSplit[0].upper() == UserNameType.upper():	# make sure file type is whats desired (culture)
				FileType =  NameSplit[1]
				if FileType[0] == 'F':							# make sure if first name
					if FileType[1] == UserGenderType.upper():	# pick right gender type
						#~ print 'yes' # get random first name
						FileNameFirst = FileName				# change variable name to avoid first/last name file opens dont go bad
						fF = open(FileNameFirst, "r+")			# open file
						NumOfLines = mapcount(fF)				# count number of lines
						RanLine = random.randint(0, NumOfLines)	# pick a random number within line numbers
						lines = fF.readlines()					# read in all the lines
						NewFirstName = lines[RanLine-1]			# pick line that was randomly chosen
						#~ f = close(FileName)					# no close function?
						success += 1
						
				if FileType[0] == 'L':							# make sure if last name
					#~ print 'yes' # get random Last name
					FileNameLast = FileName
					fL = open(FileNameLast, "r+")
					NumOfLines = mapcount(fL)					# count number of lines
					RanLine = random.randint(0, NumOfLines)
					lines = fL.readlines()
					NewLastName = lines[RanLine-1]
					#~ f = close(FileName)
					success += 1

	if success == 2:
		#~ print NewFirstName.strip('\n') + ' ' + NewLastName	# print new name, strip reline 
		return NewFirstName.strip('\n') + ' ' + NewLastName
	else:
		return 'Error: name files not found'

class simpleapp_tk(Tkinter.Tk):
    def __init__(self,parent):
        Tkinter.Tk.__init__(self,parent)
        self.parent = parent
        self.initialize()

    def initialize(self):
        self.grid()
		
        ### entry widget Gender
        self.Gender = Tkinter.StringVar(self)
        self.Gender.set('M')
        self.Genders = ['M', 'F']
        #~ self.entryMenuGen = Tkinter.StringVar()
        self.genMenu = Tkinter.StringVar()
        self.genMenu = Tkinter.OptionMenu(self, self.Gender, *self.Genders)
        self.genMenu.grid(column=2,row=1,columnspan=2, sticky='EW')
		
		# button generate
        button = Tkinter.Button(self,text=u"Generate", command=self.OnButtonClick)
        button.grid(column=1,row=2, columnspan=3, sticky='EW')
		
		#~ # label name type
        self.labelNameType = Tkinter.StringVar()
        labelName = Tkinter.Label(self,textvariable=self.labelNameType, anchor="w")# ,fg="black",bg="grey")
        labelName.grid(column=0,row=0, columnspan=2, sticky='EW')
        self.labelNameType.set(u"Enter culture type:")
        
        
        # label gender type
        self.labelGenderType = Tkinter.StringVar()
        labelGen = Tkinter.Label(self,textvariable=self.labelGenderType, anchor="w")
        labelGen.grid(column=0,row=1, columnspan=2, sticky='EW')
        self.labelGenderType.set(u"Enter gender:")
        
        # label output title
        self.labelOutPutTitle = Tkinter.StringVar()
        labelOuttitle = Tkinter.Label(self,textvariable=self.labelOutPutTitle, anchor="w")
        labelOuttitle.grid(column=0,row=3, columnspan=2, sticky='EW')
        self.labelOutPutTitle.set(u"Random Name:")

		# label output name
        self.labelOutputString = Tkinter.StringVar()
        labelOutput = Tkinter.Entry(self,textvariable=self.labelOutputString)
        labelOutput.grid(column=2,row=3, columnspan=2, sticky='EW')
        self.labelOutputString.set(u"name name")
        
        self.grid_columnconfigure(8,weight=1)
        self.resizable(True,True)
        #~ self.update()
        #~ self.geometry(self.geometry())    

		### entry widget type & change choices
    def MakeChoices(self):
		self.choices = ListOfNewTypes
		self.TypeChoice = Tkinter.StringVar(self)
		self.TypeChoice .set(self.choices[0])
		#~ self.typeMenu = Tkinter.OptionMenu(self, self.typeMenu, *self.choices)
		self.typeMenu = Tkinter.StringVar()
		self.typeMenu = Tkinter.OptionMenu(self, self.TypeChoice, *self.choices)
		self.typeMenu.grid(column=2,row=0,columnspan=2, sticky='EW')
		
    def OnButtonClick(self):
        #~ self.labelVariable.set( self.entryVariable.get()+" (You clicked the button)" )
        FinalOutput = PrintRandomName(self.TypeChoice.get(), self.Gender.get())
        self.labelOutputString.set(FinalOutput)

### MAIN
if __name__ == "__main__":
	# get current directory (cwd) to search for files in
	CurDir = os.getcwd()
	ListOfNewTypes = []
	i = 0														# count types found
	dirList = os.listdir(CurDir)								# get all file names in cwd
	for FileName in dirList:									# for all elements in directory list
		#if FileName !=  ('NameGen.py' or 'NameGen.pyc'):		# make sure we are not reading the script file
		if any(FileName in s for s in IgnoreFileList):	# find out if type is known
			pass
		else:
			# all name files have 1 underscore seperating culture type and first/last & gender
			NameSplit = FileName.split('_');
			if any(NameSplit[0] in s for s in ListOfNewTypes):	# find out if type is known
				pass
			else:	
				i += 1
				#~ ListOfNewTypes[i] = NameSplit[0]
				ListOfNewTypes.extend([NameSplit[0]])			# add new type
	
	# prepare gui
	app = simpleapp_tk(None)
	app.title('X-Bar Random Name Generator')
	app.MakeChoices()											# set ListOfNewTypes in class
	app.mainloop()
