function load(mimetype) {
    var qmlToLoad = "";

    // Open all text files in text editor
    // With that fix it is possible to open LICENSE file
    // which was recognised as text/x-pascal
    if (mimetype.substring(0, 5) === "text/")
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
       pageStack.push(Qt.resolvedUrl(qmlToLoad + ".qml"))
    }
    else
    {
        console.debug("Unknown MIME type: "+ mimetype);
        runUnknownTypeDialog();
    }
    return mimetype;
}
