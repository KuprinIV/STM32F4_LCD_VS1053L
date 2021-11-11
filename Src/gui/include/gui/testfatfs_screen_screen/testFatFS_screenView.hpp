#ifndef TESTFATFS_SCREENVIEW_HPP
#define TESTFATFS_SCREENVIEW_HPP

#include <gui_generated/testfatfs_screen_screen/testFatFS_screenViewBase.hpp>
#include <gui/testfatfs_screen_screen/testFatFS_screenPresenter.hpp>
#include <gui/containers/folderItem.hpp>
#include <gui/containers/fileItem.hpp>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "tests.h"
#include "fatfs.h"
#include "main.h"

using namespace std;

#define FILE_NAME_BUFFER_SIZE (_MAX_LFN + 1)

class testFatFS_screenView : public testFatFS_screenViewBase
{
public:
    testFatFS_screenView();
    virtual ~testFatFS_screenView();
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void backButtonFSPressed();
    virtual void startTestFatFS();
    virtual void closeModalWindow();
protected:
    list<folderItem*> foldersItemList;
    list<fileItem*> filesItemList;

private:
    /*
     * Callback Declarations
     */
    touchgfx::Callback<testFatFS_screenView, const touchgfx::Unicode::UnicodeChar*> onFolderItemClickCallback;
    touchgfx::Callback<testFatFS_screenView, const touchgfx::Unicode::UnicodeChar*> onFileItemClickCallback;

    /*
     * Callback Handler Declarations
     */
    void onFolderItemClickCallbackHandler(const touchgfx::Unicode::UnicodeChar* folder);
    void onFileItemClickCallbackHandler(const touchgfx::Unicode::UnicodeChar* file);
    void trimPath(char* path);
    void showModalWindow(const char* msg);
    FRESULT scan_files(char* path);


    DIR currentDirObject;
    bool isPathRoot(char* path);
    char currentPath[255] = {0};
    sdState sd_state;
};

#endif // TESTFATFS_SCREENVIEW_HPP
