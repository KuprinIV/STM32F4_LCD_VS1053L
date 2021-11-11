#ifndef FOLDERITEM_HPP
#define FOLDERITEM_HPP

#include <gui_generated/containers/folderItemBase.hpp>
#include <string.h>

class folderItem : public folderItemBase
{
public:
    folderItem();
    virtual ~folderItem() {}

    virtual void initialize();

    void setFolderItemName(touchgfx::Unicode::UnicodeChar* str);
protected:
private:
    touchgfx::Unicode::UnicodeChar limitedName[18];
};

#endif // FOLDERITEM_HPP
