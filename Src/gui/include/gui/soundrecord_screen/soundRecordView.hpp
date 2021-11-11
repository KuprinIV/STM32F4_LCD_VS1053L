#ifndef SOUNDRECORDVIEW_HPP
#define SOUNDRECORDVIEW_HPP

#include <gui_generated/soundrecord_screen/soundRecordViewBase.hpp>
#include <gui/soundrecord_screen/soundRecordPresenter.hpp>
#include "BitmapDatabase.hpp"
#include "main.h"
#include "vs1053.h"

class soundRecordView : public soundRecordViewBase
{
public:
    soundRecordView();
    virtual ~soundRecordView() {codecHardwareReset();}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void onBackButtonPressed();
    virtual void micRadioButtonSelected();
    virtual void lineRadioButtonSelected();
    virtual void recordCtrlButtonPressed();
    virtual void stopButtonPressed();
    virtual void monitoringSliderValueChanged(int value);
    virtual void micGainPickerValueChanged(uint8_t value);
    virtual void sampleFreqGainPickerValueChanged(uint8_t value);
    virtual void handleTickEvent();
protected:
    void setStatusString(const char* str);
    void setMonitoringVolume(int volume);
private:
    bool isRecording = false;
    bool isSDMounted = false;
    bool isRecordingPaused = false;
    commandStruct fileCommand;
    recordingParams recParams;
    uint8_t monitoringVolume = 84;
    uint16_t micGain = 512;
    uint16_t samplingFreq = 48000;
    uint8_t inputType = 0; // 0 - mic, 1 - line
    int byterate = 0;
    int recordingTimeInSec = 0;
    std::vector<string> micGainValuesList = {"auto", "1/16", "1/4", "1/2", "1", "2", "4", "8", "16", "32", "64"};
    uint16_t gainValues[11] = {0, 64, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65535};
    std::vector<string> sampleFreqValuesList = {"8000", "16000", "24000", "48000"};
    uint16_t sampleFreqs[4] = {8000, 16000, 24000, 48000};
    string fileErrorsNames[20] = {	"FR_OK",
    		"FR_DISK_ERR",
    		"FR_INT_ERR",
    		"FR_NOT_READY",
    		"FR_NO_FILE",
    		"FR_NO_PATH",
    		"FR_INVALID_NAME",
    		"FR_DENIED",
    		"FR_EXIST",
    		"FR_INVALID_OBJECT",
    		"FR_WRITE_PROTECTED",
    		"FR_INVALID_DRIVE",
    		"FR_NOT_ENABLED",
    		"FR_NO_FILESYSTEM",
    		"FR_MKFS_ABORTED",
    		"FR_TIMEOUT",
    		"FR_LOCKED",
    		"FR_NOT_ENOUGH_CORE",
    		"FR_TOO_MANY_OPEN_FILES",
    		"FR_INVALID_PARAMETER"	};
};

#endif // SOUNDRECORDVIEW_HPP
