/*
 *  .file oglplus/names/buffer_target.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/buffer_target.txt' instead.
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

switch(GLenum(value))
{
#if defined GL_ARRAY_BUFFER
	case GL_ARRAY_BUFFER: return "ARRAY_BUFFER";
#endif
#if defined GL_ATOMIC_COUNTER_BUFFER
	case GL_ATOMIC_COUNTER_BUFFER: return "ATOMIC_COUNTER_BUFFER";
#endif
#if defined GL_COPY_READ_BUFFER
	case GL_COPY_READ_BUFFER: return "COPY_READ_BUFFER";
#endif
#if defined GL_COPY_WRITE_BUFFER
	case GL_COPY_WRITE_BUFFER: return "COPY_WRITE_BUFFER";
#endif
#if defined GL_DRAW_INDIRECT_BUFFER
	case GL_DRAW_INDIRECT_BUFFER: return "DRAW_INDIRECT_BUFFER";
#endif
#if defined GL_ELEMENT_ARRAY_BUFFER
	case GL_ELEMENT_ARRAY_BUFFER: return "ELEMENT_ARRAY_BUFFER";
#endif
#if defined GL_PIXEL_PACK_BUFFER
	case GL_PIXEL_PACK_BUFFER: return "PIXEL_PACK_BUFFER";
#endif
#if defined GL_PIXEL_UNPACK_BUFFER
	case GL_PIXEL_UNPACK_BUFFER: return "PIXEL_UNPACK_BUFFER";
#endif
#if defined GL_TEXTURE_BUFFER
	case GL_TEXTURE_BUFFER: return "TEXTURE_BUFFER";
#endif
#if defined GL_TRANSFORM_FEEDBACK_BUFFER
	case GL_TRANSFORM_FEEDBACK_BUFFER: return "TRANSFORM_FEEDBACK_BUFFER";
#endif
#if defined GL_UNIFORM_BUFFER
	case GL_UNIFORM_BUFFER: return "UNIFORM_BUFFER";
#endif
	default:;
}

