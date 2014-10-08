.pragma library

function printSize(size)
{
    if (size >= 1073741824)
    {
        return parseInt(size/1073741824) + "," + size%1073741824 + " Gio";
    }

    if (size >= 1048576)
    {
        return parseInt(size/1048576) + "," + size%1048576 + " Mio";
    }

    if (size >= 1024)
    {
        return parseInt(size/1024) + "," + size%1024 + " Kio";
    }

    return size + " o";
}

function getNameOfFile(path)
{
    var name = String(path);

    return name.substring(name.lastIndexOf('/')+1);

}

function relToAbs(rel)
{
    var abs = Qt.resolvedUrl(rel)
    return abs.substring(7); //remove "file://"
}
