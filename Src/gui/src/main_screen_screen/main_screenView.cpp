#include <gui/main_screen_screen/main_screenView.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "stm32f4xx_hal.h"
#include "fatfs.h"

int backlightSB_value = 0;

main_screenView::main_screenView()
{
	if(backlightSB_value == 0){
		backlightSB_value = backlightSB.getValue();
	}
	else{
		backlightSB.setValue(backlightSB_value);
	}
	presenter->getSDStatus(&sd_state);
	sd_state_prev = sd_state;
}

void main_screenView::setupScreen()
{
    main_screenViewBase::setupScreen();
    presenter->getSDStatus(&sd_state);
    sd_state_prev = sd_state;
    sdCardIcon.setVisible(sd_state.isMounted == FS_IS_MOUNTED);
}

void main_screenView::tearDownScreen()
{
    main_screenViewBase::tearDownScreen();
}

void main_screenView::handleTickEvent()
{
	FRESULT res = FR_OK;

	presenter->getSDStatus(&sd_state);

	if(sd_state.isConnected != sd_state_prev.isConnected){
		sd_state_prev = sd_state;
		if(sd_state.isConnected == SD_IS_CONNECTED){
			res = FAT_FS_Mount();
			if(res == FR_OK){
				sdCardIcon.setVisible(true);
				sd_state.isMounted = FS_IS_MOUNTED;
				presenter->setSDStatus(&sd_state);
			}
			else{
				showModalWindow("FS mount error");
				modalWndShowConter = 25;
			}
		}else{
			res = FAT_FS_Unmount();
			if(res != FR_OK){
				showModalWindow("FS unmount error");
				modalWndShowConter = 25;
			}
			sdCardIcon.setVisible(false);
			sd_state.isMounted = FS_NOT_MOUNTED;
			presenter->setSDStatus(&sd_state);
		}
		sdCardIcon.invalidate();
	}
	if(modalWndShowConter > 0){
		modalWndShowConter--;
	}else if(modalWndShowConter == 0){
		closeModalWindow();
		modalWndShowConter = -1;
	}

}

void main_screenView::goToSoundRecordScreen()
{
	if(sd_state.isMounted == FS_IS_MOUNTED){
		application().gotosoundRecordScreenNoTransition();
	}else{
		showModalWindow("FS isn't mounted");
	}
}

void main_screenView::goToSDFileViewerScreen()
{
	if(sd_state.isMounted == FS_IS_MOUNTED){
		application().gototestFatFS_screenScreenNoTransition();
	}else{
		showModalWindow("FS isn't mounted");
	}
}

void main_screenView::backlightSB_valueChanged(int value)
{
	backlightSB_value = value;
	TIM2->CCR1 = value;
    Unicode::snprintf(textArea2Buffer, TEXTAREA2_SIZE, "%2d", value);
    textArea2.setWildcard(textArea2Buffer);
    textArea2.invalidate();
    textArea2.resizeToCurrentText();
    textArea2.invalidate();
}

void main_screenView::showModalWindow(const char* msg)
{
	int x_pos = 0;
	// draw message
	Unicode::fromUTF8((const uint8_t*)msg, modalWindowMessageTextBuffer, MODALWINDOWMESSAGETEXT_SIZE);
	modalWindowMessageText.setWildcard(modalWindowMessageTextBuffer);
	x_pos =  (backgroundBox.getWidth() - modalWindowMessageText.getTextWidth())/2; // align status text to center
	modalWindowMessageText.setX(x_pos);
	modalWindowMessageText.invalidate();
	modalWindowMessageText.resizeToCurrentText();
	modalWindowMessageText.invalidate();
	// set visible
	modalWindow.show();
	closeModalWndButton.setVisible(true);
	modalWindowMessageText.setVisible(true);
}

void main_screenView::closeModalWindow()
{
	modalWindow.hide();
	closeModalWndButton.setVisible(false);
	modalWindowMessageText.setVisible(false);
}

