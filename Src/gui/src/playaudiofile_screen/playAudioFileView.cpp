#include <gui/playaudiofile_screen/playAudioFileView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

playAudioFileView::playAudioFileView() : playSettingsChangedCallback(this, &playAudioFileView::onPlaySettingsChanged)
{
	fileCommand = *getCommand();
}

playAudioFileView::~playAudioFileView()
{
	for(string* f : audioFilesList){
		delete f;
	}
	audioFilesList.clear();
	// close currently opened file
	f_closedir(&currentDIR);
	fileCommand.command = STOP_READ_FILE;
	fileCommand.bitrate = 0;
	sendCommand(&fileCommand);
}

void playAudioFileView::handleTickEvent()
{
	uint16_t pb_max = 0;

	fileCommand = *getCommand();
	switch(fileCommand.status){
		case STATUS_IDLE:
		default:
			if((filePos < (audioFilesList.size() - 1)) && isPlaying){
				onNextButtonPressed();
			}else{
				if(isPlaying){
					playControlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
					playControlButton.setIconXY(18, 14);
					playControlButton.invalidate();
				}
				isPlaying = false;
				onNextButtonPressed();
			}
			break;

		case FILE_OPENED:
			isFileOpened = true;
			if(isStoppedDuringPlay && !isFilePaused){
				isStoppedDuringPlay = false;
				fileCommand.command = START_READ_FILE;
				sendCommand(&fileCommand);
			}
			break;

		case FILE_READING:
			if(!isPlaying || isFilePaused){
				playControlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID), Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID));
				playControlButton.setIconXY(17, 14);
				playControlButton.invalidate();
			}
			isPlaying = true;
			isFilePaused = false;
			if(fileCommand.bitrate != 0){
				bitrate = (int)fileCommand.bitrate;
				setDurationTime((int)((float)fileCommand.fileSize/((float)bitrate)));
			}
			break;

		case FILE_PAUSED:
			if(!isFilePaused){
				playControlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
				playControlButton.setIconXY(18, 14);
				playControlButton.invalidate();
			}
			isFilePaused = true;
			break;

		case FILE_STOPPED:
			isFileOpened = false;

			setDurationTime(-1);
			setPlayedTime(-1);
			bitrate = 0;

			if(isStoppedDuringPlay && !isFilePaused){
//				playControlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID), Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID));
//				playControlButton.setIconXY(17, 14);
//				playControlButton.invalidate();
			}else{
				if(isPlaying){
					playControlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
					playControlButton.setIconXY(18, 14);
					playControlButton.invalidate();
				}
				isPlaying = false;
			}
			fileCommand.command = OPEN_FILE;
			sendCommand(&fileCommand);
			break;

		case FILE_ERROR:
			if(isPlaying){
				playControlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
				playControlButton.setIconXY(18, 14);
				playControlButton.invalidate();
			}
			isPlaying = false;
			isFileOpened = false;
			break;
	}

	switch(fileCommand.rewindStatus){
		case REWIND_SUCCESSFULL:
			isFileRewindStarted = false;
			break;

		case REWIND_NOT_ALLOWED:
			break;

		case REWIND_ERROR:
			break;
	}
	resetRewindStatus();
	// update file playing progress bar
	pb_max = playingFileProgress.getMaxValue();
	if(pb_max != (uint16_t)(fileCommand.fileSize>>16) && fileCommand.fileSize > 0){
		playingFileProgress.setValueRange(0, fileCommand.fileSize>>16);
	}
	if(fileCommand.fileSize > 0 && !isFileRewindStarted){
		playingFileProgress.setValue(fileCommand.playedDataSize>>16);
		playingFileProgress.invalidate();
		if(bitrate != 0){
			setPlayedTime((int)((float)fileCommand.playedDataSize/((float)bitrate)));
		}
	}else if(isFileRewindStarted && isPlaying){
		setPlayedTime((int)((float)(playingFileProgress.getValue()<<16)/(float)bitrate));
	}
}

void playAudioFileView::setupScreen()
{
	uint8_t settings_data[4] = {0};

    playAudioFileViewBase::setupScreen();
    isPlaying = false;
    // get opened file name and path
    presenter->getCurrentFileToOpen(currentFilePath);
    getFileName(currentFilePath);
    // get audio files list in current directory
    presenter->getCurrentDIRObject(&currentDIR);
    getAudioFilesList();
    // open chosen file
    setCurrentFilePath(currentFilePath);
    fileCommand.command = OPEN_FILE;
    sendCommand(&fileCommand);
    // set playing settings changing callback
    playerSettingsWnd.setPlaySettingsChangedCallback(playSettingsChangedCallback);
    // read settings data from flash
    readData(0, settings_data, 4);
    if(!((settings_data[0] == 0xFF) && (settings_data[1] == 0xFF) && (settings_data[2] == 0xFF) && (settings_data[3] == 0xFF))){ // flash isn't erased
    	memcpy(playSettings, settings_data, 4);
    }
    playerSettingsWnd.initSettings(playSettings);
}

