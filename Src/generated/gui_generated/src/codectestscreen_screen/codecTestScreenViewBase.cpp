/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/codectestscreen_screen/codecTestScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

codecTestScreenViewBase::codecTestScreenViewBase() :
    buttonCallback(this, &codecTestScreenViewBase::buttonCallbackHandler),
    sliderValueChangedCallback(this, &codecTestScreenViewBase::sliderValueChangedCallbackHandler),
    radioButtonSelectedCallback(this, &codecTestScreenViewBase::radioButtonSelectedCallbackHandler),
    carryFreqNumPickerPickerValueChangedCallback(this, &codecTestScreenViewBase::carryFreqNumPickerPickerValueChangedCallbackHandler),
    modFreqNumPickerPickerValueChangedCallback(this, &codecTestScreenViewBase::modFreqNumPickerPickerValueChangedCallbackHandler),
    deviationNumPickerPickerValueChangedCallback(this, &codecTestScreenViewBase::deviationNumPickerPickerValueChangedCallbackHandler)
{

    __background.setPosition(0, 0, 272, 480);
    __background.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    backgroundBox.setPosition(0, 0, 272, 480);
    backgroundBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 255, 255));

    testControlBtn.setXY(56, 337);
    testControlBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    testControlBtn.setLabelText(touchgfx::TypedText(T_CODECSTARTTESTBTNLABEL));
    testControlBtn.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    testControlBtn.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    testControlBtn.setAction(buttonCallback);

    volumeLeftSB.setXY(72, 112);
    volumeLeftSB.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_INDICATORS_SLIDER_HORIZONTAL_SMALL_ROUND_KNOB_ID));
    volumeLeftSB.setupHorizontalSlider(3, 7, 0, 0, 125);
    volumeLeftSB.setValueRange(0, 254);
    volumeLeftSB.setValue(127);
    volumeLeftSB.setNewValueCallback(sliderValueChangedCallback);

    volumeRightSB.setXY(72, 156);
    volumeRightSB.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_INDICATORS_SLIDER_HORIZONTAL_SMALL_ROUND_KNOB_ID));
    volumeRightSB.setupHorizontalSlider(3, 7, 0, 0, 125);
    volumeRightSB.setValueRange(0, 254);
    volumeRightSB.setValue(127);
    volumeRightSB.setNewValueCallback(sliderValueChangedCallback);

    textArea1.setXY(40, 117);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID13));

    textArea2.setXY(40, 161);
    textArea2.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea2.setLinespacing(0);
    textArea2.setTypedText(touchgfx::TypedText(T_SINGLEUSEID14));

    textArea3.setXY(72, 79);
    textArea3.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea3.setLinespacing(0);
    textArea3.setTypedText(touchgfx::TypedText(T_SINGLEUSEID15));

    backButton.setXY(56, 405);
    backButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    backButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID16));
    backButton.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    backButton.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    backButton.setAction(buttonCallback);

    sineTestRadioButton.setXY(43, 23);
    sineTestRadioButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_NORMAL_ID));
    sineTestRadioButton.setSelected(true);
    sineTestRadioButton.setDeselectionEnabled(false);

    textArea4.setXY(92, 33);
    textArea4.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea4.setLinespacing(0);
    textArea4.setTypedText(touchgfx::TypedText(T_SINGLEUSEID67));

    pcmTestRadioButton.setXY(154, 23);
    pcmTestRadioButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_NORMAL_ID));
    pcmTestRadioButton.setSelected(false);
    pcmTestRadioButton.setDeselectionEnabled(false);

    textArea5.setXY(198, 33);
    textArea5.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea5.setLinespacing(0);
    textArea5.setTypedText(touchgfx::TypedText(T_SINGLEUSEID68));

    carryFreqNumPicker.setXY(108, 199);
    carryFreqNumPicker.setPickerValueChangedCallback(carryFreqNumPickerPickerValueChangedCallback);

    modFreqNumPicker.setXY(108, 240);
    modFreqNumPicker.setPickerValueChangedCallback(modFreqNumPickerPickerValueChangedCallback);

    deviationNumPicker.setXY(108, 282);
    deviationNumPicker.setPickerValueChangedCallback(deviationNumPickerPickerValueChangedCallback);

    textArea6.setXY(9, 206);
    textArea6.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea6.setLinespacing(0);
    textArea6.setTypedText(touchgfx::TypedText(T_SINGLEUSEID69));

    textArea7.setXY(14, 247);
    textArea7.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea7.setLinespacing(0);
    textArea7.setTypedText(touchgfx::TypedText(T_SINGLEUSEID70));

    textArea8.setXY(15, 289);
    textArea8.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea8.setLinespacing(0);
    textArea8.setTypedText(touchgfx::TypedText(T_SINGLEUSEID71));

    add(__background);
    add(backgroundBox);
    add(testControlBtn);
    add(volumeLeftSB);
    add(volumeRightSB);
    add(textArea1);
    add(textArea2);
    add(textArea3);
    add(backButton);
    add(sineTestRadioButton);
    add(textArea4);
    add(pcmTestRadioButton);
    add(textArea5);
    add(carryFreqNumPicker);
    add(modFreqNumPicker);
    add(deviationNumPicker);
    add(textArea6);
    add(textArea7);
    add(textArea8);
    radioButtonGroup1.add(sineTestRadioButton);
    radioButtonGroup1.add(pcmTestRadioButton);
    radioButtonGroup1.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
}

