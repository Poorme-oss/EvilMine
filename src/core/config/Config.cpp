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

#include <algorithm>
#include <cstring>
#include <uv.h>
#include <cinttypes>


#include "core/config/Config.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/cpu/Cpu.h"
#include "base/io/log/Log.h"
#include "base/kernel/interfaces/IJsonReader.h"
#include "crypto/common/Assembly.h"


#ifdef evildragon_ALGO_RANDOMX
#   include "crypto/rx/RxConfig.h"
#endif


#ifdef evildragon_FEATURE_OPENCL
#   include "backend/opencl/OclConfig.h"
#endif


#ifdef evildragon_FEATURE_CUDA
#   include "backend/cuda/CudaConfig.h"
#endif


namespace evildragon {

static const char *kCPU     = "cpu";

#ifdef evildragon_ALGO_RANDOMX
static const char *kRandomX = "randomx";
#endif

#ifdef evildragon_FEATURE_OPENCL
static const char *kOcl     = "opencl";
#endif

#ifdef evildragon_FEATURE_CUDA
static const char *kCuda    = "cuda";
#endif


#if defined(evildragon_FEATURE_NVML) || defined (evildragon_FEATURE_ADL)
static const char *kHealthPrintTime = "health-print-time";
#endif


class ConfigPrivate
{
public:
    CpuConfig cpu;

#   ifdef evildragon_ALGO_RANDOMX
    RxConfig rx;
#   endif

#   ifdef evildragon_FEATURE_OPENCL
    OclConfig cl;
#   endif

#   ifdef evildragon_FEATURE_CUDA
    CudaConfig cuda;
#   endif

#   if defined(evildragon_FEATURE_NVML) || defined (evildragon_FEATURE_ADL)
    uint32_t healthPrintTime = 60;
#   endif
};

}


evildragon::Config::Config() :
    d_ptr(new ConfigPrivate())
{
}


evildragon::Config::~Config()
{
    delete d_ptr;
}


const evildragon::CpuConfig &evildragon::Config::cpu() const
{
    return d_ptr->cpu;
}


#ifdef evildragon_FEATURE_OPENCL
const evildragon::OclConfig &evildragon::Config::cl() const
{
    return d_ptr->cl;
}
#endif


#ifdef evildragon_FEATURE_CUDA
const evildragon::CudaConfig &evildragon::Config::cuda() const
{
    return d_ptr->cuda;
}
#endif


#ifdef evildragon_ALGO_RANDOMX
const evildragon::RxConfig &evildragon::Config::rx() const
{
    return d_ptr->rx;
}
#endif


#if defined(evildragon_FEATURE_NVML) || defined (evildragon_FEATURE_ADL)
uint32_t evildragon::Config::healthPrintTime() const
{
    return d_ptr->healthPrintTime;
}
#endif


bool evildragon::Config::isShouldSave() const
{
    if (!isAutoSave()) {
        return false;
    }

#   ifdef evildragon_FEATURE_OPENCL
    if (cl().isShouldSave()) {
        return true;
    }
#   endif

#   ifdef evildragon_FEATURE_CUDA
    if (cuda().isShouldSave()) {
        return true;
    }
#   endif

    return (m_upgrade || cpu().isShouldSave());
}


bool evildragon::Config::read(const IJsonReader &reader, const char *fileName)
{
    if (!BaseConfig::read(reader, fileName)) {
        return false;
    }

    d_ptr->cpu.read(reader.getValue(kCPU));

#   ifdef evildragon_ALGO_RANDOMX
    if (!d_ptr->rx.read(reader.getValue(kRandomX))) {
        m_upgrade = true;
    }
#   endif

#   ifdef evildragon_FEATURE_OPENCL
    d_ptr->cl.read(reader.getValue(kOcl));
#   endif

#   ifdef evildragon_FEATURE_CUDA
    d_ptr->cuda.read(reader.getValue(kCuda));
#   endif

#   if defined(evildragon_FEATURE_NVML) || defined (evildragon_FEATURE_ADL)
    d_ptr->healthPrintTime = reader.getUint(kHealthPrintTime, d_ptr->healthPrintTime);
#   endif

    return true;
}


void evildragon::Config::getJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;

    doc.SetObject();

    auto &allocator = doc.GetAllocator();

    Value api(kObjectType);
    api.AddMember(StringRef(kApiId),                    m_apiId.toJSON(), allocator);
    api.AddMember(StringRef(kApiWorkerId),              m_apiWorkerId.toJSON(), allocator);

    doc.AddMember(StringRef(kApi),                      api, allocator);
    doc.AddMember(StringRef(kHttp),                     m_http.toJSON(doc), allocator);
    doc.AddMember(StringRef(kAutosave),                 isAutoSave(), allocator);
    doc.AddMember(StringRef(kBackground),               isBackground(), allocator);
    doc.AddMember(StringRef(kColors),                   Log::isColors(), allocator);
    doc.AddMember(StringRef(kTitle),                    title().toJSON(), allocator);

#   ifdef evildragon_ALGO_RANDOMX
    doc.AddMember(StringRef(kRandomX),                  rx().toJSON(doc), allocator);
#   endif

    doc.AddMember(StringRef(kCPU),                      cpu().toJSON(doc), allocator);

#   ifdef evildragon_FEATURE_OPENCL
    doc.AddMember(StringRef(kOcl),                      cl().toJSON(doc), allocator);
#   endif

#   ifdef evildragon_FEATURE_CUDA
    doc.AddMember(StringRef(kCuda),                     cuda().toJSON(doc), allocator);
#   endif

    doc.AddMember(StringRef(Pools::kDonateLevel),       m_pools.donateLevel(), allocator);
    doc.AddMember(StringRef(Pools::kDonateOverProxy),   m_pools.proxyDonate(), allocator);
    doc.AddMember(StringRef(kLogFile),                  m_logFile.toJSON(), allocator);
    doc.AddMember(StringRef(Pools::kPools),             m_pools.toJSON(doc), allocator);
    doc.AddMember(StringRef(kPrintTime),                printTime(), allocator);
#   if defined(evildragon_FEATURE_NVML) || defined (evildragon_FEATURE_ADL)
    doc.AddMember(StringRef(kHealthPrintTime),          healthPrintTime(), allocator);
#   endif
    doc.AddMember(StringRef(Pools::kRetries),           m_pools.retries(), allocator);
    doc.AddMember(StringRef(Pools::kRetryPause),        m_pools.retryPause(), allocator);
    doc.AddMember(StringRef(kSyslog),                   isSyslog(), allocator);

#   ifdef evildragon_FEATURE_TLS
    doc.AddMember(StringRef(kTls),                      m_tls.toJSON(doc), allocator);
#   endif

    doc.AddMember(StringRef(kUserAgent),                m_userAgent.toJSON(), allocator);
    doc.AddMember(StringRef(kVerbose),                  Log::verbose(), allocator);
    doc.AddMember(StringRef(kWatch),                    m_watch, allocator);
    doc.AddMember(StringRef(kPauseOnBattery),           isPauseOnBattery(), allocator);
}
