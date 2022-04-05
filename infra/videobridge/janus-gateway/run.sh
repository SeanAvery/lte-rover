/janus/bin/janus --config ./rtp-live.jcfg &
cd /janus/html
python3 -m http.server 8000
