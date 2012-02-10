/*
 *  .file oglplus/enums/stencil_operation.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/stencil_operation.txt' instead.
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#if OGLPLUS_DOCUMENTATION_ONLY

/// KEEP
Keep,
/// ZERO
Zero,
/// REPLACE
Replace,
/// INCR
Incr,
/// DECR
Decr,
/// INVERT
Invert,
/// INCR_WRAP
IncrWrap,
/// DECR_WRAP
DecrWrap

#else // !OGLPLUS_DOCUMENTATION_ONLY

#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined GL_KEEP
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
Keep = GL_KEEP
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_ZERO
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
Zero = GL_ZERO
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_REPLACE
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
Replace = GL_REPLACE
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_INCR
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
Incr = GL_INCR
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_DECR
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
Decr = GL_DECR
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_INVERT
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
Invert = GL_INVERT
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_INCR_WRAP
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
IncrWrap = GL_INCR_WRAP
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#if defined GL_DECR_WRAP
# if OGLPLUS_LIST_NEEDS_COMMA
,
# endif
DecrWrap = GL_DECR_WRAP
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

#endif // !OGLPLUS_DOCUMENTATION_ONLY

