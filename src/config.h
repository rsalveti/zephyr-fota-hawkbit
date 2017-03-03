/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __FOTA_CONFIG_H_
#define __FOTA_CONFIG_H_

#define BACKEND_HAWKBIT			0
#define BACKEND_BLUEMIX			1

/*
 * Whether to use hawkBit or Bluemix for device management.
 *
 * Bluemix support is built in regardless of this setting , e.g. for
 * use publishing sensor data.
 */
#define CONFIG_DM_BACKEND		BACKEND_HAWKBIT

#define CONFIG_BLUEMIX_ORG		"mstsez"

#define CONFIG_SENSORHUB_SUPPORT	1

#endif /* __FOTA_CONFIG_H_ */
