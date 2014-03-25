demo issue #2757
===============
https://code.google.com/p/webrtc/issues/detail?id=2757&q=transportchannelproxy&colspec=ID%20Pri%20Mstone%20ReleaseBlock%20Area%20Status%20Owner%20Summary

DESCRIPION
This is a native application using Qt4. To run it, start into your webrtc/trunk folder:
```
git clone https://github.com/TemasysCommunications/webRTC-Issue-2757.git
cp webRTC-Issue-2757/all.gyp ./
gclient runhooks
ninja -C out/Debug demoBug
./out/Debug/demoBug
```