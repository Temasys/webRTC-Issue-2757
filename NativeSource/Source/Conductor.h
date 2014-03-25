/*
 * Conductor.h
 *
 *  Created on: Nov 14, 2013
 *      Author: game
 */

#ifndef CONDUCTOR_H_
#define CONDUCTOR_H_

//libjingle
#include "talk/app/webrtc/videosourceinterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/app/webrtc/jsep.h"

#include "talk/base/common.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/logging.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/base/refcount.h"

//Perso
#include "MediaConstraintsNative.h"
#include <iostream>
#include <assert.h>

//QT
#include <qdebug.h>
#include <qcoreapplication.h>
Q_DECLARE_METATYPE(std::string)
//#include <qtconcurrentrun.h>


/** class Conductor
 *
 *
 */
class Conductor:
    public QObject,
    public webrtc::PeerConnectionObserver,
	public webrtc::CreateSessionDescriptionObserver
{
  Q_OBJECT
    public:

        Conductor(QObject *parent,std::string name);
        bool Initialize(talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream);
        void AttemptConnect();
        void Stop();

        bool connection_active() const;

        ~Conductor();

        //Personal Class function

        //////////////////////////////////////////////////////////////
        //
        //															//
		bool CreatePC();
		void CreateOffer();
		void SetRemoteOffer(std::string type, std::string sdp);
		void addIceCandidate(std::string id, std::string sdp, int label);
		//															//
		//////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////
		// PeerConnectionObserver implementation.
		//																			  //
		virtual void OnError();
		virtual void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed);
		virtual void OnAddStream(webrtc::MediaStreamInterface * stream);
		virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream);
		virtual void OnRenegotiationNeeded();
		virtual void OnIceChange();
		virtual void OnIceCandidate(const webrtc::IceCandidateInterface * candidate);
		//																			  //
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		//  CreateSessionDescriptionObserver implementation.
		//																			  //
		virtual void OnSuccess(webrtc::SessionDescriptionInterface * desc);
		virtual void OnFailure(const std::string & error);
		//																			  //
		////////////////////////////////////////////////////////////////////////////////

        talk_base::scoped_refptr<webrtc::PeerConnectionInterface> _peer_connection;
        talk_base::scoped_refptr<webrtc::MediaStreamInterface> _mystream;
        talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _peer_connection_factory;
        MediaConstraintsNative *_constrains;
   	    QString 				_serverUrl;


	signals:
	    void sendBye();
	    void sendDescription(std::string type,std::string sdp);
	    void sendCandidate(std::string mid,int line,std::string sdp);
	public slots:
		void callbackBye();
	    void callbackDescription(std::string type,std::string sdp);
	    void callbackCandidate(std::string mid,int line,std::string sdp);


	private:
        const std::string _TAG;

	private slots:
			void DeleteAll();
};
#endif /* CONDUCTOR_H_ */
