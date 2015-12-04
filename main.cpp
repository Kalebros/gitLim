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

#include "gitlim.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GitLim gitLimiter(&a,new QTextStream(stdout));
    gitLimiter.setGitPath(QStringLiteral("C:\\Program Files (x86)\\Git\\bin"));
    gitLimiter.setWorkingDirectory(QStringLiteral("C:/Users/Bardo/Workbench/Qt/Git-Lim/"));

    gitLimiter.prepareEnvironment();
    QObject::connect(&gitLimiter,SIGNAL(finished()),&a,SLOT(quit()));

    gitLimiter.start();

    return a.exec();
}
