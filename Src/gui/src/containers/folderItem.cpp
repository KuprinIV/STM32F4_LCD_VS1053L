#include <gui/containers/folderItem.hpp>

folderItem::folderItem()
{

}

void folderItem::initialize()
{
    folderItemBase::initialize();
}

void folderItem::setFolderItemName(touchgfx::Unicode::UnicodeChar* str)
{
	Unicode::strncpy(folderNameBuffer, str, FOLDERNAME_SIZE);

	if(Unicode::strlen(folderNameBuffer) > 17){
		Unicode::strncpy(limitedName, folderNameBuffer, 14);
		limitedName[14] = '.';
		limitedName[15] = '.';
		limitedName[16] = '.';
		limitedName[17] = '\0';
	}
	else{
		Unicode::strncpy(limitedName, folderNameBuffer, 18);
	}
	folderName.setWildcard(limitedName);

	folderName.invalidate();
	folderName.resizeToCurrentText();
	folderName.invalidate();
}

