# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from ubuntu_docviewer_app.tests import DocviewerAppTestCase

import logging
logger = logging.getLogger(__name__)


class TestMainWindow(DocviewerAppTestCase):

    """Tests the TOC features"""
    scenarios = [
        ('Chapter 1',
            {'content_label': 'Chapter 1. Introduction'
             }),

        ('Chapter 3',
            {'content_label': 'Chapter 3. Package Management',
             })
    ]

    def setUp(self):
        super(TestMainWindow, self).setUp()
        self.filepath = 'ubuntu_docviewer_app/files/serverguide.pdf'

    def test_go_to_chapters_in_toc(self):
        """" Testing going to chapters from Table Of Contents     """

        self.launch_app()
        pdf = self.app.main_view.open_PdfView()
        pdf.reveal_bottom_edge_page()
        contents_page = self.app.main_view.open_PdfContentsPage()
        content_line, page_no = contents_page.\
            get_content_and_line_pageindex(self.content_label)
        contents_page.click_content_line(content_line)

        word_in_currentpage = pdf.get_currentpage_number().split()
        pdfview_currentpage = word_in_currentpage[1]

        self.assertEquals(pdfview_currentpage, page_no)
