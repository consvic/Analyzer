/**
 * Copyright (c) 2016 Abelardo López Lagunas
 *
 * @file    types.h
 *
 * @author  Abelardo López Lagunas
 *
 * @date    Mon 16 May 2016 11:43 DST
 *
 * @brief   These are some type definitions that are required by functions in
 +          the FileIO and UserDefined files.
 *
 * References:
 *          Had to place the type definitions here I can import them in all
 *          the required places without breaking things.
 *
 * Restrictions:
 *          None that I know of.
 *
 * Revision history:
 *          Mon 16 May 2016 -- File created
 *
 */

#ifndef Hash_types_h
#define Hash_types_h

#include <stdlib.h>

#define BUFSIZE 256   // Buffer size for string management
/**
 * @enum myTypes
 *
 * @brief This is an enum that identifies types of tiny C variables
 *
 * The @c myTypes enum describes the possible types of tiny C variables
 * [@cerror, @cinteger, @creal]. They are the basis for type checking by
 * the compiler.
 *
 */
enum myTypes {error = EXIT_FAILURE, integer, real};

#endif
