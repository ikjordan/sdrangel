///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <sstream>
#include <iostream>

#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QDateTime>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>

#include "ui_aaroniartsaoutputgui.h"
#include "gui/colormapper.h"
#include "gui/glspectrum.h"
#include "gui/basicdevicesettingsdialog.h"
#include "gui/dialogpositioner.h"
#include "dsp/dspengine.h"
#include "dsp/dspcommands.h"
#include "mainwindow.h"
#include "util/simpleserializer.h"
#include "device/deviceapi.h"
#include "device/deviceuiset.h"
#include "aaroniartsaoutputgui.h"


AaroniaRTSAOutputGui::AaroniaRTSAOutputGui(DeviceUISet *deviceUISet, QWidget* parent) :
	DeviceGUI(parent),
	ui(new Ui::AaroniaRTSAOutputGui),
	m_settings(),
	m_sampleSink(0),
	m_acquisition(false),
	m_streamSampleRate(0),
	m_streamCenterFrequency(0),
	m_lastEngineState(DeviceAPI::StNotStarted),
    m_samplesCount(0),
    m_tickCount(0),
    m_doApplySettings(true),
    m_forceSettings(true)
{
    m_deviceUISet = deviceUISet;
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_paletteGreenText.setColor(QPalette::WindowText, Qt::green);
    m_paletteWhiteText.setColor(QPalette::WindowText, Qt::white);

    ui->setupUi(getContents());
    sizeToContents();
    getContents()->setStyleSheet("#AaroniaRTSAOutputGui { background-color: rgb(64, 64, 64); }");
    m_helpURL = "plugins/samplesink/localoutput/readme.md";

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(openDeviceSettingsDialog(const QPoint &)));

	displaySettings();

	connect(&m_statusTimer, SIGNAL(timeout()), this, SLOT(updateStatus()));
	m_statusTimer.start(500);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateHardware()));

    m_sampleSink = (AaroniaRTSAOutput*) m_deviceUISet->m_deviceAPI->getSampleSink();

	connect(&m_inputMessageQueue, SIGNAL(messageEnqueued()), this, SLOT(handleInputMessages()), Qt::QueuedConnection);
	m_sampleSink->setMessageQueueToGUI(&m_inputMessageQueue);

    m_forceSettings = true;
    sendSettings();
    makeUIConnections();
}

AaroniaRTSAOutputGui::~AaroniaRTSAOutputGui()
{
    m_statusTimer.stop();
    m_updateTimer.stop();
	delete ui;
}

void AaroniaRTSAOutputGui::blockApplySettings(bool block)
{
    m_doApplySettings = !block;
}

void AaroniaRTSAOutputGui::destroy()
{
	delete this;
}

void AaroniaRTSAOutputGui::resetToDefaults()
{
    m_settings.resetToDefaults();
    displaySettings();
    m_forceSettings = true;
    sendSettings();
}

QByteArray AaroniaRTSAOutputGui::serialize() const
{
    return m_settings.serialize();
}

bool AaroniaRTSAOutputGui::deserialize(const QByteArray& data)
{
    qDebug("AaroniaRTSAOutputGui::deserialize");

    if (m_settings.deserialize(data))
    {
        displaySettings();
        m_forceSettings = true;
        sendSettings();

        return true;
    }
    else
    {
        return false;
    }
}

bool AaroniaRTSAOutputGui::handleMessage(const Message& message)
{
    if (AaroniaRTSAOutput::MsgConfigureAaroniaRTSAOutput::match(message))
    {
        const AaroniaRTSAOutput::MsgConfigureAaroniaRTSAOutput& cfg = (AaroniaRTSAOutput::MsgConfigureAaroniaRTSAOutput&) message;

        if (cfg.getForce()) {
            m_settings = cfg.getSettings();
        } else {
            m_settings.applySettings(cfg.getSettingsKeys(), cfg.getSettings());
        }

        blockApplySettings(true);
        displaySettings();
        blockApplySettings(false);
        return true;
    }
	else if (AaroniaRTSAOutput::MsgStartStop::match(message))
    {
	    AaroniaRTSAOutput::MsgStartStop& notif = (AaroniaRTSAOutput::MsgStartStop&) message;
        blockApplySettings(true);
        ui->startStop->setChecked(notif.getStartStop());
        blockApplySettings(false);

        return true;
    }
    else if (AaroniaRTSAOutput::MsgReportSampleRateAndFrequency::match(message))
    {
        AaroniaRTSAOutput::MsgReportSampleRateAndFrequency& notif = (AaroniaRTSAOutput::MsgReportSampleRateAndFrequency&) message;
        m_streamSampleRate = notif.getSampleRate();
        m_streamCenterFrequency = notif.getCenterFrequency();
        updateSampleRateAndFrequency();

        return true;
    }
	else
	{
		return false;
	}
}

