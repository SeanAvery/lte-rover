# ffmpeg -i /data/lte-rover/vid-io -vcodec libx264 -pix_fmt bgr24 -an -f rtp rtp://192.168.1.8:5004 -loglevel debug

ffmpeg -f rawvideo -pix_fmt bgr32 -s 1156*724 -i /data/lte-rover/vid-io -an -f rtp rtp://192.168.1.8:5004 -loglevel debug

