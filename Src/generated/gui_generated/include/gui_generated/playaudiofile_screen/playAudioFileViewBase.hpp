/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef PLAYAUDIOFILEVIEWBASE_HPP
#define PLAYAUDIOFILEVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/playaudiofile_screen/playAudioFilePresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/Color.hpp>
#include <gui/containers/playerSettings.hpp>

class playAudioFileViewBase : public touchgfx::View<playAudioFilePresenter>
{
public:
    playAudioFileViewBase();
    virtual ~playAudioFileViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void onPlayCtrlBtnPressed()
    {
        // Override and implement this function in playAudioFile
    }

    virtual void onBackButtonPressed()
    {
        // Override and implement this function in playAudioFile
    }

    virtual void onPrevButtonPressed()
    {
        // Override and implement this function in playAudioFile
    }

    virtual void onNextButtonPressed()
    {
        // Override and implement this function in playAudioFile
    }

    virtual void volumeSliderValueChanged(int value)
    {
        // Override and implement this function in playAudioFile
    }

    virtual void onFileProgressValueChanged(int value)
    {
        // Override and implement this function in playAudioFile
    }

    virtual void onFileProgressChangeInited(int value)
    {
        // Override and implement this function in playAudioFile
    }

    virtual void playSettingButtonPressed()
    {
        // Override and implement this function in playAudioFile
    }

    virtual void closeModalWindowButtonPressed()
    {
        // Override and implement this function in playAudioFile
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box box1;
    touchgfx::ButtonWithLabel backButton;
    touchgfx::IconButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > playControlButton;
    touchgfx::IconButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > nextButton;
    touchgfx::IconButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > prevButton;
    touchgfx::IconButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > > flexButton1;
    touchgfx::TextAreaWithOneWildcard fileName;
    touchgfx::Slider volumeSlider;
    touchgfx::TextArea textArea1;
    touchgfx::Slider playingFileProgress;
    touchgfx::Container playTimeContainer;
    touchgfx::TextAreaWithOneWildcard playingTimeText;
    touchgfx::TextAreaWithOneWildcard fileDurationText;
    touchgfx::IconButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > > playingSettingsButton;
    touchgfx::ModalWindow modalWindow;
    playerSettings playerSettingsWnd;
    touchgfx::TextButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > > closeModalWindowButton;

    /*
     * Wildcard Buffers
     */
    static const uint16_t FILENAME_SIZE = 20;
    touchgfx::Unicode::UnicodeChar fileNameBuffer[FILENAME_SIZE];
    static const uint16_t PLAYINGTIMETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar playingTimeTextBuffer[PLAYINGTIMETEXT_SIZE];
    static const uint16_t FILEDURATIONTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar fileDurationTextBuffer[FILEDURATIONTEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<playAudioFileViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<playAudioFileViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;
    touchgfx::Callback<playAudioFileViewBase, const touchgfx::Slider&, int> sliderValueStartedChangeCallback;
    touchgfx::Callback<playAudioFileViewBase, const touchgfx::Slider&, int> sliderValueChangedCallback;
    touchgfx::Callback<playAudioFileViewBase, const touchgfx::Slider&, int> sliderValueConfirmedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void sliderValueStartedChangeCallbackHandler(const touchgfx::Slider& src, int value);
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);
    void sliderValueConfirmedCallbackHandler(const touchgfx::Slider& src, int value);

};

#endif // PLAYAUDIOFILEVIEWBASE_HPP
