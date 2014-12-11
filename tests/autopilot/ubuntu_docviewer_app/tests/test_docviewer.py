# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from autopilot.matchers import Eventually
from testtools.matchers import Equals, Contains

from ubuntu_docviewer_app.tests import DocviewerAppTestCase


class TestMainWindow(DocviewerAppTestCase):

    def setUp(self):
        super(TestMainWindow, self).setUp()

    def test_open_text_file(self):

        self.filepath = 'ubuntu_docviewer_app/files/plaintext.txt'

        self.launch_app()

        text_area = self.app.main_view.select_single(
            "TextArea", objectName="textAreaMain")

        self.assertThat(
            text_area.text, Eventually(Equals('TEST\n')))

    def test_open_image_file(self):

        self.filepath = 'ubuntu_docviewer_app/files/ubuntu-touch.jpg'

        self.launch_app()

        image_item = self.app.main_view.select_single(
            "QQuickImage", objectName="imageRenderer")

        self.assertThat(
            image_item.status, Eventually(Equals(1)))
        self.assertThat(
            image_item.source, Contains(self.filepath))

    def test_unknown_file_type(self):
        self.filepath = 'ubuntu_docviewer_app/files/unknown.type'

        self.launch_app()
        dialog = self.app.main_view.select_single("Dialog",
                                                  objectName="unknownDialog")
        self.assertThat(dialog.visible,
                        Eventually(Equals(True)))

    def test_open_pdf_file(self):
        self.filepath = 'ubuntu_docviewer_app/files/UbuntuPhone.pdf'

        self.launch_app()

        pdf = self.app.main_view.select_single("PdfView")
        self.assertThat(pdf.title,
                        Eventually(Equals("UbuntuPhone.pdf")))
