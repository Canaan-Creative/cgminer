/*
 * Copyright 2016 Mikeqin <Fengling.Qin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#ifndef _AVALON7_H_
#define _AVALON7_H_

#include "util.h"
#include "i2c-context.h"

#ifdef USE_AVALON7

#define AVA7_DEFAULT_FAN_MIN	10 /* % */
#define AVA7_DEFAULT_FAN_MAX	100

#define AVA7_DEFAULT_TEMP_TARGET	65
#define AVA7_DEFAULT_TEMP_OVERHEAT	85
#define AVA7_MM711_TEMP_FREQADJ	70

#define AVA7_DEFAULT_VOLTAGE_MIN	3889
#define AVA7_DEFAULT_VOLTAGE_MAX	5059

#define AVA7_DEFAULT_FREQUENCY_MIN	100
#define AVA7_DEFAULT_FREQUENCY_MAX	500
#define AVA7_MM711_FREQUENCY_MAX	500

#define AVA7_DEFAULT_MODULARS	7	/* Only support 6 modules maximum with one AUC */
#define AVA7_DEFAULT_MINER_MAX	8
#define AVA7_DEFAULT_ASIC_MAX	40
#define AVA7_DEFAULT_PLL_CNT	7

#define AVA7_MM711_MINER_CNT	4
#define AVA7_MM711_ASIC_CNT	18

#define AVA7_DEFAULT_POLLING_DELAY	20 /* ms */
#define AVA7_DEFAULT_NTIME_OFFSET 4

#define AVA7_DEFAULT_SMARTSPEED_OFF 0
#define AVA7_DEFAULT_SMARTSPEED_MODE1 1
#define AVA7_DEFAULT_SMARTSPEED_MODE2 2
#define AVA7_DEFAULT_SMART_SPEED	(AVA7_DEFAULT_SMARTSPEED_MODE2)

#define AVA7_DEFAULT_IIC_DETECT	false

#define AVA7_PWM_MAX	0x3FF
#define AVA7_DRV_DIFFMAX	1024

#define AVA7_MODULE_DETECT_INTERVAL	30 /* 30 s */

#define AVA7_AUC_VER_LEN	12	/* Version length: 12 (AUC-YYYYMMDD) */
#define AVA7_AUC_SPEED		400000
#define AVA7_AUC_XDELAY  	19200	/* 4800 = 1ms in AUC (11U14)  */
#define AVA7_AUC_P_SIZE		64

#define AVA7_CONNECTER_AUC	1
#define AVA7_CONNECTER_IIC	2

/* avalon7 protocol package type from MM protocol.h
 * https://github.com/Canaan-Creative/MM/blob/avalon7/firmware/protocol.h */
#define AVA7_MM_VER_LEN	15
#define AVA7_MM_DNA_LEN	8
#define AVA7_H1	'C'
#define AVA7_H2	'N'

#define AVA7_P_COINBASE_SIZE	(6 * 1024 + 64)
#define AVA7_P_MERKLES_COUNT	30

#define AVA7_P_COUNT	40
#define AVA7_P_DATA_LEN 32

/* Broadcase with block iic_write*/
#define AVA7_P_DETECT	0x10

/* Broadcase With non-block iic_write*/
#define AVA7_P_STATIC	0x11
#define AVA7_P_JOB_ID	0x12
#define AVA7_P_COINBASE	0x13
#define AVA7_P_MERKLES	0x14
#define AVA7_P_HEADER	0x15
#define AVA7_P_TARGET	0x16

/* Broadcase or Address */
#define AVA7_P_SET	0x20
#define AVA7_P_FINISH	0x21
#define AVA7_P_SET_VOLT 0x22
#define AVA7_P_SET_FREQ 0x23

/* Have to with I2C address */
#define AVA7_P_POLLING	0x30
#define AVA7_P_REQUIRE	0x31
#define AVA7_P_TEST	0x32
#define AVA7_P_RSTMMTX	0x33
#define AVA7_P_GET_VOLT 0x34

/* Back to host */
#define AVA7_P_ACKDETECT	0x40
#define AVA7_P_STATUS		0x41
#define AVA7_P_NONCE		0x42
#define AVA7_P_TEST_RET		0x43
#define AVA7_P_STATUS_LW        0x44
#define AVA7_P_STATUS_HW        0x45
#define AVA7_P_STATUS_VOLT	0x46
#define AVA7_P_STATUS_FREQ	0x47
#define AVA7_P_STATUS_M	0x48
#define AVA7_P_STATUS_PLL	0x49

#define AVA7_MODULE_BROADCAST	0
/* Endof avalon7 protocol package type */

#define AVA7_MM711_PREFIXSTR	"711"
#define AVA7_MM_VERNULL		"NONE"

#define AVA7_TYPE_MM711		711
#define AVA7_TYPE_NULL		00

#define AVA7_IIC_RESET		0xa0
#define AVA7_IIC_INIT		0xa1
#define AVA7_IIC_DEINIT		0xa2
#define AVA7_IIC_XFER		0xa5
#define AVA7_IIC_INFO		0xa6

