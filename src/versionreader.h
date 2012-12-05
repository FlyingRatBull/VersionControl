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


#ifndef VERSIONREADER_H
#define VERSIONREADER_H

class VersionReader
{
	public:
		explicit VersionReader();
		
		/**
		 * @brief Reads version values from a file
		 *
		 * @param file Filename of file to read from
		 * @return TRUE if seccussfully read; FALSE otherwise
		 **/
		virtual bool read(const QString& file) = 0;
		
		/**
		 * @brief Major part of version
		 * 
		 * X.y.z-e
		 *
		 * @return int
		 **/
		int major() const;
		
		/**
		 * @brief Minor part of version
		 * 
		 * x.Y.z-e
		 *
		 * @return int
		 **/
		int minor() const;
		
		/**
		 * @brief Patch part of version
		 * 
		 * x.y.Z-e
		 *
		 * @return int
		 **/
		int patch() const;
		
		/**
		 * @brief Extra part of version
		 * 
		 * x.y.z.-E
		 *
		 * @return int
		 **/
		int extra() const;
		
	protected:
		/**
		 * @brief Sets the version information contained in the file
		 *
		 * @param major Major part
		 * @param minor Minor part
		 * @param patch Patch part
		 * @param extra Extra part
		 * @return void
		 **/
		void setVersion(int major, int minor = 0, int patch = 0, int extra = 0);
		
	private:
		int			m_major;
		int			m_minor;
		int			m_patch;
		int			m_extra;
};

#endif // VERSIONREADER_H
