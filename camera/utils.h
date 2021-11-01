#include <sys/ioctl.h>
#include <sys/cdefs.h>
#include <sys/_types/_errno_t.h>
#include <errno.h>

// __BEGIN_DECLS
// extern int * __error(void);
// #define errno (*__error())
// __END_DECLS

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

static int cam_ioctl(int fd, unsigned long int request, void *arg, const char *log_msg = nullptr) {
  int err = HANDLE_EINTR(ioctl(fd, request, arg));
  if (err != 0 && log_msg) {
  }
  return err;
}