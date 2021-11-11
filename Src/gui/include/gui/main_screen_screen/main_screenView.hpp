#ifndef MAIN_SCREENVIEW_HPP
#define MAIN_SCREENVIEW_HPP

#include <gui_generated/main_screen_screen/main_screenViewBase.hpp>
#include <gui/main_screen_screen/main_screenPresenter.hpp>
#include "main.h"

class main_screenView : public main_screenViewBase
{
public:
    main_screenView();
    virtual ~main_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void goToSoundRecordScreen();
    virtual void goToSDFileViewerScreen();
    virtual void backlightSB_valueChanged(int value);
    virtual void closeModalWindow();
    virtual void handleTickEvent();

protected:
    sdState sd_state;
    sdState sd_state_prev;
    int modalWndShowConter = -1;

private:
    void showModalWindow(const char* msg);
};

#endif // MAIN_SCREENVIEW_HPP
