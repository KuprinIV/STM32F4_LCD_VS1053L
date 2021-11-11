#include <gui/oscilloscope_screen/oscilloscopeView.hpp>

oscilloscopeView::oscilloscopeView()
{

}

void oscilloscopeView::setupScreen()
{
    oscilloscopeViewBase::setupScreen();
    dynamicGraph1.setTouchable(false);
    xFullScale = dynamicGraph1.getGraphRangeXMax() + 1;
    //init pickers values
    xScalePicker.setValueIndex(3);
    xScalePickerValueChanged(3);
    xScalePicker.setValuesList(&xScaleValuesList);
    xScalePicker.setCycled(false);

    yScalePicker.setValueIndex(6);
    yScalePickerValueChanged(6);
    yScalePicker.setValuesList(&yScaleValuesList);
    yScalePicker.setCycled(false);

    //fill sine array
    for(int i = 0; i < 64; i++){
    	sineArray[i] = 1000*sin(M_PI*i/32);
    }
}

void oscilloscopeView::tearDownScreen()
{
    oscilloscopeViewBase::tearDownScreen();
}

void oscilloscopeView::handleTickEvent()
{
	if(isRunning){
//		dynamicGraph1.addDataPoint(sineArray[index]);
//		index = (index+1)&0x3F;
	    for(int i = 0; i < 1024; i++){
	    	dynamicGraph1.addDataPoint(sineArray[i&0x3F]);
	    }
	}
}

void oscilloscopeView::backButtonPressed()
{
	application().gotomain_screenScreenNoTransition();
}

void oscilloscopeView::scopeCtrlButtonPressed()
{
	if(!isRunning){
		isRunning = true;
		scopeCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID), Bitmap(BITMAP_DARK_ICONS_PAUSE_32_ID));
		scopeCtrlButton.setIconXY(17, 14);
		//disable radio buttons
		micRadioButton.setTouchable(false);
		lineRadioButton.setTouchable(false);
	}else{
		isRunning = false;
		scopeCtrlButton.setIconBitmaps(Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID), Bitmap(BITMAP_DARK_ICONS_PLAY_32_ID));
		scopeCtrlButton.setIconXY(18, 14);
		//enable radio buttons
		micRadioButton.setTouchable(true);
		lineRadioButton.setTouchable(true);
	}
	scopeCtrlButton.invalidate();
}

void oscilloscopeView::micRadioButtonSelected()
{

}

void oscilloscopeView::lineRadioButtonSelected()
{

}

void oscilloscopeView::xScalePickerValueChanged(uint8_t value)
{
	int low_range = xFullScale - xFullScale/xScaleValues[value];
	dynamicGraph1.setGraphRangeX(low_range, xFullScale-1);
	dynamicGraph1MajorXAxisGrid.setScale(xScaleValues[value]);
}

void oscilloscopeView::yScalePickerValueChanged(uint8_t value)
{
	dynamicGraph1.setScale(yScaleValues[value]);
	dynamicGraph1MajorYAxisGrid.setScale(yScaleValues[value]);
}
