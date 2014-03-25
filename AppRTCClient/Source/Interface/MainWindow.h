#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

//libjingle
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"
//QT
#include <qstring.h>
//Perso
#include "MainWindowObserver.h"

// Pure virtual interface for the main window.
class MainWindow
{
 public:
  virtual ~MainWindow() {}

  enum UI {
    CONNECT_TO_SERVER,
    STREAMING
  };

  virtual void RegisterObserver(MainWindowObserver* callback) = 0;
  virtual void MessageBox(const QString caption, const QString text, bool is_error) = 0;
  virtual UI current_ui() = 0;
  virtual void SwitchToConnectUI() = 0;
  virtual void SwitchToStreamingUI() = 0;

  virtual void StartLocalRenderer(webrtc::VideoTrackInterface* local_video) = 0;
  virtual void StopLocalRenderer() = 0;
  virtual void StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video) = 0;
  virtual void StopRemoteRenderer() = 0;
};

#endif /* MAINWINDOW_H_ */
