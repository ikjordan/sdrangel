/**
 * SDRangel
 * This is the web REST/JSON API of SDRangel SDR software. SDRangel is an Open Source Qt5/OpenGL 3.0+ (4.3+ in Windows) GUI and server Software Defined Radio and signal analyzer in software. It supports Airspy, BladeRF, HackRF, LimeSDR, PlutoSDR, RTL-SDR, SDRplay RSP1 and FunCube    ---   Limitations and specifcities:    * In SDRangel GUI the first Rx device set cannot be deleted. Conversely the server starts with no device sets and its number of device sets can be reduced to zero by as many calls as necessary to /sdrangel/deviceset with DELETE method.   * Preset import and export from/to file is a server only feature.   * Device set focus is a GUI only feature.   * The following channels are not implemented (status 501 is returned): ATV and DATV demodulators, Channel Analyzer NG, LoRa demodulator   * The device settings and report structures contains only the sub-structure corresponding to the device type. The DeviceSettings and DeviceReport structures documented here shows all of them but only one will be or should be present at a time   * The channel settings and report structures contains only the sub-structure corresponding to the channel type. The ChannelSettings and ChannelReport structures documented here shows all of them but only one will be or should be present at a time    --- 
 *
 * OpenAPI spec version: 7.0.0
 * Contact: f4exb06@gmail.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

/*
 * SWGADSBDemodAircraftState.h
 *
 * 
 */

#ifndef SWGADSBDemodAircraftState_H_
#define SWGADSBDemodAircraftState_H_

#include <QJsonObject>


#include <QString>

#include "SWGObject.h"
#include "export.h"

namespace SWGSDRangel {

class SWG_API SWGADSBDemodAircraftState: public SWGObject {
public:
    SWGADSBDemodAircraftState();
    SWGADSBDemodAircraftState(QString* json);
    virtual ~SWGADSBDemodAircraftState();
    void init();
    void cleanup();

    virtual QString asJson () override;
    virtual QJsonObject* asJsonObject() override;
    virtual void fromJsonObject(QJsonObject &json) override;
    virtual SWGADSBDemodAircraftState* fromJson(QString &jsonString) override;

    QString* getIcao();
    void setIcao(QString* icao);

    QString* getCallsign();
    void setCallsign(QString* callsign);

    float getLatitude();
    void setLatitude(float latitude);

    float getLongitude();
    void setLongitude(float longitude);

    qint32 getAltitude();
    void setAltitude(qint32 altitude);

    qint32 getGroundSpeed();
    void setGroundSpeed(qint32 ground_speed);


    virtual bool isSet() override;

private:
    QString* icao;
    bool m_icao_isSet;

    QString* callsign;
    bool m_callsign_isSet;

    float latitude;
    bool m_latitude_isSet;

    float longitude;
    bool m_longitude_isSet;

    qint32 altitude;
    bool m_altitude_isSet;

    qint32 ground_speed;
    bool m_ground_speed_isSet;

};

}

#endif /* SWGADSBDemodAircraftState_H_ */
