#ifndef PLAYAUDIOFILEPRESENTER_HPP
#define PLAYAUDIOFILEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include "main.h"

using namespace touchgfx;

class playAudioFileView;

class playAudioFilePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    playAudioFilePresenter(playAudioFileView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~playAudioFilePresenter() {};

    void getCurrentFileToOpen(char* path){
    	model->getCurrentFilePathToOpen(path);
    }

    void getCurrentFolderPath(char* path){
    	model->getCurrentFolderPath(path);
    }

    void getCurrentDIRObject(DIR* dir){
    	model->getCurrentDIRObject(dir);
    }

private:
    playAudioFilePresenter();

    playAudioFileView& view;
};

#endif // PLAYAUDIOFILEPRESENTER_HPP
