##############################################################################
##
## Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
## All rights reserved.
##
## This file is part of the QicsTable software.
##
## See the top level README file for license terms under which this
## software can be used, distributed, or modified.
##
##############################################################################

TEMPLATE = lib

TARGET = qicstable

include(../qicstable_config.pri)

DESTDIR = ../lib

CONFIG += $$LIB_CONFIG

unix {
    CLEAN_FILES += $(DESTDIR)/lib$(QMAKE_TARGET)*
}

win32 {
    DEFINES += QICS_MAKEDLL
    CLEAN_FILES += $(DLLDESTDIR)/$(QMAKE_TARGET).lib \
    $(DLLDESTDIR)/$(QMAKE_TARGET).dll
}

INSTALL_INCLUDE_SUFFIX = qicstable

CONFIG += warn-on

INCLUDEPATH += ../include ../addons/printing ../addons/table.tree

INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/md5

HEADERS += ../include/QicsNamespace.h \
            ../include/QicsGridInfo.h \
            ../include/QicsStyle.h \
            ../include/QicsCellStyle.h \
            ../include/QicsRepeatingCellStyle.h \
            ../include/QicsGridStyle.h \
            ../include/QicsStyleManager.h \
            ../include/QicsSelectionManager.h \
            ../include/QicsDimensionManager.h \
            ../include/QicsMappedDimensionManager.h \
            ../include/QicsSpanManager.h \
            ../include/QicsSorter.h \
            ../include/QicsCellCommon.h \
            ../include/QicsGridCommon.h \
            ../include/QicsCell.h \
            ../include/QicsRow.h \
            ../include/QicsColumn.h \
            ../include/QicsRepeatingRow.h \
            ../include/QicsRepeatingColumn.h \
            ../include/QicsMainGrid.h \
            ../include/QicsHeader.h \
            ../include/QicsRowHeader.h \
            ../include/QicsColumnHeader.h \
            ../include/QicsTableRegionDrag.h \
            ../include/QicsRowColumnDrag.h \
            ../include/QicsGrid.h \
            ../include/QicsScreenGrid.h \
            ../include/QicsTableGrid.h \
            ../include/QicsHeaderGrid.h \
            ../include/QicsCellDisplay.h \
            ../include/QicsTextCellDisplay.h \
            ../include/QicsTextCellDisplayHelpers.h \
            ../include/QicsCheckCellDisplay.h \
            ../include/QicsComboCellDisplay.h \
            ../include/QicsWidgetCellDisplay.h \
            ../include/QicsComboCheckCellDisplay.h \
            ../include/QicsComboTabCellDisplay.h \
            ../include/QicsListCellDisplay.h \
            ../include/QicsRadioCellDisplay.h \
            ../include/QicsProgressCellDisplay.h \
            ../include/QicsDataItemFormatter.h \
            ../include/QicsDataModel.h \
            ../include/QicsDataItem.h \
            ../include/QicsDataModelDefault.h \
            ../include/QicsScroller.h \
            ../include/QicsScrollBarScroller.h \
            ../include/QicsScrollManager.h \
            ../include/QicsUtil.h \
            ../include/QicsRegion.h \
            ../include/QicsICell.h \
            ../include/QicsTableCommon.h \
            ../include/QicsTable.h \
            ../include/QicsPasteValidator.h \
            ../include/QicsGridLayout.h \
            ../include/QicsMouseMap.h \
            ../include/QicsDateTimeCellDisplay.h \
            ../include/QicsPushButtonCellDisplay.h \
            ../include/QicsKeyboardManager.h \
            ../include/QicsGridGeometry.h \
            ../include/QicsFilter.h \
            ../include/QicsNavigator.h \
            ../include/QicsRuler.h \
            ../include/QicsRubberBand.h \
            ../include/QicsHTMLExport.h \
            ../include/QicsSelection.h \
            ../include/QicsAbstractAttributeController.h \
            ../include/QicsCommonAttributeController.h \
            ../include/QicsRegionalAttributeController.h \
            ../include/QicsQtModelAttributeController.h \
            ../include/QicsDataModelQtModelAdapter.h \
            ../include/QicsCommonAttrs.h \
            ../include/QicsCellRegion.h \
            ../include/QicsAbstractClipboardDelegate.h \
            ../include/QicsAbstractFilterDelegate.h \
            ../include/QicsRegexpFilterDelegate.h \
            ../include/QicsListFilterDelegate.h \
            ../include/QicsEnumerator.h \
            ../include/QicsSpan.h \
            ../include/QicsAbstractSorterDelegate.h \
            ../include/QicsStandardSorterDelegate.h \
            ../addons/table.tree/QicsPopupDialog.h \
            ../addons/table.tree/QicsCheckPopup.h \
            ../addons/table.tree/QicsGroupBar.h \
            ../addons/table.tree/QicsGroupInfo.h \
            ../addons/table.tree/QicsSectionBar.h \
            ../addons/table.tree/QicsSortBar.h \
            ../addons/table.tree/QicsTreeDataModel.h \
            ../addons/table.tree/QicsTreeTable.h \
            ../addons/table.tree/QicsGroupCellDisplay.h \
            ../addons/table.tree/QicsSpecialRowData.h \
            ../addons/table.tree/QicsExpandableStaticRowData.h \
            ../addons/table.tree/QicsTreeTableGrid.h \
            ../addons/table.tree/QicsTreeHeaderGrid.h \
            ../addons/table.tree/QicsSummarizer.h

