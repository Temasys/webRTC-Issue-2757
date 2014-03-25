#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "talk/base/messagehandler.h"
#include "talk/base/messagequeue.h"
#include <qstring.h>

//interface to inherit for using the GAEChannel
class MessageHandlerDeep //: public talk_base::MessageHandler
{
    public:
        //virtual void OnMessage(Message * msg) = 0; FROM MESSAGE HANDLER
        virtual void OnMessage(std::string msg) = 0;
        virtual void OnOpen() = 0;
        virtual void OnClose() = 0;
        virtual void OnError(int code, std::string description) = 0;

 };
#endif // MESSAGEHANDLER_H
