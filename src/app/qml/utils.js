.pragma library

function printSize(size)
{
    if (size >= 1073741824)
    {
        return (size / 1073741824).toFixed(2) + " GiB";
    }

    if (size >= 1048576)
    {
        return (size / 1048576).toFixed(2) + " MiB";
    }

    if (size >= 1024)
    {
        return parseInt(size / 1024) + " KiB";
    }

    return size + " byte";
}

function getNameOfFile(path)
{
    return path.toString().substring(path.lastIndexOf('/') + 1);
}
