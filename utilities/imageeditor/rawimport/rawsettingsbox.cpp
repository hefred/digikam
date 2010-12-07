/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-08-11
 * Description : Raw import settings box
 *
 * Copyright (C) 2008-2010 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "rawsettingsbox.moc"

// Qt includes

#include <QButtonGroup>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QToolBox>
#include <QToolButton>

// KDE includes

#include <kapplication.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <ktabwidget.h>
#include <kvbox.h>
#include <kdebug.h>

// LibKDcraw includes

#include <libkdcraw/dcrawsettingswidget.h>
#include <libkdcraw/rnuminput.h>
#include <libkdcraw/version.h>
#include <libkdcraw/rexpanderbox.h>

// Local includes

#include "colorgradientwidget.h"
#include "curveswidget.h"
#include "histogrambox.h"
#include "histogramwidget.h"
#include "iccpreviewwidget.h"
#include "imagecurves.h"
#include "imagedialog.h"
#include "imagehistogram.h"
#include "globals.h"

using namespace KDcrawIface;

namespace Digikam
{

class RawSettingsBox::RawSettingsBoxPriv
{
public:

    enum AllColorsColorType
    {
        AllColorsRed = 0,
        AllColorsGreen,
        AllColorsBlue
    };

public:

    RawSettingsBoxPriv() :
        optionGroupName("RAW Import Settings"),
        optionHistogramChannelEntry("Histogram Channel"),
        optionHistogramScaleEntry("Histogram Scale"),
        optionBrightnessEntry("Brightness"),
        optionContrastEntry("Contrast"),
        optionGammaEntry("Gamma"),
        optionSaturationEntry("Saturation"),
        optionFineExposureEntry("FineExposure"),
        optionCurvePrefix("RawCurve"),
        optionSettingsPageEntry("Settings Page"),
        optionDecodingSettingsTabEntry("Decoding Settings Tab")
    {
        infoBox                = 0;
        advExposureBox         = 0;
        gammaLabel             = 0;
        gammaInput             = 0;
        saturationLabel        = 0;
        saturationInput        = 0;
        fineExposureLabel      = 0;
        fineExposureInput      = 0;
        contrastInput          = 0;
        contrastLabel          = 0;
        curveBox               = 0;
        curveWidget            = 0;
        resetCurveBtn          = 0;
        decodingSettingsBox    = 0;
        postProcessSettingsBox = 0;
        tabView                = 0;
        abortBtn               = 0;
        updateBtn              = 0;
        rawdecodingBox         = 0;
        brightnessLabel        = 0;
        brightnessInput        = 0;
    }

    const QString optionGroupName;
    const QString optionHistogramChannelEntry;
    const QString optionHistogramScaleEntry;
    const QString optionBrightnessEntry;
    const QString optionContrastEntry;
    const QString optionGammaEntry;
    const QString optionSaturationEntry;
    const QString optionFineExposureEntry;
    const QString optionCurvePrefix;
    const QString optionSettingsPageEntry;
    const QString optionDecodingSettingsTabEntry;

    QWidget*             advExposureBox;
    QWidget*             curveBox;
    QWidget*             rawdecodingBox;

    QLabel*              brightnessLabel;
    QLabel*              contrastLabel;
    QLabel*              gammaLabel;
    QLabel*              saturationLabel;
    QLabel*              fineExposureLabel;

    QPushButton*         abortBtn;
    QPushButton*         updateBtn;

    QToolButton*         resetCurveBtn;

    KTabWidget*          tabView;

    CurvesWidget*        curveWidget;

    ImageDialogPreview*  infoBox;

    RExpanderBox*        postProcessSettingsBox;

    RIntNumInput*        contrastInput;
    RIntNumInput*        brightnessInput;

    RDoubleNumInput*     gammaInput;
    RDoubleNumInput*     saturationInput;
    RDoubleNumInput*     fineExposureInput;

