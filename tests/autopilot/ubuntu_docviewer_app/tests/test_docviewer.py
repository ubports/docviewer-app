# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from __future__ import absolute_import

from pprint import pprint

from autopilot.matchers import Eventually
from testtools.matchers import Equals, NotEquals

from ubuntu_docviewer_app.tests import DocviewerTestCase
import os


class TestMainWindow(DocviewerTestCase):

    def setUp(self):
        super(TestMainWindow, self).setUp()

    def tearDown(self):
        super(TestMainWindow, self).tearDown()

    def test_open_text_file(self):
        
        filePath = 'ubuntu_docviewer_app/files/plaintext.txt'

        if os.path.exists(self.local_location):
            self.launch_test_local(filePath)
        else:
            self.launch_test_installed(self.sample_dir + filePath)
    
        #Check if the app is correclty launched
        self.assertThat(
            self.ubuntusdk.get_qml_view().visible, Eventually(Equals(True)))

        textArea = self.ubuntusdk.get_object("TextArea", "textAreaMain")

        #Check if textarea is no longer empty
        self.assertThat(
            textArea.text, Eventually(NotEquals(False)))

    def test_read_text_file_mimeType(self):
        filePath = 'ubuntu_docviewer_app/files/plaintext.txt'

        if os.path.exists(self.local_location):
            self.launch_test_local(filePath)
        else:
            self.launch_test_installed(self.sample_dir + filePath)

        self.check_mimeType()
        
    def test_open_image_file(self):
        
        filePath = 'ubuntu_docviewer_app/files/ubuntu-touch.jpg'

        if os.path.exists(self.local_location):
            self.launch_test_local(filePath)
        else:
            self.launch_test_installed(self.sample_dir + filePath)
    
        #Check if the app is correclty launched
        self.assertThat(
            self.ubuntusdk.get_qml_view().visible, Eventually(Equals(True)))

        imageItem = self.ubuntusdk.get_object("QQuickImage", "imageItem")

        #Check if status of Image is "Ready"
        self.assertThat(
            imageItem.status, Eventually(Equals(1)))

    def test_read_image_file_mimeType(self):
        filePath = 'ubuntu_docviewer_app/files/ubuntu-touch.jpg'

        if os.path.exists(self.local_location):
            self.launch_test_local(filePath)
        else:
            self.launch_test_installed(self.sample_dir + filePath)
        self.check_mimeType()


    def check_mimeType(self):
        mimetypeItem = self.ubuntusdk.get_object("SingleValue", "mimetypeItem")
        
        self.assertThat(
            mimetypeItem.value, Eventually(NotEquals(False)))
    
    def test_unknown_file_type(self):
        filePath = 'ubuntu_docviewer_app/files/unknown.type'
        
        if os.path.exists(self.local_location):
            self.launch_test_local(filePath)
        else:
            self.launch_test_installed(self.sample_dir + filePath)

        self.assertThat(
            self.app.select_single("Dialog", title="Unknow type").visible, Eventually(Equals(True)))
