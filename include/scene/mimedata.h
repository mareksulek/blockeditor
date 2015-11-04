#ifndef MIMEDATA_H
#define MIMEDATA_H

#include <QMimeData>

class AbstractElement;

class MimeData : public QMimeData
{

public:
    explicit MimeData( AbstractElement *el = 0);

    AbstractElement *element() const { return _element; }
    bool hasElement() const { return NULL != _element ;}

private:
    AbstractElement *_element;
};

#endif // MAINWINDOW_H
