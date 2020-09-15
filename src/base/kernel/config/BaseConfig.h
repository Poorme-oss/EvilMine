/* evildragon
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018-2020 SChernykh   <https://github.com/SChernykh>
 * Copyright 2016-2020 evildragon       <https://github.com/evildragon>, <support@evildragon.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef evildragon_BASECONFIG_H
#define evildragon_BASECONFIG_H


#include "base/kernel/config/Title.h"
#include "base/kernel/interfaces/IConfig.h"
#include "base/net/http/Http.h"
#include "base/net/stratum/Pools.h"


#ifdef evildragon_FEATURE_TLS
#   include "base/net/tls/TlsConfig.h"
#endif


namespace evildragon {


class IJsonReader;


class BaseConfig : public IConfig
{
public:
    static const char *kApi;
    static const char *kApiId;
    static const char *kApiWorkerId;
    static const char *kAutosave;
    static const char *kBackground;
    static const char *kColors;
    static const char *kDryRun;
    static const char *kHttp;
    static const char *kLogFile;
    static const char *kPauseOnBattery;
    static const char *kPrintTime;
    static const char *kSyslog;
    static const char *kTitle;
    static const char *kUserAgent;
    static const char *kVerbose;
    static const char *kWatch;

#   ifdef evildragon_FEATURE_TLS
    static const char *kTls;
#   endif

    BaseConfig() = default;

    inline bool isAutoSave() const                          { return m_autoSave; }
    inline bool isBackground() const                        { return m_background; }
    inline bool isDryRun() const                            { return m_dryRun; }
    inline bool isPauseOnBattery() const                    { return m_pauseOnBattery; }
    inline bool isSyslog() const                            { return m_syslog; }
    inline const char *logFile() const                      { return m_logFile.data(); }
    inline const char *userAgent() const                    { return m_userAgent.data(); }
    inline const Http &http() const                         { return m_http; }
    inline const Pools &pools() const                       { return m_pools; }
    inline const String &apiId() const                      { return m_apiId; }
    inline const String &apiWorkerId() const                { return m_apiWorkerId; }
    inline const Title &title() const                       { return m_title; }
    inline uint32_t printTime() const                       { return m_printTime; }

#   ifdef evildragon_FEATURE_TLS
    inline const TlsConfig &tls() const                     { return m_tls; }
#   endif

    inline bool isWatch() const override                    { return m_watch && !m_fileName.isNull(); }
    inline const String &fileName() const override          { return m_fileName; }
    inline void setFileName(const char *fileName) override  { m_fileName = fileName; }

    bool read(const IJsonReader &reader, const char *fileName) override;
    bool save() override;

    void printVersions();

protected:
    bool m_autoSave         = true;
    bool m_background       = false;
    bool m_dryRun           = false;
    bool m_pauseOnBattery   = false;
    bool m_syslog           = false;
    bool m_upgrade          = false;
    bool m_watch            = true;
    Http m_http;
    Pools m_pools;
    String m_apiId;
    String m_apiWorkerId;
    String m_fileName;
    String m_logFile;
    String m_userAgent;
    Title m_title;
    uint32_t m_printTime = 60;

#   ifdef evildragon_FEATURE_TLS
    TlsConfig m_tls;
#   endif

private:
    void setVerbose(const rapidjson::Value &value);
};


} // namespace evildragon


#endif /* evildragon_BASECONFIG_H */
