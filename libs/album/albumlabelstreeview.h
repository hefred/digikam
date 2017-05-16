/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2014-05-17
 * Description : Album Labels Tree View.
 *
 * Copyright (C) 2014-2015 Mohamed Anwer <m dot anwer at gmx dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef ALBUMLABELSTREEVIEW_H
#define ALBUMLABELSTREEVIEW_H

// Qt includes

#include <QTreeWidget>

// Local includes

#include "coredbconstants.h"
#include "album.h"
#include "statesavingobject.h"
#include "imagelisterrecord.h"

namespace Digikam
{

class AlbumLabelsTreeView : public QTreeWidget, public StateSavingObject
{
    Q_OBJECT

public:

    enum Labels
    {
        Ratings = 0,
        Picks,
        Colors
    };

public:

    explicit AlbumLabelsTreeView(QWidget* const parent = 0, bool setCheckable = false);
    ~AlbumLabelsTreeView();

    /**
     * @return true if the tree widget is checkable
     *         and false if not
     */
    bool isCheckable() const;

    /**
     * @return true if Loading state function is running
     */
    bool isLoadingState() const;

    /**
     * @return a Pixmap of a 30*30 pixels golden star used
     *         for rating and widget icon
     */
    QPixmap goldenStarPixmap(bool fillin=true) const;

    /**
     * @brief Creates a 30*30 rectangular pixmap with
     *        specific color
     *
     * @param color wanted to be set
     * @return pixmap has a rectangle filled with the color
     */
    QPixmap colorRectPixmap(const QColor& color) const;

    /**
     * @brief Provide the current selection from the tree-view
     *        hierarchy
     *
     * @return a QHash with three keys: "Ratings", "Picks", and
     *         "Colors", every key dedicated to an int list which
     *         holds the rows selected
     */
    QHash<Labels, QList<int> > selectedLabels();

    /**
     * @brief Loading and saving state function inherited from
     *        StateSavingObject
     */
    void doLoadState();
    void doSaveState();

    /**
     * @brief Restores the selection state from the AlbumHistory
     *        class
     *
     * @param neededLabels is a QHash to restore the selection
     *        from it, the hash is formatted just like the hash
     *        generated from @see selectedLabels()
     */
    void restoreSelectionFromHistory(QHash<Labels, QList<int> > neededLabels);

    /**
     * @brief Emits a signal to the search handler to set the Current
     *        album from currently selected labels
     */
    void setCurrentAlbum();

private:

    /**
     * @brief Initializing the tree-view hierarchy
     */
    void initTreeView();
    void initRatingsTree();
    void initPicksTree();
    void initColorsTree();

private Q_SLOTS:

    void slotSettingsChanged();

Q_SIGNALS:

    void signalSetCurrentAlbum();

private:

    class Private;
    Private* const d;
};

// --------------------------------------------------------------------

class AlbumLabelsSearchHandler : public QObject
{
    Q_OBJECT

public:

    explicit AlbumLabelsSearchHandler(AlbumLabelsTreeView* const treeWidget);
    ~AlbumLabelsSearchHandler();

    /**
     * @return Album pointer of the currently selected labels
     */
    Album* albumForSelectedItems() const;

    /**
     * @brief Gets the list of images generated, for exporting
     *
     * @return QUrl List of images Urls
     */
    QList<QUrl> imagesUrls() const;

    /**
     * @return A string for a name generated by @see
     *         generateAlbumNameForExporting()
     */
    QString generatedName() const;

    /**
     * @brief Restores the selection of the tree-view from history
     *
     * @param neededLabels a hash to restore selection from it
     */
    void restoreSelectionFromHistory(const QHash<AlbumLabelsTreeView::Labels, QList<int> >& neededLabels);

    /**
     * @return true if the tree-view is restoring the selection state
     *         from history to block searching until the restoring
     *         is done
     */
    bool isRestoringSelectionFromHistory() const;

private:

    /**
     * @brief Create XML for currently selected labels
     *
     * @param selectedLabels to generate XML for it
     * @return XML generated
     */
    QString createXMLForCurrentSelection(const QHash<AlbumLabelsTreeView::Labels, QList<int> >& selectedLabels);

    /**
     * @brief Passes the generated XML to CoreDB class
     *        search methods
     *
     * @param xml generated from @see createXMLForCurrentSelection
     * @return A pointer of the search album created by the CoreDB
     */
    SAlbum* search(const QString& xml) const;

    /**
     * @brief Generates album name based on the selected labels for
     *        example if rating 2 and 5 are selected then the name is
     *        "Rating: 2, 5", If there are any selected picks for example
     *        Pending, then the generated name is "Rating: 2, 5 | Picks: Pending"
     */
    void generateAlbumNameForExporting(const QList<int>& ratings, const QList<int>& colorsList, const QList<int>& picksList);

    /**
     * @brief Passes the current album DB url to a DBJobsManager
     *        to get images urls in this album
     */
    void imagesUrlsForCurrentAlbum();

    /**
     * @return "Labels Album" if the tree-widget is not checkable
     *         and returns "Exporting Album" if it was checkable
     */
    QString getDefaultTitle() const;

private Q_SLOTS:

    void slotSelectionChanged();
    void slotCheckStateChanged();
    void slotSetCurrentAlbum();
    void slotResult();
    void slotData(const QList<ImageListerRecord>& data);

Q_SIGNALS:

    void checkStateChanged(Album* album, Qt::CheckState checkState);

private:

    class Private;
    Private* const d;
};

} // namespace Digikam
#endif // ALBUMLABELSTREEVIEW_H
