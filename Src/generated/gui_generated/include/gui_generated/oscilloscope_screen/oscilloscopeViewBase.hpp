/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef OSCILLOSCOPEVIEWBASE_HPP
#define OSCILLOSCOPEVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/oscilloscope_screen/oscilloscopePresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/graph/GraphWrapAndOverwrite.hpp>
#include <touchgfx/widgets/graph/GraphElements.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <gui/containers/numPicker.hpp>
#include <touchgfx/widgets/RadioButton.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/RadioButtonGroup.hpp>

class oscilloscopeViewBase : public touchgfx::View<oscilloscopePresenter>
{
public:
    oscilloscopeViewBase();
    virtual ~oscilloscopeViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void backButtonPressed()
    {
        // Override and implement this function in oscilloscope
    }

    virtual void scopeCtrlButtonPressed()
    {
        // Override and implement this function in oscilloscope
    }

    virtual void micRadioButtonSelected()
    {
        // Override and implement this function in oscilloscope
    }

    virtual void lineRadioButtonSelected()
    {
        // Override and implement this function in oscilloscope
    }

    virtual void xScalePickerValueChanged(uint8_t value)
    {
        // Override and implement this function in oscilloscope
    }

    virtual void yScalePickerValueChanged(uint8_t value)
    {
        // Override and implement this function in oscilloscope
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
    touchgfx::GraphWrapAndOverwrite<8000> dynamicGraph1;
    touchgfx::GraphElementLine dynamicGraph1Line1;
    touchgfx::PainterRGB888 dynamicGraph1Line1Painter;
    touchgfx::GraphElementGridX dynamicGraph1MajorXAxisGrid;
    touchgfx::GraphElementGridY dynamicGraph1MajorYAxisGrid;
touchgfx::GraphElementVerticalGapLine dynamicGraph1VerticalFrontline;
    touchgfx::Box box3;
    numPicker xScalePicker;
    numPicker yScalePicker;
    touchgfx::RadioButton micRadioButton;
    touchgfx::RadioButton lineRadioButton;
    touchgfx::TextArea textArea1;
    touchgfx::TextArea textArea2;
    touchgfx::ButtonWithLabel backButton;
    touchgfx::IconButtonStyle< touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > > scopeCtrlButton;
    touchgfx::RadioButtonGroup<2> radioButtonGroup1;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<oscilloscopeViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<oscilloscopeViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;
    touchgfx::Callback<oscilloscopeViewBase, const touchgfx::AbstractButton&> radioButtonSelectedCallback;
    touchgfx::Callback<oscilloscopeViewBase, uint8_t> xScalePickerPickerValueChangedCallback;
    touchgfx::Callback<oscilloscopeViewBase, uint8_t> yScalePickerPickerValueChangedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src);
    void xScalePickerPickerValueChangedCallbackHandler(uint8_t value);
    void yScalePickerPickerValueChangedCallbackHandler(uint8_t value);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 4080;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // OSCILLOSCOPEVIEWBASE_HPP