#define AVA7_FREQ_INIT_MODE	0x0
#define AVA7_FREQ_CUTOFF_MODE	0x1
#define AVA7_FREQ_TEMPADJ_MODE	0x2
#define AVA7_FREQ_PLLADJ_MODE	0x3

#define AVA7_DEFAULT_FAVG_TIME	(15 * 60.0)
#define AVA7_DEFAULT_FREQADJ_TIME	60

#define AVA7_DEFAULT_DELTA_T	0
#define AVA7_DEFAULT_DELTA_FREQ	100

#define AVA7_VOLT_ADC_RATIO	(3.3 / 1024.0 * 125.0 / 43.0 * 10000.0)

struct avalon7_pkg {
	uint8_t head[2];
	uint8_t type;
	uint8_t opt;
	uint8_t idx;
	uint8_t cnt;
	uint8_t data[32];
	uint8_t crc[2];
};
#define avalon7_ret avalon7_pkg

struct avalon7_info {
	/* Public data */
	uint32_t newnonce;

	int polling_first;
	int mm_count;
	int xfer_err_cnt;
	int pool_no;

	struct timeval firsthash;
	struct timeval last_fan_adj;
	struct timeval last_stratum;
	struct timeval last_detect;
	struct timeval last_fadj;
	struct timeval last_tcheck;

	cglock_t update_lock;

	struct pool pool0;
	struct pool pool1;
	struct pool pool2;

	/* For connecter */
	char auc_version[AVA7_AUC_VER_LEN + 1];

	int auc_speed;
	int auc_xdelay;
	int auc_sensor;

	struct i2c_ctx *i2c_slaves[AVA7_DEFAULT_MODULARS];

	uint8_t connecter; /* AUC or IIC */

	/* For modulars */
	bool enable[AVA7_DEFAULT_MODULARS];

	struct timeval elapsed[AVA7_DEFAULT_MODULARS];
	struct timeval last_favg[AVA7_DEFAULT_MODULARS];

	uint8_t mm_dna[AVA7_DEFAULT_MODULARS][AVA7_MM_DNA_LEN + 1];
	char mm_version[AVA7_DEFAULT_MODULARS][AVA7_MM_VER_LEN + 1];
	uint32_t total_asics[AVA7_DEFAULT_MODULARS];

	int mod_type[AVA7_DEFAULT_MODULARS];
	uint8_t miner_count[AVA7_DEFAULT_MODULARS];
	uint8_t asic_count[AVA7_DEFAULT_MODULARS];
	int toverheat[AVA7_DEFAULT_MODULARS];
	int temp_target[AVA7_DEFAULT_MODULARS];
	int fan_pct[AVA7_DEFAULT_MODULARS];

	uint32_t set_voltage[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX];

	uint32_t freq_mode[AVA7_DEFAULT_MODULARS];
	unsigned int set_frequency[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX];

	int led_red[AVA7_DEFAULT_MODULARS];
	uint8_t cutoff[AVA7_DEFAULT_MODULARS];

	int miner_pll[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX][AVA7_DEFAULT_PLL_CNT];

	int get_voltage[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX];
	int get_frequency[AVA7_DEFAULT_MODULARS];
	int fan_cpm[AVA7_DEFAULT_MODULARS];
	int temp[AVA7_DEFAULT_MODULARS];
	int last_maxtemp[AVA7_DEFAULT_MODULARS];

	uint64_t local_works[AVA7_DEFAULT_MODULARS];
	uint64_t local_works_i[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX];
	uint64_t hw_works[AVA7_DEFAULT_MODULARS];
	uint64_t hw_works_i[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX];
	int error_code[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX + 1];
	int chipmatching_work[AVA7_DEFAULT_MODULARS][AVA7_DEFAULT_MINER_MAX][AVA7_DEFAULT_ASIC_MAX];

	int polling_err_cnt[AVA7_DEFAULT_MODULARS];
};

struct avalon7_iic_info {
	uint8_t iic_op;
	union {
		uint32_t aucParam[2];
		uint8_t slave_addr;
	} iic_param;
};

#define AVA7_WRITE_SIZE (sizeof(struct avalon7_pkg))
#define AVA7_READ_SIZE AVA7_WRITE_SIZE

#define AVA7_SEND_OK 0
#define AVA7_SEND_ERROR -1

extern char *set_avalon7_fan(char *arg);
extern char *set_avalon7_freq(char *arg);
extern char *set_avalon7_voltage(char *arg);
extern int opt_avalon7_temp_target;
extern int opt_avalon7_overheat;
extern int opt_avalon7_polling_delay;
extern int opt_avalon7_aucspeed;
extern int opt_avalon7_aucxdelay;
extern int opt_avalon7_ntime_offset;
extern int opt_avalon7_smart_speed;
extern bool opt_avalon7_iic_detect;
extern int opt_avalon7_freqadj_time;
extern int opt_avalon7_delta_temp;
extern int opt_avalon7_delta_freq;
extern int opt_avalon7_freqadj_temp;
#endif /* USE_AVALON7 */
#endif	/* _AVALON7_H_ */