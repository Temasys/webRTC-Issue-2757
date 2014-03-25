#include "ActivityManager.h"


cricket::VideoCapturer* OpenVideoCaptureDevice()
{
  talk_base::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(cricket::DeviceManagerFactory::Create());
  if (!dev_manager->Init())
  {
    LOG(LS_ERROR) << "Can't create device manager";
    return NULL;
  }
  std::vector<cricket::Device> devs;
  if (!dev_manager->GetVideoCaptureDevices(&devs))
  {
    LOG(LS_ERROR) << "Can't enumerate video devices";
    return NULL;
  }
  std::vector<cricket::Device>::iterator dev_it = devs.begin();
  cricket::VideoCapturer* capturer = NULL;
  for (; dev_it != devs.end(); ++dev_it)
  {
    capturer = dev_manager->CreateVideoCapturer(*dev_it);
    if (capturer != NULL)
      break;
  }
  return capturer;
}

ActivityManager::ActivityManager(QObject *parent)
    :QObject(parent)
{
    _conductorOne = new talk_base::RefCountedObject<Conductor>(this,"ONE");
    _conductorTwo = new talk_base::RefCountedObject<Conductor>(this,"TWO");
    QObject::connect(_conductorOne,SIGNAL(sendBye()),_conductorTwo,SLOT(callbackBye()));
    QObject::connect(_conductorOne,SIGNAL(sendDescription(std::string,std::string)),
    		_conductorTwo,SLOT(callbackDescription(std::string,std::string)));
    QObject::connect(_conductorOne,SIGNAL(sendCandidate(std::string,int,std::string)),
    		_conductorTwo,SLOT(callbackCandidate(std::string,int,std::string)));
    QObject::connect(_conductorTwo,SIGNAL(sendBye()),_conductorOne,SLOT(callbackBye()));
    QObject::connect(_conductorTwo,SIGNAL(sendDescription(std::string,std::string)),
    		_conductorOne,SLOT(callbackDescription(std::string,std::string)));
    QObject::connect(_conductorTwo,SIGNAL(sendCandidate(std::string,int,std::string)),
    		_conductorOne,SLOT(callbackCandidate(std::string,int,std::string)));

    _peer_connection_factory  = webrtc::CreatePeerConnectionFactory();
    talk_base::scoped_refptr<webrtc::AudioTrackInterface> audio_track(_peer_connection_factory->CreateAudioTrack("Audio", _peer_connection_factory->CreateAudioSource(NULL)));
	talk_base::scoped_refptr<webrtc::VideoTrackInterface> video_track(
			_peer_connection_factory->CreateVideoTrack("Video",_peer_connection_factory->CreateVideoSource(OpenVideoCaptureDevice(),NULL)));

	_globalstream = _peer_connection_factory->CreateLocalMediaStream("Stream");

	_globalstream->AddTrack(audio_track);
	_globalstream->AddTrack(video_track);
}

ActivityManager::~ActivityManager()
{
	_globalstream = NULL;
	_conductorOne = NULL;
	_conductorTwo = NULL;
	_peer_connection_factory = NULL;
}

void ActivityManager::Start()
{
	std::cout << "START" << std::endl;
	std::cout << "Conductor One Init" << std::endl;
    _conductorOne->Initialize(_globalstream) ;
    std::cout << "Conductor Two Init" << std::endl;
    _conductorTwo->Initialize(_globalstream);
    std::cout << "Conductor One connect to TWO" << std::endl;
    _conductorOne->AttemptConnect();
    QTimer::singleShot(5000, this, SLOT(Stop()));
}

void ActivityManager::Stop()
{
	std::cout << "STOP" << std::endl;
    _conductorOne->Stop();
	QCoreApplication::exit(EXIT_SUCCESS);

}
