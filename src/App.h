/* evildragon
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
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

#ifndef evildragon_APP_H
#define evildragon_APP_H


#include "base/kernel/interfaces/IConsoleListener.h"
#include "base/kernel/interfaces/ISignalListener.h"
#include "base/tools/Object.h"


namespace evildragon {


class Console;
class Controller;
class Network;
class Process;
class Signals;


class App : public IConsoleListener, public ISignalListener
{
public:
    evildragon_DISABLE_COPY_MOVE_DEFAULT(App)

    App(Process *process);
    ~App() override;

    int exec();

protected:
    void onConsoleCommand(char command) override;
    void onSignal(int signum) override;

private:
    bool background(int &rc);
    void close();

    Console *m_console          = nullptr;
    Controller *m_controller    = nullptr;
    Signals *m_signals          = nullptr;
};


} /* namespace evildragon */


#endif /* evildragon_APP_H */