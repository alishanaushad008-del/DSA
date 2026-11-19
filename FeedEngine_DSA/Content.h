#ifndef CONTENT_H
#define CONTENT_H

class Content {

public:
    virtual void display() const = 0;

    virtual ~Content() {}
};

#endif