# ffmpeg -i /data/lte-rover/vid-io -vcodec libx264 -pix_fmt bgr24 -an -f rtp rtp://192.168.1.8:5004 -loglevel debug

# ffmpeg -f rawvideo -framerate 30 -pix_fmt bgr24 -s 386*242 -i /data/lte-rover/vid-io -an -f rtp rtp://192.168.1.8:5004 -loglevel debug

ffmpeg -f rawvideo -framerate 30 -pix_fmt bgr24 -s 1164*874 -i /data/lte-rover/vid-io -an -f rtp rtp://192.168.1.8:5004 -loglevel debug

