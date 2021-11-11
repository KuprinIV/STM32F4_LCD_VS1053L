#include <gui/codectestscreen_screen/codecTestScreenView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

namespace{
	__attribute__ ((section (".TouchGFX_Framebuffer"))) uint8_t FM_stimulData[19200]; //4800 samples 16-bit 2 channels
}

codecTestScreenView::codecTestScreenView()
{

}

void codecTestScreenView::handleTickEvent()
{
	if(isPCMTestSelected){
		cmd = *getCommand();
		if(cmd.status == PCM_TEST_STARTED){
			if(!isPCMTestStarted){
				testControlBtn.setLabelText(touchgfx::TypedText(T_CODECSTOPTESTBTNLABEL));
				testControlBtn.invalidate();
				// disable radio buttons
				sineTestRadioButton.setTouchable(false);
				pcmTestRadioButton.setTouchable(false);
			}
			isPCMTestStarted = true;
		}else if(cmd.status == STATUS_IDLE){
			if(isPCMTestStarted){
				testControlBtn.setLabelText(touchgfx::TypedText(T_CODECSTARTTESTBTNLABEL));
				testControlBtn.invalidate();
				// enable radio buttons
				sineTestRadioButton.setTouchable(true);
				pcmTestRadioButton.setTouchable(true);
			}
			isPCMTestStarted = false;
		}
	}
}

void codecTestScreenView::setupScreen()
{
    codecTestScreenViewBase::setupScreen();

    carryFreqNumPicker.setVisible(false);
    modFreqNumPicker.setVisible(false);
    deviationNumPicker.setVisible(false);
    textArea6.setVisible(false);
    textArea7.setVisible(false);
    textArea8.setVisible(false);

    carryFreqNumPicker.setValueIndex(3);
    carryFreqNumPicker.setValuesList(&carryFreqValuesList);
    carryFreqNumPicker.setCycled(true);

    modFreqNumPicker.setValueIndex(2);
    modFreqNumPicker.setValuesList(&modFreqValuesList);
    modFreqNumPicker.setCycled(true);

    deviationNumPicker.setValueIndex(10);
    deviationNumPicker.setValuesList(&deviationValuesList);
    deviationNumPicker.setCycled(true);

	currentCarryFreq = 1000;
	currentModFreq = 100;
	currentDeviation = 10;

    codecHardwareReset();
}

void codecTestScreenView::tearDownScreen()
{
    codecTestScreenViewBase::tearDownScreen();
}

void codecTestScreenView::codecTestControlBtnPressed()
{
	if(isSineTestSelected){
		if(!isSineTestStarted){
			isSineTestStarted = true;
			testControlBtn.setLabelText(touchgfx::TypedText(T_CODECSTOPTESTBTNLABEL));
			testControlBtn.invalidate();
			// disable radio buttons
			sineTestRadioButton.setTouchable(false);
			pcmTestRadioButton.setTouchable(false);

			startSineTest();
			setVolume(volumeLeft, volumeRight);
		}
		else{
			isSineTestStarted = false;
			testControlBtn.setLabelText(touchgfx::TypedText(T_CODECSTARTTESTBTNLABEL));
			testControlBtn.invalidate();
			// enable radio buttons
			sineTestRadioButton.setTouchable(true);
			pcmTestRadioButton.setTouchable(true);

			stopSineTest();
		}
	}
	if(isPCMTestSelected){
		if(!isPCMTestStarted){
			startPCMTest();
		}else{
			stopPCMTest();
		}
	}
}

void codecTestScreenView::backButtonPressed()
{
	if(isSineTestSelected){
		isSineTestSelected = false;
		stopSineTest();
	}
	if(isPCMTestSelected){
		isPCMTestSelected = false;
		stopPCMTest();
	}
	isPCMTestStarted = false;
	isSineTestStarted = false;
	application().gotomain_screenScreenNoTransition();
}

