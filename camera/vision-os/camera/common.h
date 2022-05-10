#include <sys/ioctl.h>

// keep trying if x gets interrupted by a signal
#define HANDLE_EINTR(x)                                       \
  ({                                                          \
    decltype(x) ret;                                          \
    int try_cnt = 0;                                          \
    do {                                                      \
      ret = (x);                                              \
    } while (ret == -1 && errno == EINTR && try_cnt++ < 100); \
    ret;                                                      \
  })

static void ioctl_err() {
  if (errno == EBADF) {
    std::cout << "bad file descriptor" << std::endl;
  }  else if (errno == EFAULT) {
    std::cout << "inaccessible memory" << std::endl;
  } else if (errno == EINVAL) {
    std::cout << "request or argp is not valid" << std::endl;
  } else if (errno == ENOTTY) {
    std::cout << "fd is not associated with a character special device" << std::endl;
  } else {
    std::cout << "unrecognized ioctl errno: " << errno << std::endl;
  } 
}

static int cam_ioctl(int fd, unsigned long int request, void *arg, const char *log_msg = nullptr) {
  int err = HANDLE_EINTR(ioctl(fd, request, arg));
  if (err != 0 && log_msg) {
    // LOG(util::string_format("%s: %d", log_msg, err).c_str());
    std::cout << "errno: " << errno << std::endl;
    ioctl_err();
  }
  return err;
}

