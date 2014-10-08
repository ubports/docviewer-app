#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>

class FileReader: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString fileString READ getfileString NOTIFY fileStringChanged)
    Q_PROPERTY(int error READ getError NOTIFY errorChanged)

public:
    explicit FileReader(QObject *parent = 0);
    ~FileReader();

protected:
    QString getPath() { return path; }
    void setPath(QString p);

    QString getfileString() { return fileString; }

    int getError() { return error; }

    QString path;
    QString fileString;
    int error;

private:
    void readString();

Q_SIGNALS:
    void pathChanged();
    void fileStringChanged();
    void errorChanged();
};

#endif // FILEREADER_H