void playAudioFileView::tearDownScreen()
{
    playAudioFileViewBase::tearDownScreen();
}

void playAudioFileView::onBackButtonPressed()
{
	fileCommand = *getCommand();
	if(fileCommand.status == FILE_READING || fileCommand.status == FILE_PAUSED){
		fileCommand.command = STOP_READ_FILE;
		fileCommand.bitrate = 0;
		sendCommand(&fileCommand);
	}
	application().gototestFatFS_screenScreenNoTransition();
}

void playAudioFileView::onPrevButtonPressed()
{
		if(isPlaying){
			isStoppedDuringPlay = true;
		}
		if(filePos > 0){
			filePos--;
		}else{
			filePos = audioFilesList.size() - 1;
		}
		memset(currentFilePath, 0, sizeof(currentFilePath));
		sprintf(currentFilePath, "%s", audioFilesList.at(filePos)->c_str());
		getFileName(currentFilePath);
		// close file
		fileCommand.command = STOP_READ_FILE;
		fileCommand.bitrate = 0;
		sendCommand(&fileCommand);
		setCurrentFilePath(currentFilePath);
}

void playAudioFileView::onPlayCtrlBtnPressed()
{
	if(!isFileOpened) return;

	if(!isPlaying){
		isFilePaused = false;
		//send command to play file
		fileCommand.command = START_READ_FILE;
		fileCommand.volume = 254 - volumeVal;
	}else{
		if(isFilePaused){
			fileCommand.command = RESUME_READ;
		}else{
			fileCommand.command = PAUSE_READ;
		}
	}
	sendCommand(&fileCommand);
}

void playAudioFileView::onNextButtonPressed()
{
	if(isPlaying){
		isStoppedDuringPlay = true;
	}
	if(filePos < audioFilesList.size() - 1){
		filePos++;
	}else{
		filePos = 0;
	}
	memset(currentFilePath, 0, sizeof(currentFilePath));
	sprintf(currentFilePath, "%s", audioFilesList. at(filePos)->c_str());
	getFileName(currentFilePath);
	// close file
	fileCommand.command = STOP_READ_FILE;
	fileCommand.bitrate = 0;
	sendCommand(&fileCommand);
	setCurrentFilePath(currentFilePath);
}

void playAudioFileView::volumeSliderValueChanged(int value)
{
	volumeVal = value;
	fileCommand.command = SET_VOLUME;
	fileCommand.volume = 254 - value;
	sendCommand(&fileCommand);
}

void playAudioFileView::onFileProgressValueChanged(int value)
{
	fileCommand.command = REWIND_FILE;
	fileCommand.rewindOffset = value;
	sendCommand(&fileCommand);
}

void playAudioFileView::onFileProgressChangeInited(int value)
{
	isFileRewindStarted = true;
}

void playAudioFileView::playSettingButtonPressed()
{
	playerSettingsWnd.setVisible(true);
	closeModalWindowButton.setVisible(true);
	modalWindow.show();
}

void playAudioFileView::closeModalWindowButtonPressed()
{
	uint8_t settings_data[4] = {0};
	uint16_t settings[2] = {0};
	// save playing setting in flash
	readData(0, settings_data, 4);
	memcpy(settings, settings_data, 4);
	if((settings[0] != fileCommand.bass_treble_settings[0]) || (settings[1] != fileCommand.bass_treble_settings[1])){
		memcpy(settings_data, fileCommand.bass_treble_settings, 4);
		eraseSector(SECTOR_ERASE_4KB, 0);
		while(isFlashBusy() == 1){}
		writePage(0, settings_data, 4);
	}
	// hide settings window
	playerSettingsWnd.setVisible(false);
	closeModalWindowButton.setVisible(false);
	modalWindow.hide();
}

void playAudioFileView::setFileNameString(const char* text)
{
	int x_pos = 0;

	Unicode::fromUTF8((const uint8_t*)text, fileNameBuffer, FILENAME_SIZE);
	fileName.setWildcard(fileNameBuffer);

	x_pos = (box1.getWidth() - fileName.getTextWidth())/2; // align status text to center
	fileName.setX(x_pos);

	fileName.invalidate();
	box1.invalidate();
	fileName.resizeToCurrentText();
	fileName.invalidate();
}

