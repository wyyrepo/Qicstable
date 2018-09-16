/*********************************************************************
**
** Copyright (C) 2002-2014 Integrated Computer Solutions, Inc.
** All rights reserved.
**
** This file is part of the QicsTable software.
**
** See the top level README file for license terms under which this
** software can be used, distributed, or modified.
**
**********************************************************************/

#include "QicsStyleManager.h"

#include "QicsDimensionManager.h"
#include "QicsSpanManager.h"
#include "QicsDataModel.h"
#include "QicsAbstractAttributeController.h"
#include "QicsCellDisplay.h"

#define SAVE_SPACE

//----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS
//----------------------------------------------------------------------------

QicsStyleManager::QicsStyleManager(Qics::QicsGridType type, QicsGridInfo *grid_info, QObject *parent)
    : QObject(parent),
        myType(type),
        myReportChanges(true),
        myBaseSM(0),
        myGridInfo(grid_info),
        myModelAttributeController(0)
{
    myDefaultStyle = new QicsCellStyle(type, true);
    myGridStyle = new QicsGridStyle(type, true);

    mySpanManager = new QicsSpanManager(grid_info);
    connect(mySpanManager, SIGNAL(spanChanged(const QicsSpan &)), SIGNAL(spanChanged(const QicsSpan &)));
}

QicsStyleManager::QicsStyleManager(QicsStyleManager *base_sm, QicsGridInfo *grid_info, QObject *parent)
    : QObject(parent),
        myType(base_sm->type()),
        myReportChanges(true),
        myModelAttributeController(0)
{
    myBaseSM = base_sm;
    myGridInfo = grid_info;

    myDefaultStyle = new QicsCellStyle(type());
    myGridStyle = new QicsGridStyle(type());

    connect(base_sm, SIGNAL(cellPropertyChanged(const QicsRegion &,
        QicsCellStyle::QicsCellStyleProperty, bool)),
        this, SIGNAL(cellPropertyChanged(const QicsRegion &,
        QicsCellStyle::QicsCellStyleProperty, bool)));
    connect(base_sm, SIGNAL(gridPropertyChanged(QicsGridStyle::QicsGridStyleProperty)),
        this, SIGNAL(gridPropertyChanged(QicsGridStyle::QicsGridStyleProperty)));

    // Create a new span manager with the same spans as the base span manager
    mySpanManager = new QicsSpanManager(*(base_sm->spanManager()));

    connect(mySpanManager, SIGNAL(spanChanged(const QicsSpan &)),
        this, SIGNAL(spanChanged(const QicsSpan &)));
}

QicsStyleManager::~QicsStyleManager()
{
    delete myModelAttributeController;
    myModelAttributeController = 0;

    deleteCellStyleVectors(myVectorOfModelColumns);
    deleteCellStyleVectors(myVectorOfVisualColumns);

    deleteCellStyles(myVectorOfModelRowStyles);
    deleteCellStyles(myVectorOfVisualRowStyles);
    deleteCellStyles(myVectorOfModelColumnStyles);
    deleteCellStyles(myVectorOfVisualColumnStyles);

    deleteRepeatingCellStyles(myVectorOfRepeatingRowStyles);
    deleteRepeatingCellStyles(myVectorOfRepeatingColumnStyles);

    delete myDefaultStyle;
    delete myGridStyle;
    delete mySpanManager;

    myDefaultStyle = 0;
    myGridStyle = 0;
    mySpanManager = 0;
}

void QicsStyleManager::setModelAttributeController(QicsAbstractAttributeController* mac)
{
    myModelAttributeController = mac;

    if (myModelAttributeController) {
        myModelAttributeController->setDefaultStyle(myDefaultStyle);
        myModelAttributeController->setGridInfo(myGridInfo);
    }

    setDataModel(myGridInfo->dataModel());
}

void QicsStyleManager::deleteCellStyleVectors(QicsCellStylePVPV &vcols)
{
    const int size = vcols.size();
    for (int c = 0; c < size; ++c)
        if (vcols.at(c)) {
            deleteCellStyles(*(vcols.at(c)));
            delete vcols.at(c);
        }
    vcols.clear();
}

void QicsStyleManager::deleteCellStyles(QicsCellStylePV &csv)
{
    qDeleteAll(csv);
    csv.clear();
}

void QicsStyleManager::deleteRepeatingCellStyles(QicsRepeatingCellStylePV &rcsv)
{
    qDeleteAll(rcsv);
    rcsv.clear();
}

///////////////////////////////////////////////////////////////////////
//////////////////      Get/Set Styles Methods       //////////////////
///////////////////////////////////////////////////////////////////////

QicsCellStyle *QicsStyleManager::getCellStyle(int row, int col) const
{
    if (row < 0 || col < 0) return 0;

    // get style from MAC
    if (myModelAttributeController)
        return myModelAttributeController->cellStyle(row, col);

    const QicsCellStylePV *the_row_vec = myVectorOfModelColumns.value(col, 0);
    if (!the_row_vec)
        return 0;

    return the_row_vec->value(row, 0);
}

void QicsStyleManager::setCellStyle(int row, int col, QicsCellStyle *cs)
{
    if (row < 0 || col < 0 || !cs)
        return;

    // Check if this is cell displayer and inform it about changes
    QicsCellDisplay *cd = (QicsCellDisplay*)getCellProperty(row, col, QicsCellStyle::CellDisplayer);
    if (cd)
        cd->aboutToClear(gridInfo(), row, col);

    // set style via MAC
    if (myModelAttributeController) {
        myModelAttributeController->setCellStyle(row, col, cs);
        return;
    }

    // else, set style via standard attr model
    // trick - setting one of properties to a cell will result in its creation
    QicsCellStyle::QicsCellStyleProperty pr = (QicsCellStyle::QicsCellStyleProperty)0;
    setCellProperty(row, col, false, pr, cs->getValue(pr));

    const QicsCellStylePV &the_row_vec = *(myVectorOfModelColumns.at(col));
    *the_row_vec[row] = *cs;
}


///////////////////////////////////////////////////////////////////////
///////////////////      Xml Methods                ///////////////////
///////////////////////////////////////////////////////////////////////

void QicsStyleManager::setDefaultsFromDomXml(const QDomElement& e, QicsTable* table)
{
    // MAC
    if (myModelAttributeController)
        myModelAttributeController->defaultStyle()->configureFromDomXml(e, table);
    else
        myDefaultStyle->configureFromDomXml(e, table);
}

