# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# Copyright (C) 2013, 2014 Canonical Ltd
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

"""docviewer app autopilot helpers."""

import logging
from autopilot import logging as autopilot_logging
logger = logging.getLogger(__name__)

from autopilot.introspection import dbus
import ubuntuuitoolkit


class DocviewerException(ubuntuuitoolkit.ToolkitException):

    """Exception raised when there are problems with the docviewer."""


class DocviewerApp(object):

    """Autopilot helper object for the docviewer application."""

    def __init__(self, app_proxy):
        self.app = app_proxy
        self.main_view = self.app.select_single(MainView)

    @property
    def pointing_device(self):
        return self.app.pointing_device


class MainView(ubuntuuitoolkit.MainView):

    """A helper that makes it easy to interact with the docviewer-app."""

    def __init__(self, *args):
        super(MainView, self).__init__(*args)
        self.visible.wait_for(True)

    def open_PdfView(self):
        """Open the PdfView Page.

        :return the PdfView Page

        """
        return self.wait_select_single(PdfView)

    @autopilot_logging.log_action(logger.info)
    def open_PdfContentsPage(self):
        """Open the PdfContents Page.

        :return the PdfContents Page

        """
        return self.wait_select_single(PdfContentsPage)

    @autopilot_logging.log_action(logger.info)
    def get_PdfViewGotoDialog(self):
        """Return a dialog emulator"""
        return self.wait_select_single(objectName="PdfViewGotoDialog")

    @autopilot_logging.log_action(logger.info)
    def go_to_page_from_dialog(self, page_no):
        """ Go to page from get_PfdViewGotoDialog """
        textfield = self.wait_select_single(
            "TextField", objectName="goToPageTextField")
        textfield.write(page_no)
        go_button = self.wait_select_single("Button", objectName="GOButton")
        self.pointing_device.click_object(go_button)

    @autopilot_logging.log_action(logger.info)
    def click_go_to_page_button(self):
        """Click the go_to_page header button."""
        header = self.get_header()
        header.click_action_button('gotopage')


class Page(ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase):

    """Autopilot helper for Pages."""

    def __init__(self, *args):
        super(Page, self).__init__(*args)
        # XXX we need a better way to keep reference to the main view.
        # --elopio - 2014-01-31
        self.main_view = self.get_root_instance().select_single(MainView)


class PageWithBottomEdge(MainView):
    """
    An emulator class that makes it easy to interact with the bottom edge
    swipe page
    """
    def __init__(self, *args):
        super(PageWithBottomEdge, self).__init__(*args)

    def reveal_bottom_edge_page(self):
        """Bring the bottom edge page to the screen"""
        self.bottomEdgePageLoaded.wait_for(True)
        try:
            action_item = self.wait_select_single(objectName='bottomEdgeTip')
            action_item.visible.wait_for(True)
            action_item.stretched.wait_for(True)
            start_x = (action_item.globalRect.x +
                       (action_item.globalRect.width * 0.5))
            start_y = (action_item.globalRect.y + 1)
            stop_y = start_y - (self.height * 0.5)
            self.pointing_device.drag(start_x, start_y,
                                      start_x, stop_y, rate=2)
            self.isReady.wait_for(True)
        except dbus.StateNotFoundError:
            logger.error('BottomEdge element not found.')
            raise


class PdfView(PageWithBottomEdge):
    """Autopilot helper for PdfView page."""

    @autopilot_logging.log_action(logger.info)
    def toggle_header_visibility(self):
        """Show/hide page header by clicking on the center of main view"""
        self.pointing_device.click_object(self)

    def get_currentpage_number(self):
        """return the value of the currentPage property"""
        return self.currentPage


class PdfContentsPage(Page):
    """Autopilot helper for PdfContents page."""

    @autopilot_logging.log_action(logger.info)
    def get_content_and_line_pageindex(self, labelText):
        content_line, page_no = self._get_listitem(labelText)
        return content_line, page_no

    def _get_listitem(self, labelText):
        view_item = self.select_single(
            "QQuickListView", objectName="view")
        list_items_count = view_item.count

        index = 0
        for index in range(list_items_count):
            list_item = self.select_single(
                "UCListItem", objectName="delegate{}".format(index))
            while list_item.y > (view_item.contentY +
                                 view_item.globalRect.height):
                self.scroll_pdfcontentspage()
            list_item_layout = list_item.select_single(
                "UCListItemLayout",
                objectName="listItemLayout{}".format(index))
            layout_main_slot = list_item_layout.select_single(
                "UCThreeLabelsSlot")
            try:
                main_slot_title = layout_main_slot.select_single(
                    "UCLabel", textSize=3)
                if main_slot_title.text == labelText:
                    page_no = list_item.select_single(
                        "UCLabel", objectName="pageindex").text
                    return main_slot_title, page_no
                    break
            except dbus.StateNotFoundError:
                logger.error('ToC entry title label not found.')
                raise

    @autopilot_logging.log_action(logger.info)
    def click_content_line(self, content_line):
        self.pointing_device.click_object(content_line)

    @autopilot_logging.log_action(logger.info)
    def scroll_pdfcontentspage(self):
        action_item = self.select_single("QQuickListView")
        start_x = (action_item.globalRect.x +
                   (action_item.globalRect.width * 0.5))
        start_y = (action_item.globalRect.y +
                   (action_item.height * 0.8))
        stop_y = start_y - (action_item.height * 0.7)
        self.pointing_device.drag(start_x, start_y,
                                  start_x, stop_y, rate=2)
        action_item.moving.wait_for(False)
