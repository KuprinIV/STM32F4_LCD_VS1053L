#ifndef PLAYERSETTINGS_HPP
#define PLAYERSETTINGS_HPP

#include <gui_generated/containers/playerSettingsBase.hpp>

class playerSettings : public playerSettingsBase
{
public:
    playerSettings();
    virtual ~playerSettings() {isSettingInited = false;}

    virtual void initialize();
    virtual void diffRadioButtonSelected();
    virtual void diffRadioButtonDeselected();
    virtual void bassAmpSliderValueChanged(int value);
    virtual void bassFreqSliderValueChanged(int value);
    virtual void trebleAmpSliderValueChanged(int value);
    virtual void trebleFreqSliderValueChanged(int value);
    void initSettings(uint16_t* val);
protected:
private:
    uint8_t bassAmplification = 0;
    uint8_t bassLowFreq = 0;
    uint8_t trebleAmplification = 0;
    uint8_t trebleStartFreq = 0;
    bool isDiffEnabled = false;
    bool isSettingInited = false;
    uint16_t settingsVals[2] = {0};
};

#endif // PLAYERSETTINGS_HPP