    DcrawSettingsWidget* decodingSettingsBox;
};

RawSettingsBox::RawSettingsBox(const KUrl& url, QWidget* parent)
    : EditorToolSettings(parent), d(new RawSettingsBoxPriv)
{
    setButtons(Default|Ok|Cancel);
    setTools(Histogram);
    setHistogramType(LRGBC);

    QGridLayout* gridSettings = new QGridLayout(plainPage());
    d->tabView                = new KTabWidget(plainPage());

    // - RAW Decoding view --------------------------------------------------------------

    d->rawdecodingBox      = new QWidget(d->tabView);
    QGridLayout* rawGrid   = new QGridLayout(d->rawdecodingBox);
    d->decodingSettingsBox = new DcrawSettingsWidget(d->rawdecodingBox,
            DcrawSettingsWidget::SIXTEENBITS | DcrawSettingsWidget::COLORSPACE);
    d->decodingSettingsBox->setObjectName("RawSettingsBox Expander");

    KFileDialog* inputDlg  = d->decodingSettingsBox->inputProfileUrlEdit()->fileDialog();
    inputDlg->setPreviewWidget(new ICCPreviewWidget(inputDlg));

    KFileDialog* outputDlg = d->decodingSettingsBox->outputProfileUrlEdit()->fileDialog();
    outputDlg->setPreviewWidget(new ICCPreviewWidget(outputDlg));

    d->abortBtn = new QPushButton(d->rawdecodingBox);
    d->abortBtn->setText(i18n("Abort"));
    d->abortBtn->setIcon(SmallIcon("dialog-cancel"));
    d->abortBtn->setEnabled(false);
    d->abortBtn->setToolTip(i18n("Abort the current Raw image preview."));

    d->updateBtn = new QPushButton(d->rawdecodingBox);
    d->updateBtn->setText(i18n("Update"));
    d->updateBtn->setIcon(SmallIcon("view-refresh"));
    d->updateBtn->setEnabled(false);
    d->updateBtn->setToolTip(i18n("Generate a Raw image preview using current settings."));

    rawGrid->addWidget(d->decodingSettingsBox, 0, 0, 1, 3);
    rawGrid->addWidget(d->abortBtn,            1, 0, 1, 1);
    rawGrid->addWidget(d->updateBtn,           1, 2, 1, 1);
    rawGrid->setColumnStretch(1, 10);
    rawGrid->setSpacing(spacingHint());
    rawGrid->setMargin(spacingHint());

    // - Post-processing view --------------------------------------------------------------

    d->postProcessSettingsBox = new RExpanderBox(d->tabView);
    d->postProcessSettingsBox->setObjectName("PostProcessingSettingsBox Expander");

    d->advExposureBox              = new QWidget(d->postProcessSettingsBox);
    QGridLayout* advExposureLayout = new QGridLayout(d->advExposureBox);

    d->brightnessLabel = new QLabel(i18n("Brightness:"), d->advExposureBox);
    d->brightnessInput = new RIntNumInput(d->advExposureBox);
    d->brightnessInput->setRange(-100, 100, 1);
    d->brightnessInput->setDefaultValue(0);
    d->brightnessInput->setSliderEnabled(true);
    d->brightnessInput->input()->setWhatsThis(i18n("Set here the brightness adjustment of the image."));

    d->contrastLabel = new QLabel(i18n("Contrast:"), d->advExposureBox);
    d->contrastInput = new RIntNumInput(d->advExposureBox);
    d->contrastInput->setRange(-100, 100, 1);
    d->contrastInput->setDefaultValue(0);
    d->contrastInput->setSliderEnabled(true);
    d->contrastInput->input()->setWhatsThis(i18n("Set here the contrast adjustment of the image."));

    d->gammaLabel = new QLabel(i18n("Gamma:"), d->advExposureBox);
    d->gammaInput = new RDoubleNumInput(d->advExposureBox);
    d->gammaInput->setDecimals(2);
    d->gammaInput->setRange(0.1, 3.0, 0.01);
    d->gammaInput->setDefaultValue(1.0);
    d->gammaInput->input()->setWhatsThis(i18n("Set here the gamma adjustment of the image"));

    d->saturationLabel = new QLabel(i18n("Saturation:"), d->advExposureBox);
    d->saturationInput = new RDoubleNumInput(d->advExposureBox);
    d->saturationInput->setDecimals(2);
    d->saturationInput->setRange(0.0, 2.0, 0.01);
    d->saturationInput->setDefaultValue(1.0);
    d->saturationInput->input()->setWhatsThis(i18n("Set here the color saturation correction."));

    d->fineExposureLabel = new QLabel(i18n("Exposure (E.V):"), d->advExposureBox);
    d->fineExposureInput = new RDoubleNumInput(d->advExposureBox);
    d->fineExposureInput->setDecimals(2);
    d->fineExposureInput->setRange(-3.0, 3.0, 0.1);
    d->fineExposureInput->setDefaultValue(0.0);
    d->fineExposureInput->input()->setWhatsThis(i18n("This value in E.V will be used to perform "
            "an exposure compensation of the image."));

    advExposureLayout->addWidget(d->brightnessLabel,   0, 0, 1, 1);
    advExposureLayout->addWidget(d->brightnessInput,   0, 1, 1, 2);
    advExposureLayout->addWidget(d->contrastLabel,     1, 0, 1, 1);
    advExposureLayout->addWidget(d->contrastInput,     1, 1, 1, 2);
    advExposureLayout->addWidget(d->gammaLabel,        2, 0, 1, 1);
    advExposureLayout->addWidget(d->gammaInput,        2, 1, 1, 2);
    advExposureLayout->addWidget(d->saturationLabel,   3, 0, 1, 1);
    advExposureLayout->addWidget(d->saturationInput,   3, 1, 1, 2);
    advExposureLayout->addWidget(d->fineExposureLabel, 4, 0, 1, 1);
    advExposureLayout->addWidget(d->fineExposureInput, 4, 1, 1, 2);
    advExposureLayout->setRowStretch(5, 10);
    advExposureLayout->setSpacing(0);
    advExposureLayout->setMargin(spacingHint());

    // ---------------------------------------------------------------

    d->curveBox              = new QWidget(d->postProcessSettingsBox);
    QGridLayout* curveLayout = new QGridLayout(d->curveBox);

    ColorGradientWidget* vGradient = new ColorGradientWidget(Qt::Vertical, 10, d->curveBox);
    vGradient->setColors( QColor( "white" ), QColor( "black" ) );

    QLabel* spacev = new QLabel(d->curveBox);
    spacev->setFixedWidth(1);

    d->curveWidget = new CurvesWidget(256, 192, d->curveBox);
    d->curveWidget->setWhatsThis(i18n("This is the curve adjustment of the image luminosity"));

    d->resetCurveBtn = new QToolButton(d->curveBox);
    d->resetCurveBtn->setFixedSize(11, 11);
    d->resetCurveBtn->setIcon(SmallIcon("document-revert", 8));
    d->resetCurveBtn->setFocusPolicy(Qt::NoFocus);
    d->resetCurveBtn->setAutoRaise(true);
    d->resetCurveBtn->setToolTip(i18n("Reset curve to linear"));

    QLabel* spaceh = new QLabel(d->curveBox);
    spaceh->setFixedHeight(1);

    ColorGradientWidget* hGradient = new ColorGradientWidget(Qt::Horizontal, 10, d->curveBox);
    hGradient->setColors( QColor( "black" ), QColor( "white" ) );

    curveLayout->addWidget(vGradient,        0, 0, 1, 1);
    curveLayout->addWidget(spacev,           0, 1, 1, 1);
    curveLayout->addWidget(d->curveWidget,   0, 2, 1, 1);
    curveLayout->addWidget(spaceh,           1, 2, 1, 1);
    curveLayout->addWidget(d->resetCurveBtn, 1, 0, 2, 2);
    curveLayout->addWidget(hGradient,        2, 2, 1, 1);
    curveLayout->setRowStretch(3, 10);
    curveLayout->setSpacing(0);
    curveLayout->setMargin(spacingHint());

    // ---------------------------------------------------------------

    d->postProcessSettingsBox->addItem(d->advExposureBox, i18n("Exposure"), QString("exposure"), true);
    d->postProcessSettingsBox->addItem(d->curveBox,       i18n("Luminosity Curve"), QString("luminositycurve"), false);
    d->postProcessSettingsBox->setItemIcon(0, SmallIcon("contrast"));
    d->postProcessSettingsBox->setItemIcon(1, SmallIcon("adjustcurves"));
    d->postProcessSettingsBox->addStretch();

    // - Image info view --------------------------------------------------------------

    d->infoBox = new ImageDialogPreview(d->postProcessSettingsBox);
    d->infoBox->showPreview(url);

    // ---------------------------------------------------------------

    d->decodingSettingsBox->setItemIcon(DcrawSettingsWidget::DEMOSAICING,     SmallIcon("kdcraw"));
    d->decodingSettingsBox->setItemIcon(DcrawSettingsWidget::WHITEBALANCE,    SmallIcon("whitebalance"));
    d->decodingSettingsBox->setItemIcon(DcrawSettingsWidget::CORRECTIONS,     SmallIcon("lensdistortion"));
    d->decodingSettingsBox->setItemIcon(DcrawSettingsWidget::COLORMANAGEMENT, SmallIcon("colormanagement"));
    d->decodingSettingsBox->updateMinimumWidth();

    d->tabView->insertTab(0, d->rawdecodingBox,         i18n("Raw Decoding"));
    d->tabView->insertTab(1, d->postProcessSettingsBox, i18n("Post Processing"));
    d->tabView->insertTab(2, d->infoBox,                i18n("Info"));

    // ---------------------------------------------------------------

    button(Default)->setText(i18n("Reset"));
    button(Default)->setIcon(KIcon(SmallIcon("document-revert")));
    button(Default)->setToolTip(i18n("Reset all settings to default values."));

    button(Ok)->setText(i18n("Import"));
    button(Ok)->setIcon(KIcon(SmallIcon("dialog-ok")));
    button(Ok)->setToolTip(i18n("Import image to editor using current settings."));

    button(Cancel)->setText(i18n("Use Default"));
    button(Cancel)->setIcon(KIcon(SmallIcon("go-home")));
    button(Cancel)->setToolTip(i18n("Use general Raw decoding settings to load this image in editor."));

    // ---------------------------------------------------------------

    gridSettings->addWidget(d->tabView, 0, 0, 1, 5);
    gridSettings->setColumnStretch(2, 10);
    gridSettings->setSpacing(spacingHint());
    gridSettings->setMargin(0);

    // ---------------------------------------------------------------

    connect(d->resetCurveBtn, SIGNAL(clicked()),
            this, SLOT(slotResetCurve()));

    connect(d->updateBtn, SIGNAL(clicked()),
            this, SIGNAL(signalUpdatePreview()));

    connect(d->abortBtn, SIGNAL(clicked()),
            this, SIGNAL(signalAbortPreview()));

    connect(d->decodingSettingsBox, SIGNAL(signalSettingsChanged()),
            this, SLOT(slotDemosaicingChanged()));

    connect(d->curveWidget, SIGNAL(signalCurvesChanged()),
            this, SIGNAL(signalPostProcessingChanged()));

    connect(d->brightnessInput, SIGNAL(valueChanged(int)),
            this, SIGNAL(signalPostProcessingChanged()));

    connect(d->contrastInput, SIGNAL(valueChanged(int)),
            this, SIGNAL(signalPostProcessingChanged()));

    connect(d->gammaInput, SIGNAL(valueChanged(double)),
            this, SIGNAL(signalPostProcessingChanged()));

    connect(d->saturationInput, SIGNAL(valueChanged(double)),
            this, SIGNAL(signalPostProcessingChanged()));

    connect(d->fineExposureInput, SIGNAL(valueChanged(double)),
            this, SIGNAL(signalPostProcessingChanged()));
}

RawSettingsBox::~RawSettingsBox()
{
    delete d->curveWidget;
    delete d;
}

void RawSettingsBox::slotDemosaicingChanged()
{
    enableUpdateBtn(true);
}

void RawSettingsBox::enableUpdateBtn(bool b)
{
    d->updateBtn->setEnabled(b);
}

bool RawSettingsBox::updateBtnEnabled()
{
    return d->updateBtn->isEnabled();
}

void RawSettingsBox::setBusy(bool b)
{
    d->decodingSettingsBox->setEnabled(!b);
    d->abortBtn->setEnabled(b);
}

void RawSettingsBox::setDemosaicedImage(DImg& img)
{
    d->curveWidget->stopHistogramComputation();
    d->curveWidget->updateData(img.bits(), img.width(), img.height(), img.sixteenBit());
}

void RawSettingsBox::setPostProcessedImage(DImg& img)
{
    histogramBox()->histogram()->stopHistogramComputation();
    histogramBox()->histogram()->updateData(img.bits(), img.width(), img.height(), img.sixteenBit());
}

void RawSettingsBox::resetSettings()
{
    d->decodingSettingsBox->resetToDefault();
    d->brightnessInput->slotReset();
    d->contrastInput->slotReset();
    d->gammaInput->slotReset();
    d->saturationInput->slotReset();
    d->fineExposureInput->slotReset();
    slotResetCurve();
}

void RawSettingsBox::slotResetCurve()
{
    d->curveWidget->reset();
    emit signalPostProcessingChanged();
}

CurvesWidget* RawSettingsBox::curvesWidget() const
{
    return d->curveWidget;
}

void RawSettingsBox::readSettings()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(d->optionGroupName);