void AaroniaRTSAOutputGui::handleInputMessages()
{
    Message* message;

    while ((message = m_inputMessageQueue.pop()) != 0)
    {
        //qDebug("AaroniaRTSAOutputGui::handleInputMessages: message: %s", message->getIdentifier());

        if (DSPSignalNotification::match(*message))
        {
            DSPSignalNotification* notif = (DSPSignalNotification*) message;

            if (notif->getSampleRate() != m_streamSampleRate) {
                m_streamSampleRate = notif->getSampleRate();
            }

            m_streamCenterFrequency = notif->getCenterFrequency();

            qDebug("AaroniaRTSAOutputGui::handleInputMessages: DSPSignalNotification: SampleRate:%d, CenterFrequency:%llu", notif->getSampleRate(), notif->getCenterFrequency());

            updateSampleRateAndFrequency();
            DSPSignalNotification *fwd = new DSPSignalNotification(*notif);
            m_sampleSink->getInputMessageQueue()->push(fwd);

            delete message;
        }
        else
        {
            if (handleMessage(*message)) {
                delete message;
            }
        }
    }
}

void AaroniaRTSAOutputGui::updateSampleRateAndFrequency()
{
    m_deviceUISet->getSpectrum()->setSampleRate(m_streamSampleRate);
    m_deviceUISet->getSpectrum()->setCenterFrequency(m_streamCenterFrequency);
    ui->deviceRateText->setText(tr("%1k").arg((float)m_streamSampleRate / 1000));
    blockApplySettings(true);
    ui->centerFrequency->setText(QString("%L1").arg(m_streamCenterFrequency / 1000));
    blockApplySettings(false);
}

void AaroniaRTSAOutputGui::displaySettings()
{
    blockApplySettings(true);

    ui->centerFrequency->setText(QString("%L1").arg(m_streamCenterFrequency / 1000));
    ui->deviceRateText->setText(tr("%1k").arg(m_streamSampleRate / 1000.0));

	blockApplySettings(false);
}

void AaroniaRTSAOutputGui::sendSettings()
{
    if (!m_updateTimer.isActive()) {
        m_updateTimer.start(100);
    }
}

void AaroniaRTSAOutputGui::on_startStop_toggled(bool checked)
{
    if (m_doApplySettings)
    {
        AaroniaRTSAOutput::MsgStartStop *message = AaroniaRTSAOutput::MsgStartStop::create(checked);
        m_sampleSink->getInputMessageQueue()->push(message);
    }
}

void AaroniaRTSAOutputGui::updateHardware()
{
    if (m_doApplySettings)
    {
        qDebug() << "AaroniaRTSAOutputGui::updateHardware";
        AaroniaRTSAOutput::MsgConfigureAaroniaRTSAOutput* message =
                AaroniaRTSAOutput::MsgConfigureAaroniaRTSAOutput::create(m_settings, m_settingsKeys, m_forceSettings);
        m_sampleSink->getInputMessageQueue()->push(message);
        m_forceSettings = false;
        m_settingsKeys.clear();
        m_updateTimer.stop();
    }
}

void AaroniaRTSAOutputGui::updateStatus()
{
    int state = m_deviceUISet->m_deviceAPI->state();

    if(m_lastEngineState != state)
    {
        switch(state)
        {
            case DeviceAPI::StNotStarted:
                ui->startStop->setStyleSheet("QToolButton { background:rgb(79,79,79); }");
                break;
            case DeviceAPI::StIdle:
                ui->startStop->setStyleSheet("QToolButton { background-color : blue; }");
                break;
            case DeviceAPI::StRunning:
                ui->startStop->setStyleSheet("QToolButton { background-color : green; }");
                break;
            case DeviceAPI::StError:
                ui->startStop->setStyleSheet("QToolButton { background-color : red; }");
                QMessageBox::information(this, tr("Message"), m_deviceUISet->m_deviceAPI->errorMessage());
                break;
            default:
                break;
        }

        m_lastEngineState = state;
    }
}

void AaroniaRTSAOutputGui::openDeviceSettingsDialog(const QPoint& p)
{
    if (m_contextMenuType == ContextMenuDeviceSettings)
    {
        BasicDeviceSettingsDialog dialog(this);
        dialog.setUseReverseAPI(m_settings.m_useReverseAPI);
        dialog.setReverseAPIAddress(m_settings.m_reverseAPIAddress);
        dialog.setReverseAPIPort(m_settings.m_reverseAPIPort);
        dialog.setReverseAPIDeviceIndex(m_settings.m_reverseAPIDeviceIndex);

        dialog.move(p);
        new DialogPositioner(&dialog, false);
        dialog.exec();

        m_settings.m_useReverseAPI = dialog.useReverseAPI();
        m_settings.m_reverseAPIAddress = dialog.getReverseAPIAddress();
        m_settings.m_reverseAPIPort = dialog.getReverseAPIPort();
        m_settings.m_reverseAPIDeviceIndex = dialog.getReverseAPIDeviceIndex();
        m_settingsKeys.append("useReverseAPI");
        m_settingsKeys.append("reverseAPIAddress");
        m_settingsKeys.append("reverseAPIPort");
        m_settingsKeys.append("reverseAPIDeviceIndex");

        sendSettings();
    }

    resetContextMenuType();
}

void AaroniaRTSAOutputGui::makeUIConnections()
{
    QObject::connect(ui->startStop, &ButtonSwitch::toggled, this, &AaroniaRTSAOutputGui::on_startStop_toggled);
}
