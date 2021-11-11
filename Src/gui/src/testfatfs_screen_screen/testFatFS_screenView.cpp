#include <gui/testfatfs_screen_screen/testFatFS_screenView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

testFatFS_screenView::testFatFS_screenView():
onFolderItemClickCallback(this, &testFatFS_screenView::onFolderItemClickCallbackHandler), onFileItemClickCallback(this, &testFatFS_screenView::onFileItemClickCallbackHandler)
{

}

testFatFS_screenView::~testFatFS_screenView()
{
    // delete item objects
	if(foldersItemList.size() > 0){
		for(folderItem* f : foldersItemList){
			delete f;
		}
		foldersItemList.clear();
	}
	if(filesItemList.size() > 0){
		for(fileItem* f : filesItemList){
			delete f;
		}
		filesItemList.clear();
	}
}

void testFatFS_screenView::setupScreen()
{
    testFatFS_screenViewBase::setupScreen();

    presenter->getSDStatus(&sd_state);
    if(sd_state.isMounted == FS_IS_MOUNTED)
    {
    	presenter->getCurrentFolderPath(currentPath); //init current path
    	if(scan_files(currentPath) != FR_OK){
    		showModalWindow("Scan files error");
    	}
    }
    else{
    	showModalWindow("FS isn't mounted");
    }
    fileListScrollContainer.setMaxVelocity(10);
    fileListScrollContainer.setScrollDurationSpeedup(3);
}

void testFatFS_screenView::tearDownScreen()
{
    testFatFS_screenViewBase::tearDownScreen();
}

void testFatFS_screenView::backButtonFSPressed()
{
	f_closedir(&currentDirObject);
	application().gotomain_screenScreenNoTransition();
}

void testFatFS_screenView::startTestFatFS()
{

}

void testFatFS_screenView::onFolderItemClickCallbackHandler(const touchgfx::Unicode::UnicodeChar* folder)
{
	int offset = 0;
	char folder_name[FILE_NAME_BUFFER_SIZE] = {0};

	Unicode::toUTF8(folder, (uint8_t*)folder_name, FILE_NAME_BUFFER_SIZE);

	if(strcmp(folder_name, "...") == 0){
		f_closedir(&currentDirObject);
		trimPath(currentPath);
	}else{
		offset = strlen(currentPath);
		if(isPathRoot(currentPath)){
			sprintf(&currentPath[offset], "%s", folder_name);
		}
		else{
			sprintf(&currentPath[offset], "/%s", folder_name);
		}
	}
	//update current folder path
	presenter->setCurrentFolderPath(currentPath);

	//update interface
	if(scan_files(currentPath) != FR_OK){
		showModalWindow("Scan files error");
	}
}

void testFatFS_screenView::onFileItemClickCallbackHandler(const touchgfx::Unicode::UnicodeChar* file)
{
	char full_path[FILE_NAME_BUFFER_SIZE] = {0};
	char file_name[FILE_NAME_BUFFER_SIZE] = {0};

	Unicode::toUTF8(file, (uint8_t*)file_name, FILE_NAME_BUFFER_SIZE);

	if(strstr(file_name, ".mp3") != NULL || strstr(file_name, ".wav") != NULL){
		sprintf(full_path, "%s/%s", currentPath, file_name);
		presenter->setCurrentFileToOpen(full_path);
		presenter->setCurrentDIRObject(&currentDirObject);
		application().gotoplayAudioFileScreenNoTransition();
	}
}

void testFatFS_screenView::showModalWindow(const char* msg)
{
	int x_pos = 0;
	// draw message
	Unicode::fromUTF8((const uint8_t*)msg, modalWindowMessageTextBuffer, MODALWINDOWMESSAGETEXT_SIZE);
	modalWindowMessageText.setWildcard(modalWindowMessageTextBuffer);
	x_pos =  (box1.getWidth() - modalWindowMessageText.getTextWidth())/2; // align status text to center
	modalWindowMessageText.setX(x_pos);
	modalWindowMessageText.invalidate();
	modalWindowMessageText.resizeToCurrentText();
	modalWindowMessageText.invalidate();
	// set visible
	modalWindow.show();
	closeModalWndButton.setVisible(true);
	modalWindowMessageText.setVisible(true);
}

void testFatFS_screenView::closeModalWindow()
{
	modalWindow.hide();
	closeModalWndButton.setVisible(false);
	modalWindowMessageText.setVisible(false);
}

FRESULT testFatFS_screenView::scan_files(char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;   /* This function is assuming non-Unicode cfg. */
    //interface variables
    folderItem* folderTmp;
    fileItem* fileTmp;
    int16_t currentY = 0;
    touchgfx::Unicode::UnicodeChar tempName[FILE_NAME_BUFFER_SIZE];

    // delete item objects
	for(folderItem* f : foldersItemList){
		delete f;
	}
	for(fileItem* f : filesItemList){
		delete f;
	}
    foldersItemList.clear();
    filesItemList.clear();
    fileListScrollContainer.removeAll();
    fileListScrollContainer.invalidate();

    if(!isPathRoot(path)){
    	folderTmp = new folderItem();
    	Unicode::snprintf(tempName, FILE_NAME_BUFFER_SIZE, "...");
		folderTmp->setFolderItemName(tempName);
		folderTmp->setX(0);
		folderTmp->setOnFolderItemClickedCallback(onFolderItemClickCallback);
		foldersItemList.push_back(folderTmp);
    }
	if(!isPathRoot(path))
		f_closedir(&currentDirObject);
    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        currentDirObject = dir;

        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */

            fn = fno.fname;

            if (fno.fattrib & AM_HID) continue;             /* Ignore hidden directories and files*/

            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
            	folderTmp = new folderItem();
            	Unicode::fromUTF8((const uint8_t*)fn, tempName, FILE_NAME_BUFFER_SIZE);
        		folderTmp->setFolderItemName(tempName);
        		folderTmp->setX(0);
        		folderTmp->setOnFolderItemClickedCallback(onFolderItemClickCallback);
        		foldersItemList.push_back(folderTmp);
            } else {                                       /* It is a file. */
				fileTmp = new fileItem();
				Unicode::fromUTF8((const uint8_t*)fn, tempName, FILE_NAME_BUFFER_SIZE);
				fileTmp->setFileItemName(tempName);
				fileTmp->setFileSize((uint32_t)fno.fsize);
				fileTmp->setX(0);
				fileTmp->setOnFileItemClickedCallback(onFileItemClickCallback);
				filesItemList.push_back(fileTmp);
            }
        }
        //update files list
		for(folderItem* f : foldersItemList){
			f->setY(currentY);
			currentY += f->getHeight();
			fileListScrollContainer.add(*f);
		}
		for(fileItem* f : filesItemList){
			f->setY(currentY);
			currentY += f->getHeight();
			fileListScrollContainer.add(*f);
		}
    }
    fileListScrollContainer.invalidate();

    return res;
}

bool testFatFS_screenView::isPathRoot(char* path)
{
	if(strcmp(path, SDPath) == 0){
		return true;
	}
	return false;
}

void testFatFS_screenView::trimPath(char* path)
{
	int last_slash_index = 0;
	for(int i = 0; (i < FILE_NAME_BUFFER_SIZE) || (path[i] == 0); i++){
		if(path[i] == '/'){
			last_slash_index = i;
		}
	}
	//cut off path
	for(int i = (last_slash_index > 2) ? (last_slash_index) : (last_slash_index+1); (i < FILE_NAME_BUFFER_SIZE) || (path[i] == 0); i++){
		path[i] = 0;
	}
}
