#ifndef REMARK_FBFILE_H
#define REMARK_FBFILE_H

#include <string>
#include <memory>
#include <tuple>
#include <functional>
#include <linux/fb.h>

class FbFile {
public:
    explicit FbFile(const std::string& file_path);
    ~FbFile();

    enum FbError { NO_ERROR, FD, IOCTL_FIX, IOCTL_VAR, MMAP };
    inline std::tuple<FbError, int> getError() const { return std::make_tuple(fb_error, last_errno); };
    void refresh();

    inline int screenBytesSize() const{
        return fix_screen_info.smem_len;
    }

    const unsigned char* screen() const;

    inline fb_var_screeninfo varScreenInfo () const { return var_screen_info; }
    inline fb_fix_screeninfo fixScreenInfo () const { return fix_screen_info; }
private:
    FbError fb_error;
    int last_errno;
    int fd;
    fb_fix_screeninfo fix_screen_info;
    fb_var_screeninfo var_screen_info;

    using screen_mem_t = std::unique_ptr<unsigned char, std::function<void(unsigned char*)>>;
    screen_mem_t screen_memory;

    void getFixScreenInfo();
};

#endif //REMARK_FBFILE_H
