/* ============================================================
 * File  : imageplugin_solarize.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-02-14
 * Description : 
 * 
 * Copyright 2004 by Renchi Raju
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

// KDE includes.
  
#include <klocale.h>
#include <kgenericfactory.h>
#include <klibloader.h>
#include <kaction.h>
#include <kcursor.h>
#include <kdebug.h>

// Local includes.

#include "imageeffect_solarize.h"
#include "imageplugin_solarize.h"

K_EXPORT_COMPONENT_FACTORY( digikamimageplugin_solarize,
                            KGenericFactory<ImagePlugin_Solarize>("digikam"));

ImagePlugin_Solarize::ImagePlugin_Solarize(QObject *parent, const char*,
                                                   const QStringList &)
                    : Digikam::ImagePlugin(parent, "ImagePlugin_Solarize")
{
    new KAction(i18n("Solarize Image..."), 0, 
                this, SLOT(slotSolarize()),
                actionCollection(), "implugcore_solarize");
                
    
    kdDebug() << "ImagePlugin_Solarize plugin loaded" << endl;
}

ImagePlugin_Solarize::~ImagePlugin_Solarize()
{
}

QStringList ImagePlugin_Solarize::guiDefinition() const
{
    QStringList guiDef;
    guiDef.append("MenuBar/Menu/Fi&lters/Generic/Action/implugcore_solarize/ ");
    return guiDef;
}

void ImagePlugin_Solarize::slotSolarize()
{
    DigikamSolarizeImagesPlugin::ImageEffect_Solarize dlg(parentWidget());
    dlg.exec();
}


#include "imageplugin_solarize.moc"
