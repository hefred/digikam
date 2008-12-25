/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2007-27-08
 * Description : an tool bar action object to display logo
 *
 * Copyright (C) 2007-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "dlogoaction.h"
#include "dlogoaction.moc"

// Qt includes.

#include <QPalette>
#include <QPixmap>
#include <QBoxLayout>
#include <QTimer>
#include <QPainter>

// KDE includes.

#include <kurllabel.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <ktoolinvocation.h>
#include <kstandarddirs.h>
#include <klocale.h>

// Local includes.

#include "daboutdata.h"

namespace Digikam
{

class DLogoActionPriv
{
public:

    DLogoActionPriv()
    {
        alignOnright = true;
        timer        = 0;
        urlLabel     = 0;
        pos          = 0;
    }

    bool       alignOnright;

    int        pos;
    int        size;

    QTimer    *timer;

    QPixmap    logoPix;
    QPixmap    animPix;

    KUrlLabel *urlLabel;
};

DLogoAction::DLogoAction(QObject* parent, bool alignOnright)
           : KAction(parent), d(new DLogoActionPriv)
{
    setText("digikam.org");
    setIcon(KIcon("digikam"));

    d->logoPix      = QPixmap(KStandardDirs::locate("data", "digikam/data/banner-digikam.png"));
    d->alignOnright = alignOnright;
    d->size         = d->logoPix.height()-6;
    d->timer        = new QTimer();

    connect(d->timer, SIGNAL(timeout()),
            this, SLOT(slotTimeout()));
}

DLogoAction::~DLogoAction()
{
    delete d;
}

void DLogoAction::start()
{
    d->pos = 0;
    d->timer->start(100);
}

void DLogoAction::stop()
{
    d->pos = 0;
    d->timer->stop();
    d->urlLabel->setPixmap(d->logoPix);
}

bool DLogoAction::running() const
{
    return d->timer->isActive();
}

void DLogoAction::slotTimeout()
{
    d->pos     = (d->pos + 10) % 360;
    d->animPix = d->logoPix;
    QPainter p(&d->animPix);

    p.translate(d->logoPix.width()-(d->size/2)-4, 2 + d->size/2);

    if (d->timer->isActive())
    {
        p.setPen(QPen(parentWidget()->palette().color(QPalette::Text)));
        p.rotate( d->pos );
    }
    else
    {
        p.setPen(QPen(parentWidget()->palette().color(QPalette::Dark)));
    }

    for ( int i=0 ; i<12 ; i++ )
    {
        p.drawLine(d->size/2-4, 0, d->size/2-1, 0);
        p.rotate(30);
    }

    p.end();

    d->urlLabel->setPixmap(d->animPix);
}

QWidget* DLogoAction::createWidget(QWidget * parent)
{
    QToolBar *bar = qobject_cast<QToolBar*>(parent);

    // This action should only be used in a toolbar
    Q_ASSERT(bar != NULL);

    QWidget* container  = new QWidget(parent);
    QHBoxLayout* layout = new QHBoxLayout(container);
    d->urlLabel         = new KUrlLabel(webProjectUrl().url(), QString(), bar);
    d->urlLabel->setMargin(0);
    d->urlLabel->setScaledContents(false);
    d->urlLabel->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    d->urlLabel->setToolTip(i18n("Visit digiKam project website"));
    d->urlLabel->setPixmap(d->logoPix);
    d->urlLabel->setFocusPolicy(Qt::NoFocus);

    layout->setMargin(0);
    layout->setSpacing(0);

    if (d->alignOnright)
        layout->addStretch();

    layout->addWidget(d->urlLabel);

    connect(d->urlLabel, SIGNAL(leftClickedUrl(const QString&)),
            this, SLOT(slotProcessUrl(const QString&)));

    return container;
}

void DLogoAction::slotProcessUrl(const QString& url)
{
    KToolInvocation::invokeBrowser(url);
}

} // namespace Digikam
