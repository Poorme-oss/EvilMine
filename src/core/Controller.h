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

#ifndef evildragon_CONTROLLER_H
#define evildragon_CONTROLLER_H


#include "base/kernel/Base.h"
#include "base/tools/Object.h"


namespace evildragon {


class Job;
class Miner;
class Network;


class Controller : public Base
{
public:
    evildragon_DISABLE_COPY_MOVE_DEFAULT(Controller)

    Controller(Process *process);
    ~Controller() override;

    int init() override;
    void start() override;
    void stop() override;

    Miner *miner() const;
    Network *network() const;
    void execCommand(char command);

private:
    Miner *m_miner     = nullptr;
    Network *m_network = nullptr;
};


} // namespace evildragon


#endif /* evildragon_CONTROLLER_H */