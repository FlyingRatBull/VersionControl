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

#define TR_CONSOLE(...) qPrintable(QObject::tr(__VA_ARGS__))


QString										mainAction;
QHash<QString, QVariant>	mainOptions;


void printHelp()
{
	qDebug("%s %s %s",
				 TR_CONSOLE("Usage:"),
				 qPrintable(QCoreApplication::applicationFilePath().split(QDir::separator()).last()),
				 TR_CONSOLE("Action [Options]"));
	qDebug("  %s", TR_CONSOLE("Where Action is one of:"));
	qDebug("    set      major[.minor[.patch[-extra]]]      %s", TR_CONSOLE("Set version information"));
	qDebug("    raise    major|minor|patch|extra            %s", TR_CONSOLE("Raise the specified field"));
	qDebug("                                                %s", TR_CONSOLE("This sets all following fields to 0"));
	qDebug("  %s", TR_CONSOLE("And Options is none or any of:"));
	qDebug("    --output-dir | -o         path              %s", TR_CONSOLE("Directory to write output files into"));
	qDebug("                                                %s", TR_CONSOLE("Default is current directory"));
}


void initOptions()
{
	// Init default values for options
	mainOptions["output-dir"]	=	QDir::currentPath();
}


bool scanAction(const QString& action)
{
	if(action == "set" || action == "raise")
	{
		mainAction	=	action;
		return true;
	}
	
	qCritical("%s", TR_CONSOLE("Unknown action given:"));
	qCritical("  %s", qPrintable(action));
	
	return false;
}


bool scanOption(const QString& option, const QStringList& args, int& idx, bool isShort = false)
{
	if(isShort ? option == "o" : option == "output-dir")
	{
		if(args.count() <= idx + 1)
		{
			qCritical("%s output-dir: %s", TR_CONSOLE("Option"), TR_CONSOLE("Path required"));
			return false;
		}
		
		// Check given path
		QString	path(args[++idx]);
		
		if(!QFileInfo(path).isDir())
		{
			qCritical("%s output-dir: %s", TR_CONSOLE("Option"), TR_CONSOLE("Invalid path given (not a directory)"));
			return false;
		}
		
		mainOptions["output-dir"]	=	path;
	}
	
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
		qCritical("%s", TR_CONSOLE("Unknown option(s) given:", "Unrecognized option(s)", failedOptions.count()));
		
		foreach(const QString& option, failedOptions)
			qCritical("  %s", qPrintable(option));
		
		return false;
	}
	
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
		qCritical("%s", TR_CONSOLE("Not enough arguments given!"));
		printHelp();
		
		return 1;
	}
	
	// Scan actions
	if(!scanAction(app.arguments().first()))
		return 1;
	
	// Scan options
	if(!scanOptions(app.arguments().mid(1)))
		return 1;
	
	return 0;
}
