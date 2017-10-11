#include "FrequencyDivider.h"

FrequencyDivider::FrequencyDivider(uint32_t divider){
  _divider = divider;
}

uint8_t FrequencyDivider::clockDiv(){

	if(_counter == _divider){
		_counter = 1;
		return true;
	}
	_counter++;
	return false;
}

