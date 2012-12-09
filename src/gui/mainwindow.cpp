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

#include "mainwindow.h"

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QDialog(parent)
{
	// Set up ui
	m_ui	=	new Ui::MainWindow();
	m_ui->setupUi(this);
	
	m_cVMajor	=	0;
	m_cVMinor	=	0;
	m_cVPatch	=	0;
	m_cVExtra	=	0;
	
	// Read current version
	QFile			versionFile("VERSION");
	QString		version("0.0.0");
	QRegExp		versionReg("^\\s*([0-9]+)(\\.([0-9]+))?(\\.([0-9]+)(\\-([0-9]+))?)?\\s*$");
	
	if(versionFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		version	=	versionFile.readLine().trimmed();
		
		if(versionReg.exactMatch(version))
			m_ui->currentVersion->setText(version);
		
		m_cVMajor	=	versionReg.capturedTexts()[1].toInt();
		
		if(versionReg.captureCount() > 2)
			m_cVMinor	=	versionReg.capturedTexts()[3].toInt();
		
		if(versionReg.captureCount() > 4)
			m_cVPatch	=	versionReg.capturedTexts()[5].toInt();
		
		if(versionReg.captureCount() > 6)
			m_cVExtra	=	versionReg.capturedTexts()[7].toInt();
		
		m_ui->setMajor->setValue(m_cVMajor);
		m_ui->setMinor->setValue(m_cVMinor);
		m_ui->setPatch->setValue(m_cVPatch);
		m_ui->setExtra->setValue(m_cVExtra);
	}
	
	
	connect(m_ui->setMajor, SIGNAL(valueChanged(int)), SLOT(calculateNewVersion()));
	connect(m_ui->setMinor, SIGNAL(valueChanged(int)), SLOT(calculateNewVersion()));
	connect(m_ui->setPatch, SIGNAL(valueChanged(int)), SLOT(calculateNewVersion()));
	connect(m_ui->setExtra, SIGNAL(valueChanged(int)), SLOT(calculateNewVersion()));
	
	connect(m_ui->raiseMajor, SIGNAL(toggled(bool)), SLOT(calculateNewVersion()));
	connect(m_ui->raiseMinor, SIGNAL(toggled(bool)), SLOT(calculateNewVersion()));
	connect(m_ui->raisePatch, SIGNAL(toggled(bool)), SLOT(calculateNewVersion()));
	connect(m_ui->raiseExtra, SIGNAL(toggled(bool)), SLOT(calculateNewVersion()));
	
	on_actionRaise_toggled();
}


MainWindow::~MainWindow()
{
	delete m_ui;
}


void MainWindow::on_actionRaise_toggled()
{
	m_ui->setMajor->setEnabled(m_ui->actionSet->isChecked());
	m_ui->setMinor->setEnabled(m_ui->actionSet->isChecked());
	m_ui->setPatch->setEnabled(m_ui->actionSet->isChecked());
	m_ui->setExtra->setEnabled(m_ui->actionSet->isChecked());
	
	m_ui->raiseMajor->setEnabled(m_ui->actionRaise->isChecked());
	m_ui->raiseMinor->setEnabled(m_ui->actionRaise->isChecked());
	m_ui->raisePatch->setEnabled(m_ui->actionRaise->isChecked());
	m_ui->raiseExtra->setEnabled(m_ui->actionRaise->isChecked());
	
	m_ui->actionSet->setChecked(!m_ui->actionRaise->isChecked());
	
	calculateNewVersion();
}


void MainWindow::on_actionSet_toggled()
{
	m_ui->setMajor->setEnabled(m_ui->actionSet->isChecked());
	m_ui->setMinor->setEnabled(m_ui->actionSet->isChecked());
	m_ui->setPatch->setEnabled(m_ui->actionSet->isChecked());
	m_ui->setExtra->setEnabled(m_ui->actionSet->isChecked());
	
	m_ui->raiseMajor->setEnabled(m_ui->actionRaise->isChecked());
	m_ui->raiseMinor->setEnabled(m_ui->actionRaise->isChecked());
	m_ui->raisePatch->setEnabled(m_ui->actionRaise->isChecked());
	m_ui->raiseExtra->setEnabled(m_ui->actionRaise->isChecked());
	
	m_ui->actionRaise->setChecked(!m_ui->actionSet->isChecked());
	
	calculateNewVersion();
}


void MainWindow::on_buttonBox_accepted()
{
	QProcess		versionCtl;
	QStringList	args;
	
	// Fill arguments
	if(m_ui->actionRaise->isChecked())
	{
		args.append("raise");
		
		if(m_ui->raiseMajor->isChecked())
			args.append("major");
		else if(m_ui->raiseMinor->isChecked())
			args.append("minor");
		else if(m_ui->raisePatch->isChecked())
			args.append("patch");
		else
			args.append("extra");
	}
	else
	{
		args.append("set");
		
		QString	version(QString("%1.%2.%3").arg(m_ui->setMajor->value()).arg(m_ui->setMinor->value()).arg(m_ui->setPatch->value()));
		
		if(m_ui->setExtra->value() > 0)
			version.append(QString("-%1").arg(m_ui->setExtra->value()));
		
		args.append(version);
	}
	
	// Try to start from PATH
	versionCtl.start("version-ctl", args);
	
	qDebug("PATH: %s", getenv("PATH"));
	
	// Try to start from current application dir
	bool	started	=	versionCtl.waitForStarted();
	
	if(!started)
		versionCtl.start(QApplication::applicationDirPath() + QDir::separator() + "version-ctl", args);
	
	if((!started && !versionCtl.waitForStarted()) || !versionCtl.waitForFinished())
	{
		QMessageBox::critical(this, tr("Program not found"), tr("Could not run \"version-ctl\"!"));
		reject();
		return;
	}
	
	if(versionCtl.exitCode() != 0)
	{
		QMessageBox::critical(this, tr("Program failed"), tr("\"version-ctl\" exited with status code %1:\n\n%2").arg(versionCtl.exitCode()).arg(QString::fromLocal8Bit(versionCtl.readAllStandardError())));
		reject();
		return;
	}
	
	accept();
}


void MainWindow::calculateNewVersion()
{
	int	major	=	m_cVMajor;
	int	minor	=	m_cVMinor;
	int	patch	=	m_cVPatch;
	int	extra	=	m_cVExtra;
	
	if(m_ui->actionRaise->isChecked())
	{
		if(m_ui->raiseMajor->isChecked())
		{
			major++;
			minor	=	0;
			patch	=	0;
			extra	=	0;
		}
		else if(m_ui->raiseMinor->isChecked())
		{
			minor++;
			patch	=	0;
			extra	=	0;
		}
		else if(m_ui->raisePatch->isChecked())
		{
			patch++;
			extra	=	0;
		}
		else
			extra++;
	}
	else
	{
		major	=	m_ui->setMajor->value();
		minor	=	m_ui->setMinor->value();
		patch	=	m_ui->setPatch->value();
		extra	=	m_ui->setExtra->value();
	}
	
	QString	version(QString("%1.%2.%3").arg(major).arg(minor).arg(patch));
	
	if(extra > 0)
		version.append(QString("-%1").arg(extra));
	
	m_ui->newVersion->setText(version);
}
