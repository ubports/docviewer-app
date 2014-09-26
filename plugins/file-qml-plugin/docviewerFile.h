#ifndef DOCVIEWERFILE_H
#define DOCVIEWERFILE_H

#include <QString>
#include <QObject>
#include <QProcess>
#include <QDateTime>

class DocviewerFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString path READ getPath WRITE setPath NOTIFY pathChanged )
    Q_PROPERTY( QString mimetype READ getMimetype NOTIFY mimetypeChanged )
    Q_PROPERTY( qint64 size READ getSize NOTIFY sizeChanged )
    Q_PROPERTY( QDateTime lastModified READ getLastmodified NOTIFY lastmodifiedChanged )
    Q_PROPERTY( int error READ getError NOTIFY errorChanged )
    
public:
    explicit DocviewerFile(QObject *parent = 0);
    ~DocviewerFile();
    
protected:
    QString getPath() { return path; }
    void setPath(QString p);

    QString getMimetype() { return mimetype; }

    qint64 getSize() { return size; }

    QDateTime getLastmodified() { return lastmodified; }

    int getError() { return error; }
    
    QString path;
    QString mimetype;
    qint64 size;
    QDateTime lastmodified;
    int error;

private:
    void open();
    QProcess *mimeTypeProcess;

public slots:
    void s_readMimeType();
    void s_finished(int exitCode);

Q_SIGNALS:
    void pathChanged();
    void mimetypeChanged();
    void sizeChanged();
    void lastmodifiedChanged();
    void errorChanged();
};

#endif // DOCVIEWERFILE_H
