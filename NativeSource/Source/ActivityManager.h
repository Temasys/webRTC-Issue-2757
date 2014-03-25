#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include "talk/base/refcount.h"

#include "Conductor.h"

#include <QObject>
#include <QTimer>

#include <QDebug>

class ActivityManager: public QObject
{
    Q_OBJECT
public:
    ActivityManager(QObject *parent = 0);
    ~ActivityManager();
    void Start();

private:
    talk_base::scoped_refptr<Conductor> _conductorOne;
    talk_base::scoped_refptr<Conductor> _conductorTwo;
    talk_base::scoped_refptr<webrtc::MediaStreamInterface> _globalstream;
    talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _peer_connection_factory;
private slots:
	void Stop();


};

#endif // ACTIVITYMANAGER_H
