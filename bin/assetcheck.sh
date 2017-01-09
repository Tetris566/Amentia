#!/bin/bash

# assetcheck.sh is a script that is ran against the 
# Google Dropbox folder:  Assets_NewVersions_NeedPushToGit

# Verify you are running assetcheck from the correct directory
currdir=${PWD##*/}
reqdir='KOA_ProductionBuild'
if [ "$currdir" != "$reqdir" ]; then
	echo "INVALID ACCESS: change directory to KOA_ProductionBuild"
	exit
fi

# extract the simplified last commit
LAST_COMMIT=$(git log -n 1 --pretty=format:"%h" 2>&1)
echo $LAST_COMMIT

# check to see if an error log exists; create if needed
if [ -f assetcheck.ERRORLOG ]; then 
	echo "assetcheck.ERRORLOG: found"
else 
	echo "assetcheck.ERRORLOG: **missing**"
	echo "Creating assetcheck.ERRORLOG"
	echo -e "#assetcheck.ERRORLOG\n" > assetcheck.ERRORLOG
fi



#check to see if there is a new file log; create if needed
if [ -f assetCheck.NEWFILES ]; then
	echo -e "assetcheck.NEWFILES: found\n"
else
	echo "assetcheck.NEWFILES: **missing**"
	echo -e "Createing assetcheck.NEWFILES\n"
	echo -e "#assetcheck.NEWFILES\n" > assetcheck.NEWFILES
fi

# make sure you are sitting next to the ./Assets_NewVersions_NeedPushToGit
if [ -d $Assets_NewVersions_NeedPushToGit ]; then
	#echo "Number of directories: $(find $Assets_NewVersions_NeedPushToGit -maxdepth 1 -type f | wc -l)"
	#numfiles=($Assets_NewVersions_NeedPushToGit*)
	#numfiles=${#numfiles[@]}
	#
	#numdirs=(*/)
	#numdirs=${#numdirs[@]}
	#(( numfiles -= numdirs ))
	#echo $numfiles
	numfiles=$(find Assets_NewVersions_NeedPushToGit -type f )
	file_list=(`echo $numfiles | tr ' ' ' '`)
	echo -e "Number of new files: ${#file_list[@]}\n"
	echo -e "Number of new files: ${#file_list[@]}\n" >> assetcheck.NEWFILES

	dirname=Assets_NewVersions_NeedPushToGit
	dirlen=${#dirname}+1
	
	echo ${file_list[0]:$dirlen}
	echo ${file_list[1]:$dirlen}
	echo ${file_list[2]:$dirlen}
	echo ${file_list[0]:$dirlen} >> assetcheck.NEWFILES
	echo ${file_list[1]:$dirlen} >> assetcheck.NEWFILES
	echo ${file_list[2]:$dirlen} >> assetcheck.NEWFILES
	
	else "Assets_NewVersions_NeedPushToGit/ dir not found!"
fi

#pathToDir=${PWD##*/}

#ls c:/Users/Moses/Documents/_school/_CURRENT_SUMMER_2016/KOA_Production/_BackupProduction/Assets_NewVersions_NeedPushToGit

#ls Assets_NewVersions_NeedPushToGit -R
