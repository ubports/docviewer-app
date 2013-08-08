function getSimpleMimetype(longMimetype)
{
    var str = longMimetype;
    str = str.substring(0, str.indexOf(";"));
    return str;
}

function load(mimetype) {
    if (mimetype.indexOf(";") !== -1)
        mimetype = getSimpleMimetype(mimetype);

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
    else if (mimetype === "application/pdf")
    {
        qmlToLoad = "PdfView";
    }

    if (qmlToLoad != "")
    {
        var component = Qt.createComponent(qmlToLoad + ".qml");
        if (component.status === Component.Ready)
        {
            component.createObject(pageMain);
        }
        else
        {
            console.debug(component.errorString());
        }

    }
    else
    {
        runUnknowTypeDialog();
    }
}
