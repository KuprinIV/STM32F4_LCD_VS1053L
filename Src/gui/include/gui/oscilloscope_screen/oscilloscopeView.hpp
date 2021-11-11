#ifndef OSCILLOSCOPEVIEW_HPP
#define OSCILLOSCOPEVIEW_HPP

#include <gui_generated/oscilloscope_screen/oscilloscopeViewBase.hpp>
#include <gui/oscilloscope_screen/oscilloscopePresenter.hpp>
#include "BitmapDatabase.hpp"
#include <math.h>

class oscilloscopeView : public oscilloscopeViewBase
{
public:
    oscilloscopeView();
    virtual ~oscilloscopeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void backButtonPressed();
    virtual void scopeCtrlButtonPressed();
    virtual void micRadioButtonSelected();
    virtual void lineRadioButtonSelected();
    virtual void xScalePickerValueChanged(uint8_t value);
    virtual void yScalePickerValueChanged(uint8_t value);

protected:

private:
    bool isRunning = false;
    float sineArray[64] = {0};
    int index = 0;
    int xFullScale;

    std::vector<string> xScaleValuesList = {/*"0,1ms", "0,2ms", "0,5ms", "1ms", "2ms",*/ "5ms", "10ms", "20ms", "50ms", "0,1s", "0,2s", "0,5s", "1s"};
    uint16_t xScaleValues[8] = {/*10000, 5000, 2000, 1000, 500,*/ 200, 100, 50, 20, 10, 5, 2, 1};

    std::vector<string> yScaleValuesList = {"1mV", "2mV", "5mV", "10mV", "20mV", "50mV", "0,1V", "0,2V", "0,5V", "1V"};
    uint16_t yScaleValues[10] = {1000, 500, 200, 100, 50, 20, 10, 5, 2, 1};
};

#endif // OSCILLOSCOPEVIEW_HPP
