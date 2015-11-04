#ifndef COMMAND_H
#define COMMAND_H




class Command {
public:
    explicit Command();

    virtual void execute() = 0;

    virtual void undo() = 0;

protected:

};

#endif
