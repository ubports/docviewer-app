# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# Copyright (C) 2013-2016 Canonical Ltd
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""docviewer app autopilot tests."""

import os
import logging

import fixtures
import ubuntu_docviewer_app

from autopilot.testcase import AutopilotTestCase
from autopilot import logging as autopilot_logging

import ubuntuuitoolkit

logger = logging.getLogger(__name__)


class BaseTestCaseWithPatchedHome(AutopilotTestCase):

    """A common test case class that provides several useful methods for
    ubuntu-docviewer-app tests.

    """

    local_build_location = os.path.dirname(os.path.dirname(os.getcwd()))
    sdk_build_location = os.path.join(os.path.dirname(local_build_location),
                                      os.path.basename(local_build_location) +
                                      '-build')

    local_build_location_qml = os.path.join(local_build_location, 'src', 'app',
                                            'qml', 'ubuntu-docviewer-app.qml')
    local_build_location_binary = os.path.join(local_build_location, 'src',
                                               'app', 'ubuntu-docviewer-app')
    sdk_build_location_qml = os.path.join(sdk_build_location, 'src', 'app',
                                          'qml', 'ubuntu-docviewer-app.qml')
    sdk_build_location_binary = os.path.join(sdk_build_location, 'src',
                                             'app', 'ubuntu-docviewer-app')
    installed_location_binary = os.path.join('usr', 'bin',
                                             'ubuntu-docviewer-app')
    installed_location_qml = os.path.join('usr', 'share',
                                          'ubuntu-docviewer-app', 'qml',
                                          'ubuntu-docviewer-app.qml')

    def get_launcher_method_and_type(self):
        if os.path.exists(self.local_build_location_binary):
            launcher = self.launch_test_local
            test_type = 'local'
        elif os.path.exists(self.sdk_build_location_binary):
            launcher = self.launch_test_sdk
            test_type = 'sdk'
        elif os.path.exists(self.installed_location_binary):
            launcher = self.launch_test_installed
            test_type = 'deb'
        else:
            launcher = self.launch_test_click
            test_type = 'click'
        return launcher, test_type

    def setUp(self):
        super(BaseTestCaseWithPatchedHome, self).setUp()
        self.launcher, self.test_type = self.get_launcher_method_and_type()
        self.real_home_dir = os.getenv('HOME')
        self.home_dir = self._patch_home()

        # Unset the current locale to ensure locale-specific data
        # (day and month names, first day of the week, …) doesn’t get
        # in the way of test expectations.
        self.useFixture(fixtures.EnvironmentVariable('LC_ALL', newvalue='C'))

    @autopilot_logging.log_action(logger.info)
    def launch_test_local(self):
        return self.launch_test_application(
            self.local_build_location_binary,
            self.filepath,
            app_type='qt',
            emulator_base=ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase)

    @autopilot_logging.log_action(logger.info)
    def launch_test_sdk(self):
        return self.launch_test_application(
            self.sdk_build_location_binary,
            self.filepath,
            app_type='qt',
            emulator_base=ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase)

    @autopilot_logging.log_action(logger.info)
    def launch_test_installed(self):
        return self.launch_test_application(
            self.installed_location_binary,
            os.path.join(os.path.join('usr', 'lib', 'python3',
                                      'dist-packages'), self.filepath),
            app_type='qt',
            emulator_base=ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase)

    @autopilot_logging.log_action(logger.info)
    def launch_test_click(self):
        return self.launch_click_package(
            "com.ubuntu.docviewer",
            emulator_base=ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase)

    def _patch_home(self):
        """ mock /home for testing purposes to preserve user data
        """

        # if running on non-phablet device,
        # run in temp folder to avoid mucking up home
        # bug 1316746
        # bug 1376423
        if self.test_type is not 'click':
            temp_dir_fixture = fixtures.TempDir()
            self.useFixture(temp_dir_fixture)
            temp_dir = temp_dir_fixture.path

            # before we set fixture, copy xauthority if needed
            self.useFixture(fixtures.EnvironmentVariable('HOME',
                                                         newvalue=temp_dir))

            logger.debug("Patched home to fake home directory %s" % temp_dir)


class DocviewerAppTestCase(BaseTestCaseWithPatchedHome):

    """Base test case that launches the ubuntu-docviewer-app."""

    def setUp(self):
        super(DocviewerAppTestCase, self).setUp()

    def launch_app(self):
        self.app = ubuntu_docviewer_app.DocviewerApp(self.launcher())
