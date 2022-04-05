ffmpeg -re -stream_loop -1 -i test.mp4 -an -c:v vp8 -f rtp -sdp_file video.sdp "rtp://127.0.0.1:5004"
