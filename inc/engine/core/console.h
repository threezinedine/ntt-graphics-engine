#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "engine/core/color.h"

/**
 * The console module is singleton which is used for interacting with the command line / terminal, which show the
 *      message with color.
 */

/**
 * Setup the next character output color in the console, which can be used for printing colored messages in the console.
 * The behavior of this function depends on the specific console implementation, but it generally sets the color for the
 *      next character output in the console, which can be used for subsequent text output until the color is changed
 *      again.
 *
 * @param color The color to be set for the next character output in the console. This parameter is passed to the
 *      console implementation, so that it can determine how to set the color for the console output accordingly. The
 *      behavior of this parameter depends on the specific console implementation, but it generally indicates the color
 *      that should be used for the next character output in the console. This parameter can not be NULL or trigger an
 *      assertion failure.
 */
void ntt_ConsoleSetColor(enum ntt_Color color);

/**
 * Return the console output color to the default color (usually white or gray).
 */
void ntt_ConsoleResetColor();

/**
 * Print a message to the console.
 *
 * @param message The message to be printed to the console. This parameter can not be NULL or trigger an assertion
 *      failure.
 * @param ... The additional arguments for formatting the message, which is used in conjunction with the format
 *      specifiers in the message string. The behavior of these arguments depends on the specific console
 *      implementation, but it generally follows the rules of the standard printf function.
 */
void ntt_ConsolePrint(const char* message, ...);

#endif /* _CONSOLE_H_ */