SOURCES += QicsGridInfo.cpp \
            QicsStyle.cpp \
            QicsCellStyle.cpp \
            QicsRepeatingCellStyle.cpp \
            QicsGridStyle.cpp \
            QicsStyleManager.cpp \
            QicsSelectionManager.cpp \
            QicsDimensionManager.cpp \
            QicsMappedDimensionManager.cpp \
            QicsSpanManager.cpp \
            QicsSorter.cpp \
            QicsCellCommon.cpp \
            QicsGridCommon.cpp \
            QicsCell.cpp \
            QicsRow.cpp \
            QicsColumn.cpp \
            QicsRepeatingRow.cpp \
            QicsRepeatingColumn.cpp \
            QicsMainGrid.cpp \
            QicsHeader.cpp \
            QicsRowHeader.cpp \
            QicsColumnHeader.cpp \
            QicsTableRegionDrag.cpp \
            QicsRowColumnDrag.cpp \
            QicsGrid.cpp \
            QicsScreenGrid.cpp \
            QicsTableGrid.cpp \
            QicsHeaderGrid.cpp \
            QicsCellDisplay.cpp \
            QicsTextCellDisplayHelpers.cpp \
            QicsTextCellDisplay.cpp \
            QicsCheckCellDisplay.cpp \
            QicsComboCellDisplay.cpp \
            QicsWidgetCellDisplay.cpp \
            QicsComboCheckCellDisplay.cpp \
            QicsComboTabCellDisplay.cpp \
            QicsListCellDisplay.cpp \
            QicsRadioCellDisplay.cpp \
            QicsProgressCellDisplay.cpp \
            QicsDataItemFormatter.cpp \
            QicsDataModel.cpp \
            QicsDataItem.cpp \
            QicsDataModelDefault.cpp \
            QicsScrollBarScroller.cpp \
            QicsScrollManager.cpp \
            QicsUtil.cpp \
            QicsTableCommon.cpp \
            QicsTable.cpp \
            QicsPasteValidator.cpp \
            QicsGridLayout.cpp \
            QicsMouseMap.cpp \
            QicsDateTimeCellDisplay.cpp \
            QicsPushButtonCellDisplay.cpp \
            QicsFilter.cpp \
            QicsKeyboardManager.cpp \
            QicsGridGeometry.cpp \
            QicsHTMLExport.cpp \
            QicsListFilterDelegate.cpp \
            QicsRegexpFilterDelegate.cpp \
            QicsAbstractFilterDelegate.cpp \
            QicsAbstractAttributeController.cpp \
            QicsRegionalAttributeController.cpp \
            QicsCommonAttributeController.cpp \
            QicsQtModelAttributeController.cpp \
            QicsDataModelQtModelAdapter.cpp \
            QicsCommonAttrs.cpp \
            QicsCellRegion.cpp \
            QicsAbstractClipboardDelegate.cpp \
            QicsEnumerator.cpp \
            QicsAbstractSorterDelegate.cpp \
            QicsStandardSorterDelegate.cpp \
            QicsRuler.cpp \
            QicsNavigator.cpp \
            QicsRubberBand.cpp \
            ../addons/table.tree/QicsPopupDialog.cpp \
            ../addons/table.tree/QicsCheckPopup.cpp \
            ../addons/table.tree/QicsGroupBar.cpp \
            ../addons/table.tree/QicsGroupInfo.cpp \
            ../addons/table.tree/QicsSectionBar.cpp \
            ../addons/table.tree/QicsSortBar.cpp \
            ../addons/table.tree/QicsTreeDataModel.cpp \
            ../addons/table.tree/QicsTreeTable.cpp \
            ../addons/table.tree/QicsGroupCellDisplay.cpp \
            ../addons/table.tree/QicsSpecialRowData.cpp \
            ../addons/table.tree/QicsExpandableStaticRowData.cpp \
            ../addons/table.tree/QicsTreeTableGrid.cpp \
            ../addons/table.tree/QicsTreeHeaderGrid.cpp

