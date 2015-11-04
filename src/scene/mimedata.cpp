#include "scene/mimedata.h"
#include "QDebug"
#include "item/abstractelement.h"

MimeData::MimeData( AbstractElement *el)
    : QMimeData()
{

    _element = el;
}
