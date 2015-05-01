# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from autopilot.matchers import Eventually
from testtools.matchers import Equals, GreaterThan

from ubuntu_docviewer_app.tests import DocviewerAppTestCase


class TestMainWindow(DocviewerAppTestCase):

    def setUp(self):
        super(TestMainWindow, self).setUp()

    def go_to_page_no(self, page_no):
        self.app.main_view.get_PdfViewGotoDialog()
        self.app.main_view.go_to_page_from_dialog(page_no)

    def test_open_text_file(self):

        self.filepath = 'ubuntu_docviewer_app/files/plaintext.txt'

        self.launch_app()

        text_area = self.app.main_view.select_single(
            "TextArea", objectName="textAreaMain")

        self.assertThat(
            text_area.text, Eventually(Equals('TEST\n')))

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

        pdf = self.app.main_view.select_single(
            "VerticalView", objectName="pdfView")
        self.assertThat(pdf.contentHeight,
                        Eventually(GreaterThan(0)))

    def test_go_to_page_pdf_file(self):
        self.filepath = 'ubuntu_docviewer_app/files/FCM-91.pdf'
        page_no = "3"

        self.launch_app()
        pdf = self.app.main_view.open_PdfView()
        pdf.toggle_header_visibility()
        pdf.click_go_to_page_button()
        self.go_to_page_no(page_no)

        self.assertThat(
            self.app.main_view.select_single(
                "VerticalView", objectName="pdfView").currentPageIndex,
            Eventually(Equals(int(page_no) - 1)))

    def test_pdf_toc(self):
        """" Testing pdf Table Of Contents """
        self.filepath = 'ubuntu_docviewer_app/files/serverguide.pdf'

        self.launch_app()
        pdf = self.app.main_view.open_PdfView()

        pdf.reveal_bottom_edge_page()

        contents_page = self.app.main_view.open_PdfContentsPage()

        # Go to "Chapter one" in pdf document
        page_no = contents_page.get_page_number("Chapter 1. Introduction")
        contents_page.click_content_line("Chapter 1. Introduction")
