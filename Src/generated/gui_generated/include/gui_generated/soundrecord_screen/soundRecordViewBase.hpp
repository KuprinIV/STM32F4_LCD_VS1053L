/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SOUNDRECORDVIEWBASE_HPP
#define SOUNDRECORDVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/soundrecord_screen/soundRecordPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/RadioButton.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/Slider.hpp>
#include <gui/containers/numPicker.hpp>
#include <touchgfx/widgets/RadioButtonGroup.hpp>

class soundRecordViewBase : public touchgfx::View<soundRecordPresenter>
{
public:
    soundRecordViewBase();
    virtual ~soundRecordViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void micRadioButtonSelected()
    {
        // Override and implement this function in soundRecord
    }

    virtual void lineRadioButtonSelected()
    {
        // Override and implement this function in soundRecord
    }

    virtual void onBackButtonPressed()
    {
        // Override and implement this function in soundRecord
    }

    virtual void recordCtrlButtonPressed()
    {
        // Override and implement this function in soundRecord
    }

    virtual void stopButtonPressed()
    {
        // Override and implement this function in soundRecord
    }

    virtual void monitoringSliderValueChanged(int value)
    {
        // Override and implement this function in soundRecord
    }

    virtual void micGainPickerValueChanged(uint8_t value)
    {
        // Override and implement this function in soundRecord
    }

    virtual void sampleFreqGainPickerValueChanged(uint8_t value)
    {
        // Override and implement this function in soundRecord
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
    touchgfx::Box box2;
    touchgfx::RadioButton micRadioButton;
    touchgfx::TextArea textArea1;
    touchgfx::TextArea textArea2;
    touchgfx::RadioButton lineRadioButton;
    touchgfx::TextArea textArea3;
    touchgfx::ButtonWithLabel backButton;
    touchgfx::IconButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > stopRecordButton;
    touchgfx::IconButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > recordCtrlButton;
    touchgfx::TextAreaWithOneWildcard recordStatusText;
    touchgfx::Slider monitoringVolumeSlider;
    touchgfx::TextArea textArea4;
    touchgfx::TextArea textArea5;
    numPicker micGainPicker;
    touchgfx::TextArea textArea6;
    numPicker samplingFreqPicker;
    touchgfx::RadioButtonGroup<2> radioButtonGroup1;

    /*
     * Wildcard Buffers
     */
    static const uint16_t RECORDSTATUSTEXT_SIZE = 24;
    touchgfx::Unicode::UnicodeChar recordStatusTextBuffer[RECORDSTATUSTEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<soundRecordViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<soundRecordViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;
    touchgfx::Callback<soundRecordViewBase, const touchgfx::Slider&, int> sliderValueChangedCallback;
    touchgfx::Callback<soundRecordViewBase, const touchgfx::AbstractButton&> radioButtonSelectedCallback;
    touchgfx::Callback<soundRecordViewBase, uint8_t> micGainPickerPickerValueChangedCallback;
    touchgfx::Callback<soundRecordViewBase, uint8_t> samplingFreqPickerPickerValueChangedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);
    void radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src);
    void micGainPickerPickerValueChangedCallbackHandler(uint8_t value);
    void samplingFreqPickerPickerValueChangedCallbackHandler(uint8_t value);

};

#endif // SOUNDRECORDVIEWBASE_HPP