QDomElement QicsStyleManager::toDomXml(const QString& tag, QDomDocument* doc)
{
    QDomElement styleElement = doc->createElement(tag);
    int size = 0;
    // Span Manager
    QDomElement smElement = mySpanManager->toDomXml(doc);
    styleElement.appendChild(smElement);

    // MAC
    if (myModelAttributeController) {

        int rows = gridInfo()->dataModel()->numRows();
        int cols = gridInfo()->dataModel()->numColumns();

        // cells
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                const QicsCellStyle *style = myModelAttributeController->cellStyle(row,col);
                if (style) {
                    QDomElement cellElement = style->toDomXml("ModelCell", doc);
                    cellElement.setAttribute("column",QString::number(col));
                    cellElement.setAttribute("row",QString::number(row));
                    styleElement.appendChild(cellElement);
                }
            }
        }

        // rows
        for (int row = 0; row < rows; ++row) {
            const QicsCellStyle *style = myModelAttributeController->rowStyle(row);
            if (style) {
                QDomElement rowElement = style->toDomXml("ModelRow", doc);
                rowElement.setAttribute("row",QString::number(row));
                styleElement.appendChild(rowElement);
            }
        }

        // columns
        for (int col = 0; col < cols; ++col) {
            const QicsCellStyle *style = myModelAttributeController->columnStyle(col);
            if (style) {
                QDomElement columnElement = style->toDomXml("ModelColumn", doc);
                columnElement.setAttribute("column",QString::number(col));
                styleElement.appendChild(columnElement);
            }
        }

        // defaults
        QDomElement defaultElement = myModelAttributeController->defaultStyle()->toDomXml("Defaults", doc);
        styleElement.appendChild(defaultElement);


    }
    else {
        //Save defaults
        QDomElement defaultElement = myDefaultStyle->toDomXml("Defaults", doc);
        styleElement.appendChild(defaultElement);

        //Save Model Row Props
        myVectorOfModelRowStyles.size();
        for(int row=0; row < size; ++row) {
            const QicsCellStyle *style = myVectorOfModelRowStyles.at(row);
            if(style)
            {
                QDomElement rowElement = style->toDomXml("ModelRow", doc);
                rowElement.setAttribute("row",QString::number(row));
                styleElement.appendChild(rowElement);
            }
        }

        //Save Model Column Props
        size = myVectorOfModelColumnStyles.size();
        for( int col=0; col < size; ++col ) {
            const QicsCellStyle *style = myVectorOfModelColumnStyles.at(col);
            if(style)
            {
                QDomElement columnElement = style->toDomXml("ModelColumn", doc);
                columnElement.setAttribute("column",QString::number(col));
                styleElement.appendChild(columnElement);
            }
        }

        // Save Model Indiv. Cell Props
        size = myVectorOfModelColumns.size();
        for( int col=0; col < size; ++col ) {
            const QicsCellStylePV *styleVec = myVectorOfModelColumns.at(col);
            if(styleVec)
            {
                const QicsCellStylePV &the_row_vec = *styleVec;
                int rowSze = the_row_vec.size();
                for( int row=0; row < rowSze; ++row ) {
                    const QicsCellStyle *style = the_row_vec.at(row);
                    if(style)
                    {
                        QDomElement cellElement = style->toDomXml("ModelCell", doc);
                        cellElement.setAttribute("column",QString::number(col));
                        cellElement.setAttribute("row",QString::number(row));
                        styleElement.appendChild(cellElement);
                    }
                }
            }
        }
    }

    //Save Visual Row Props
    size = myVectorOfVisualRowStyles.size();
    for( int row=0; row < size; ++row ) {
        const QicsCellStyle *style = myVectorOfVisualRowStyles.at(row);
        if(style) {
            QDomElement rowElement = style->toDomXml("VisualRow", doc);
            rowElement.setAttribute("row",QString::number(row));
            styleElement.appendChild(rowElement);
        }
    }

    //Save Visual Column Props
    size = myVectorOfVisualColumnStyles.size();
    for( int col=0; col < size; ++col ) {
        const QicsCellStyle *style = myVectorOfVisualColumnStyles.at(col);
        if(style) {
            QDomElement columnElement = style->toDomXml("VisualColumn", doc);
            columnElement.setAttribute("column",QString::number(col));
            styleElement.appendChild(columnElement);
        }
    }

    // Save Visual Indiv. Cell Props
    size = myVectorOfVisualColumns.size();
    for( int col=0; col < size; ++col ) {
        const QicsCellStylePV *styleVec = myVectorOfVisualColumns.at(col);
        if(styleVec) {
            const QicsCellStylePV &the_row_vec = *(myVectorOfVisualColumns.at(col));
            int rowSize = the_row_vec.size();
            for( int row=0; row < rowSize; ++row ) {
                const QicsCellStyle *style = the_row_vec.at(row);
                if(style) {
                    QDomElement cellElement = style->toDomXml("VisualCell", doc);
                    cellElement.setAttribute("column",QString::number(col));
                    cellElement.setAttribute("row",QString::number(row));
                    styleElement.appendChild(cellElement);
                }
            }
        }
    }

    //Save Repeating Row Props
    size = myVectorOfRepeatingRowStyles.size();
    for( int row=0; row < size; ++row ) {
        const QicsCellStyle *style = myVectorOfRepeatingRowStyles.at(row);
        if(style) {
            QDomElement rowElement = style->toDomXml("RepeatingRow", doc);
            styleElement.appendChild(rowElement);
        }
    }

    //Save Repeating Column Props
    size = myVectorOfRepeatingColumnStyles.size();
    for( int col=0; col < size; ++col ) {
        const QicsCellStyle *style = myVectorOfRepeatingColumnStyles.at(col);
        if(style) {
            QDomElement columnElement = style->toDomXml("RepeatingColumn", doc);
            styleElement.appendChild(columnElement);
        }
    }

    //Save Grid Props
    QDomElement gridElement = myGridStyle->toDomXml("Grid", doc);
    styleElement.appendChild(gridElement);

    return styleElement;
}

void QicsStyleManager::configureFromDomXml(const QDomElement& e, QicsTable* table)
{
    bool olds = myReportChanges;
    setReportChanges(false);

    QDomElement child = e.firstChildElement();

    while (!child.isNull()) {
        const QString tag = child.tagName();
        if( tag == "ModelRow" )
            setRowFromDomXml(child, table, false);
        else if( tag == "VisualRow" )
            setRowFromDomXml(child, table, true);
        else if( tag == "ModelColumn" )
            setColumnFromDomXml(child, table, false);
        else if( tag == "VisualColumn" )
            setColumnFromDomXml(child, table,true);
        else if( tag == "ModelCell" )
            setCellFromDomXml(child, table, false);
        else if( tag == "VisualCell" )
            setCellFromDomXml(child, table, true);
        else if( tag == "RepeatingRow" )
            setRepeatingRowFromDomXml(child, table);
        else if( tag == "RepeatingColumn" )
            setRepeatingColumnFromDomXml(child, table);
        else if( tag == "Grid" )
            setGridFromDomXml(child, table);
        else if( tag == "Defaults" )
            setDefaultsFromDomXml(child, table);
        else if( (tag == "SpanList") && mySpanManager )
            mySpanManager->configureFromDomXml(child);

        child = child.nextSiblingElement();
    }

    setReportChanges(olds);
}

