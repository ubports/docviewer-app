Dependencies
============
**DEPENDENCIES ARE NEEDED TO BE INSTALLED TO BUILD AND RUN THE APP**.

A complete list of dependencies for the project can be found in ubuntu-docviewer-app/debian/control

The following essential packages are also required to develop this app:
* [ubuntu-sdk](http://developer.ubuntu.com/start)
* intltool   - run  `sudo apt-get install intltool` 

Compiling
=========

* Install poppler's development files:
    sudo apt install libpoppler-qt5-dev

* Install Qt5 private development files:
    sudo apt install qtdeclarative5-private-dev qtbase5-private-dev

* If you want to compile an arm click package, you need to install that package
  to the arm compilation environment. For example when using QtCreator for
  Ubuntu Touch, open Options -> Ubuntu -> Maintain, and then enter:

    apt install libpoppler-qt5-dev:armhf qtdeclarative5-private-dev:armhf qtbase5-private-dev:armhf

LibreOffice viewer support
==========================

* If you want to enable the LibreOffice viewer feature, you'll need to have a
  LibreOffice 5.0 installation on your development environment. At the time of
  writing, LibreOffice 5.0 is not yet in the archive, but you can easily install
  it from the pre-releases PPA. To do so, type the following commands on a
  terminal:

    sudo apt-get purge libreoffice-core
    sudo add-apt-repository ppa:libreoffice/libreoffice-prereleases
    sudo apt-get update
    sudo apt-get install libreoffice

* Then to compile the LibreOffice viewer plugin you'll simply need to install
  the LibreOffice Kit development files:

    sudo apt install libreofficekit-dev
