/*
 *  .file oglplus/names/texture_mag_filter.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/texture_mag_filter.txt' instead.
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

switch(GLenum(value))
{
#if defined GL_NEAREST
	case GL_NEAREST: return "NEAREST";
#endif
#if defined GL_LINEAR
	case GL_LINEAR: return "LINEAR";
#endif
	default:;
}

