/***************************************************************************/
// Git-Lim, evaluador de limites de subida para Git/GitHub
// Copyright (C) 2015-2016  Antonio Ramírez Marti (morgulero@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/***************************************************************************/

#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QSettings>

#include "gitlim.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //PARSER
    QCommandLineParser parser;

    parser.setApplicationDescription("Check for files' size limits, as in GitHub's 100MiB size limit; use it before make a commit");
    parser.addHelpOption();

    QCommandLineOption workingDirectory("d",QObject::tr("Set repository directory"),QObject::tr("Repository absolute path,using / as in Unix Syntax"));
    parser.addOption(workingDirectory);

    QCommandLineOption gitPath(QStringList() << "p" << "path",QObject::tr("Set GIT path"),QObject::tr("GIT absolute path, using / as in Unix syntax"));
    parser.addOption(gitPath);

    parser.process(a);

    if(parser.isSet(gitPath)) {
        QString path=parser.value(gitPath);
        path=path.replace(QStringLiteral("/"),QString("\\"));
        QSettings settings(QStringLiteral("ARM"),QStringLiteral("gitlim"));
        settings.setValue(QStringLiteral("gitPath"),path);
    }

    QSettings settings(QStringLiteral("ARM"),QStringLiteral("gitLim"));
    QString gPath=settings.value(QStringLiteral("gitPath"),QStringLiteral("C:\\Program Files (x86)\\Git\\bin")).toString();
    QString wDirectory;
    if(!parser.isSet(workingDirectory))
        wDirectory.clear();
    else {
        wDirectory=parser.value(workingDirectory);
        if(!wDirectory.endsWith('/')) wDirectory=wDirectory.append('/');
    }

    GitLim gitLimiter(&a,new QTextStream(stdout));
    gitLimiter.setGitPath(gPath);
    gitLimiter.setWorkingDirectory(wDirectory);

    gitLimiter.prepareEnvironment();
    QObject::connect(&gitLimiter,SIGNAL(finished()),&a,SLOT(quit()));

    gitLimiter.start();

    return a.exec();
}
