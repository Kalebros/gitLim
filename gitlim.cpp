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

#include "gitlim.h"

GitLim::GitLim(QObject *parent, QTextStream *out) : QObject(parent)
{
    _workingDirectory.clear();
    _gitPath.clear();
    _isEnvironmentReady=false;
    _processEnvironment=QProcessEnvironment();
    output=out;

    connect(&_gitProcess,SIGNAL(finished(int)),this,SLOT(gitProcessFinished()));
}

bool GitLim::isReady()
{
    return (_isEnvironmentReady && !_workingDirectory.isEmpty() && !_gitPath.isEmpty());
}

void GitLim::setWorkingDirectory(QString wDirectory)
{
    _workingDirectory=wDirectory;
}

void GitLim::setGitPath(QString gPath)
{
    _gitPath=gPath;

    //Windows correction
    ///@todo Implementar
}

void GitLim::prepareEnvironment()
{
    _processEnvironment=QProcessEnvironment::systemEnvironment();
    QString path=_processEnvironment.value(QStringLiteral("PATH"));
    path=path.append(_gitPath.prepend(';'));
    _processEnvironment.remove(QStringLiteral("PATH"));
    _processEnvironment.insert(QStringLiteral("PATH"),path);
}

void GitLim::start()
{
    if(_gitProcess.state()==QProcess::Running || _gitProcess.state()==QProcess::Starting) {
        _gitProcess.kill();
        _gitProcess.waitForFinished();
    }

    _gitProcess.setProcessEnvironment(_processEnvironment);
    _gitProcess.setWorkingDirectory(_workingDirectory);
    _gitProcess.start(QStringLiteral("cmd.exe"),QStringList()<<"/C"<<"git ls-files");
}

void GitLim::gitProcessFinished()
{
    QString res(_gitProcess.readAll());

    checkFiles(res.split("\n",QString::SkipEmptyParts));
}

void GitLim::checkFiles(QStringList fileList)
{
    bool found=false;
    foreach(QString file,fileList) {
        QFileInfo f(file.prepend(_workingDirectory));
        if(f.size()>(100*1024*1024)) {
            *output << f.absoluteFilePath()<<": size over 100 MiB"<<endl;
            found=true;
        }
    }
    if(!found) *output << "CORRECT."<<endl;
    else *output << "PUSH WILL FAIL!!"<<endl;

    output->flush();

    end();
}

void GitLim::end()
{
    emit this->finished();
}