void QicsStyleManager::setCellFromDomXml(const QDomElement& e, QicsTable* table, bool visual_coords)
{
    int row = e.attribute("row").toInt();
    int col = e.attribute("column").toInt();

    if (!visual_coords) {
        if (myModelAttributeController) {
            QicsCellStyle *cs = new QicsCellStyle(Qics::TableGrid);
            cs->configureFromDomXml(e, table);

            myModelAttributeController->setCellStyle(row, col, cs);

            delete cs;
            return;
        }
    }

    QicsCellStylePVPV *p_styles = 0;

    if (visual_coords)
        p_styles = &myVectorOfVisualColumns;
    else
        p_styles = &myVectorOfModelColumns;

    QicsCellStylePVPV &styles = *p_styles;

    // Verify that there is a cell there in the first place, If not
    // expand the table to reference it.
    if(styles.size() <= col) {
        // Ok, there aren't even the required number of columns
        // in this vector. let's expand the number of columns.
        // This doesnt give us rows, just places (columns) to
        // put them
        //SLOW ME DOWN BABY, lets speed this up.
        styles.resize(col+10);
    }
    // See if we have a ROW located here, it might just be null
    // so we will have to create one.
    if (!styles.at(col)) {
        //Ok this just gives us a row vector, it has no real rows
        //in it.
        styles[col] = new QicsCellStylePV();
    }
    // Now lets verify that we have enough rows,
    if (styles.at(col)->size() <= row) {
        //Ok, not enough rows, lets add enough +10.
        // SLOW ME DOWN BABY
        styles.at(col)->resize(row+10);
    }
    // lets see if we have a cell at this Place, if there
    // is no Style there, then we will need to create one,
    // then set its values.

    // This funny cast seems needed, put the pointer into
    // an easy to read reference.
    QicsCellStylePV &the_row_vec = *(styles.at(col));
    QicsCellStyle* cellStyle = 0;
    if (!the_row_vec.at(row)) {
        cellStyle = new QicsCellStyle(type());
        the_row_vec[row] = cellStyle;
    }

    // By this point we have verified that either
    // everything existed to get here with out memory
    // errors, or we have created everything that we need
    // just set it and be done with it.
    the_row_vec.at(row)->configureFromDomXml(e, table);

    // #### TODO: FIGURE OUT HOW TO EMIT ALL THESE CHANGES
}

void QicsStyleManager::setRowFromDomXml(const QDomElement& e, QicsTable* table, bool visual_coords)
{
    int row = e.attribute("row").toInt();

    if (!visual_coords) {
        if (myModelAttributeController) {
            QicsCellStyle *cs = new QicsCellStyle(Qics::TableGrid);
            cs->configureFromDomXml(e, table);

            myModelAttributeController->setRowStyle(row, cs);

            delete cs;
            return;
        }
    }

    //QicsCellStylePVPV *p_styles = 0;
    QicsCellStylePV *p_row_styles = 0;

    if (visual_coords) {
        //p_styles = &myVectorOfVisualColumns;
        p_row_styles = &myVectorOfVisualRowStyles;
    }
    else {
        //p_styles = &myVectorOfModelColumns;
        p_row_styles = &myVectorOfModelRowStyles;
    }

    // Only used if we want to clear cell values
    QicsCellStylePV &row_styles = *p_row_styles;

    // First, try to see if there is something to set:
    if (row_styles.size() <= row) {
        // SLOW ME Down baby, revisit for speed.
        row_styles.resize(row+10);
    }

    // now see if there is a style there
    if (!row_styles.at(row))
        row_styles[row] = new QicsCellStyle(type());

    // set it, we should have created all we need by this point
    row_styles.at(row)->configureFromDomXml(e, table);

    // #### TODO: FIGURE OUT EMITS
}

void QicsStyleManager::setColumnFromDomXml(const QDomElement& e, QicsTable* table, bool visual_coords)
{
    int col = e.attribute("column").toInt();

    if (!visual_coords) {
        if (myModelAttributeController) {
            QicsCellStyle *cs = new QicsCellStyle(Qics::TableGrid);
            cs->configureFromDomXml(e, table);

            myModelAttributeController->setColumnStyle(col, cs);

            delete cs;
            return;
        }
    }

    //QicsCellStylePVPV *p_styles;
    QicsCellStylePV *p_col_styles;

    if (visual_coords) {
        //p_styles = &myVectorOfVisualColumns;
        p_col_styles = &myVectorOfVisualColumnStyles;
    }
    else {
        //p_styles = &myVectorOfModelColumns;
        p_col_styles = &myVectorOfModelColumnStyles;
    }

    // Only used if we need to clear cell attrs
    QicsCellStylePV &col_styles = *p_col_styles;

    // First, try to see if there is something to set:
    if (col_styles.size() <= col) {
        // SLOW ME DOWN BABY (think about this with speed)
        col_styles.resize(col+10);
    }

    // now see if there is a style there
    if (!col_styles.at(col))
        col_styles[col] = new QicsCellStyle(type());

    // set it, we should have created all we need by this point
    col_styles.at(col)->configureFromDomXml(e, table);
}

void QicsStyleManager::setRepeatingRowFromDomXml(const QDomElement& e, QicsTable* table)
{
    // THIS WILL NOT WORK DO TO LACK OF INTERVAL

    unsigned int interval = 2;

    QicsRepeatingCellStyle *rcs = 0;

    int row = e.attribute("row").toInt();

    QicsRepeatingCellStylePV &styles = myVectorOfRepeatingRowStyles;
    QicsRepeatingCellStylePV::iterator iter, iter_end(styles.end());

    for (iter = styles.begin(); iter != iter_end; ++iter) {
        rcs = *iter;
        if ((rcs->start() == row) && (rcs->interval() == interval))
            break;

        rcs = 0;
    }

    if (!rcs) {
        //XXX
        qDeleteAll(styles);
        styles.clear();
        rcs = new QicsRepeatingCellStyle(Qics::RowIndex, row, interval);
        styles.push_back(rcs);
    }

    rcs->configureFromDomXml(e, table);
}

void QicsStyleManager::setRepeatingColumnFromDomXml(const QDomElement& e, QicsTable* table)
{
    // THIS WILL NOT WORK DO TO LACK OF INTERVAL

    unsigned int interval = 2;

    QicsRepeatingCellStyle *rcs = 0;

    int row = e.attribute("column").toInt();

    QicsRepeatingCellStylePV &styles = myVectorOfRepeatingColumnStyles;
    QicsRepeatingCellStylePV::iterator iter, iter_end(styles.end());

    for (iter = styles.begin(); iter != iter_end; ++iter) {
        rcs = *iter;
        if ((rcs->start() == row) && (rcs->interval() == interval))
            break;

        rcs = 0;
    }

    if (!rcs) {
        rcs = new QicsRepeatingCellStyle(Qics::RowIndex, row, interval);
        styles.push_back(rcs);
    }

    rcs->configureFromDomXml(e, table);
}

