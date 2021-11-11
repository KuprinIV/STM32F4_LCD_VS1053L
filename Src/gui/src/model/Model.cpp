#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C"
{
	extern osMessageQueueId_t messageQueueHandle;
}

Model::Model() : modelListener(0)
{
	setCurrentFolderPath(SDPath);
}

void Model::tick()
{
	osMessageQueueGet(messageQueueHandle, &sdIsConnected, NULL, 100);
	if(sdIsConnected != sdIsConnectedPrev){
		sdIsConnectedPrev = sdIsConnected;
		if(sdIsConnected){
			sdCurrentState.isConnected = SD_IS_CONNECTED;
		}else{
			sdCurrentState.isConnected = SD_IS_NOT_CONNECTED;
		}
	}
	taskYIELD();
}

void Model::getSDStatus(sdState* sd_st)
{
	memcpy(sd_st, &sdCurrentState, sizeof(sdState));
}

void Model::setSDStatus(sdState* sd_st)
{
	memcpy(&sdCurrentState, sd_st, sizeof(sdState));
}

void Model::setCurrentFilePathToOpen(char* path)
{
	sprintf(currentFilePathToOpen, "%s", path);
}

void Model::getCurrentFilePathToOpen(char* dst_path)
{
	sprintf(dst_path, "%s", currentFilePathToOpen);
}

void Model::setCurrentFolderPath(char* path)
{
	sprintf(currentFolderPath, "%s", path);
}

void Model::getCurrentFolderPath(char* dst_path)
{
	sprintf(dst_path, "%s", currentFolderPath);
}

void Model::setCurrentDIRObject(DIR* dir)
{
	memcpy(&currentDir, dir, sizeof(currentDir));
}

void Model::getCurrentDIRObject(DIR* dir)
{
	memcpy(dir, &currentDir, sizeof(currentDir));
}
