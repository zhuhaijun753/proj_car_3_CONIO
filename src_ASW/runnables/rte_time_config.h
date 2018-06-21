/**
 * \file 	rte_time_config.h
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	16.09.2016
 *
 * \brief Configuration parameters for the timing measurement
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
 *
 * \copyright Copyright ©2016
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */
#ifndef RTE_TIME_CONFIG_H_
#define RTE_TIME_CONFIG_H_

#define RTE_TIMING_ANALYSIS		(1)
#define RTE_MAX_CORES			(3)
#define RTE_MAX_TASKS_PER_CORE	(5)
#define RTE_TASKNAME_SIZE		(20)
#define RTE_PRESENT_TOTAL_TASKS	(4)
#define RTE_NO_OF_BUFFERS		(2)
#define RTE_NO_OF_PARAMS		(4)


#endif /* RTE_TIME_CONFIG_H_ */
