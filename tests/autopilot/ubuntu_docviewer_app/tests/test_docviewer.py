# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from autopilot.matchers import Eventually
from testtools.matchers import Equals, NotEquals

from ubuntu_docviewer_app.tests import DocviewerAppTestCase
import os


class TestMainWindow(DocviewerAppTestCase):

    def setUp(self):
        super(TestMainWindow, self).setUp()

    def _check_mimeType(self):
        mimetypeItem = self.app.main_view.select_single(
            "SingleValue", objectName="mimetypeItem")

        self.assertThat(
            mimetypeItem.value, Eventually(NotEquals(False)))

    def test_open_text_file(self):

        filePath = 'ubuntu_docviewer_app/files/plaintext.txt'

        self.launchApp(filePath)

        textArea = self.app.main_view.select_single(
            "TextArea", objectName="textAreaMain")

        # Check if textarea is no longer empty
        self.assertThat(
            textArea.text, Eventually(NotEquals(False)))

    def test_open_image_file(self):

        filePath = 'ubuntu_docviewer_app/files/ubuntu-touch.jpg'

        self.launchApp(filePath)

        imageItem = self.app.main_view.select_single(
            "QQuickImage", objectName="imageItem")

        # Check if status of Image is "Ready"
        self.assertThat(
            imageItem.status, Eventually(Equals(1)))

    def test_read_image_file_mimeType(self):
        filePath = 'ubuntu_docviewer_app/files/ubuntu-touch.jpg'

        self.launchApp(filePath)

        self._check_mimeType()

    def test_unknown_file_type(self):
        filePath = 'ubuntu_docviewer_app/files/unknown.type'

        self.launchApp(filePath)

        self.assertThat(
            self.app.select_single("Dialog",
                                   objectName="unknownDialog").visible,
            Eventually(Equals(True)))

    def test_open_pdf_file_type(self):
        filePath = 'ubuntu_docviewer_app/files/UbuntuPhone.pdf'

        self.launchApp(filePath)

        self.assertThat(
            self.app.select_many("Label", text="UbuntuPhone.pdf")[0].visible,
            Eventually(Equals(True)))

    def test_open_pdf_file_mimeType(self):
        filePath = 'ubuntu_docviewer_app/files/UbuntuPhone.pdf'

        self.launchApp(filePath)

        self._check_mimeType()
