/* evildragon
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018-2019 SChernykh   <https://github.com/SChernykh>
 * Copyright 2016-2019 evildragon       <https://github.com/evildragon>, <support@evildragon.com>
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


#include <uv.h>


#include "base/io/Signals.h"
#include "base/kernel/interfaces/ISignalListener.h"
#include "base/tools/Handle.h"


static const int signums[evildragon::Signals::kSignalsCount] = { SIGHUP, SIGINT, SIGTERM };


evildragon::Signals::Signals(ISignalListener *listener)
    : m_listener(listener)
{
    for (size_t i = 0; i < kSignalsCount; ++i) {
        uv_signal_t *signal = new uv_signal_t;
        signal->data        = this;

        m_signals[i] = signal;

        uv_signal_init(uv_default_loop(), signal);
        uv_signal_start(signal, Signals::onSignal, signums[i]);
    }
}


evildragon::Signals::~Signals()
{
    stop();
}


void evildragon::Signals::stop()
{
    if (!m_signals[0]) {
        return;
    }

    for (size_t i = 0; i < kSignalsCount; ++i) {
        Handle::close(m_signals[i]);
        m_signals[i] = nullptr;
    }
}


void evildragon::Signals::onSignal(uv_signal_t *handle, int signum)
{
    static_cast<Signals *>(handle->data)->m_listener->onSignal(signum);
}
