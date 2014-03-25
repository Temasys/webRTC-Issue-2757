/*
 * MainWindowObserver.h
 *
 *  Created on: Nov 15, 2013
 *      Author: game
 */

#ifndef MAINWINDOWOBSERVER_H_
#define MAINWINDOWOBSERVER_H_

#include <QUrl>

class MainWindowObserver
{
    public:
        virtual void StartLogin(QUrl url, QByteArray html) = 0;
        virtual void DisconnectFromServer() = 0;
        virtual void Close() = 0;

        virtual ~MainWindowObserver(){}
};
#endif /* MAINWINDOWOBSERVER_H_ */

