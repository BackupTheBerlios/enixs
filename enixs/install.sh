#! /bin/bash

#===============================================================================
#  Define a function for displaying the usage information.
#===============================================================================
function usage () {
	echo ""
	echo "   Usage:  install.sh [-lang <language>] [-deinstall]"
	echo ""
	echo "   Parameters:"
    echo "      -lang <language>  Specify the language for user interactions."
	echo "                        Possible values are en and de. (Default: en)"
	echo "      -deinstall        Remove all components of the EniXs software."
	echo ""
}

#===============================================================================
#  Parse the arguments.
#===============================================================================
lang=en
deinstall=no

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
		elif [ "$1" = "-deinstall" ]; then
			deinstall=yes
			shift
		else
			usage
			exit 1
		fi
		done
fi

#===============================================================================
#  Check if the current user is "root".
#===============================================================================
id=`id | sed s/\(.*// | sed s/uid=//`

if [ "$id" -ne "0" ]; then
	echo ""
	if [ "$lang" = "en" ]; then
		echo "   You must be root to execute this installation script."
	else
		echo "   Sie müssen root sein, um dieses Installationsskript auszuführen."
	fi
	echo ""
    exit 1
fi

#===============================================================================
#  Check if EniXs should be deinstalled.
#===============================================================================
if [ "$deinstall" = "yes" ]; then
	instdir=`grep "export ENIXSDIR" /usr/bin/enixs | sed s/"export ENIXSDIR="//`
	rm -rf $instdir
	rm -f /usr/bin/enixs

	echo ""
	if [ "$lang" = "en" ]; then
		echo "   Your EniXs installation has been removed completely."
	else
		echo "   Ihre EniXs-Installation wurde vollständig entferent."
	fi
	echo ""
    exit 1
fi

#===============================================================================
#  Check if the TAR file exists in the current directory.
#===============================================================================
if [ ! -e ./enixs.tar.gz ]; then
	echo ""
	if [ "$lang" = "en" ]; then
		echo "   The installation file enixs.tar.gz does not exist "
		echo "   in the current directory."
	else
		echo "   Die Installationsdatei enixs.tar.gz ist nicht "
		echo "   im aktuellen Verzeichnis vorhanden."
	fi
	echo ""
    exit 1
fi

#===============================================================================
#  Request the installation directory from the user.
#===============================================================================
echo ""
if [ "$lang" = "en" ]; then
	echo -n "Please enter the installation path "
else
	echo -n "Bitte geben Sie das Installationverzeichnis ein "
fi
echo "[/usr/local]:"

read instdir

if [ -z $instdir ]; then
	instdir="/usr/local"
fi

#===============================================================================
#  Install the TAR file into the installation directory.
#===============================================================================
srcdir=`pwd`

(cd $instdir && tar xzf $srcdir/enixs.tar.gz)
chown -R root.root $instdir/enixs

#===============================================================================
#  Create the start script in /usr/bin.
#===============================================================================
script=/usr/bin/enixs
echo "#! /bin/bash" > $script
echo "" >> $script
echo "export ENIXSDIR=$instdir/enixs" >> $script
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$ENIXSDIR/lib" >> $script
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:/opt/sapdb/interfaces/odbc/lib" >> $script
echo "\$ENIXSDIR/bin/enixs -lang $lang \$@" >> $script

chmod 755 $script

exit 0