void QicsStyleManager::setGridFromDomXml(const QDomElement& e, QicsTable*)
{
    myGridStyle->configureFromDomXml(e);
}

///////////////////////////////////////////////////////////////////////
///////////////////      Get Property Methods       ///////////////////
///////////////////////////////////////////////////////////////////////

/*
* Finding a cell property means looking at cell styles in the
* following order:
*
* - visual cell
* - model cell
* - visual column
* - repeating column
* - model column
* - visual row
* - repeating row
* - model row
* - default
*
*/

void *QicsStyleManager::getCellProperty(int row, int col,
                                  QicsCellStyle::QicsCellStyleProperty name,
                                  int visual_row, int visual_col) const
{
    void *val = 0;

    const bool do_visual = (visual_row >= 0) && (visual_col >= 0);

    if (do_visual && (val = cellProp(visual_row, visual_col, name, myVectorOfVisualColumns)))
        return val;

    if (myModelAttributeController && (val = myModelAttributeController->cellProperty(row, col, name)))
        return val;

    if ((val = cellProp(row, col, name, myVectorOfModelColumns)))
        return val;

    if (do_visual && (val = columnProp(visual_col, name, myVectorOfVisualColumnStyles)))
        return val;

    if ((val = repeatingColumnProp(visual_col, name, myVectorOfRepeatingColumnStyles)))
        return val;

    if (myModelAttributeController && (val = myModelAttributeController->columnProperty(col, name)))
        return val;

    if ((val = columnProp(col, name, myVectorOfModelColumnStyles)))
        return val;

    if (do_visual && (val = rowProp(visual_row, name, myVectorOfVisualRowStyles)))
        return val;

    if ((val = repeatingRowProp(visual_row, name, myVectorOfRepeatingRowStyles)))
        return val;

    if (myModelAttributeController && (val = myModelAttributeController->rowProperty(row, name)))
        return val;

    if ((val = rowProp(row, name, myVectorOfModelRowStyles)))
        return val;

    val = defaultProp(name);

    if (val || !myBaseSM)
        return val;

    return myBaseSM->getCellProperty(row, col, name, visual_row, visual_col);
}

/*
* Finding a row property means looking at cell styles in the
* following order:
*
* - visual row
* - repeating row
* - model row
* - default
*
*/
void *QicsStyleManager::getRowProperty(int row,
                                 QicsCellStyle::QicsCellStyleProperty name,
                                 int visual_row) const
{
    void *val = 0;

    if ((visual_row >= 0) && (val = rowProp(visual_row, name, myVectorOfVisualRowStyles))) {
        return val;
    }
    else if ((val = repeatingRowProp(visual_row, name, myVectorOfRepeatingRowStyles))) {
        return val;
    }
    else if (myModelAttributeController && (val = myModelAttributeController->rowProperty(row, name))) {
        return val;
    }
    else if ((val = rowProp(row, name, myVectorOfModelRowStyles))) {
        return val;
    }
    else {
        val = defaultProp(name);

        if (val || !myBaseSM)
            return val;

        return myBaseSM->getRowProperty(row, name, visual_row);
    }
}

void *QicsStyleManager::getColumnProperty(int col,
                                    QicsCellStyle::QicsCellStyleProperty name,
                                    int visual_col) const
{
    void *val = 0;

    if ((visual_col >= 0) &&
        (val = columnProp(visual_col, name, myVectorOfVisualColumnStyles))) {
        return val;
    }
    else if ((val = repeatingColumnProp(visual_col, name, myVectorOfRepeatingColumnStyles))) {
        return val;
    }
    else if (myModelAttributeController && (val = myModelAttributeController->columnProperty(col, name))) {
        return val;
    }
    else if ((val = columnProp(col, name, myVectorOfModelColumnStyles))) {
        return val;
    }
    else {
        val = defaultProp(name);

        if (val || !myBaseSM)
            return val;

        return myBaseSM->getColumnProperty(col, name, visual_col);
    }
}

void *QicsStyleManager::getGridProperty(QicsGridStyle::QicsGridStyleProperty name) const
{
    void *val = myGridStyle->getValue(name);

    if (!val)
        val = myBaseSM->getGridProperty(name);

    return val;
}

void *QicsStyleManager::getRepeatingRowProperty(int row, unsigned int interval,
                                          QicsCellStyle::QicsCellStyleProperty name)
                                          const
{
    void *val = 0;

    if ((val = repeatingRowProp(row, interval, name, myVectorOfRepeatingRowStyles))) {
        return val;
    }
    else {
        val = defaultProp(name);

        if (val || !myBaseSM)
            return val;

        return myBaseSM->getRepeatingRowProperty(row, interval, name);
    }
}

void *QicsStyleManager::getRepeatingColumnProperty(int col, unsigned int interval,
                                             QicsCellStyle::QicsCellStyleProperty name)
                                             const
{
    void *val = 0;

    if ((val = repeatingColumnProp(col, interval, name, myVectorOfRepeatingColumnStyles))) {
        return val;
    }
    else {
        val = defaultProp(name);

        if (val || !myBaseSM)
            return val;

        return myBaseSM->getRepeatingColumnProperty(col, interval, name);
    }
}

void *QicsStyleManager::getDefaultProperty(QicsCellStyle::QicsCellStyleProperty name) const
{
    void *val = defaultProp(name);

    if (val || !myBaseSM)
        return val;

    return myBaseSM->getDefaultProperty(name);
}

QicsCellStyle *QicsStyleManager::getDefaultCellStyle() const
{
    // MAC
    if (myModelAttributeController) {
        QicsCellStyle *s = myModelAttributeController->defaultStyle();
        if (s)
            return s;
    }

    return myDefaultStyle;
}

void *QicsStyleManager::cellProp(int row, int col,
                           QicsCellStyle::QicsCellStyleProperty prop,
                           const QicsCellStylePVPV &styles) const
{
    const QicsCellStylePV *the_row_vec = styles.value(col, 0);
    if (!the_row_vec)
        return 0;

    const QicsCellStyle *style = the_row_vec->value(row, 0);
    if (style)
        return style->getValue(prop);

    return 0;
}

void *QicsStyleManager::rowProp(int row, QicsCellStyle::QicsCellStyleProperty prop,
                          const QicsCellStylePV &styles) const
{
    const QicsCellStyle *style = styles.value(row, 0);
    if (style)
        return style->getValue(prop);

    return 0;
}

void *QicsStyleManager::columnProp(int col, QicsCellStyle::QicsCellStyleProperty prop,
                             const QicsCellStylePV &styles) const
{
    const QicsCellStyle *style = styles.value(col, 0);

    if (style)
        return style->getValue(prop);

    return 0;
}

