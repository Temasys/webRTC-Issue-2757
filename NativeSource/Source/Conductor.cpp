/*
 * Conductor.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: game
 */

#include "Conductor.h"

class DummySetSessionDescriptionObserver:
    public webrtc::SetSessionDescriptionObserver
{
    public:
        static DummySetSessionDescriptionObserver * Create()
        {
            return new talk_base::RefCountedObject<DummySetSessionDescriptionObserver>();
        }

        virtual void OnSuccess()
        {
            LOG(INFO) << "Dummy session description: " << __FUNCTION__;
        }

        virtual void OnFailure(const std::string & error)
        {
            LOG(INFO) << "Dummy session description: " << __FUNCTION__ << " " << error;
        }

    protected:
        DummySetSessionDescriptionObserver()
        {
        }

        ~DummySetSessionDescriptionObserver()
        {
        }
};



Conductor::Conductor(QObject *parent,std::string name)
:QObject(parent),_TAG(name)
{
	 webrtc::MediaConstraintsInterface::Constraint cAudio(webrtc::MediaConstraintsInterface::kOfferToReceiveAudio,"true");
	webrtc::MediaConstraintsInterface::Constraint cVideo(webrtc::MediaConstraintsInterface::kOfferToReceiveVideo,"true");
	webrtc::MediaConstraintsInterface::Constraints cAll;
	webrtc::MediaConstraintsInterface::Constraints cNULL;
	cAll.push_back(cAudio);
	cAll.push_back(cVideo);
	_constrains = new MediaConstraintsNative(cAll,cNULL);
	_serverUrl= "";

}

bool Conductor::Initialize(talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	ASSERT(_peer_connection.get() == NULL);

	if(_peer_connection_factory == NULL)
		_peer_connection_factory  = webrtc::CreatePeerConnectionFactory();

	if (!_peer_connection_factory.get())
	{
		return false;
	}

	webrtc::PeerConnectionInterface::IceServers servers;
	webrtc::PeerConnectionInterface::IceServer server;
	server.uri = std::string(_serverUrl.toLocal8Bit().constData());
	servers.push_back(server);
	_peer_connection =  _peer_connection_factory->CreatePeerConnection(servers,NULL,NULL,this);

	if (!_peer_connection.get())
	{
		return false;
	}
	_mystream = stream;

	if (!_peer_connection->AddStream(_mystream, _constrains))
	{
		LOG(LS_ERROR) << "Adding stream to PeerConnection failed";
	}

	return _peer_connection.get() != NULL;
}

void Conductor::AttemptConnect()
{
	this->CreateOffer();
}

void Conductor::Stop()
{
	Q_EMIT(sendBye());
	this->DeleteAll();
}

Conductor::~Conductor()
{
	assert(_peer_connection.get() == NULL);
	assert(_peer_connection_factory.get() == NULL);
}

///////////////////////////////////////////////
// PeerConnectionClientObserver implementation.
//

void Conductor::CreateOffer()
{
	std::cout <<  _TAG << " " << __FUNCTION__ << std::endl;
    assert(_peer_connection.get() != NULL);
    _peer_connection->CreateOffer(this,_constrains);
}


void Conductor::SetRemoteOffer(std::string type, std::string sdp)
{
	std::cout << "SET REMOTE DESCRIPTION FROM " << _TAG << " WITH TYPE: " << type << std::endl;
	webrtc::SessionDescriptionInterface * session_description(webrtc::CreateSessionDescription(type, sdp));
	_peer_connection->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), session_description);
	if (session_description->type() == webrtc::SessionDescriptionInterface::kOffer)
		_peer_connection->CreateAnswer(this, _constrains);
}

void  Conductor::addIceCandidate(std::string id, std::string sdp, int label)
{
	std::cout << _TAG << " " << __FUNCTION__ << std::endl;
	talk_base::scoped_ptr<webrtc::IceCandidateInterface> candidate(webrtc::CreateIceCandidate(id,label,sdp));
	if (!candidate.get())
	{
		LOG(WARNING) << "Can't parse received candidate message.";
		return;
	}
	if(_peer_connection.get())
	{
		if (!_peer_connection -> AddIceCandidate(candidate.get()))
		{
			LOG(WARNING) << "Failed to apply the received candidate";
			return;
		}
	}
}

//																			  //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// PeerConnectionObserver implementation.
//																			  //
void Conductor::OnError()
{
	LOG(LS_ERROR) <<  _TAG << " " << __FUNCTION__ ;
}

void Conductor::OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed)
{
	LOG(INFO) <<  _TAG << " " << __FUNCTION__ ;
}

// Called when a remote stream is added
void Conductor::OnAddStream(webrtc::MediaStreamInterface * stream)
{
    std::cout << _TAG << " " << __FUNCTION__ << ": " << stream -> label() << std::endl;
}

void Conductor::OnRemoveStream(webrtc::MediaStreamInterface * stream)
{
    LOG(INFO) << _TAG << " " << __FUNCTION__ << ": " << stream -> label();
    stream->AddRef();
    stream->Release();
}

void Conductor::OnRenegotiationNeeded()
{}
void Conductor::OnIceChange()
{}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface * candidate)
{
	std::string sdp;
	if (!candidate->ToString(&sdp))
	{
		LOG(LS_ERROR) << "Failed to serialize candidate";

		return;
	}
	qRegisterMetaType<std::string>();
	Q_EMIT(sendCandidate(candidate->sdp_mid(),candidate->sdp_mline_index(),sdp));
}
//																			  //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  CreateSessionDescriptionObserver implementation.
//																			  //

void Conductor::OnSuccess(webrtc::SessionDescriptionInterface * desc)
{
	std::cout << "SET LOCAL DESCRIPTION FROM " << _TAG << " WITH TYPE: " << desc->type() << std::endl;
	_peer_connection -> SetLocalDescription(DummySetSessionDescriptionObserver::Create(),desc);

	std::string sdp;
	desc -> ToString(&sdp);
	qRegisterMetaType<std::string>();
	Q_EMIT(sendDescription(desc->type(),sdp));
}

void Conductor::OnFailure(const std::string & error)
{
	LOG(LS_ERROR) << _TAG << " " << __FUNCTION__ << "\n\n";
}
//																			  //
////////////////////////////////////////////////////////////////////////////////

void Conductor::callbackBye()
{
	DeleteAll();
}
void Conductor::callbackDescription(std::string type,std::string sdp)
{
	this->SetRemoteOffer(type,sdp);
}
void Conductor::callbackCandidate(std::string mid,int line,std::string sdp)
{
	this->addIceCandidate(mid,sdp,line);
}


void Conductor::DeleteAll()
{
	//Delete the peer connections and stop all rendering
	std::cout << _TAG <<" Delete Local Stream" << std::endl;
	_mystream = NULL;
	LOG(INFO) << _TAG <<" Destroy Peer Connection";
	_peer_connection = NULL;
	LOG(INFO) << _TAG <<" Destroy Peer Connection Factory";
	_peer_connection_factory = NULL;
	delete _constrains;
}
