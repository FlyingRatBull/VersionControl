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


#include "versionfilewriter.h"

#include <QtCore/QFile>

bool VersionFileWriter::write(const QString &file, int major, int minor, int patch, int extra)
{
	QFile	outfile(file);
	
	if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	
	QString	versionString(QString("%1.%2.%3").arg(major).arg(minor).arg(patch));
	
	if(extra > 0)
		versionString.append(QString("-%1").arg(extra));
	
	versionString.append("\n");
	outfile.write(versionString.toAscii());
	
	outfile.close();
	
	return true;
}
