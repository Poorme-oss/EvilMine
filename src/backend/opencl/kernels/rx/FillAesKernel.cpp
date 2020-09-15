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


#include "backend/opencl/kernels/rx/FillAesKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void evildragon::FillAesKernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t gthreads        = threads * 4;
    static const size_t lthreads = 64;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void fillAes1Rx4_scratchpad(__global void* state, __global void* out, uint batch_size, uint rx_version)
// __kernel void fillAes4Rx4_entropy(__global void* state, __global void* out, uint batch_size, uint rx_version)
void evildragon::FillAesKernel::setArgs(cl_mem state, cl_mem out, uint32_t batch_size, uint32_t rx_version)
{
    setArg(0, sizeof(cl_mem), &state);
    setArg(1, sizeof(cl_mem), &out);
    setArg(2, sizeof(uint32_t), &batch_size);
    setArg(3, sizeof(uint32_t), &rx_version);
}