void *QicsStyleManager::repeatingRowProp(int row,
                                   QicsCellStyle::QicsCellStyleProperty prop,
                                   const QicsRepeatingCellStylePV &styles) const
{
    if (styles.isEmpty())
        return 0;

    QicsRepeatingCellStylePV::const_iterator iter;
    QicsRepeatingCellStylePV::const_iterator iter_end = styles.constEnd();

    //QSet<int> hiddenRows = gridInfo()->dimensionManager()->myHiddenRows;
    //int hiddenRowsCount = hiddenRows.count();
    //bool skipPrevRepeating = false;
    //foreach (int hiddenRow, hiddenRows)
    //    skipPrevRepeating |= (hiddenRow > row);
    //if (skipPrevRepeating)
    //    hiddenRowsCount = 0;

    for (iter = styles.constBegin(); iter != iter_end; ++iter) {
        const QicsRepeatingCellStyle *rcs = *iter;

        if (row < rcs->start())
            continue;

        if (((/*hiddenRowsCount +*/ row - rcs->start()) % rcs->interval()) == 0) {
            void *val = rcs->getValue(prop);
            if (val)
                return val;
        }
    }

    return 0;
}

void *QicsStyleManager::repeatingRowProp(int start_row, unsigned int interval,
                                   QicsCellStyle::QicsCellStyleProperty prop,
                                   const QicsRepeatingCellStylePV &styles) const
{
    if (styles.isEmpty())
        return 0;

    QicsRepeatingCellStylePV::const_iterator iter, iter_end(styles.constEnd());

    for (iter = styles.constBegin(); iter != iter_end; ++iter) {
        const QicsRepeatingCellStyle *rcs = *iter;

        if ((rcs->start() == start_row) && (rcs->interval() == interval))
            return (rcs->getValue(prop));
    }

    return 0;
}

void *QicsStyleManager::repeatingColumnProp(int col,
                                      QicsCellStyle::QicsCellStyleProperty prop,
                                      const QicsRepeatingCellStylePV &styles) const
{
    if (styles.isEmpty())
        return 0;

    QicsRepeatingCellStylePV::const_iterator iter;
    QicsRepeatingCellStylePV::const_iterator iter_end = styles.constEnd();

    //QSet<int> hiddenColumns = gridInfo()->dimensionManager()->myHiddenColumns;
    //int hiddenColumnsCount = hiddenColumns.count();
    //bool skipPrevRepeating = false;
    //foreach (int hiddenColumn, hiddenColumns)
    //    skipPrevRepeating |= (hiddenColumn > col);

    //if (skipPrevRepeating)
    //    hiddenColumnsCount = 0;

    for (iter = styles.constBegin(); iter != iter_end; ++iter) {
        const QicsRepeatingCellStyle *rcs = *iter;

        if (col < rcs->start())
            continue;

        if (((/*hiddenColumnsCount +*/ col - rcs->start()) % rcs->interval()) == 0) {
            void *val = rcs->getValue(prop);
            if (val)
                return val;
        }
    }

    return 0;
}

void *QicsStyleManager::repeatingColumnProp(int col, unsigned int interval,
                                      QicsCellStyle::QicsCellStyleProperty prop,
                                      const QicsRepeatingCellStylePV &styles) const
{
    if (styles.isEmpty())
        return 0;

    QicsRepeatingCellStylePV::const_iterator iter, iter_end(styles.constEnd());

    for (iter = styles.constBegin(); iter != iter_end; ++iter) {
        const QicsRepeatingCellStyle *rcs = *iter;

        if ((rcs->start() == col) && (rcs->interval() == interval))
            return (rcs->getValue(prop));
    }

    return 0;
}

void *QicsStyleManager::defaultProp(QicsCellStyle::QicsCellStyleProperty prop) const
{
    // MAC
    if (myModelAttributeController && myModelAttributeController->isDefaultPropertySupported(prop))
        return myModelAttributeController->defaultProperty(prop);

    return myDefaultStyle->getValue(prop);
}

///////////////////////////////////////////////////////////////////////
///////////////////      Set Property Methods       ///////////////////
///////////////////////////////////////////////////////////////////////

// This method will expand the internal table to just the right
// size to add this cell property if there isnt a cell there.
// always presume that the wonderful cell world is very small.
// we only need to build the table as big as we absolutly must have
// it.
void QicsStyleManager::setCellProperty(int row, int col, bool visual_coords,
                                  QicsCellStyle::QicsCellStyleProperty name,
                                  const void *val)
{
    if ( row < 0 || col < 0 )
        return;

    // Check if this is cell displayer and inform it about changes
    if (name == QicsCellStyle::CellDisplayer) {
        QicsCellDisplay *cd = visual_coords ?
            (QicsCellDisplay*)getCellProperty(-1, -1, name, row, col) :
        (QicsCellDisplay*)getCellProperty(row, col, name);
        if (cd)
            cd->aboutToClear(gridInfo(),
            visual_coords ? row : gridInfo()->modelRowIndex(row),
            visual_coords ? col : gridInfo()->modelColumnIndex(col));
    }

    // MAC
    if (!visual_coords) {
        if (myModelAttributeController && myModelAttributeController->setCellProperty(row, col, name, val)) {

            if (myReportChanges)
                emit cellPropertyChanged(QicsRegion(row, col, row, col), name, visual_coords);

            return;
        }
    }

    QicsCellStylePVPV *p_styles;

    if (visual_coords)
        p_styles = &myVectorOfVisualColumns;
    else
        p_styles = &myVectorOfModelColumns;

    QicsCellStylePVPV &styles = *p_styles;

    // Verify that there is a cell there in the first place, If not
    // expand the table to reference it.
    if(styles.size() <= col) {
        // Ok, there aren't even the required number of columns
        // in this vector. let's expand the number of columns.
        // This doesnt give us rows, just places (columns) to
        // put them
        //SLOW ME DOWN BABY, lets speed this up.
        styles.resize(col+10);
    }
    // See if we have a ROW located here, it might just be null
    // so we will have to create one.
    if (!styles.at(col)) {
        //Ok this just gives us a row vector, it has no real rows
        //in it.
        styles[col] = new QicsCellStylePV();
    }
    // Now lets verify that we have enough rows,
    if (styles.at(col)->size() <= row) {
        //Ok, not enough rows, lets add enough +10.
        // SLOW ME DOWN BABY
        styles.at(col)->resize(row+10);
    }
    // lets see if we have a cell at this Place, if there
    // is no Style there, then we will need to create one,
    // then set its values.

    // This funny cast seems needed, put the pointer into
    // an easy to read reference.
    QicsCellStylePV &the_row_vec = *(styles.at(col));
    if (!the_row_vec.at(row)) {
        the_row_vec[row] = new QicsCellStyle(type());
    }

    // By this point we have verified that either
    // everything existed to get here with out memory
    // errors, or we have created everything that we need
    // just set it and be done with it.
    the_row_vec.at(row)->setValue(name,val);

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(row, col, row, col),
        name, visual_coords);
}

