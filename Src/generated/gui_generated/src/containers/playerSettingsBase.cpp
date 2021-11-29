/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/playerSettingsBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

playerSettingsBase::playerSettingsBase() :
    sliderValueChangedCallback(this, &playerSettingsBase::sliderValueChangedCallbackHandler),
    radioButtonSelectedCallback(this, &playerSettingsBase::radioButtonSelectedCallbackHandler),
    radioButtonDeselectedCallback(this, &playerSettingsBase::radioButtonDeselectedCallbackHandler),
    playSettingsChangedCallback(0)
{
    setWidth(250);
    setHeight(400);
    box1.setPosition(0, 0, 250, 400);
    box1.setColor(touchgfx::Color::getColorFromRGB(0, 255, 255));

    textArea1.setXY(104, 47);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_8ZG5));

    diffRadioButton.setXY(38, 13);
    diffRadioButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_PAGEINDICATOR_DOT_INDICATOR_SMALL_NORMAL_ID), touchgfx::Bitmap(BITMAP_BLUE_PAGEINDICATOR_DOT_INDICATOR_SMALL_NORMAL_ID), touchgfx::Bitmap(BITMAP_BLUE_PAGEINDICATOR_DOT_INDICATOR_SMALL_HIGHLIGHT_ID), touchgfx::Bitmap(BITMAP_BLUE_PAGEINDICATOR_DOT_INDICATOR_SMALL_HIGHLIGHT_ID));
    diffRadioButton.setSelected(false);
    diffRadioButton.setDeselectionEnabled(true);

    textArea2.setXY(63, 10);
    textArea2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea2.setLinespacing(0);
    textArea2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_28GJ));

    bassAmpSlider.setXY(38, 92);
    bassAmpSlider.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_INDICATORS_SLIDER_HORIZONTAL_SMALL_ROUND_KNOB_ID));
    bassAmpSlider.setupHorizontalSlider(3, 7, 0, 0, 125);
    bassAmpSlider.setValueRange(0, 15);
    bassAmpSlider.setValue(0);
    bassAmpSlider.setNewValueCallback(sliderValueChangedCallback);

    bassBoostText.setXY(70, 72);
    bassBoostText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    bassBoostText.setLinespacing(0);
    Unicode::snprintf(bassBoostTextBuffer, BASSBOOSTTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_OY7N).getText());
    bassBoostText.setWildcard(bassBoostTextBuffer);
    bassBoostText.resizeToCurrentText();
    bassBoostText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_3O3T));

    bassFreqSlider.setXY(38, 146);
    bassFreqSlider.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_INDICATORS_SLIDER_HORIZONTAL_SMALL_ROUND_KNOB_ID));
    bassFreqSlider.setupHorizontalSlider(3, 7, 0, 0, 125);
    bassFreqSlider.setValueRange(2, 15);
    bassFreqSlider.setValue(2);
    bassFreqSlider.setNewValueCallback(sliderValueChangedCallback);

    bassFreqText.setXY(44, 126);
    bassFreqText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    bassFreqText.setLinespacing(0);
    Unicode::snprintf(bassFreqTextBuffer, BASSFREQTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_3XVT).getText());
    bassFreqText.setWildcard(bassFreqTextBuffer);
    bassFreqText.resizeToCurrentText();
    bassFreqText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_GOM1));

    textArea3.setXY(97, 194);
    textArea3.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea3.setLinespacing(0);
    textArea3.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UYUC));

    trebleAmpSlider.setXY(40, 239);
    trebleAmpSlider.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_INDICATORS_SLIDER_HORIZONTAL_SMALL_ROUND_KNOB_ID));
    trebleAmpSlider.setupHorizontalSlider(3, 7, 0, 0, 125);
    trebleAmpSlider.setValueRange(0, 15);
    trebleAmpSlider.setValue(8);
    trebleAmpSlider.setNewValueCallback(sliderValueChangedCallback);

    trebleBoostText.setXY(64, 219);
    trebleBoostText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    trebleBoostText.setLinespacing(0);
    Unicode::snprintf(trebleBoostTextBuffer, TREBLEBOOSTTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_FYD5).getText());
    trebleBoostText.setWildcard(trebleBoostTextBuffer);
    trebleBoostText.resizeToCurrentText();
    trebleBoostText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_DUZU));

    trebleFreqSlider.setXY(40, 293);
    trebleFreqSlider.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_SLIDER_HORIZONTAL_SMALL_ROUND_EDGE_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_SMALL_INDICATORS_SLIDER_HORIZONTAL_SMALL_ROUND_KNOB_ID));
    trebleFreqSlider.setupHorizontalSlider(3, 7, 0, 0, 125);
    trebleFreqSlider.setValueRange(1, 15);
    trebleFreqSlider.setValue(1);
    trebleFreqSlider.setNewValueCallback(sliderValueChangedCallback);

    trebleFreqText.setXY(31, 273);
    trebleFreqText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    trebleFreqText.setLinespacing(0);
    Unicode::snprintf(trebleFreqTextBuffer, TREBLEFREQTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_2OT0).getText());
    trebleFreqText.setWildcard(trebleFreqTextBuffer);
    trebleFreqText.resizeToCurrentText();
    trebleFreqText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_QL5I));

    add(box1);
    add(textArea1);
    add(diffRadioButton);
    add(textArea2);
    add(bassAmpSlider);
    add(bassBoostText);
    add(bassFreqSlider);
    add(bassFreqText);
    add(textArea3);
    add(trebleAmpSlider);
    add(trebleBoostText);
    add(trebleFreqSlider);
    add(trebleFreqText);
    radioButtonGroup1.add(diffRadioButton);
    radioButtonGroup1.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
    radioButtonGroup1.setRadioButtonDeselectedHandler(radioButtonDeselectedCallback);
}

void playerSettingsBase::initialize()
{

}

void playerSettingsBase::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &bassAmpSlider)
    {
        //bassAmpSliderValueChangedAction
        //When bassAmpSlider value changed call virtual function
        //Call bassAmpSliderValueChanged
        bassAmpSliderValueChanged(value);
    }
    else if (&src == &bassFreqSlider)
    {
        //bassFreqSliderValueChangedAction
        //When bassFreqSlider value changed call virtual function
        //Call bassFreqSliderValueChanged
        bassFreqSliderValueChanged(value);
    }
    else if (&src == &trebleAmpSlider)
    {
        //trebleAmpSliderValueChangedAction
        //When trebleAmpSlider value changed call virtual function
        //Call trebleAmpSliderValueChanged
        trebleAmpSliderValueChanged(value);
    }
    else if (&src == &trebleFreqSlider)
    {
        //trebleFreqSliderValueChangedAction
        //When trebleFreqSlider value changed call virtual function
        //Call trebleFreqSliderValueChanged
        trebleFreqSliderValueChanged(value);
    }
}

void playerSettingsBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &diffRadioButton)
    {
        //diffRadioButtonSelectedAction
        //When diffRadioButton selected call virtual function
        //Call diffRadioButtonSelected
        diffRadioButtonSelected();
    }
}

void playerSettingsBase::radioButtonDeselectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &diffRadioButton)
    {
        //diffRadioButtonDeselectedAction
        //When diffRadioButton deselected call virtual function
        //Call diffRadioButtonDeselected
        diffRadioButtonDeselected();
    }
}

