/*
 * MediaConstraintsNative.h
 *
 *  Created on: Dec 18, 2013
 *      Author: game
 */

#ifndef MEDIACONSTRAINTSNATIVE_H_
#define MEDIACONSTRAINTSNATIVE_H_

#include "talk/app/webrtc/mediaconstraintsinterface.h"


class MediaConstraintsNative : public webrtc::MediaConstraintsInterface
{
 public:
  virtual ~MediaConstraintsNative();
  MediaConstraintsNative();
  MediaConstraintsNative(
                  const webrtc::MediaConstraintsInterface::Constraints& mandatory,
                  const webrtc::MediaConstraintsInterface::Constraints& optional);

  virtual const webrtc::MediaConstraintsInterface::Constraints& GetMandatory() const;
  virtual const webrtc::MediaConstraintsInterface::Constraints& GetOptional() const;

 private:
  webrtc::MediaConstraintsInterface::Constraints mandatory_;
  webrtc::MediaConstraintsInterface::Constraints optional_;
};


#endif /* MEDIACONSTRAINTSNATIVE_H_ */