void QicsStyleManager::setRowProperty(int row, bool visual_coords,
                                 QicsCellStyle::QicsCellStyleProperty name,
                                 const void *val, bool override)
{
    // MAC
    if (!visual_coords) {
        if (myModelAttributeController && myModelAttributeController->setRowProperty(row, name, val)) {

            if (myReportChanges)
                emit cellPropertyChanged(QicsRegion(row, 0, row, Qics::QicsLAST_COLUMN), name, visual_coords);

            return;
        }
    }

    QicsCellStylePVPV *p_styles;
    QicsCellStylePV *p_row_styles;

    if (visual_coords) {
        p_styles = &myVectorOfVisualColumns;
        p_row_styles = &myVectorOfVisualRowStyles;
    }
    else {
        p_styles = &myVectorOfModelColumns;
        p_row_styles = &myVectorOfModelRowStyles;
    }

    QicsCellStylePVPV &styles = *p_styles;
    QicsCellStylePV &row_styles = *p_row_styles;

    // First, try to see if there is something to set:
    if (row_styles.size() <= row ) {
        // SLOW ME Down baby, revisit for speed.
        row_styles.resize(row+10);
    }

    // now see if there is a style there
    QicsCellStyle *style = row_styles.at(row);
    if (!style) {
        row_styles[row] = style = new QicsCellStyle(type());
    }

    // set it, we should have created all we need by this point
    style->setValue(name, val);

    // unset any cell styles for this row

    // go through each cell in the row and unset this property
    if (override) {
        QicsCellStylePVPV::const_iterator iter(styles.constBegin());
        QicsCellStylePVPV::const_iterator iter_end(styles.constEnd());

        while (iter != iter_end) {
            const QicsCellStylePV *colvec = *iter;
            if (colvec && (colvec->size() > row)) {
                QicsCellStyle *s = colvec->at(row);
                if (s) s->clear(name);
            }
            ++iter;
        }
    }

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(row, 0, row, Qics::QicsLAST_COLUMN),
            name, visual_coords);
}

void QicsStyleManager::setColumnProperty(int col, bool visual_coords,
                                    QicsCellStyle::QicsCellStyleProperty name,
                                    const void *val, bool override)
{
    // MAC
    if (!visual_coords) {
        if (myModelAttributeController && myModelAttributeController->setColumnProperty(col, name, val)) {

            if (myReportChanges)
                emit cellPropertyChanged(QicsRegion(0, col, Qics::QicsLAST_ROW, col), name, visual_coords);

            return;
        }
    }

    QicsCellStylePVPV *p_styles;
    QicsCellStylePV *p_col_styles;

    if (visual_coords) {
        p_styles = &myVectorOfVisualColumns;
        p_col_styles = &myVectorOfVisualColumnStyles;
    }
    else {
        p_styles = &myVectorOfModelColumns;
        p_col_styles = &myVectorOfModelColumnStyles;
    }

    QicsCellStylePVPV &styles = *p_styles;
    QicsCellStylePV &col_styles = *p_col_styles;

    // First, try to see if there is something to set:
    if (col_styles.size() <= col ) {
        // SLOW ME DOWN BABY (think about this with speed)
        col_styles.resize(col+10);
    }

    // now see if there is a style there
    QicsCellStyle *style = col_styles.at(col);
    if (!style) {
        col_styles[col] = style = new QicsCellStyle(type());
    }

    // set it, we should have created all we need by this point
    style->setValue(name, val);

    // go through each cell in the column and unset this property
    if(override && (styles.size() > col)) {
        const QicsCellStylePV *colvec = styles.at(col);
        if (colvec) {
            QicsCellStylePV::const_iterator iter(colvec->constBegin());
            QicsCellStylePV::const_iterator iter_end(colvec->constEnd());

            while(iter != iter_end) {
                QicsCellStyle *s = *iter;
                if (s)
                    s->clear(name);
                ++iter;
            }
        }
    }

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(0, col, Qics::QicsLAST_ROW, col),
            name, visual_coords);
}

void QicsStyleManager::setGridProperty(QicsGridStyle::QicsGridStyleProperty name,
                                  const void *val)
{
    myGridStyle->setValue(name, val);

    emit gridPropertyChanged(name);
}

void QicsStyleManager::setRepeatingRowProperty(int row, unsigned int interval,
                                          QicsCellStyle::QicsCellStyleProperty name,
                                          const void *val)
{
    QicsRepeatingCellStyle *rcs = 0;

    QicsRepeatingCellStylePV &styles = myVectorOfRepeatingRowStyles;
    QicsRepeatingCellStylePV::iterator iter, iter_end(styles.end());

    for (iter = styles.begin(); iter != iter_end; ++iter) {
        rcs = *iter;
        if ((rcs->start() == row) && (rcs->interval() == interval))
            break;

        rcs = 0;
    }

    if (rcs == 0) {
        rcs = new QicsRepeatingCellStyle(Qics::RowIndex, row, interval);
        styles.push_back(rcs);
    }

    rcs->setValue(name, val);

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(0, 0, Qics::QicsLAST_ROW, Qics::QicsLAST_COLUMN),
            name, true);
}

void QicsStyleManager::setRepeatingColumnProperty(int col, unsigned int interval,
                                             QicsCellStyle::QicsCellStyleProperty name,
                                             const void *val)
{
    QicsRepeatingCellStyle *rcs = 0;

    QicsRepeatingCellStylePV &styles = myVectorOfRepeatingColumnStyles;
    QicsRepeatingCellStylePV::iterator iter, iter_end(styles.end());

    for (iter = styles.begin(); iter != iter_end; ++iter) {
        rcs = *iter;
        if ((rcs->start() == col) && (rcs->interval() == interval))
            break;

        rcs = 0;
    }

    if (rcs == 0) {
        rcs = new QicsRepeatingCellStyle(Qics::ColumnIndex, col, interval);
        styles.push_back(rcs);
    }

    rcs->setValue(name, val);

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(0, 0, Qics::QicsLAST_ROW, Qics::QicsLAST_COLUMN),
            name, true);
}

