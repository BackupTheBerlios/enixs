#! /bin/bash

#===============================================================================
#  Define some functions.
#===============================================================================
function usage () {
	echo ""
	echo "   Usage:  newplugin.sh [-lang <language>]"
	echo ""
	echo "   Parameters:"
    echo "      -lang <language>  Specify the language for user interactions."
	echo "                        Possible values are en and de. (Default: en)"
	echo ""
}

function uppercase() {
	UPPER=$(echo $1 | tr a-z A-Z)
}

function lowercase() {
	LOWER=$(echo $1 | tr A-Z a-z)
}

function propercase() {
	FIRST=$(echo $1 | awk '{print substr($1, 1, 1)}' | tr a-z A-Z)
	REST=$(echo $1 | awk '{print substr($1, 2)}' | tr A-Z a-z)
	PROPER=$FIRST$REST
}

function replace-string() {
	sed s/$1/$2/g < $3 > $3.tmp
	mv $3.tmp $3
}

#===============================================================================
#  Parse the arguments.
#===============================================================================
lang=en

if [ $# -gt 0 ]; then
	while [ $# -gt 0 ]; do
		if [ "$1" = "-lang" ]; then
			if [ "$2" = "en" ]; then
				lang=$2
			elif [ "$2" = "de" ]; then
				lang=$2
			else
				usage
				exit 1
			fi
			shift 2
		else
			usage
			exit 1
		fi
		done
fi

#===============================================================================
#  Request the name of the new plugin from the user.
#===============================================================================
echo ""
if [ "$lang" = "en" ]; then
	echo -n "Please enter the name of the new plugin: "
else
	echo -n "Bitte geben Sie den Namen des neuen Plugins ein: "
fi

read plugin

if [ -z $plugin ]; then
	if [ "$lang" = "en" ]; then
		echo "You have to enter a name for the new plugin."
	else
		echo "Sie müssen einen Namen für das neue Plugin eingeben."
	fi
	exit 1
fi

uppercase $plugin
lowercase $plugin
propercase $plugin

#===============================================================================
#  Create the directory for the new plugin and copy the files from the
#  template directory.
#===============================================================================
mkdir $LOWER

(cd template && tar czf ../$plugin/$LOWER.tar.gz --exclude=obj *)

cd $LOWER 
tar xzf $LOWER.tar.gz
rm $LOWER.tar.gz

#===============================================================================
#  Rename the icons in the bitmaps directory.
#===============================================================================
cd bitmaps

for i in $( ls template*.xpm )
do
	sed s/template/$LOWER/g < $i > $i.new
	mv $i.new $i
	x=`echo $i | awk '{print substr($1, 9)}'`
	mv $i $LOWER$x
done

cd ..

#===============================================================================
#  Replace all occurences of the string "template" in the source code.
#===============================================================================
for i in $( ls )
do
	if [ -d "$i" ]
	then
		continue
	fi
	replace-string "<template>" $LOWER  $i
	replace-string "<Template>" $PROPER $i
	replace-string "template"   $LOWER  $i
	replace-string "Template"   $PROPER $i
	if [ "$i" != "plugin.pro" ]
	then
		replace-string "TEMPLATE"   $UPPER  $i
	fi
done

mv template.h   $LOWER.h
mv template.cpp $LOWER.cpp
mkdir obj
chmod 755 makemake

exit 0
