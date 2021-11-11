#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdio.h>
#include <vector>
#include <string>
#include <touchgfx/Callback.hpp>
#include "cmsis_os.h"
#include "main.h"
#include "fatfs.h"

using namespace std;

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void setSDStatus(sdState* sd_st);
    void getSDStatus(sdState* sd_st);

    void setCurrentFilePathToOpen(char* path);
    void getCurrentFilePathToOpen(char* dst_path);

    void setCurrentFolderPath(char* path);
    void getCurrentFolderPath(char* dst_path);

    void setCurrentDIRObject(DIR* dir);
    void getCurrentDIRObject(DIR* dir);

protected:
    ModelListener* modelListener;
    int sdIsConnected = 0;
    int sdIsConnectedPrev = 0;
    char currentFilePathToOpen[255] = {0};
    char currentFolderPath[255] = {0};
    vector<string*> audioFilesList;
    int currentFilePositionInList = 0;
    DIR currentDir;
    sdState sdCurrentState;
};

#endif // MODEL_HPP
