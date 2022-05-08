Snapdragon 821 Vision Stack WIP

# why

openpilot codebase is good reference material but not well suited as robotics framework

plus 821 support has ended and the comma C3 dev board/cameras are way too expensive if you want to develop your own product

lets split software up into composable tools, and make a cheap/usable qualcomm dev board

# hardware setup

Leeco Le Pro 3 running NEOS

# mvp software

1. camera capture
2. encoder
3. rtp streamer

# reach goals

1. cython bindings
2. replacable camera/lens (imx 219 fisheye)
