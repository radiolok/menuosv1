/*Copyright (c) 2013-2026, Artem Kashkanov
All rights reserved. BSD 2-Clause, see LICENSE.*/

/**
 *  \file termdisp.h
 *  \brief Terminal display: the menu is kept in a text frame buffer and
 *  printed to the console (UART on MCUs, a string in host tests).
 *
 *  Frame layout for a 16-char, 5-row display:
 *  \code
 *  +----------------+
 *  |Folder 1        |
 *  >Folder 2        <      selected row
 *  |config 1       5|
 *  |                |
 *  |                |
 *  +----------------+
 *                  ^       edit cursor (config editor)
 *  \endcode
 */
#ifndef TERMDISP_H_
#define TERMDISP_H_

#include <stdint.h>
#include <stddef.h>

/** Size of a buffer that always fits one rendered frame. */
#define TERMDISP_FRAME_SIZE ((DISPSTRLENGTH + 3) * (DISPSTRNUMB + 3) + 1)

/**
 *  \brief Render the current frame into \p buf
 *
 *  \param [out] buf destination, at least TERMDISP_FRAME_SIZE bytes
 *  \param [in] size size of \p buf
 *  \return number of characters written (without the terminating zero)
 */
size_t TermDispRender(char *buf, size_t size);

#endif /* TERMDISP_H_ */
