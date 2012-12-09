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


#include "versionheaderreader.h"

#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

bool VersionHeaderReader::read(const QString &file)
{
	QFile	infile(file);
	
	if(!infile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QRegExp		versionReg("^\\s*#define\\s+VERSION\\s+\\\"([0-9]+)(\\.([0-9]+))?(\\.([0-9]+)(\\-([0-9]+))?)?\\s*\\\"\\s*$");
	
	while(!infile.atEnd())
	{
		// The line may only contain up to 40 characters
		QString	line(infile.readLine(40));
		
		if(versionReg.exactMatch(line))
		{
			// Set version information
			int	major	=	0;
			int	minor	=	0;
			int	patch	=	0;
			int	extra	=	0;
			
			major	=	versionReg.capturedTexts()[1].toInt();
			
			if(versionReg.captureCount() > 2)
				minor	=	versionReg.capturedTexts()[3].toInt();
			
			if(versionReg.captureCount() > 4)
				patch	=	versionReg.capturedTexts()[5].toInt();
			
			if(versionReg.captureCount() > 6)
				extra	=	versionReg.capturedTexts()[7].toInt();
			
			setVersion(major, minor, patch, extra);
			
			return true;
		}
	}
	
	return false;
}