void QicsStyleManager::setDefaultProperty(QicsCellStyle::QicsCellStyleProperty name,
                                     const void *val)
{
    // MAC
    if (myModelAttributeController && myModelAttributeController->setDefaultProperty(name, val)) {
        if (myReportChanges)
            emit cellPropertyChanged(QicsRegion(0, 0, Qics::QicsLAST_ROW, Qics::QicsLAST_COLUMN), name, false);

        return;
    }

    myDefaultStyle->setValue(name, val);

    QicsCellStylePVPV::iterator iter, iter_end(myVectorOfModelColumns.end());

    // go through each cell in the table and unset this property if it exists
    for (iter = myVectorOfModelColumns.begin(); iter != iter_end; ++iter) {
        QicsCellStylePV *colvec = *iter;

        if (colvec) {
            QicsCellStylePV::iterator iter2, iter2_end((*colvec).end());

            for (iter2 = (*colvec).begin(); iter2 != iter2_end; ++iter2) {
                QicsCellStyle *style = *iter2;

                if (style)
                    style->clear(name);
            }
        }
    }

    // now go through the list of row styles and remove the property

    QicsCellStylePV::iterator iter3, iter3_end(myVectorOfModelRowStyles.end());

    for (iter3 = myVectorOfModelRowStyles.begin(); iter3 != iter3_end; ++iter3) {
        QicsCellStyle *style = *iter3;

        if (style)
            style->clear(name);
    }

    // finally, go through the list of column styles and remove the property

    for (iter3 = myVectorOfModelColumnStyles.begin();
        iter3 != myVectorOfModelColumnStyles.end(); ++iter3) {
        QicsCellStyle *style = *iter3;

        if (style)
            style->clear(name);
    }

    // report the change
    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(0, 0, Qics::QicsLAST_ROW, Qics::QicsLAST_COLUMN),
            name, false);
}

///////////////////////////////////////////////////////////////////////
///////////////////     Clear Property Methods      ///////////////////
///////////////////////////////////////////////////////////////////////
void QicsStyleManager::clearCellProperty(int row, int col, bool visual_coords,
                                    QicsCellStyle::QicsCellStyleProperty name)

{
    // Check if this is cell displayer and inform it about death
    if (name == QicsCellStyle::CellDisplayer) {
        QicsCellDisplay *cd = visual_coords ?
            (QicsCellDisplay*)getCellProperty(-1, -1, QicsCellStyle::CellDisplayer, row, col) :
        (QicsCellDisplay*)getCellProperty(row, col, QicsCellStyle::CellDisplayer, -1, -1);
        if (cd)
            cd->aboutToClear(gridInfo(),
            visual_coords ? row : gridInfo()->modelRowIndex(row),
            visual_coords ? col : gridInfo()->modelColumnIndex(col));
    }

    // MAC
    if (!visual_coords && myModelAttributeController && myModelAttributeController->clearCellProperty(row, col, name)) {
        if (myReportChanges)
            emit cellPropertyChanged(QicsRegion(row, col, row, col), name, visual_coords);

        return;
    }

    QicsCellStylePVPV *p_styles;

    if (visual_coords)
        p_styles = &myVectorOfVisualColumns;
    else
        p_styles = &myVectorOfModelColumns;

    QicsCellStylePVPV &styles = *p_styles;

    QicsCellStylePV *the_row_vec = styles.value(col, 0);
    if (!the_row_vec)
        return;

    clearStyleGivenVectorOfRows(*the_row_vec, row, name,
#ifdef SAVE_SPACE
        true
#else
        false
#endif
    );

    // report the change
    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(row, col, row, col),
            name, visual_coords);
}

void QicsStyleManager::clearRowProperty(int row, bool visual_coords,
                                   QicsCellStyle::QicsCellStyleProperty name)
{
    // MAC
    if (!visual_coords && myModelAttributeController && myModelAttributeController->clearRowProperty(row, name)) {

        if (myReportChanges)
            emit cellPropertyChanged(QicsRegion(row, 0, row, Qics::QicsLAST_COLUMN), name, visual_coords);

        return;
    }

    QicsCellStylePV *p_row_styles;

    if (visual_coords)
        p_row_styles = &myVectorOfVisualRowStyles;
    else
        p_row_styles = &myVectorOfModelRowStyles;

    QicsCellStylePV &row_styles = *p_row_styles;

    QicsCellStyle *cs = row_styles.value(row, 0);
    if (!cs)
        return;

    cs->clear(name);

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(row, 0, row, Qics::QicsLAST_COLUMN),
            name, visual_coords);
}

void QicsStyleManager::clearColumnProperty(int col, bool visual_coords,
                                      QicsCellStyle::QicsCellStyleProperty name)
{
    // MAC
    if (!visual_coords && myModelAttributeController && myModelAttributeController->clearColumnProperty(col, name)) {

        if (myReportChanges)
            emit cellPropertyChanged(QicsRegion(0, col, Qics::QicsLAST_ROW, col), name, visual_coords);

        return;
    }

    QicsCellStylePV *p_col_styles;

    if (visual_coords)
        p_col_styles = &myVectorOfVisualColumnStyles;
    else
        p_col_styles = &myVectorOfModelColumnStyles;

    QicsCellStylePV col_styles = *p_col_styles;

    QicsCellStyle *cs = col_styles.value(col, 0);
    if (!cs)
        return;

    cs->clear(name);

    if (myReportChanges)
        emit cellPropertyChanged(QicsRegion(0, col, Qics::QicsLAST_ROW, col),
            name, visual_coords);
}

void QicsStyleManager::clearRepeatingRowProperty(int row, unsigned int interval,
                                            QicsCellStyle::QicsCellStyleProperty name)
{
    QicsRepeatingCellStylePV::iterator iter, iter_end(myVectorOfRepeatingRowStyles.end());

    for (iter = myVectorOfRepeatingRowStyles.begin();
        iter != iter_end; ++iter) {
        QicsRepeatingCellStyle *rcs = *iter;

        if ((rcs->start() == row) && (rcs->interval() == interval)) {
            rcs->clear(name);
            return;
        }
    }
}

void QicsStyleManager::clearRepeatingColumnProperty(int col, unsigned int interval,
                                               QicsCellStyle::QicsCellStyleProperty name)
{
    QicsRepeatingCellStylePV::iterator iter, iter_end(myVectorOfRepeatingColumnStyles.end());

    for (iter = myVectorOfRepeatingColumnStyles.begin();
        iter != iter_end; ++iter) {
        QicsRepeatingCellStyle *rcs = *iter;

        if ((rcs->start() == col) && (rcs->interval() == interval)) {
            rcs->clear(name);
            return;
        }
    }
}

void QicsStyleManager::clearStyleGivenVectorOfRows(QicsCellStylePV &row_vec,
                                              int row,
                                              QicsCellStyle::QicsCellStyleProperty name,
                                              bool save_space)
{
    QicsCellStyle *cs = row_vec.value(row, 0);
    if (!cs)
        return;

    cs->clear(name);

    if (save_space && cs->isEmpty()) {
        delete row_vec.at(row);
        row_vec[row] = 0;
    }
}

///////////////////////////////////////////////////////////////////////
//////////////////      Insert/Delete Methods       ///////////////////
///////////////////////////////////////////////////////////////////////

void QicsStyleManager::addRows(int num)
{
    //insertRows(num, myGridInfo->dataModel()->numRows());
    if (myModelAttributeController)
        myModelAttributeController->insertRows(num, INT_MAX);
}

