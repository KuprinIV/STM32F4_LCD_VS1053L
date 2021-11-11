/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef EXTERNALSTIMULATORVIEWBASE_HPP
#define EXTERNALSTIMULATORVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/externalstimulator_screen/externalStimulatorPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/containers/numPicker.hpp>

class externalStimulatorViewBase : public touchgfx::View<externalStimulatorPresenter>
{
public:
    externalStimulatorViewBase();
    virtual ~externalStimulatorViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void backButtonPressed()
    {
        // Override and implement this function in externalStimulator
    }

    virtual void testCtrlButtonPressed()
    {
        // Override and implement this function in externalStimulator
    }

    virtual void maxVolumeSliderValueChanged(int value)
    {
        // Override and implement this function in externalStimulator
    }

    virtual void stimTypePickerValueChanged(uint8_t value)
    {
        // Override and implement this function in externalStimulator
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
    touchgfx::TextArea textArea1;
    touchgfx::Slider maxVolumeSlider;
    touchgfx::ButtonWithLabel backButton;
    touchgfx::ButtonWithLabel testCtrlButton;
    touchgfx::BoxProgress testProgressBar;
    touchgfx::Box box2;
    touchgfx::TextAreaWithOneWildcard testProgressText;
    numPicker stimTypePicker;
    touchgfx::TextArea textArea2;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TESTPROGRESSTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar testProgressTextBuffer[TESTPROGRESSTEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<externalStimulatorViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<externalStimulatorViewBase, const touchgfx::Slider&, int> sliderValueChangedCallback;
    touchgfx::Callback<externalStimulatorViewBase, uint8_t> stimTypePickerPickerValueChangedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);
    void stimTypePickerPickerValueChangedCallbackHandler(uint8_t value);

};

#endif // EXTERNALSTIMULATORVIEWBASE_HPP
