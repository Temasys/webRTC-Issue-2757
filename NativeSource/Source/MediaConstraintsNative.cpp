#include "MediaConstraintsNative.h"

MediaConstraintsNative::~MediaConstraintsNative()
{}

MediaConstraintsNative::MediaConstraintsNative()
{}

MediaConstraintsNative::MediaConstraintsNative(
                                                        const MediaConstraintsInterface::Constraints& mandatory,
                                                        const MediaConstraintsInterface::Constraints& optional)
    : mandatory_(mandatory), optional_(optional)
{}

const webrtc::MediaConstraintsInterface::Constraints& MediaConstraintsNative::GetMandatory() const
{
  return mandatory_;
}

const webrtc::MediaConstraintsInterface::Constraints& MediaConstraintsNative::GetOptional() const
{
  return optional_;
}
