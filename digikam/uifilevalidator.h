/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-11-02
 * Description : validate / fix ui files
 *
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
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

#ifndef UIFILEVALIDATOR_H
#define UIFILEVALIDATOR_H

// Qt includes

#include <QXmlDefaultHandler>

class QString;
class QFile;

namespace Digikam
{

class ToolbarHandler : public QXmlDefaultHandler
{
public:

    virtual bool startElement(const QString & namespaceURI, const QString & localName, const QString & qName,
                              const QXmlAttributes & atts);
};

// --------------------------------------------------------

class UiFileValidatorPriv;

class UiFileValidator
{
public:

    UiFileValidator(const QString& filename);
    ~UiFileValidator();

    bool isValid() const;
    bool fixConfigFile();

private:

    bool isReadable(QFile& file) const;
    bool isWritable(QFile& file) const;

    UiFileValidatorPriv* const d;
};

}  // namespace Digikam

#endif // UIFILEVALIDATOR_H
