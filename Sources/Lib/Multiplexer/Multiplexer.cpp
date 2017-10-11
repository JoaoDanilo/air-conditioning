#include "Multiplexer.h"

uint8_t Multiplexer::select(uint8_t select, uint8_t inputCounter, uint8_t inputTemperature){

	switch(select){
		case 0:
			return inputCounter;
		case 1:
			return inputTemperature;
	}
}
