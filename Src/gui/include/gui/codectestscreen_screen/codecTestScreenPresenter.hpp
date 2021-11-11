#ifndef CODECTESTSCREENPRESENTER_HPP
#define CODECTESTSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class codecTestScreenView;

class codecTestScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    codecTestScreenPresenter(codecTestScreenView& v);

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

    virtual ~codecTestScreenPresenter() {};

private:
    codecTestScreenPresenter();

    codecTestScreenView& view;
};

#endif // CODECTESTSCREENPRESENTER_HPP
