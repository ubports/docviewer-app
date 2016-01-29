Running Autopilot tests
=======================

As of wily 15.10, some autopilot tests are broken in trunk. As such autopilot failures
can be excused until they are fixed in trunk.

The Doc Viewer app follows a test driven development where autopilot tests are run before every merge into trunk. If you are submitting your bugfix/patch to the Doc Viewer app, please follow the following steps below to ensure that all tests pass before proposing a merge request.

If you are looking for more info about Autopilot or writing AP tests for the doc viewer app, here are some useful links to help you:

- [Ubuntu - Quality](http://developer.ubuntu.com/start/quality)
- [Autopilot - Python](https://developer.ubuntu.com/api/autopilot/python/1.5.0/)

For help and options on running tests, see:

- [Autopilot tests](https://developer.ubuntu.com/en/start/platform/guides/running-autopilot-tests/)

Prerequisites
=============

Install the following autopilot packages required to run the tests,

    $ sudo apt-get install python3-autopilot libautopilot-qt ubuntu-ui-toolkit-autopilot python3-autopilot-vis

Running tests on the desktop
============================

Using terminal:

*  Branch the Doc Viewer app code, for example,

    $ bzr branch lp:ubuntu-docviewer-app
    
*  Navigate to the tests/autopilot directory.

    $ cd ubuntu-docviewer-app/tests/autopilot

*  run all tests.

    $ autopilot3 run -vv ubuntu_docviewer_app

* to list all tests:

    $ autopilot3 list ubuntu_docviewer_app

 To run only one test (for instance: ubuntu_docviewer_app.tests.test_docviewer.TestMainWindow.test_open_pdf_file)


    $ autopilot3 run -vv ubuntu_docviewer_app.tests.test_docviewer.TestMainWindow.test_open_pdf_file

* Debugging tests using autopilot vis

    $ autopilot3 launch -i Qt qmlscene app/ubuntu-docviewer-app.qml

    $ autopilot3 vis

Running tests using Ubuntu SDK
==============================

Refer this [tutorial](https://developer.ubuntu.com/en/start/platform/guides/running-autopilot-tests/) to run tests on Ubuntu SDK: 

Running tests on device or emulator:
====================================

Using autopkg:

*  Branch the Doc Viewer app code, for example,

    $ bzr branch lp:ubuntu-docviewer-app

*  Navigate to the source directory.

    $ cd ubuntu-docviewer-app

*  Build a click package
    
    $ click-buddy .

*  Run the tests on device (assumes only one click package in the directory)

    $ adt-run . *.click --- ssh -s adb -- -p <PASSWORD>
