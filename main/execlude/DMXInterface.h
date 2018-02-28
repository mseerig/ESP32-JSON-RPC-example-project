/*
 * DMCInterface.h
 *
 *  Created on: 19.02.2018
 *      Author: marcel.seerig
 */

#ifndef MAIN_DMXINTERFACE_H_
#define MAIN_DMXINTERFACE_H_

#include "DMXUniverse.h"
#include "UART.h"

typedef enum{
	Master = 0,
	Slave = 1,
}dmx_mode_t;

class DMX{
public:
	DMX(UART *uart, dmx_mode_t mode);

	~DMX();

	DmxUniverse* getUniverse();
	void setUniverse(DmxUniverse* universe);

	// MASTER
	void print(DmxUniverse *universe=nullptr);
	void startAutomaticPrint(int fps);

	// SLAVE
	void startListener();

private:
	dmx_mode_t 	m_mode;
	UART 		*m_uart;
	DmxUniverse *m_universe;
};

#endif /* MAIN_DMXINTERFACE_H_ */
