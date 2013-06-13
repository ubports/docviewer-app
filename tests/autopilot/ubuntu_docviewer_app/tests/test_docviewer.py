# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from __future__ import absolute_import

from autopilot.matchers import Eventually
from testtools.matchers import Equals

from ubuntu_docviewer_app.tests import DocviewerTestCase


class TestMainWindow(DocviewerTestCase):

    def setUp(self):
        super(TestMainWindow, self).setUp()
        #self.assertThat(
        #    self.main_window.get_qml_view().visible, Eventually(Equals(True)))

    def tearDown(self):
        super(TestMainWindow, self).tearDown()

    def test_open_text_file(self):
        #launch app with file argument (absolute path)
        #hack for the moment
        filePath = '/tmp/docviewer'

        self.launch_test_local(filePath)

        #add launch function to init :-)

        #verify textbox is no longer empty
        # or
        #verify the file size displayed is not 0 (maybe easier ?)
