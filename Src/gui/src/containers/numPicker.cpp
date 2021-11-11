#include <gui/containers/numPicker.hpp>

numPicker::numPicker()
{

}

void numPicker::initialize()
{
    numPickerBase::initialize();
}

void numPicker::goPrevValueButtonPressed()
{
	if(pickerValuesList.size() > 0){
		if(currentValueIndex > 0){
			currentValueIndex--;
		}else{
			if(isCycled){
				currentValueIndex = pickerValuesList.size() - 1;
			}else{
				currentValueIndex = 0;
			}
		}
		setPickerText(pickerValuesList.at(currentValueIndex));
		emitPickerValueChangedCallback(currentValueIndex);
	}
}

void numPicker::goNextValueButtonPressed()
{
	if(pickerValuesList.size() > 0){
		if(currentValueIndex < pickerValuesList.size() - 1){
			currentValueIndex++;
		}else{
			if(isCycled){
				currentValueIndex = 0;
			}else{
				currentValueIndex = pickerValuesList.size() - 1;
			}
		}
		setPickerText(pickerValuesList.at(currentValueIndex));
		emitPickerValueChangedCallback(currentValueIndex);
	}
}

void numPicker::setValuesList(std::vector<string>* values_list)
{
	pickerValuesList.reserve(values_list->size());
	pickerValuesList = *values_list;
	if(pickerValuesList.size() > 0){
		setPickerText(pickerValuesList.at(currentValueIndex));
	}
}

void numPicker::setValueIndex(int8_t index)
{
	currentValueIndex = index;
}

int8_t numPicker::getValueIndex()
{
	return currentValueIndex;
}

void numPicker::setPickerText(string text)
{
	int16_t x_pos = 0;
	Unicode::fromUTF8((uint8_t*)text.c_str(), pickerTextBuffer, PICKERTEXT_SIZE);
	pickerText.setWildcard(pickerTextBuffer);

	x_pos = (this->getWidth() - pickerText.getTextWidth())/2;
	pickerText.setX(x_pos);

	pickerText.invalidate();
	box1.invalidate();
	pickerText.resizeToCurrentText();
	pickerText.invalidate();
}

void numPicker::setCycled(bool isCycled)
{
	this->isCycled = isCycled;
}
