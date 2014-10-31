import QtQuick 2.3
import Ubuntu.Components 1.1
import com.ubuntu.popplerqmlplugin 1.0

import "utils.js" as Utils

Page {
    id: pageMain
    title: Utils.getNameOfFile(file.path);

    // Disable header auto-hide
    flickable: null

    // TODO: Restore zooming
    ListView {
        id: pdfView
        anchors {
            fill: parent
            leftMargin: units.gu(1)
            rightMargin: units.gu(1)
        }
        spacing: units.gu(2)

        clip: true
        focus: false
        boundsBehavior: Flickable.StopAtBounds

        cacheBuffer: height

        highlightFollowsCurrentItem: false
        keyNavigationWraps: false

        // TODO: Not a good way to have spacing
        header: Item {
            width: parent.width
            height: units.gu(2)
        }

        footer: Item {
            width: parent.width
            height: units.gu(2)
        }

        model: Poppler {
            id: poppler

            /* FIXME: Don't set 'path' property directly, but set it through onCompleted signal.
               By doing otherwise, PDF pages are loaded two times, but only
               the first delegates are working. Asking to the image provider
               to get the second ones, makes the app instable.
               (e.g. We have a PDF document with 10 pages. The plugin loads
               them twice - 2x10 = 20 pages - but only the first 10 are shown.
               While trying to get the 11th, the app crashes). */
            Component.onCompleted: path = file.path

            onPagesLoaded: {
                activity.running = false;

                pdfView.currentIndex = 0

                var title = getDocumentInfo("Title")
                if (title !== "")
                    titleLabel.text = title
            }
        }

        delegate: PdfPage {}

        onWidthChanged: {
            /* On resizing window, pages size changes but contentY is still the same.
               For that reason, it shows the wrong page (which is settled at the same contentY).
               We need to force flickable to show the current page. */
            //pdfView.positionViewAtIndex(currentIndex, ListView.Contain)
        }

        onContentYChanged: {
            // FIXME: On wheeling up, ListView automatically center currentItem to the view.
            //        This causes some strange "jump" of ~200px in contentY
            var i = pdfView.indexAt(pdfView.width * 0.5, contentY + (pdfView.height * 0.5))

            if (i < 0) {
                // returned index could be -1 when the delegate spacing is shown at the center of the view (e.g. while scrolling pages)
                i = pdfView.indexAt(pdfView.width * 0.5, contentY + (pdfView.height * 0.5) + units.gu(4))
            }

            if (i !== -1) {
                currentPageLabel.text = i18n.tr("Page %1 of %2").arg(i + 1).arg(pdfView.count)

                if (!pdfView.flickingVertically) {
                    pdfView.currentIndex = i
                }
            }
        }
    }

    ActivityIndicator {
        id: activity
        anchors.centerIn: parent

        running: true
    }

    // *** HEADER ***
    state: "default"
    states: [
        PageHeadState {
            name: "default"
            head: pageMain.head

            contents: Column {
                anchors.centerIn: parent

                Label {
                    id: titleLabel
                    text: Utils.getNameOfFile(file.path)
                    font.weight: Font.DemiBold
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                    id: currentPageLabel
                    text: i18n.tr("Page %1 of %2").arg(pdfView.currentIndex + 1).arg(pdfView.count)
                    fontSize: "small"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            backAction: Action {
                iconName: "back"
                text: (pageStack.depth > 1) ? i18n.tr("Back") : i18n.tr("Close")
                onTriggered: {
                    if (pageStack.depth > 1) {
                        // Go back to Welcome page
                        pageStack.pop();
                    } else {
                        // File has been imported through Content Hub (or was not chosen through WelcomePage)
                        // Close the application and show our source app (e.g. ubuntu-filemanager-app if used to open a document)
                        Qt.quit()
                    }
                }
            }

            actions: [
                Action {
                    iconName: "search"
                   // onTriggered: pageMain.state = "search"
                    //Disable it until we provide search in Poppler plugin.
                    enabled: false
                },

                Action {
                    iconName: "browser-tabs"
                    text: "Go to page..."
                    enabled: false
                },

                Action {
                    text: i18n.tr("Details")
                    iconName: "info"
                    onTriggered: pageStack.push(Qt.resolvedUrl("DetailsPage.qml"))
                }
            ]
        }

       /* PageHeadState {
            id: headerState
            name: "search"
            head: pageMain.head
            actions: [
                Action {
                    iconName: "go-up"
                },

                Action {
                    iconName: "go-down"
                }
            ]

            backAction: Action {
                id: leaveSearchAction
                text: "back"
                iconName: "back"
                onTriggered: pageMain.state = "default"
            }
        }*/
    ]
}