void QicsStyleManager::addColumns(int num)
{
    //insertColumns(num, myGridInfo->dataModel()->numColumns());
    if (myModelAttributeController)
        myModelAttributeController->insertColumns(num, INT_MAX);
}

void QicsStyleManager::insertRows(int num, int start_position)
{
    if (gridInfo()->ignoreModelSizeChanges())
        return;

    // NOTE:  We don't need to do anything to the visual cell style vectors.
    //        They don't change when rows are inserted/deleted into/from
    //        the model.

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of rows.
    if ((start_position < 0) || (num <= 0))
        return;

    // MAC
    if (myModelAttributeController)
        myModelAttributeController->insertRows(num, start_position);


    // Move Spans
    mySpanManager->insertRows(num, start_position);

    // First, we do the cell styles

    QicsCellStylePVPV::iterator iter, iter_end(myVectorOfModelColumns.end());

    // iterate through all columns
    for (iter = myVectorOfModelColumns.begin(); iter != iter_end; ++iter) {
        QicsCellStylePV *column = *iter;

        // for insertions out of the current size of the column, do nothing
        if (!column || (start_position >= column->size()))
            continue;

        QicsCellStylePV::iterator pos = column->begin() + start_position;

        column->insert(pos, num, 0);
    }

    // Next, the row styles

    if (start_position < myVectorOfModelRowStyles.size()) {
        QicsCellStylePV::iterator pos = myVectorOfModelRowStyles.begin() + start_position;
        myVectorOfModelRowStyles.insert(pos, num, 0);
    }
}

void QicsStyleManager::insertColumns(int num, int start_position)
{
    if (gridInfo()->ignoreModelSizeChanges())
        return;

    // NOTE:  We don't need to do anything to the visual cell style vectors.
    //        They don't change when columns are inserted/deleted into/from
    //        the model.

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of columns.

    if ((start_position < 0) || (num <= 0))
        return;

    // MAC
    if (myModelAttributeController)
        myModelAttributeController->insertColumns(num, start_position);


    // Move Spans
    mySpanManager->insertColumns(num, start_position);

    // First, we do the cell styles

    if (start_position < myVectorOfModelColumns.size()) {
        QicsCellStylePVPV::iterator pos = myVectorOfModelColumns.begin() + start_position;
        myVectorOfModelColumns.insert(pos, num, 0);
    }

    // Next, the column styles

    if (start_position < myVectorOfModelColumnStyles.size()) {
        QicsCellStylePV::iterator pos = myVectorOfModelColumnStyles.begin() + start_position;
        myVectorOfModelColumnStyles.insert(pos, num, 0);
    }
}

void QicsStyleManager::setDataModel(QicsDataModel *sm)
{
    // MAC
    if (myModelAttributeController) {
        if (sm)
            myModelAttributeController->reinit(sm->numRows(), sm->numColumns());
        else
            myModelAttributeController->reinit(0,0);
    }
}

void QicsStyleManager::deleteRows(int num, int start_position)
{
    if (gridInfo()->ignoreModelSizeChanges())
        return;

    // NOTE:  We don't need to do anything to the visual cell style vectors.
    //        They don't change when rows are inserted/deleted into/from
    //        the model.

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of rows.
    if ((start_position < 0) || (num <= 0))
        return;

    // MAC
    if (myModelAttributeController)
        myModelAttributeController->deleteRows(num, start_position);


    // Move Spans
    mySpanManager->deleteRows(num, start_position);

    // First, we do the cell styles

    QicsCellStylePVPV::iterator iter, iter_end(myVectorOfModelColumns.end());

    // iterate through all columns
    for (iter = myVectorOfModelColumns.begin(); iter != iter_end; ++iter) {
        QicsCellStylePV *column = *iter;

        // for insertions out of the current size of the column, do nothing
        if (!column || (start_position >= column->size()))
            continue;

        QicsCellStylePV::iterator start_pos = column->begin() + start_position;

        QicsCellStylePV::iterator end_pos;
        if ((start_position + num) >= column->size())
            end_pos = column->end();
        else
            end_pos = start_pos + num;

        qDeleteAll(start_pos, end_pos);
        column->erase(start_pos, end_pos);
    }

    // Next, the row styles

    if (start_position < myVectorOfModelRowStyles.size()) {
        QicsCellStylePV::iterator start_pos = myVectorOfModelRowStyles.begin() + start_position;

        QicsCellStylePV::iterator end_pos;
        if ((start_position + num) >= myVectorOfModelRowStyles.size())
            end_pos = myVectorOfModelRowStyles.end();
        else
            end_pos = start_pos + num;

        qDeleteAll(start_pos, end_pos);
        myVectorOfModelRowStyles.erase(start_pos, end_pos);
    }
}

void QicsStyleManager::deleteColumns(int num, int start_position)
{
    if (gridInfo()->ignoreModelSizeChanges())
        return;

    // NOTE:  We don't need to do anything to the visual cell style vectors.
    //        They don't change when columns are inserted/deleted into/from
    //        the model.

    // make sure we are in bounds..
    // and no one is trying to clobber us with a bad number of columns.

    if ((start_position < 0) || (num <= 0))
        return;

    // MAC
    if (myModelAttributeController)
        myModelAttributeController->deleteColumns(num, start_position);


    // Move Spans
    mySpanManager->deleteColumns(num, start_position);

    // First, we do the cell styles

    if (start_position < myVectorOfModelColumns.size()) {
        QicsCellStylePVPV::iterator start_pos = myVectorOfModelColumns.begin() + start_position;

        QicsCellStylePVPV::iterator end_pos;
        if ((start_position + num) >= myVectorOfModelColumns.size())
            end_pos = myVectorOfModelColumns.end();
        else
            end_pos= myVectorOfModelColumns.begin() + (start_position + num);

        QicsCellStylePVPV::iterator iter;

        // Clear the cell styles in these columns first
        for (iter = start_pos; iter < end_pos; ++iter) {
            QicsCellStylePV *styles = *iter;

            if (styles)
                deleteCellStyles(*styles);
        }

        // Now free the columns themselves
        myVectorOfModelColumns.erase(start_pos, end_pos);
    }

    // Next, the column styles

    if (start_position < myVectorOfModelColumnStyles.size()) {
        QicsCellStylePV::iterator start_pos = myVectorOfModelColumnStyles.begin() + start_position;

        QicsCellStylePV::iterator end_pos;
        if ((start_position + num) >= myVectorOfModelColumnStyles.size())
            end_pos = myVectorOfModelColumnStyles.end();
        else
            end_pos= myVectorOfModelColumnStyles.begin() + (start_position + num);

        QicsCellStylePV::iterator iter;
        for (iter = start_pos; iter < end_pos; ++iter)
            delete (*iter);

        myVectorOfModelColumnStyles.erase(start_pos, end_pos);
    }
}
