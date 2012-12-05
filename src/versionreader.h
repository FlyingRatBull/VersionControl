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
		
		virtual bool read(const QString& file) = 0;
		
		int major() const;
		
		int minor() const;
		
		int patch() const;
		
		int extra() const;
		
	protected:
		void setVersion(int major, int minor = 0, int patch = 0, int extra = 0);
		
	private:
		int			m_major;
		int			m_minor;
		int			m_patch;
		int			m_extra;
};

#endif // VERSIONREADER_H