void playAudioFileView::getFileName(char* path)
{
	char file_name_tmp[255] = {0};
	char file_name[20] = {0};
	int x_pos = 0;

	sprintf(file_name_tmp, "%s", strrchr(path, '/') + 1);

	if(strlen(file_name_tmp) <= 19){
		strcpy(file_name, file_name_tmp);
	}else{
		//copy format
		strcpy(file_name + 19 - strlen(strrchr(file_name_tmp, '.')), strrchr(file_name_tmp, '.'));
		for(unsigned int i = 0; i < 19 - strlen(strrchr(file_name_tmp, '.')) - 1; i++)
		{
			file_name[i] = file_name_tmp[i];
		}
		file_name[19 - strlen(strrchr(file_name_tmp, '.'))-1] = '~';
	}

	Unicode::fromUTF8((const uint8_t*)file_name, fileNameBuffer, FILENAME_SIZE);
	fileName.setWildcard(fileNameBuffer);

	x_pos = (box1.getWidth() - fileName.getTextWidth())/2; // align status text to center
	fileName.setX(x_pos);

	fileName.invalidate();
	box1.invalidate();
	fileName.resizeToCurrentText();
	fileName.invalidate();
}

void playAudioFileView::getAudioFilesList(void)
{
	FRESULT res;
	FILINFO fno;
	char *fn;
	char folder_path[255] = {0};
	char full_path[255] = {0};
	string* temp;

	presenter->getCurrentFolderPath(folder_path);
	//fill audio files list
    for (;;) {
        res = f_readdir(&currentDIR, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fname[0] == '.') continue;             /* Ignore dot entry */

        fn = fno.fname;

        if (fno.fattrib & AM_HID || fno.fattrib & AM_DIR) continue;             /* Ignore hidden directories and files*/

        if (!(fno.fattrib & AM_DIR)) {                    /* It is a file */
        	if(strstr(fn, ".mp3") != NULL || strstr(fn, ".wav") != NULL){
        		sprintf(full_path, "%s/%s", folder_path, fn);
        		temp = new string(full_path);
        		audioFilesList.push_back(temp);
        	}
        }
    }
    //get opened file position in list
    filePos = 0;
    for(string* f : audioFilesList){
    	if(strstr(f->c_str(), currentFilePath) != NULL){
    		break;
    	}
    	filePos++;
    }
}

void playAudioFileView::setPlayedTime(int sec)
{
	int hours = 0, mins = 0, secs = 0;

	if(sec < 0){
		Unicode::snprintf(playingTimeTextBuffer, PLAYINGTIMETEXT_SIZE, "--:--");
	}else if(sec < 3600){
		Unicode::snprintf(playingTimeTextBuffer, PLAYINGTIMETEXT_SIZE, "%02d:%02d", sec/60, sec%60);
	}else{
		hours = sec/3600;
		mins = (sec - hours*3600)/60;
		secs = sec - hours*3600 - mins*60;
		Unicode::snprintf(playingTimeTextBuffer, PLAYINGTIMETEXT_SIZE, "%02d:%02d:%02d", hours, mins, secs);
	}
	playingTimeText.setWildcard(playingTimeTextBuffer);
	playingTimeText.invalidate();
	playingTimeText.resizeToCurrentText();
	playingTimeText.invalidate();
}

void playAudioFileView::setDurationTime(int sec)
{
	int x_pos = 0;
	int hours = 0, mins = 0, secs = 0;

	if(sec < 0){
		Unicode::snprintf(fileDurationTextBuffer, FILEDURATIONTEXT_SIZE, "--:--");
	}else if(sec < 3600){
		Unicode::snprintf(fileDurationTextBuffer, FILEDURATIONTEXT_SIZE, "%02d:%02d", sec/60, sec%60);
	}else{
		hours = sec/3600;
		mins = (sec - hours*3600)/60;
		secs = sec - hours*3600 - mins*60;
		Unicode::snprintf(fileDurationTextBuffer, FILEDURATIONTEXT_SIZE, "%02d:%02d:%02d", hours, mins, secs);
	}
	fileDurationText.setWildcard(fileDurationTextBuffer);

	x_pos = 240 - fileDurationText.getTextWidth();
	fileDurationText.setX(x_pos);

	fileDurationText.invalidate();
	playTimeContainer.invalidate();
	fileDurationText.resizeToCurrentText();
	fileDurationText.invalidate();
}

void playAudioFileView::onPlaySettingsChanged(uint16_t* val)
{
	fileCommand.command = SET_BASS_TREBLE;
	fileCommand.bass_treble_settings[0] = val[0];
	fileCommand.bass_treble_settings[1] = val[1];
	sendCommand(&fileCommand);
}
