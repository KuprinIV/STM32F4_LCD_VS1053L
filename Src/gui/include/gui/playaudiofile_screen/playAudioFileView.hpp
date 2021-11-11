#ifndef PLAYAUDIOFILEVIEW_HPP
#define PLAYAUDIOFILEVIEW_HPP

#include <gui_generated/playaudiofile_screen/playAudioFileViewBase.hpp>
#include <gui/playaudiofile_screen/playAudioFilePresenter.hpp>
#include <gui/containers/playerSettings.hpp>
#include "vs1053.h"
#include "fatfs.h"
#include "main.h"
#include "w25q128.h"
#include "BitmapDatabase.hpp"
#include <stdio.h>
#include <string.h>
#include <main.h>

class playAudioFileView : public playAudioFileViewBase
{
public:
    playAudioFileView();
    virtual ~playAudioFileView();
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void onBackButtonPressed();
    virtual void onPrevButtonPressed();
    virtual void onPlayCtrlBtnPressed();
    virtual void onNextButtonPressed();
    virtual void playSettingButtonPressed();
    virtual void closeModalWindowButtonPressed();
    virtual void volumeSliderValueChanged(int value);
    virtual void onFileProgressValueChanged(int value);
    virtual void onFileProgressChangeInited(int value);
    virtual void handleTickEvent();
protected:
    void setFileNameString(const char* text);

private:
    char currentFilePath[255];
    bool isFileOpened = false;
    bool isFilePaused = false;
    bool isPlaying = false;
    bool isStoppedDuringPlay = false;
    bool isFileRewindStarted = false;
    commandStruct fileCommand;
    std::vector<string*> audioFilesList;
    unsigned int filePos = 0;
    uint8_t volumeVal = 240;
    int bitrate = 0;
    DIR currentDIR;
    uint16_t playSettings[2] = {0};

    void getFileName(char* path);
    void getAudioFilesList(void);
    void setPlayedTime(int sec);
    void setDurationTime(int sec);
    void onPlaySettingsChanged(uint16_t* val);
    touchgfx::Callback<playAudioFileView, uint16_t*> playSettingsChangedCallback;
};

#endif // PLAYAUDIOFILEVIEW_HPP
