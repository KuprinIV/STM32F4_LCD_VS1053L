#ifndef EXTERNALSTIMULATORVIEW_HPP
#define EXTERNALSTIMULATORVIEW_HPP

#include <gui_generated/externalstimulator_screen/externalStimulatorViewBase.hpp>
#include <gui/externalstimulator_screen/externalStimulatorPresenter.hpp>
#include <math.h>

class externalStimulatorView : public externalStimulatorViewBase
{
public:
    externalStimulatorView();
    virtual ~externalStimulatorView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void backButtonPressed();
    virtual void testCtrlButtonPressed();
    virtual void maxVolumeSliderValueChanged(int value);
    virtual void stimTypePickerValueChanged(uint8_t value);
    virtual void handleTickEvent();
protected:
private:
    void updateTestProgress(int stepNum);
    void startTest();
    void stopTest();

    commandStruct cmd;
    std::vector<string> freqValuesList = {"250Hz", "500Hz", "750Hz", "1kHz", "1,5kHz", "2kHz", "3kHz", "4kHz", "6kHz", "8kHz"};
    uint16_t freqValues[10] = {250, 500, 750, 1000, 1500, 2000, 3000, 4000, 6000, 8000};
    uint16_t currentFreq;
    bool isPause = false;
    bool isPCMTestStarted = false;
    int currentStep = 0;
    int maxVolume;
};

#endif // EXTERNALSTIMULATORVIEW_HPP
