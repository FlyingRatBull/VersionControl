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


#include <QtCore/QString>


#ifndef VERSIONWRITER_H
#define VERSIONWRITER_H

class VersionWriter
{
	public:
		/**
		 * @brief Writes version information to a file
		 *
		 * @param file Filename of file to write to
		 * @param major Major part
		 * @param minor Minor part
		 * @param patch Patch part
		 * @param extra Extra part
		 * @return TRUE if succesfull; FALSE otherwise
		 **/
		virtual bool write(const QString& file, int major, int minor = 0, int patch = 0, int extra = 0) = 0;
};

#endif // VERSIONWRITER_H
