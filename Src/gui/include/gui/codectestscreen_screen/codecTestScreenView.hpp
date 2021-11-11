#ifndef CODECTESTSCREENVIEW_HPP
#define CODECTESTSCREENVIEW_HPP

#include <gui_generated/codectestscreen_screen/codecTestScreenViewBase.hpp>
#include <gui/codectestscreen_screen/codecTestScreenPresenter.hpp>
#include "vs1053.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

class codecTestScreenView : public codecTestScreenViewBase
{
public:
    codecTestScreenView();
    virtual ~codecTestScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void codecTestControlBtnPressed();
    virtual void backButtonPressed();
	virtual void volumeLeftSB_valueChanged(int value);
	virtual void volumeRightSB_valueChanged(int value);
    virtual void carryFreqNumPickerValueChange(uint8_t value);
    virtual void modFreqNumPickerValueChanged(uint8_t value);
    virtual void deviationNumPickerValueChanged(uint8_t value);
    virtual void sineTestRBSelected();
    virtual void pcmTestRBSelected();
	virtual void handleTickEvent();
protected:
	void startPCMTest();
	void stopPCMTest();
private:
	int volumeLeft = 127, volumeRight = 127;
	bool isSineTestSelected = true;
	bool isPCMTestSelected = false;
	bool isPCMTestStarted = false;
	bool isSineTestStarted = false;
	commandStruct cmd;
	uint16_t currentCarryFreq;
	uint16_t currentModFreq;
	uint16_t currentDeviation;

    std::vector<string> carryFreqValuesList = {"250Hz", "500Hz", "750Hz", "1kHz", "1,5kHz", "2kHz", "3kHz", "4kHz", "6kHz", "8kHz"};
    uint16_t carryFreqValues[10] = {250, 500, 750, 1000, 1500, 2000, 3000, 4000, 6000, 8000};
    std::vector<string> modFreqValuesList = {"20Hz", "50Hz", "100Hz"};
    uint16_t modFreqValues[3] = {20, 50, 100};
    std::vector<string> deviationValuesList = {"0 %", "1 %", "2 %", "3 %", "4 %", "5 %", "6 %", "7 %", "8 %", "9 %", "10 %", "11 %", "12 %",
    		"13 %", "14 %", "15 %", "16 %", "17 %", "18 %", "19 %", "20 %", "21 %", "22 %", "23 %", "24 %", "25 %"};
    uint16_t deviationValue[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
};

#endif // CODECTESTSCREENVIEW_HPP
