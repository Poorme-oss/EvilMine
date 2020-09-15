/* evildragon
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2019 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
 * Copyright 2018-2019 tevador     <tevador@gmail.com>
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


#include "crypto/rx/Rx.h"
#include "backend/common/Tags.h"
#include "backend/cpu/CpuConfig.h"
#include "backend/cpu/CpuThreads.h"
#include "base/io/log/Log.h"
#include "crypto/rx/RxConfig.h"
#include "crypto/rx/RxQueue.h"


namespace evildragon {


class RxPrivate;


static bool osInitialized   = false;
static bool msrInitialized  = false;
static RxPrivate *d_ptr     = nullptr;


class RxPrivate
{
public:
    inline RxPrivate(IRxListener *listener) : queue(listener) {}

    RxQueue queue;
};


} // namespace evildragon


evildragon::HugePagesInfo evildragon::Rx::hugePages()
{
    return d_ptr->queue.hugePages();
}


evildragon::RxDataset *evildragon::Rx::dataset(const Job &job, uint32_t nodeId)
{
    return d_ptr->queue.dataset(job, nodeId);
}


void evildragon::Rx::destroy()
{
    if (osInitialized) {
        msrDestroy();
    }

    delete d_ptr;

    d_ptr = nullptr;
}


void evildragon::Rx::init(IRxListener *listener)
{
    d_ptr = new RxPrivate(listener);
}


template<typename T>
bool evildragon::Rx::init(const T &seed, const RxConfig &config, const CpuConfig &cpu)
{
    if (seed.algorithm().family() != Algorithm::RANDOM_X) {
        if (msrInitialized) {
            msrDestroy();
            msrInitialized = false;
        }

        return true;
    }

    if (isReady(seed)) {
        return true;
    }

    if (!msrInitialized) {
        msrInit(config, cpu.threads().get(seed.algorithm()).data());
        msrInitialized = true;
    }

    if (!osInitialized) {
        setupMainLoopExceptionFrame();
        osInitialized = true;
    }

    d_ptr->queue.enqueue(seed, config.nodeset(), config.threads(cpu.limit()), cpu.isHugePages(), config.isOneGbPages(), config.mode(), cpu.priority());

    return false;
}


template<typename T>
bool evildragon::Rx::isReady(const T &seed)
{
    return d_ptr->queue.isReady(seed);
}


#ifndef evildragon_FEATURE_MSR
void evildragon::Rx::msrInit(const RxConfig &, const std::vector<CpuThread> &)
{
}


void evildragon::Rx::msrDestroy()
{
}
#endif


#ifndef evildragon_FIX_RYZEN
void evildragon::Rx::setupMainLoopExceptionFrame()
{
}
#endif


namespace evildragon {


template bool Rx::init(const RxSeed &seed, const RxConfig &config, const CpuConfig &cpu);
template bool Rx::isReady(const RxSeed &seed);
template bool Rx::init(const Job &seed, const RxConfig &config, const CpuConfig &cpu);
template bool Rx::isReady(const Job &seed);


} // namespace evildragon
