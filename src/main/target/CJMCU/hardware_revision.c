/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../CJMCU/hardware_revision.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"

#include "build/build_config.h"

#include "drivers/io_types.h"


uint8_t hardwareRevision = UNKNOWN;

void detectHardwareRevision(void)
{
    if (GPIOC->IDR & GPIO_Pin_15) {
        hardwareRevision = REV_2;
    } else {
        hardwareRevision = REV_1;
    }
}

void updateHardwareRevision(void)
{
}

ioTag_t selectMPUIntExtiConfigByHardwareRevision(void)
{
    return IO_TAG_NONE;
}
