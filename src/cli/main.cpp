/*
 *  VersionControl - Tool for handling version files
 *  Copyright (C) 2012  Oliver Becker <der.ole.becker@gmail.com>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/qglobal.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QHash>
#include <QDir>


#include "versionfilereader.h"
#include "versionheaderreader.h"
#include "versionfilewriter.h"
#include "versionheaderwriter.h"



QString										mainAction;
QHash<QString, QVariant>	mainOptions;


void printHelp()
{
	qDebug("%s %s %s",
				 qPrintable(QObject::tr("Usage:")),
				 qPrintable(QCoreApplication::applicationFilePath().split(QDir::separator()).last()),
				 qPrintable(QObject::tr("Action [Options]")));
	qDebug("  %s", qPrintable(QObject::tr("Where Action is one of:")));
	qDebug("    set      major[.minor[.patch[-extra]]]      %s", qPrintable(QObject::tr("Set version information.")));
	qDebug("    raise    major|minor|patch|extra            %s", qPrintable(QObject::tr("Raise the specified field.")));
	qDebug("                                                %s", qPrintable(QObject::tr("This sets all following fields to 0.")));
	qDebug("  %s", qPrintable(QObject::tr("And Options is none or any of:")));
	qDebug("    --output-dir | -o         path              %s", qPrintable(QObject::tr("Directory to write output files into.")));
	qDebug("                                                %s", qPrintable(QObject::tr("Default is current directory.")));
	qDebug("    --read-header | -h                          %s", qPrintable(QObject::tr("Read current header information", "First line")));
	qDebug("                                                %s", qPrintable(QObject::tr("from \"version.h\" instead of \"VERSION\".", "Second line")));
	qDebug("                                                %s", qPrintable(QObject::tr("Does not apply to action \"set\".", "Extra line")));
	qDebug("    --force | -f                                %s", qPrintable(QObject::tr("Force creation of version file when it does not exist")));
}


void initOptions()
{
	// Init default values for options
	mainOptions["output-dir"]					=	QDir::currentPath();
	mainOptions["read-version-file"]	=	"VERSION";
	mainOptions["force"]							=	false;
}


bool scanAction(const QString& action, QStringList& args)
{
	mainAction	=	action;
	
	if(action == "set")
	{
		// Check format of supplied version string
		QRegExp		versionReg("^\\s*([0-9]+)(\\.([0-9]+))?(\\.([0-9]+)(\\-([0-9]+))?)?\\s*$");
		if(!versionReg.exactMatch(args.takeFirst()))
		{
			qCritical("%s %s: %s", qPrintable(QObject::tr("Action")), qPrintable(action), qPrintable(QObject::tr("Invalid version format")));
			return false;
		}
		
		// Read version information into options
		mainOptions["set-version-major"]	=	versionReg.capturedTexts()[1].toInt();
		
		if(versionReg.captureCount() > 2)
			mainOptions["set-version-minor"]	=	versionReg.capturedTexts()[3].toInt();
		
		if(versionReg.captureCount() > 4)
			mainOptions["set-version-patch"]	=	versionReg.capturedTexts()[5].toInt();
		
		if(versionReg.captureCount() > 6)
			mainOptions["set-version-extra"]	=	versionReg.capturedTexts()[7].toInt();
		
		return true;
	}
	else if(action == "raise")
	{
		// Check format of supplied version string
		QRegExp		versionReg("^\\s*(major|minor|patch|extra)\\s*$");
		QString		version(args.takeFirst());
		
		if(!versionReg.exactMatch(version))
		{
			qCritical("%s %s: %s", qPrintable(QObject::tr("Action")), qPrintable(action), qPrintable(QObject::tr("Invalid argument given")));
			return false;
		}
		
		mainOptions["raise-version"]	=	version;
		
		return true;
	}
	else if(action == "reset")
	{
		mainOptions["read-version-file"]	=	"VERSION.old";
		return true;
	}
	
	qCritical("%s", qPrintable(QObject::tr("Unknown action given:")));
	qCritical("  %s", qPrintable(action));
	
	mainAction.clear();
	
	return false;
}


bool scanOption(const QString& option, const QStringList& args, int& idx, bool isShort = false)
{
	// Output directory
	if(isShort ? option == "o" : option == "output-dir")
	{
		if(args.count() <= idx + 1)
		{
			qCritical("%s output-dir: %s", qPrintable(QObject::tr("Option")), qPrintable(QObject::tr("Path required")));
			return false;
		}
		
		// Check given path
		QString	path(args[++idx]);
		
		if(!QFileInfo(path).isDir())
		{
			qCritical("%s output-dir: %s", qPrintable(QObject::tr("Option")), qPrintable(QObject::tr("Invalid path given (not a directory)")));
			return false;
		}
		
		mainOptions["output-dir"]	=	path;
	}
	// Read version from header
	else if(isShort ? option == "h" : option == "read-header")
	{
		mainOptions["read-version-file"]	=	(mainAction == "version.hreset" ? ".old" : "version.h");
	}
	// Force
	else if(isShort ? option == "f" : option == "force")
		mainOptions["force"]	=	true;
	
	return true;
}


bool scanOptions(const QStringList& options)
{
	// List of unrecognized options
	QStringList	failedOptions;
	
	for(int idx = 0; idx < options.count(); idx++)
	{
		const QString&	option(options.at(idx));
		
		// All options start with at least one -
		if(option.length() < 2 || option.at(0) != QChar('-'))
		{
			failedOptions.append(option);
			continue;
		}
		
		// Long options
		if(option.at(1) == QChar('-'))
		{
			QString	opt(option.mid(2));
			
			if(!scanOption(opt, options, idx))
			{
				failedOptions.append(option);
				continue;
			}
		}
		// Short options
		else
		{
			QString	opt(option.mid(1));
			
			if(!scanOption(opt, options, idx, true))
			{
				failedOptions.append(option);
				continue;
			}
		}
	}
	
	if(!failedOptions.isEmpty())
	{
		qCritical("%s", qPrintable(QObject::tr("Unknown option(s) given:", "Unrecognized option(s)", failedOptions.count())));
		
		foreach(const QString& option, failedOptions)
			qCritical("  %s", qPrintable(option));
		
		return false;
	}
	
	return true;
}


bool takeActions()
{
	// Read version information if we are not setting it explicitly
	if(mainAction	!= "set")
	{
		VersionReader		*	reader	=	0;
		
		if(mainOptions["read-version-file"].toString().replace(".old", "") == "VERSION")
			reader	=	new VersionFileReader();
		else
			reader	=	new VersionHeaderReader();
		
		if(!reader->read(mainOptions["output-dir"].toString() + QDir::separator() + mainOptions["read-version-file"].toString()))
		{
			if(mainOptions["force"].toBool())
				qDebug("%s", qPrintable(QObject::tr("Could not read version file \"%1\" - Forcing creation").arg(mainOptions["output-dir"].toString() + QDir::separator() + mainOptions["read-version-file"].toString())));
			else
			{
				qCritical("%s", qPrintable(QObject::tr("Could not read version file \"%1\" - Use --force to force creation").arg(mainOptions["output-dir"].toString() + QDir::separator() + mainOptions["read-version-file"].toString())));
				delete reader;
				return false;
			}
		}
		
		// Read version information into options
		mainOptions["set-version-major"]	=	reader->major();
		mainOptions["set-version-minor"]	=	reader->minor();
		mainOptions["set-version-patch"]	=	reader->patch();
		mainOptions["set-version-extra"]	=	reader->extra();
		
		delete reader;
	}
	
	if(mainAction == "raise")
	{
		QStringList	levels	=	QStringList() << "major" << "minor" << "patch" << "extra";
		mainOptions["set-version-" + mainOptions["raise-version"].toString()]	=	mainOptions["set-version-" + mainOptions["raise-version"].toString()].toInt() + 1;
		
		// Reset following values
		for(int i = levels.indexOf(mainOptions["raise-version"].toString()) + 1; i < 4; i++)
			mainOptions["set-version-" + levels[i]]	=	0;
	}
	
	// Backup version information
	QFile::remove(mainOptions["output-dir"].toString() + QDir::separator() + "version.h.old");
	QFile::remove(mainOptions["output-dir"].toString() + QDir::separator() + "VERSION.old");
	
	if(mainAction != "reset")
	{
		QFile::rename(mainOptions["output-dir"].toString() + QDir::separator() + "version.h", mainOptions["output-dir"].toString() + QDir::separator() + "version.h.old");
		QFile::rename(mainOptions["output-dir"].toString() + QDir::separator() + "VERSION", mainOptions["output-dir"].toString() + QDir::separator() + "VERSION.old");
	}
	
	// Write version information
	VersionWriter	*	writer	=	new VersionFileWriter();
	writer->write(mainOptions["output-dir"].toString() + QDir::separator() + "VERSION",
								mainOptions["set-version-major"].toInt(), mainOptions["set-version-minor"].toInt(), mainOptions["set-version-patch"].toInt(), mainOptions["set-version-extra"].toInt());
	delete writer;
	
	writer	=	new VersionHeaderWriter();
	writer->write(mainOptions["output-dir"].toString() + QDir::separator() + "version.h",
								mainOptions["set-version-major"].toInt(), mainOptions["set-version-minor"].toInt(), mainOptions["set-version-patch"].toInt(), mainOptions["set-version-extra"].toInt());
	delete writer;
	
	return true;
}


int main(int argc, char ** argv)
{
	QCoreApplication	app(argc, argv);
	
	// Initialize default options
	initOptions();
	
	// Parse args
		// We need at least one argument
	if(argc < 2)
	{
		qCritical("%s", qPrintable(QObject::tr("Not enough arguments given!")));
		printHelp();
		
		return 1;
	}
	
	// Get arguments without command
	QStringList	args(app.arguments().mid(1));
	
	// Scan actions
	if(!scanAction(args.takeFirst(), args))
		return 1;
	
	// Scan options
	if(!scanOptions(args))
		return 1;
	
	// Take actions
	if(!takeActions())
		return 2;
	
	return 0;
}
