#ifndef EXTERNALSTIMULATORPRESENTER_HPP
#define EXTERNALSTIMULATORPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class externalStimulatorView;

class externalStimulatorPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    externalStimulatorPresenter(externalStimulatorView& v);

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

    virtual ~externalStimulatorPresenter() {};

private:
    externalStimulatorPresenter();

    externalStimulatorView& view;
};

#endif // EXTERNALSTIMULATORPRESENTER_HPP
