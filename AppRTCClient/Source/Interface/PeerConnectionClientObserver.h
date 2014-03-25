#ifndef PEERCONNECTIONCLIENTOBSERVER
#define PEERCONNECTIONCLIENTOBSERVER

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/base/json.h"

class PeerConnectionClientObserver
{
public:

  virtual void OnDisconnected() = 0;
  virtual bool CreatePC() = 0;
  virtual void CreateOffer() = 0;
  virtual void SetRemoteOffer(std::string type, std::string sdp) = 0;
  virtual void addIceCandidate(std::string id, std::string sdp, int label) = 0;
  virtual void SetLocalMedia(webrtc::VideoTrackInterface *video_track) = 0;
  virtual void SetRemoteMedia(webrtc::VideoTrackInterface *video_track) = 0;
  virtual void OnServerConnectionFailure() = 0;
  virtual void sendMessage(Json::Value message) = 0;

 protected:
  virtual ~PeerConnectionClientObserver() {}


};

#endif
