#ifndef FILEITEM_HPP
#define FILEITEM_HPP

#include <gui_generated/containers/fileItemBase.hpp>
#include <string.h>

class fileItem : public fileItemBase
{
public:
    fileItem();
    virtual ~fileItem() {}

    virtual void initialize();
    void setFileItemName(touchgfx::Unicode::UnicodeChar* str);
    void setFileSize(uint32_t fsize);
protected:
private:
    touchgfx::Unicode::UnicodeChar limitedName[16];
};

#endif // FILEITEM_HPP
