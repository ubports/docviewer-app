# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2016 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

from ubuntu_docviewer_app.tests import DocviewerAppTestCase


class TestMainWindow(DocviewerAppTestCase):
    def test_example_test(self):
        """Just launch app, assert on main view"""
        self.filepath = 'ubuntu_docviewer_app/files/plaintext.txt'
        self.launch_app()

        main_view = self.app.main_view
        self.assertTrue(main_view)
