## This piece of code implements a bare-bones video streaming application that uses these protocols:
* RTSP - Real-Time Streaming Protocol, runs on TCP
* RTP  - Real-time Transport Protocol, runs on UDP

### RTSP lets the server and client exchange messages that control video playback, such as Init, Play, Pause and Teardown. RTP, on the other hand, is the protocol used to send the actual video stream data.
### This app only supports videos in .Mjpeg format. An example file can be found in the "Python 3 Socket Programming Assigments" .zip file [here](https://media.pearsoncmg.com/ph/esm/ecs_kurose_compnetwork_8/cw/).

## Files
* `ServerWorker.py` defines the `ServerWorker` class, basically the main server that the video content and receives/responds to RTP and RTSP requests.
* `Server.py` runs an instance of `ServerWorker`
* `Client.py` defines the `Client` class
* `ClientLauncher.py` runs an GUI instance of `Client`, which interacts with a `ServerWorker` on the server.
* `VideoStream.py` defines the `VideoStream` class, an instance of which is ran on the server. It opens the queried video file and fetches it frame by frame, keeping track of the video's current position.
* `RtpPacket.py` defines the RtpPacket class: the structure of an RTP packet and some methods for its construction.
