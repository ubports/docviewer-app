#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QObject>
#include <QProcess>
#include <QDateTime>

class File : public QObject
{
    Q_OBJECT

public:
    File(QString* _path);

    QString path;
    QString *mimeType;
    qint64 size;
    QDateTime lastModified;

    QString *error;

    Q_INVOKABLE QString getPath();
    Q_INVOKABLE QString getMimetype();
    Q_INVOKABLE qint64 getSize();
    Q_INVOKABLE QDateTime getLastModified();
    Q_INVOKABLE QString getError();

public slots:
    void s_readMimeType();
    void s_finished(int exitCode);

private:
    QProcess *mimeTypeProcess;
    void setError(QString _error);

signals:
    void mimetypeChanged();
    void errorDetected();
};

#endif // FILE_H
