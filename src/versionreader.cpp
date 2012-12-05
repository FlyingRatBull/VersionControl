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


#include "versionreader.h"

VersionReader::VersionReader()
{
	m_major	=	0;
	m_minor	=	0;
	m_patch	=	0;
	m_extra	=	0;
}


int VersionReader::major() const
{
	return m_major;
}


int VersionReader::minor() const
{
	return m_minor;
}


int VersionReader::patch() const
{
	return m_patch;
}


int VersionReader::extra() const
{
	return m_extra;
}


void VersionReader::setVersion(int major, int minor, int patch, int extra)
{
	m_major	=	major;
	m_minor	=	minor;
	m_patch	=	patch;
	m_extra	=	extra;
}
