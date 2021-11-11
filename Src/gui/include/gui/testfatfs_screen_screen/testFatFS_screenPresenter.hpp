#ifndef TESTFATFS_SCREENPRESENTER_HPP
#define TESTFATFS_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class testFatFS_screenView;

class testFatFS_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    testFatFS_screenPresenter(testFatFS_screenView& v);

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

    virtual ~testFatFS_screenPresenter() {};

    void getSDStatus(sdState* sd_st){
    	model->getSDStatus(sd_st);
    }

    void setCurrentFileToOpen(char* path){
    	model->setCurrentFilePathToOpen(path);
    }

    void setCurrentFolderPath(char* path){
    	model->setCurrentFolderPath(path);
    }

    void getCurrentFolderPath(char* path){
    	model->getCurrentFolderPath(path);
    }

    void setCurrentDIRObject(DIR* dir){
    	model->setCurrentDIRObject(dir);
    }

private:
    testFatFS_screenPresenter();

    testFatFS_screenView& view;
};

#endif // TESTFATFS_SCREENPRESENTER_HPP
