#ifndef SOUNDRECORDPRESENTER_HPP
#define SOUNDRECORDPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class soundRecordView;

class soundRecordPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    soundRecordPresenter(soundRecordView& v);

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

    virtual ~soundRecordPresenter() {};

    void getSDStatus(sdState* sd_st){
    	model->getSDStatus(sd_st);
    }

private:
    soundRecordPresenter();

    soundRecordView& view;
};

#endif // SOUNDRECORDPRESENTER_HPP
