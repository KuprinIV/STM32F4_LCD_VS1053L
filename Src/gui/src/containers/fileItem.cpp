#include <gui/containers/fileItem.hpp>

fileItem::fileItem()
{

}

void fileItem::initialize()
{
    fileItemBase::initialize();
}

void fileItem::setFileItemName(touchgfx::Unicode::UnicodeChar* str)
{
	char file_name_tmp[FILENAME_SIZE] = {0};
	char limited_file_name[16] = {0};

	Unicode::strncpy(fileNameBuffer, str, FILENAME_SIZE);

	if(Unicode::strlen(fileNameBuffer) > 15){
		Unicode::toUTF8(fileNameBuffer, (uint8_t*)file_name_tmp, FILENAME_SIZE);
		strcpy(limited_file_name + 15 - strlen(strrchr(file_name_tmp, '.')), strrchr(file_name_tmp, '.'));
		for(unsigned int i = 0; i < 15 - strlen(strrchr(file_name_tmp, '.')) - 1; i++)
		{
			limited_file_name[i] = file_name_tmp[i];
		}
		limited_file_name[15 - strlen(strrchr(file_name_tmp, '.'))-1] = '~';
		Unicode::fromUTF8((uint8_t*)limited_file_name, limitedName, 16);
	}
	else{
		Unicode::strncpy(limitedName, fileNameBuffer, 16);
	}
	fileName.setWildcard(limitedName);

	fileName.invalidate();
	fileName.resizeToCurrentText();
	fileName.invalidate();
}

void fileItem::setFileSize(uint32_t fsize)
{
	int16_t x_pos = 0;

	if(fsize < 1024){
		Unicode::snprintf(fileSizeTextBuffer, FILESIZETEXT_SIZE, "%dB", fsize);
	}else if(fsize < 1048576){
		Unicode::snprintfFloat(fileSizeTextBuffer, FILESIZETEXT_SIZE, "%.1fK", (float)fsize/1024);
	}else if(fsize < 1073741824){
		Unicode::snprintfFloat(fileSizeTextBuffer, FILESIZETEXT_SIZE, "%.1fM", (float)fsize/1048576);
	}else{
		Unicode::snprintfFloat(fileSizeTextBuffer, FILESIZETEXT_SIZE, "%.1fG", (float)fsize/1073741824);
	}
	fileSizeText.setWildcard(fileSizeTextBuffer);

	x_pos = this->getWidth() - fileSizeText.getTextWidth() - 5;
	fileSizeText.setX(x_pos);

	fileSizeText.invalidate();
	fileSizeText.resizeToCurrentText();
	fileSizeText.invalidate();
}
