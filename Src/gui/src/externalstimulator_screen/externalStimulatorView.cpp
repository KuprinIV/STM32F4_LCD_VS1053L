#include <gui/externalstimulator_screen/externalStimulatorView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

namespace{
	__attribute__ ((section (".TouchGFX_Framebuffer"))) uint8_t FM_stimulData[19200]; //4800 samples 16-bit 2 channels
}

externalStimulatorView::externalStimulatorView()
{

}

void externalStimulatorView::handleTickEvent()
{
	cmd = *getCommand();
	if(cmd.status == PCM_TEST_STARTED){
		if(!isPCMTestStarted){
			testCtrlButton.setLabelText(touchgfx::TypedText(T_CODECSTOPTESTBTNLABEL));
			testCtrlButton.invalidate();

			maxVolumeSlider.setTouchable(false);
			stimTypePicker.setTouchable(false);
		}
		isPCMTestStarted = true;
		if(cmd.externalStimulusEnded){
			cmd.externalStimulusEnded = 0;
			if(isPause){
				if(currentStep < 4){
					currentStep++;
					updateTestProgress(currentStep);
				}
			}
			isPause = !isPause;
			sendCommand(&cmd);
		}
	}else if(cmd.status == STATUS_IDLE){
		if(isPCMTestStarted){
			testCtrlButton.setLabelText(touchgfx::TypedText(T_CODECSTARTTESTBTNLABEL));
			testCtrlButton.invalidate();

			maxVolumeSlider.setTouchable(true);
			stimTypePicker.setTouchable(true);

			testProgressText.setVisible(false);
			testProgressText.invalidate();
			currentStep = 0;
		}
		isPCMTestStarted = false;
	}
}

void externalStimulatorView::setupScreen()
{
    externalStimulatorViewBase::setupScreen();
    maxVolume = maxVolumeSlider.getValue();

    stimTypePicker.setValueIndex(3);
    currentFreq = freqValues[3];
    stimTypePicker.setValuesList(&freqValuesList);
    stimTypePicker.setCycled(true);

    codecHardwareReset();
}

void externalStimulatorView::tearDownScreen()
{
    externalStimulatorViewBase::tearDownScreen();
}

void externalStimulatorView::backButtonPressed()
{
	if(isPCMTestStarted){
		stopTest();
	}
	application().gotomain_screenScreenNoTransition();
}

void externalStimulatorView::testCtrlButtonPressed()
{
	if(!isPCMTestStarted){
		startTest();
	}else{
		stopTest();
	}
}

void externalStimulatorView::maxVolumeSliderValueChanged(int value)
{
	maxVolume = value;
}

void externalStimulatorView::stimTypePickerValueChanged(uint8_t value)
{
	currentFreq = freqValues[value];
}

void externalStimulatorView::startTest()
{
	int16_t sampleVal = 0;
	float carryF = (float)M_PI*currentFreq/96000; // set stimulus

	// fill stimulus data array
	for(uint16_t i = 0; i < sizeof(FM_stimulData); i += 4){
		sampleVal = (int16_t)(0x7FFF*sin(carryF*i));
		// byte order: | left channel LSB, left channel MSB | right channel LSB, right channel MSB |
		FM_stimulData[i] = FM_stimulData[i + 2] = (uint8_t)(sampleVal & 0xFF); // LSB's
		FM_stimulData[i + 1] = FM_stimulData[i + 3] = (uint8_t)((sampleVal>>8) & 0xFF); // MSB's
	}
	setPCMDataBuffer(FM_stimulData);
	cmd = *getCommand();
	cmd.command = START_PCM_TEST;
	cmd.isExternalStimulator = 1;
	cmd.volume = 254 - (uint8_t)maxVolume + 40; // set initial volume
	sendCommand(&cmd);
	testProgressText.setVisible(true);
	testProgressText.invalidate();
	updateTestProgress(currentStep);
	isPause = false;
}

void externalStimulatorView::stopTest()
{
	cmd = *getCommand();
	cmd.command = STOP_PCM_TEST;
	sendCommand(&cmd);
	currentStep = 0;
}

void externalStimulatorView::updateTestProgress(int stepNum)
{
	int x_pos = 0;
	// update progress text
	Unicode::snprintf(testProgressTextBuffer, TESTPROGRESSTEXT_SIZE, "%d", stepNum+1);
	testProgressText.setWildcard(testProgressTextBuffer);
	x_pos = (box1.getWidth() - testProgressText.getTextWidth())/2;
	testProgressText.setX(x_pos);

	testProgressText.invalidate();
	box2.invalidate();
	testProgressText.resizeToCurrentText();
	testProgressText.invalidate();
	// update progress bar
	testProgressBar.setValue(stepNum+1);
	testProgressBar.invalidate();
}
