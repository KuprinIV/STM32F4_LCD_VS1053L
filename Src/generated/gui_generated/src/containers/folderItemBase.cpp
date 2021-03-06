/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/folderItemBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

folderItemBase::folderItemBase() :
    flexButtonCallback(this, &folderItemBase::flexButtonCallbackHandler),
    onFolderItemClickedCallback(0)
{
    setWidth(252);
    setHeight(30);
    box1.setPosition(0, 0, 252, 30);
    box1.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

    itemDirButton.setBoxWithBorderPosition(0, 0, 200, 30);
    itemDirButton.setBorderSize(0);
    itemDirButton.setBoxWithBorderColors(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255), touchgfx::Color::getColorFrom24BitRGB(128, 128, 128), touchgfx::Color::getColorFrom24BitRGB(255, 255, 255), touchgfx::Color::getColorFrom24BitRGB(128, 128, 128));
    itemDirButton.setPosition(0, 0, 200, 30);
    itemDirButton.setAction(flexButtonCallback);

    image1.setXY(6, 3);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_FOLDER_ID));

    folderName.setXY(36, 5);
    folderName.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    folderName.setLinespacing(0);
    folderNameBuffer[0] = 0;
    folderName.setWildcard(folderNameBuffer);
    folderName.resizeToCurrentText();
    folderName.setTypedText(touchgfx::TypedText(T_SINGLEUSEID22));

    add(box1);
    add(itemDirButton);
    add(image1);
    add(folderName);
}

void folderItemBase::initialize()
{

}

void folderItemBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &itemDirButton)
    {
        //onFolderItemClickedAction
        //When itemDirButton clicked emit onFolderItemClicked callback
        //Emit callback
        emitOnFolderItemClickedCallback(folderNameBuffer);
    }
}
