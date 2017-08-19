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

#include <stdbool.h>
#include <stdint.h>

#include <platform.h>

#include "common/axis.h"
#include "common/maths.h"


#include "drivers/sensor.h"
#include "drivers/compass/compass.h"
#include "drivers/serial.h"

#include "fc/rc_controls.h"

#include "flight/failsafe.h"
#include "flight/mixer.h"
#include "flight/pid.h"

#include "rx/rx.h"

#include "io/serial.h"

#include "telemetry/telemetry.h"

#include "sensors/sensors.h"
#include "sensors/compass.h"
#include "sensors/barometer.h"

#include "config/feature.h"

#include "fc/config.h"
#include "fc/controlrate_profile.h"
#include "fc/rc_modes.h"
#include "fc/rc_controls.h"
#include "target.h"


#ifdef TARGET_CONFIG
void targetConfiguration(void)
{
    serialConfigMutable()->portConfigs[1].functionMask = FUNCTION_MSP;

    modeActivationConditionsMutable(0)->modeId          = BOXANGLE;
    modeActivationConditionsMutable(0)->auxChannelIndex = AUX4 - NON_AUX_CHANNEL_COUNT;
    modeActivationConditionsMutable(0)->range.startStep = CHANNEL_VALUE_TO_STEP(900);
    modeActivationConditionsMutable(0)->range.endStep   = CHANNEL_VALUE_TO_STEP(2000);

    modeActivationConditionsMutable(1)->modeId          = BOXHEADFREE;
    modeActivationConditionsMutable(1)->auxChannelIndex = AUX1 - NON_AUX_CHANNEL_COUNT;
    modeActivationConditionsMutable(1)->range.startStep = CHANNEL_VALUE_TO_STEP(1500);
    modeActivationConditionsMutable(1)->range.endStep   = CHANNEL_VALUE_TO_STEP(2100);


    motorConfigMutable()->dev.motorPwmRate = BRUSHED_MOTORS_PWM_RATE;

    pidProfilesMutable(0)->pid[PID_ROLL].P  = 40;
    pidProfilesMutable(0)->pid[PID_ROLL].I  = 0;
    pidProfilesMutable(0)->pid[PID_ROLL].D  = 30;
    pidProfilesMutable(0)->pid[PID_PITCH].P = 58;
    pidProfilesMutable(0)->pid[PID_PITCH].I = 0;
    pidProfilesMutable(0)->pid[PID_PITCH].D = 35;

    pidProfilesMutable(0)->pid[PID_YAW].I = 0;

}
#endif
