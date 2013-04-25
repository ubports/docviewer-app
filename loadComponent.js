function getSimpleMimetype(longMimetype)
{
    var str = longMimetype;
    str = str.substring(0, str.indexOf(";"));
    return str;
}

function load(longMimetype) {

    var mimetype = getSimpleMimetype(longMimetype);
    var qmlToLoad = "";

    if (mimetype === "text/plain")
    {
        qmlToLoad = "TextView";
    }
    else if (mimetype === "image/jpeg" ||
             mimetype === "image/png" ||
             mimetype === "image/gif" ||
             mimetype === "image/tiff" ||
             mimetype === "image/x-icon" ||
             mimetype === "image/x-ms-bmp" ||
             mimetype === "image/svg+xml")
    {
        qmlToLoad = "ImageView";
    }

    if (qmlToLoad != "")
    {
        var component = Qt.createComponent(qmlToLoad + ".qml");
        if (component.status === Component.Ready)
        {
            component.createObject(pageMain);
        }

    }
    else
    {
        runUnknowTypeDialog();
    }
}
