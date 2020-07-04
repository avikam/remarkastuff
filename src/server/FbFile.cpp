#include "FbFile.h"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/mman.h>

FbFile::FbFile(const std::string& file_path) :
        fb_error(NO_ERROR),
        fd(-1),
        fix_screen_info {0 },
        var_screen_info {0 },
        screen_memory(nullptr) {
    fd = open(file_path.c_str(), O_RDWR | O_CLOEXEC);

    if (fd < 0) {
        last_errno = errno;
        fb_error = FD;
        return;
    }

    if (0 != ioctl(fd, FBIOGET_FSCREENINFO, &fix_screen_info)) {
        last_errno = errno;
        fb_error = IOCTL_FIX;
        return;
    }

    if (0 != ioctl(fd, FBIOGET_VSCREENINFO, &var_screen_info)) {
        fb_error = IOCTL_VAR;
        last_errno = errno;
        return;
    }

    int len = fix_screen_info.smem_len;
    auto* mem = static_cast<unsigned char *>(
        mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    );

    if (mem == MAP_FAILED) {
        last_errno = errno;
        fb_error = MMAP;
        return;
    }

    screen_memory = screen_mem_t(
        mem,
        [len](unsigned char* mem) { munmap(mem, len); }
    );
}

FbFile::~FbFile() {
    if (fd > 0) {
        close(fd);
        fd = -1;
    }
}

void FbFile::getFixScreenInfo() {
}

const unsigned char* FbFile::screen() const{
    if (screen_memory == nullptr)
        return nullptr;
    return screen_memory.get();
}

void FbFile::refresh() {

}
