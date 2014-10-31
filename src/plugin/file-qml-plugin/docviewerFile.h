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
    Q_PROPERTY( QDateTime creationTime READ getCreationTime NOTIFY creationTimeChanged )
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

    QDateTime getCreationTime() { return creationTime; }

    int getError() { return error; }
    
    QString path;
    QString mimetype;
    qint64 size;
    QDateTime lastmodified;
    QDateTime creationTime;
    int error;

private:
    void open();
    void readMimeType();

Q_SIGNALS:
    void pathChanged();
    void mimetypeChanged();
    void sizeChanged();
    void lastmodifiedChanged();
    void creationTimeChanged();
    void errorChanged();
};

#endif // DOCVIEWERFILE_H
