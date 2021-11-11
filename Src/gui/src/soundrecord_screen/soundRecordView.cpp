#include <gui/soundrecord_screen/soundRecordView.hpp>

soundRecordView::soundRecordView()
{

}

void soundRecordView::handleTickEvent()
{
	char timeStr[24] = {0};
	char fileErrorCode[24] = {0};
	fileCommand = *getCommand();

	switch(fileCommand.status){
		case STATUS_IDLE:
			if(isRecording){
				recordCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
				recordCtrlButton.setIconXY(18, 14);
				recordCtrlButton.invalidate();
				//enable radio buttons
				micRadioButton.setTouchable(true);
				lineRadioButton.setTouchable(true);
			}
			isRecording = false;
			setStatusString("");
			recordingTimeInSec = 0;
			break;

		case PCM_RECORDING_STARTED:
			if(!isRecording || isRecordingPaused){
				recordCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID), Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID));
				recordCtrlButton.setIconXY(17, 14);
				recordCtrlButton.invalidate();
			}
			// disable radio buttons
			if(!isRecording){
				micRadioButton.setTouchable(false);
				lineRadioButton.setTouchable(false);
			}
			isRecording = true;
			isRecordingPaused = false;
			recordingTimeInSec = fileCommand.recordingDataSize/byterate;
			sprintf(timeStr, "%02d:%02d", recordingTimeInSec/60, recordingTimeInSec%60);
			setStatusString((const char*)timeStr);
			break;

		case PCM_RECORDING_PAUSED:
			if(!isRecordingPaused){
				recordCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
				recordCtrlButton.setIconXY(18, 14);
				recordCtrlButton.invalidate();
				//enable radio buttons
				micRadioButton.setTouchable(true);
				lineRadioButton.setTouchable(true);
			}
			isRecordingPaused = true;

			break;

		case FILE_ERROR:
			sprintf(fileErrorCode, "%s", fileErrorsNames[fileCommand.fileErrorCode].c_str());
			setStatusString(fileErrorCode);
			fileCommand.status = STATUS_IDLE; //reset file error status
			fileCommand.fileErrorCode = 0;
			sendCommand(&fileCommand);
			if(isRecording){
				recordCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
				recordCtrlButton.setIconXY(18, 14);
				recordCtrlButton.invalidate();
			}
			isRecording = false;
			recordingTimeInSec = 0;
			break;

		case FILE_OPENED:
		case FILE_READING:
		case FILE_PAUSED:
		case FILE_STOPPED:
		case PCM_TEST_STARTED:
			// avoid warning during compilation
			break;
	}
}

void soundRecordView::setupScreen()
{
    soundRecordViewBase::setupScreen();
    sdState sd_st;
    presenter->getSDStatus(&sd_st);
    if(sd_st.isMounted == FS_IS_MOUNTED){
    	isSDMounted = true;
    }else{
    	isSDMounted = false;
    	setStatusString("FS isn't mounted");
    }
    fileCommand = *getCommand();
    codecHardwareReset();

    selectInput(0);

    monitoringVolume = monitoringVolumeSlider.getValue();
    setMonitoringVolume(monitoringVolume);

    micGainPicker.setValueIndex(3);
    micGainPicker.setValuesList(&micGainValuesList);
    micGainPicker.setCycled(false);

    samplingFreqPicker.setValueIndex(3);
    samplingFreqPicker.setValuesList(&sampleFreqValuesList);
    samplingFreqPicker.setCycled(true);
}

void soundRecordView::tearDownScreen()
{
    soundRecordViewBase::tearDownScreen();
}

void soundRecordView::micRadioButtonSelected()
{
	if(isSDMounted && !isRecording){
		inputType = 0;
	}
}

void soundRecordView::lineRadioButtonSelected()
{
	if(isSDMounted && !isRecording){
		inputType = 1;
	}
}

void soundRecordView::onBackButtonPressed()
{
	if(isRecording){
		fileCommand.command = STOP_PCM_RECORDING;
		sendCommand(&fileCommand);
	}
	application().gotomain_screenScreenNoTransition();
}

void soundRecordView::recordCtrlButtonPressed()
{
	if(isSDMounted){
		//hide sampling frequency picker
		textArea6.setVisible(false);
		samplingFreqPicker.setVisible(false);
		textArea6.invalidate();
		samplingFreqPicker.invalidate();

		if(!isRecording){
			//init recording params
			recParams.sample_rate = samplingFreq; // sampling frequency
			recParams.micGain = micGain; // mic gain
			recParams.maxAutoGain = (micGain == 0) ? (4096) : (0); // max auto gain 1
			recParams.channelType = inputType + 2; // left channel mono, if input is mic, or right channel mono, if input is line2
			recParams.pcmMode = 1; // linear PCM
			setRecordingParams(&recParams);

			byterate = recParams.sample_rate*2*((recParams.channelType > 1) ? (1) : (2)); // define byterate for recording time value

			fileCommand.command = START_PCM_RECORDING;
		}else{
			if(!isRecordingPaused){
				fileCommand.command = PAUSE_PCM_RECORDING;
			}else{
				fileCommand.command = RESUME_PCM_RECORDING;
			}
		}
		sendCommand(&fileCommand);
	}
}

void soundRecordView::stopButtonPressed()
{
	if(isSDMounted && isRecording){
		//show sampling frequency picker
		textArea6.setVisible(true);
		samplingFreqPicker.setVisible(true);
		textArea6.invalidate();
		samplingFreqPicker.invalidate();

		fileCommand.command = STOP_PCM_RECORDING;
		sendCommand(&fileCommand);
	}
}

void soundRecordView::monitoringSliderValueChanged(int value)
{
	setMonitoringVolume(value);
	monitoringVolume = value;
}

void soundRecordView::micGainPickerValueChanged(uint8_t value)
{
	if(isSDMounted){
		micGain = gainValues[value];
		fileCommand.command = SET_MIC_GAIN;
		fileCommand.micGain = micGain;
		sendCommand(&fileCommand);
	}
}

void soundRecordView::sampleFreqGainPickerValueChanged(uint8_t value)
{
	if(isSDMounted && !isRecording){
		samplingFreq = sampleFreqs[value];
	}
}

void soundRecordView::setMonitoringVolume(int volume)
{
	uint8_t db6_steps = 0, db05_steps = 0;

	db6_steps = (uint8_t)(14 - volume/12);
	db05_steps = (uint8_t)(volume%12);

	fileCommand.command = SET_MONITORING_VOLUME;
	fileCommand.monitoringVolume = ((db05_steps & 0x0F)<<4)|(db6_steps & 0x0F);
	sendCommand(&fileCommand);
}

void soundRecordView::setStatusString(const char* str)
{
	int x_pos = 0;

	Unicode::fromUTF8((uint8_t*)str, recordStatusTextBuffer, RECORDSTATUSTEXT_SIZE);
	recordStatusText.setWildcard(recordStatusTextBuffer);
	x_pos = (box1.getWidth() - recordStatusText.getTextWidth())/2;
	recordStatusText.setX(x_pos);

	recordStatusText.invalidate();
	box2.invalidate();
	recordStatusText.resizeToCurrentText();
	recordStatusText.invalidate();
}