void codecTestScreenView::volumeLeftSB_valueChanged(int value)
{
	volumeLeft = 254-value;
	if(isSineTestSelected){
		setVolume(volumeLeft, volumeRight);
	}
	if(isPCMTestSelected){
		cmd.command = SET_VOLUME;
		cmd.volume = 254-value;
		sendCommand(&cmd);
	}
}

void codecTestScreenView::volumeRightSB_valueChanged(int value)
{
	volumeRight = 254-value;
	if(isSineTestSelected){
		setVolume(volumeLeft, volumeRight);
	}
}

void codecTestScreenView::carryFreqNumPickerValueChange(uint8_t value)
{
	currentCarryFreq = carryFreqValues[value];
}

void codecTestScreenView::modFreqNumPickerValueChanged(uint8_t value)
{
	currentModFreq = modFreqValues[value];
}

void codecTestScreenView::deviationNumPickerValueChanged(uint8_t value)
{
	currentDeviation = deviationValue[value];
}

void codecTestScreenView::sineTestRBSelected()
{
	isSineTestSelected = true;
	isPCMTestSelected = false;

    carryFreqNumPicker.setVisible(false);
    modFreqNumPicker.setVisible(false);
    deviationNumPicker.setVisible(false);
    textArea6.setVisible(false);
    textArea7.setVisible(false);
    textArea8.setVisible(false);
	//show slider for right channel volume regulation
	volumeRightSB.setVisible(true);
	textArea2.setVisible(true);

	volumeRightSB.invalidate();
	textArea2.invalidate();
    carryFreqNumPicker.invalidate();
    modFreqNumPicker.invalidate();
    deviationNumPicker.invalidate();
    textArea6.invalidate();
    textArea7.invalidate();
    textArea8.invalidate();
}

void codecTestScreenView::pcmTestRBSelected()
{
	isSineTestSelected = false;
	isPCMTestSelected = true;

    carryFreqNumPicker.setVisible(true);
    modFreqNumPicker.setVisible(true);
    deviationNumPicker.setVisible(true);
    textArea6.setVisible(true);
    textArea7.setVisible(true);
    textArea8.setVisible(true);
	// hide unused slider for right channel volume regulation
	volumeRightSB.setVisible(false);
	textArea2.setVisible(false);

	volumeRightSB.invalidate();
	textArea2.invalidate();
    carryFreqNumPicker.invalidate();
    modFreqNumPicker.invalidate();
    deviationNumPicker.invalidate();
    textArea6.invalidate();
    textArea7.invalidate();
    textArea8.invalidate();
}

void codecTestScreenView::startPCMTest()
{
	int16_t sampleVal = 0;
	float carryF = (float)currentCarryFreq*M_PI/96000;
	float modF = (float)currentModFreq*M_PI/96000;
	float dev = (float)currentCarryFreq*currentDeviation/(200*currentModFreq);
	// fill stimulus data array
	for(uint16_t i = 0; i < sizeof(FM_stimulData); i += 4){
		sampleVal = (int16_t)(0x7FFF*sin(carryF*i + dev*sin(modF*i)));
		// byte order: | left channel LSB, left channel MSB | right channel LSB, right channel MSB |
		FM_stimulData[i] = FM_stimulData[i + 2] = (uint8_t)(sampleVal & 0xFF); // LSB's
		FM_stimulData[i + 1] = FM_stimulData[i + 3] = (uint8_t)((sampleVal>>8) & 0xFF); // MSB's
	}
	setPCMDataBuffer(FM_stimulData);
	cmd = *getCommand();
	cmd.command = START_PCM_TEST;
	cmd.isExternalStimulator = 0;
	cmd.volume = 254 - (uint8_t)volumeLeftSB.getValue();
	sendCommand(&cmd);
}

void codecTestScreenView::stopPCMTest()
{
	cmd = *getCommand();
	cmd.command = STOP_PCM_TEST;
	sendCommand(&cmd);
}
