///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2024 Jon Beniston, M7RCE                                        //
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

#ifndef INCLUDE_SOLARDYNAMICSOBSERVATORY_H
#define INCLUDE_SOLARDYNAMICSOBSERVATORY_H

#include <QtCore>
#include <QTimer>
#include <QImage>

#include "export.h"

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkDiskCache;

// This gets solar imagery from SDO (Solar Dynamics Observatory) - https://sdo.gsfc.nasa.gov/
// and LASCO images from SOHO - https://soho.nascom.nasa.gov/
class SDRBASE_API SolarDynamicsObservatory : public QObject
{
    Q_OBJECT
protected:
    SolarDynamicsObservatory();

public:

    static SolarDynamicsObservatory* create();

    ~SolarDynamicsObservatory();
    void getImagePeriodically(const QString& image, int size=512, int periodInMins=15);
    void getImage(const QString& m_image, int size);
    void getImage(const QString& m_image, QDateTime dateTime, int size=512);

    static QString getImageURL(const QString& image, int size);
    static QString getVideoURL(const QString& video, int size=512);

    static QList<int> getImageSizes();
    static const QStringList getChannelNames();
    static const QStringList getImageNames();
    static QList<int> getVideoSizes();
    static const QStringList getVideoNames();

private slots:
    void getImage();
    void handleReply(QNetworkReply* reply);

signals:
    void imageUpdated(const QImage& image);  // Called when new image is available.

private:

    QTimer m_dataTimer;             // Timer for periodic updates
    QNetworkAccessManager *m_networkManager;
    QNetworkDiskCache *m_cache;

    QString m_image;
    int m_size;
    QDateTime m_dateTime;

    void handleJpeg(const QByteArray& bytes);
    void handleIndex(const QByteArray& bytes);
    static const QStringList getImageFileNames();
    static const QStringList getVideoFileNames();

};

#endif /* INCLUDE_SOLARDYNAMICSOBSERVATORY_H */