    histogramBox()->setChannel((ChannelType)group.readEntry(d->optionHistogramChannelEntry,
                               (int) LuminosityChannel));
    histogramBox()->setScale((HistogramScale)group.readEntry(d->optionHistogramScaleEntry,
                             (int) LogScaleHistogram));
    curvesWidget()->setScaleType((HistogramScale)group.readEntry(d->optionHistogramScaleEntry,
                                 (int) LogScaleHistogram));

    d->decodingSettingsBox->readSettings(group);

    d->brightnessInput->setValue(group.readEntry(d->optionBrightnessEntry, 0));
    d->contrastInput->setValue(group.readEntry(d->optionContrastEntry, 0));
    d->gammaInput->setValue(group.readEntry(d->optionGammaEntry, 1.0));
    d->saturationInput->setValue(group.readEntry(d->optionSaturationEntry, 1.0));
    d->fineExposureInput->setValue(group.readEntry(d->optionFineExposureEntry, 0.0));

    d->curveWidget->restoreCurve(group, d->optionCurvePrefix);

    d->tabView->setCurrentIndex(group.readEntry(d->optionSettingsPageEntry, 0));
    d->postProcessSettingsBox->readSettings();
}

void RawSettingsBox::writeSettings()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(d->optionGroupName);

    group.writeEntry(d->optionHistogramChannelEntry, (int)histogramBox()->channel());
    group.writeEntry(d->optionHistogramScaleEntry,   (int)histogramBox()->scale());

    d->decodingSettingsBox->writeSettings(group);

    group.writeEntry(d->optionBrightnessEntry,   d->brightnessInput->value());
    group.writeEntry(d->optionContrastEntry,     d->contrastInput->value());
    group.writeEntry(d->optionGammaEntry,        d->gammaInput->value());
    group.writeEntry(d->optionSaturationEntry,   d->saturationInput->value());
    group.writeEntry(d->optionFineExposureEntry, d->fineExposureInput->value());

    d->curveWidget->saveCurve(group, d->optionCurvePrefix);

    group.writeEntry(d->optionSettingsPageEntry, d->tabView->currentIndex());

    group.sync();
}

DRawDecoding RawSettingsBox::settings() const
{
    DRawDecoding settings(d->decodingSettingsBox->settings());

    settings.lightness    = (double)d->brightnessInput->value()/250.0;
    settings.contrast     = (double)(d->contrastInput->value()/100.0) + 1.00;
    settings.gamma        = d->gammaInput->value();
    settings.saturation   = d->saturationInput->value();
    settings.exposureComp = d->fineExposureInput->value();

    if (d->curveWidget->curves()->isDirty())
    {
        settings.curveAdjust = d->curveWidget->curves()->getCurvePoints(LuminosityChannel);
    }

    return settings;
}

} // namespace Digikam
