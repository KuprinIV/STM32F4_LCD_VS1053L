/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/oscilloscope_screen/oscilloscopeViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>


oscilloscopeViewBase::oscilloscopeViewBase() :
    buttonCallback(this, &oscilloscopeViewBase::buttonCallbackHandler),
    flexButtonCallback(this, &oscilloscopeViewBase::flexButtonCallbackHandler),
    radioButtonSelectedCallback(this, &oscilloscopeViewBase::radioButtonSelectedCallbackHandler),
    xScalePickerPickerValueChangedCallback(this, &oscilloscopeViewBase::xScalePickerPickerValueChangedCallbackHandler),
    yScalePickerPickerValueChangedCallback(this, &oscilloscopeViewBase::yScalePickerPickerValueChangedCallbackHandler)
{

    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    __background.setPosition(0, 0, 272, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));

    box1.setPosition(0, 0, 272, 91);
    box1.setColor(touchgfx::Color::getColorFromRGB(0, 255, 255));

    box2.setPosition(0, 91, 272, 254);
    box2.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));

    dynamicGraph1.setScale(1);
    dynamicGraph1.setPosition(0, 91, 272, 254);
    dynamicGraph1.setGraphAreaMargin(0, 0, 0, 0);
    dynamicGraph1.setGraphAreaPadding(0, 0, 0, 0);
    dynamicGraph1.setGraphRangeY(-32768, 32767);

    dynamicGraph1MajorXAxisGrid.setScale(1);
    dynamicGraph1MajorXAxisGrid.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    dynamicGraph1MajorXAxisGrid.setInterval(800);
    dynamicGraph1MajorXAxisGrid.setLineWidth(1);
    dynamicGraph1.addGraphElement(dynamicGraph1MajorXAxisGrid);

    dynamicGraph1MajorYAxisGrid.setScale(1);
    dynamicGraph1MajorYAxisGrid.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    dynamicGraph1MajorYAxisGrid.setInterval(6554);
    dynamicGraph1MajorYAxisGrid.setLineWidth(1);
    dynamicGraph1.addGraphElement(dynamicGraph1MajorYAxisGrid);

    dynamicGraph1Line1.setScale(1);
    dynamicGraph1Line1Painter.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
    dynamicGraph1Line1.setPainter(dynamicGraph1Line1Painter);
    dynamicGraph1Line1.setLineWidth(2);
    dynamicGraph1.addGraphElement(dynamicGraph1Line1);

    dynamicGraph1VerticalFrontline.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    dynamicGraph1VerticalFrontline.setGapLineWidth(1);
    dynamicGraph1.addGraphElement(dynamicGraph1VerticalFrontline);

    box3.setPosition(0, 345, 272, 135);
    box3.setColor(touchgfx::Color::getColorFromRGB(0, 255, 255));

    xScalePicker.setXY(3, 53);
    xScalePicker.setPickerValueChangedCallback(xScalePickerPickerValueChangedCallback);

    yScalePicker.setXY(138, 53);
    yScalePicker.setPickerValueChangedCallback(yScalePickerPickerValueChangedCallback);

    micRadioButton.setXY(32, 4);
    micRadioButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_NORMAL_ID));
    micRadioButton.setSelected(true);
    micRadioButton.setDeselectionEnabled(false);

    lineRadioButton.setXY(149, 4);
    lineRadioButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_RADIO_BUTTONS_RADIO_ROUND_BUTTON_NORMAL_ID));
    lineRadioButton.setSelected(false);
    lineRadioButton.setDeselectionEnabled(false);

    textArea1.setXY(76, 14);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_VC74));

    textArea2.setXY(193, 14);
    textArea2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea2.setLinespacing(0);
    textArea2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_SE8P));

    backButton.setXY(56, 405);
    backButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    backButton.setLabelText(touchgfx::TypedText(T___SINGLEUSE_E40A));
    backButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    backButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    backButton.setAction(buttonCallback);

    scopeCtrlButton.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    scopeCtrlButton.setBitmapXY(0, 0);
    scopeCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
    scopeCtrlButton.setIconXY(20, 14);
    scopeCtrlButton.setPosition(111, 345, 60, 60);
    scopeCtrlButton.setAction(flexButtonCallback);

    add(__background);
    add(box1);
    add(box2);
    add(dynamicGraph1);
    add(box3);
    add(xScalePicker);
    add(yScalePicker);
    add(micRadioButton);
    add(lineRadioButton);
    add(textArea1);
    add(textArea2);
    add(backButton);
    add(scopeCtrlButton);
    radioButtonGroup1.add(micRadioButton);
    radioButtonGroup1.add(lineRadioButton);
    radioButtonGroup1.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
}

void oscilloscopeViewBase::setupScreen()
{
    xScalePicker.initialize();
    yScalePicker.initialize();
}

void oscilloscopeViewBase::xScalePickerPickerValueChangedCallbackHandler(uint8_t value)
{
    //xScalePickerValueChangedAction
    //When xScalePicker pickerValueChanged call virtual function
    //Call xScalePickerValueChanged
    xScalePickerValueChanged(value);
}

void oscilloscopeViewBase::yScalePickerPickerValueChangedCallbackHandler(uint8_t value)
{
    //yScalePickerValueChangedAction
    //When yScalePicker pickerValueChanged call virtual function
    //Call yScalePickerValueChanged
    yScalePickerValueChanged(value);
}

void oscilloscopeViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &backButton)
    {
        //backButtonPressedAction
        //When backButton clicked call virtual function
        //Call backButtonPressed
        backButtonPressed();
    }
}

void oscilloscopeViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &scopeCtrlButton)
    {
        //scopeCtrlButtonPressedAction
        //When scopeCtrlButton clicked call virtual function
        //Call scopeCtrlButtonPressed
        scopeCtrlButtonPressed();
    }
}

void oscilloscopeViewBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &micRadioButton)
    {
        //micRadioButtonSelectedAction
        //When micRadioButton selected call virtual function
        //Call micRadioButtonSelected
        micRadioButtonSelected();
    }
    else if (&src == &lineRadioButton)
    {
        //lineRadioButtonSelectedAction
        //When lineRadioButton selected call virtual function
        //Call lineRadioButtonSelected
        lineRadioButtonSelected();
    }
}
