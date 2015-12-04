#ifndef GITLIM_H
#define GITLIM_H

#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>

class GitLim : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString workingDirectory READ workingDirectory WRITE setWorkingDirectory)
    Q_PROPERTY(QString gitPath READ gitPath WRITE setGitPath)

public:

    explicit GitLim(QObject *parent = 0,QTextStream *out=0);

    bool isReady();
    QString workingDirectory() const
    { return _workingDirectory; }

    QString gitPath() const
    { return _gitPath; }

signals:

    void finished();

public slots:

    void start();
    void setWorkingDirectory(QString wDirectory);
    void setGitPath(QString gPath);
    void prepareEnvironment();

private:

    bool _isEnvironmentReady;
    QString _workingDirectory;
    QString _gitPath;
    QProcess _gitProcess;

    QProcessEnvironment _processEnvironment;
    QTextStream *output;

private slots:

    void gitProcessFinished();

    void checkFiles(QStringList fileList);

    void end();
};

#endif // GITLIM_H