void codecTestScreenViewBase::setupScreen()
{
    carryFreqNumPicker.initialize();
    modFreqNumPicker.initialize();
    deviationNumPicker.initialize();
}

void codecTestScreenViewBase::carryFreqNumPickerPickerValueChangedCallbackHandler(uint8_t value)
{
    //carryFreqNumPickerValueChangeAction
    //When carryFreqNumPicker pickerValueChanged call virtual function
    //Call carryFreqNumPickerValueChange
    carryFreqNumPickerValueChange(value);
}

void codecTestScreenViewBase::modFreqNumPickerPickerValueChangedCallbackHandler(uint8_t value)
{
    //modFreqNumPickerValueChangedAction
    //When modFreqNumPicker pickerValueChanged call virtual function
    //Call modFreqNumPickerValueChanged
    modFreqNumPickerValueChanged(value);
}

void codecTestScreenViewBase::deviationNumPickerPickerValueChangedCallbackHandler(uint8_t value)
{
    //deviationNumPickerValueChangedAction
    //When deviationNumPicker pickerValueChanged call virtual function
    //Call deviationNumPickerValueChanged
    deviationNumPickerValueChanged(value);
}

void codecTestScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &testControlBtn)
    {
        //codecTestControlBtnAction
        //When testControlBtn clicked call virtual function
        //Call codecTestControlBtnPressed
        codecTestControlBtnPressed();
    }
    else if (&src == &backButton)
    {
        //backButtonPressedAction
        //When backButton clicked call virtual function
        //Call backButtonPressed
        backButtonPressed();
    }
}

void codecTestScreenViewBase::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &volumeLeftSB)
    {
        //volumeLeftSBAction
        //When volumeLeftSB value changed call virtual function
        //Call volumeLeftSB_valueChanged
        volumeLeftSB_valueChanged(value);
    }
    else if (&src == &volumeRightSB)
    {
        //volumeRightSBAction
        //When volumeRightSB value changed call virtual function
        //Call volumeRightSB_valueChanged
        volumeRightSB_valueChanged(value);
    }
}

void codecTestScreenViewBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &sineTestRadioButton)
    {
        //sineTestRBSelectedAction
        //When sineTestRadioButton selected call virtual function
        //Call sineTestRBSelected
        sineTestRBSelected();
    }
    else if (&src == &pcmTestRadioButton)
    {
        //pcmTestRBSelectedAction
        //When pcmTestRadioButton selected call virtual function
        //Call pcmTestRBSelected
        pcmTestRBSelected();
    }
}
