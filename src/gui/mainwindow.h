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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QDialog>

namespace Ui {
	class MainWindow;
} // namespace Ui


class MainWindow : public QDialog
{
	Q_OBJECT
	
	public:
		explicit MainWindow(QWidget * parent = 0);
		
		virtual ~MainWindow();
		
private slots:
		void on_actionRaise_toggled();
		
		void on_actionSet_toggled();
		
		void on_buttonBox_accepted();
		
		void calculateNewVersion();
	
	private:
		Ui::MainWindow			*	m_ui;
		
		unsigned	int					m_cVMajor;
		unsigned	int					m_cVMinor;
		unsigned	int					m_cVPatch;
		unsigned	int					m_cVExtra;
};

#endif // MAINWINDOW_H
