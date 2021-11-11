#include <gui/containers/playerSettings.hpp>

playerSettings::playerSettings()
{

}

void playerSettings::initialize()
{
    playerSettingsBase::initialize();
}

void playerSettings::diffRadioButtonSelected()
{
	settingsVals[1] = 1;
	isDiffEnabled = true;
	if(isSettingInited){
		emitPlaySettingsChangedCallback(settingsVals);
	}
}

void playerSettings::diffRadioButtonDeselected()
{
	settingsVals[1] = 0;
	isDiffEnabled = false;
	if(isSettingInited){
		emitPlaySettingsChangedCallback(settingsVals);
	}
}

void playerSettings::bassAmpSliderValueChanged(int value)
{
	int x_pos = 0;
	// set slider value label
	if(value == 0){
		Unicode::snprintf(bassBoostTextBuffer, BASSBOOSTTEXT_SIZE, "off");
	}else{
		Unicode::snprintf(bassBoostTextBuffer, BASSBOOSTTEXT_SIZE, "%d dB", value);
	}
	bassBoostText.setWildcard(bassBoostTextBuffer);
	x_pos = (box1.getWidth() - bassBoostText.getTextWidth())/2;
	bassBoostText.setX(x_pos);
	bassBoostText.invalidate();
	bassBoostText.resizeToCurrentText();
	box1.invalidate();
	bassBoostText.invalidate();

	bassAmplification = value;
	settingsVals[0] &= 0xFF0F;
	settingsVals[0] |= ((bassAmplification & 0xF)<<4);
	if(isSettingInited){
		emitPlaySettingsChangedCallback(settingsVals);
	}
}

void playerSettings::bassFreqSliderValueChanged(int value)
{
	int x_pos = 0;
	// set slider value label
	Unicode::snprintf(bassFreqTextBuffer, BASSFREQTEXT_SIZE, "%d", value*10);
	bassFreqText.setWildcard(bassFreqTextBuffer);
	x_pos = (box1.getWidth() - bassFreqText.getTextWidth())/2;
	bassFreqText.setX(x_pos);
	bassFreqText.invalidate();
	bassFreqText.resizeToCurrentText();
	box1.invalidate();
	bassFreqText.invalidate();

	bassLowFreq = value;
	settingsVals[0] &= 0xFFF0;
	settingsVals[0] |= (bassLowFreq & 0xF);
	if(isSettingInited){
		emitPlaySettingsChangedCallback(settingsVals);
	}
}

void playerSettings::trebleAmpSliderValueChanged(int value)
{
	int x_pos = 0;
	// set slider value label
	if(value - 8 == 0){
		Unicode::snprintf(trebleBoostTextBuffer, TREBLEBOOSTTEXT_SIZE, "off");
	}else{
		Unicode::snprintf(trebleBoostTextBuffer, TREBLEBOOSTTEXT_SIZE, "%d dB", value - 8);
	}
	trebleBoostText.setWildcard(trebleBoostTextBuffer);
	x_pos = (box1.getWidth() - trebleBoostText.getTextWidth())/2;
	trebleBoostText.setX(x_pos);
	trebleBoostText.invalidate();
	trebleBoostText.resizeToCurrentText();
	box1.invalidate();
	trebleBoostText.invalidate();

	trebleAmplification = value;
	settingsVals[0] &= 0x0FFF;
	settingsVals[0] |= (((trebleAmplification - 8) & 0xF)<<12);
	if(isSettingInited){
		emitPlaySettingsChangedCallback(settingsVals);
	}
}

void playerSettings::trebleFreqSliderValueChanged(int value)
{
	int x_pos = 0;
	// set slider value label
	Unicode::snprintf(trebleFreqTextBuffer, TREBLEFREQTEXT_SIZE, "%d", value);
	trebleFreqText.setWildcard(trebleFreqTextBuffer);
	x_pos = (box1.getWidth() - trebleFreqText.getTextWidth())/2;
	trebleFreqText.setX(x_pos);
	trebleFreqText.invalidate();
	trebleFreqText.resizeToCurrentText();
	box1.invalidate();
	trebleFreqText.invalidate();

	trebleStartFreq = value;
	settingsVals[0] &= 0xF0FF;
	settingsVals[0] |= ((trebleStartFreq & 0xF)<<8);
	if(isSettingInited){
		emitPlaySettingsChangedCallback(settingsVals);
	}
}

void playerSettings::initSettings(uint16_t* val)
{
    bassAmplification = ((val[0] >> 4) & 0xF);
    bassLowFreq = (val[0] & 0xF);
    trebleAmplification = ((val[0] >> 12) & 0xF) + 8;
    trebleStartFreq = ((val[0] >> 8) & 0xF);
    isDiffEnabled = (val[1] == 1);

    bassAmpSlider.setValue(bassAmplification);
    bassFreqSlider.setValue(bassLowFreq);
    trebleAmpSlider.setValue(trebleAmplification);
    trebleFreqSlider.setValue(trebleStartFreq);
    diffRadioButton.setSelected(isDiffEnabled);

    settingsVals[0] = val[0];
    settingsVals[1] = val[1];
    emitPlaySettingsChangedCallback(settingsVals);
    isSettingInited = true;
}
