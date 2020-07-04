//
// Created by avikam on 7/4/20.
//

#ifndef REMARK_MESSAGES_H
#define REMARK_MESSAGES_H

namespace messages {
    using u32 = unsigned int;

    struct hello {
        u32 xres;			    /* visible resolution		*/
        u32 yres;
        u32 xres_virtual;		/* virtual resolution		*/
        u32 yres_virtual;
        u32 xoffset;			/* offset from virtual to visible */
        u32 yoffset;			/* resolution			*/

        u32 bits_per_pixel;		/* guess what			*/
        u32 grayscale;		    /* 0 = color, 1 = grayscale,	*/

        u32 line_length;
        u32 smem_len;
    };
}
#endif //REMARK_MESSAGES_H