#PRINTING_NOOP
HEADERS +=  ../include/QicsPrintGrid.h \
            ../include/QicsTablePrint.h \
            ../addons/printing/QicsPrintPreviewPage.h \
            ../addons/printing/QicsPrintPreviewWidget.h \
            ../addons/printing/QicsPageMetrics.h

SOURCES +=  QicsPrintGrid.cpp \
            QicsTablePrint.cpp \
            ../addons/printing/QicsPrintPreviewPage.cpp \
            ../addons/printing/QicsPrintPreviewWidget.cpp \
            ../addons/printing/QicsPageMetrics.cpp
#END_PRINTING_NOOP

#KDCHART_NOOP
contains(DEFINES, BIND_KDCHART) {
    SOURCES += ../addons/kdchart/QicsKDChartDataModelAdapter.cpp \
                ../addons/kdchart/QicsKDChartTableObject.cpp

    HEADERS += ../addons/kdchart/QicsKDChartDataModelAdapter.h \
                ../addons/kdchart/QicsKDChartTableObject.h

    INCLUDEPATH += ../addons/kdchart

    DEPENDPATH  += ../addons/kdchart

    addons_kdchart.files = ../addons/kdchart/*.h
    addons_kdchart.path = $$[QT_INSTALL_HEADERS]/$$INSTALL_INCLUDE_SUFFIX/addons/kdchart
    INSTALLS += addons_kdchart
}
#END_KDCHART_NOOP

contains(CONFIG, eval_noop) {
    HEADERS += evaldialog/QicsEvalDialog.h \
                evaldialog/QicsMagicDefs.h

    SOURCES += evaldialog/QicsEvalDialog.cpp

    DEFINES += EVALDIALOG
    INCLUDEPATH += ./evaldialog
} #END_EVAL_NOOP

win32 {
   dlltarget.path = $$[QT_INSTALL_BINS]
   INSTALLS += dlltarget
}


target.path = $$[QT_INSTALL_LIBS]/$$INSTALL_LIB_SUFFIX
INSTALLS += target

include.files = ../include/*.h
include.path = $$[QT_INSTALL_HEADERS]/$$INSTALL_INCLUDE_SUFFIX
INSTALLS += include

addons_tabletree.files = ../addons/table.tree/*.h
addons_tabletree.path = $$[QT_INSTALL_HEADERS]/$$INSTALL_INCLUDE_SUFFIX/addons/table.tree
INSTALLS += addons_tabletree

#PRINTING_NOOP
addons_printing.files = ../addons/printing/QicsPrintPreviewPage.h \
                        ../addons/printing/QicsPrintPreviewWidget.h \
                        ../addons/printing/QicsPageMetrics.h
addons_printing.path = $$[QT_INSTALL_HEADERS]/$$INSTALL_INCLUDE_SUFFIX/addons/printing
INSTALLS += addons_printing
#END_PRINTING_NOOP


