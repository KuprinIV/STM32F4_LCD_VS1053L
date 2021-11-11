/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FILEITEMBASE_HPP
#define FILEITEMBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class fileItemBase : public touchgfx::Container
{
public:
    fileItemBase();
    virtual ~fileItemBase() {}
    virtual void initialize();

    /*
     * Custom Trigger Callback Setters
     */
    void setOnFileItemClickedCallback(touchgfx::GenericCallback<const touchgfx::Unicode::UnicodeChar*>& callback)
    {
        this->onFileItemClickedCallback = &callback;
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Custom Trigger Emitters
     */
    virtual void emitOnFileItemClickedCallback(const touchgfx::Unicode::UnicodeChar* value)
    {
        if (onFileItemClickedCallback && onFileItemClickedCallback->isValid())
        {
            this->onFileItemClickedCallback->execute(value);
        }
    }

    /*
     * Member Declarations
     */
    touchgfx::Box box1;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > itemFileButton;
    touchgfx::Image image1;
    touchgfx::TextAreaWithOneWildcard fileName;
    touchgfx::TextAreaWithOneWildcard fileSizeText;

    /*
     * Wildcard Buffers
     */
    static const uint16_t FILENAME_SIZE = 256;
    touchgfx::Unicode::UnicodeChar fileNameBuffer[FILENAME_SIZE];
    static const uint16_t FILESIZETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar fileSizeTextBuffer[FILESIZETEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<fileItemBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Custom Trigger Callback Declarations
     */
    touchgfx::GenericCallback<const touchgfx::Unicode::UnicodeChar*>* onFileItemClickedCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // FILEITEMBASE_HPP
