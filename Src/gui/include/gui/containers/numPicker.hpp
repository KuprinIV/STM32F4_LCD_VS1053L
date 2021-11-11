#ifndef NUMPICKER_HPP
#define NUMPICKER_HPP

#include <gui_generated/containers/numPickerBase.hpp>

class numPicker : public numPickerBase
{
public:
    numPicker();
    virtual ~numPicker() {
    	pickerValuesList.clear();
    }

    virtual void initialize();
    virtual void goPrevValueButtonPressed();
    virtual void goNextValueButtonPressed();
    void setValuesList(std::vector<string>* values_list);
    void setValueIndex(int8_t index);
    int8_t getValueIndex();
    void setCycled(bool isCycled);

protected:
    void setPickerText(string text);

private:
    bool isCycled = false;
    uint8_t currentValueIndex = 0;
    std::vector<string> pickerValuesList;
};

#endif // NUMPICKER_HPP
