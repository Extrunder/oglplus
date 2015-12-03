/**
 *  @file oglplus/context/string_queries.hpp
 *  @brief Wrappers for GL string queries
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_CONTEXT_STRING_QUERIES_1202210920_HPP
#define OGLPLUS_CONTEXT_STRING_QUERIES_1202210920_HPP

#include <oglplus/glfunc.hpp>
#include <oglplus/string_query.hpp>

#include <cassert>

namespace oglplus {
namespace aux {

#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_2_0 || GL_ES_VERSION_3_0
class StrQueryRange
{
private:
	GLuint _index, _count;
	GLenum _param;
public:
	typedef String ValueType;

	StrQueryRange(GLuint n, GLenum p)
	 : _index(0)
	 , _count(n)
	 , _param(p)
	{ }

	bool Empty(void) const
	{
		assert(_index <= _count);
		return _index == _count;
	}

	std::size_t Size(void) const
	{
		return _count;
	}

	StrCRef Front(void) const
	{
		assert(!Empty());
		const GLubyte* result = OGLPLUS_GLFUNC(GetStringi)(
			_param,
			_index
		);
		OGLPLUS_VERIFY_SIMPLE(GetStringi);
		return StrCRef(reinterpret_cast<const char*>(result));
	}

	void Next(void)
	{
		assert(!Empty());
		++_index;
	}
};

#else

class StrQueryRange
{
private:
    String _str;
    String::const_iterator _front, _end;
    GLuint _count;
public:
	typedef String ValueType;

	StrQueryRange(GLenum p)
     : _front(nullptr)
     , _end(nullptr)
     , _count(0)
	{
        const GLubyte * ret = OGLPLUS_GLFUNC(GetString)(p);
        if (ret) {
            _str = (const char *)ret;
            if (0 < _str.size()) {
                ++_count;
                for (auto found = _str.find(" "); found != String::npos; found = _str.find(" ")) {
                    _str.replace(found, 1, "\0");
                    ++_count;
                }
            }
            _front = _str.begin();
            _end = _str.end();
        }
    }

	bool Empty(void) const
	{
        assert(_front <= _end);
        return _front == _end;
	}

	std::size_t Size(void) const
	{
		return _count;
	}

	StrCRef Front(void) const
	{
		assert(!Empty());
		return StrCRef((const GLchar*)&(*_front));
	}

	void Next(void)
	{
		assert(!Empty());
		while (_front != _end && *_front != '\0') {
            ++_front;
        }
        if (_front != _end && *_front == '\0') {
            ++_front;
        }
	}
};

#endif

} // namespace aux

namespace context {

/// Wrapper for the GL string-query-related operations
/**
 *  @ingroup ogl_context
 */
class StringQueries
{
public:
	/// Queries a string describing GL properties
	/**
	 *  @throws Error
	 *
	 *  @glsymbols
	 *  @glfunref{GetString}
	 */
	static const GLubyte* GetString(StringQuery query)
	{
		const GLubyte* result = OGLPLUS_GLFUNC(GetString)(GLenum(query));
		OGLPLUS_VERIFY_SIMPLE(GetString);
		return result;
	}

	/// Returns the vendor name
	/**
	 *  @glsymbols
	 *  @glfunref{GetString}
	 *  @gldefref{VENDOR}
	 */
	static const char* Vendor(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::Vendor)
		);
	}

	/// Returns the version string
	/**
	 *  @glsymbols
	 *  @glfunref{GetString}
	 *  @gldefref{VERSION}
	 */
	static const char* Version(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::Version)
		);
	}

#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_4_3
	/// Queries the number of supported shading language versions
	/**
	 *  @throws Error
	 *
	 *  @see ShadingLanguageVersion
	 *
	 *  @glverreq{4,3}
	 *  @glsymbols
	 *  @glfunref{Get}
	 *  @gldefref{NUM_SHADING_LANGUAGE_VERSIONS}
	 */
	static GLuint NumShadingLanguageVersions(void)
	{
		GLint result = 0;
		OGLPLUS_GLFUNC(GetIntegerv)(
			GL_NUM_SHADING_LANGUAGE_VERSIONS,
			&result
		);
		OGLPLUS_VERIFY_SIMPLE(GetIntegerv);
		return GLuint(result);
	}

	/// Returns the name of @p index -th supported shading language version
	/**
	 *  @throws Error
	 *
	 *  @see NumShadingLanguageVersions
	 *
	 *  @glverreq{4,3}
	 *  @glsymbols
	 *  @glfunref{GetStringi}
	 *  @gldefref{EXTENSIONS}
	 */
	static const GLubyte* ShadingLanguageVersion(GLuint index)
	{
		const GLubyte* result = OGLPLUS_GLFUNC(GetStringi)(
			GL_SHADING_LANGUAGE_VERSION,
			index
		);
		OGLPLUS_VERIFY_SIMPLE(GetStringi);
		return result;
	}
#endif // GL_VERSION_4_3

#if OGLPLUS_DOCUMENTATION_ONLY
	/// Returns a range of supported GLSL version strings
	/**
	 *  @glverreq{4,3}
	 *  @glsymbols
	 *  @glfunref{GetString}
	 *  @gldefref{SHADING_LANGUAGE_VERSION}
	 */
	static Range<String> ShadingLanguageVersions(void);
#elif GL_VERSION_4_3
	static aux::StrQueryRange ShadingLanguageVersions(void)
	{
		return aux::StrQueryRange(
			NumShadingLanguageVersions(),
			GL_SHADING_LANGUAGE_VERSION
		);
	}
#endif

	/// Returns the shading language version string
	/**
	 *  @glsymbols
	 *  @glfunref{GetString}
	 *  @gldefref{SHADING_LANGUAGE_VERSION}
	 */
	static const char* ShadingLanguageVersion(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::ShadingLanguageVersion)
		);
	}

	/// Returns the renderer name
	/**
	 *  @glsymbols
	 *  @glfunref{GetString}
	 *  @gldefref{RENDERER}
	 */
	static const char* Renderer(void)
	{
		return reinterpret_cast<const char*>(
			GetString(StringQuery::Renderer)
		);
	}

#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_2_0 || GL_ES_VERSION_3_0
	/// Queries the number of extension strings
	/**
	 *  @throws Error
	 *
	 *  @see GetExtension
	 *
	 *  @glsymbols
	 *  @glfunref{Get}
	 *  @gldefref{NUM_EXTENSIONS}
	 */
	static GLuint NumExtensions(void)
	{
		GLint result = 0;
		OGLPLUS_GLFUNC(GetIntegerv)(GL_NUM_EXTENSIONS, &result);
		OGLPLUS_VERIFY_SIMPLE(GetIntegerv);
		return GLuint(result);
	}
#endif


#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_3_0 || GL_ES_VERSION_3_0
	/// Returns the name of the @p index -th extension
	/**
	 *  @throws Error
	 *
	 *  @see NumExtensions
	 *
	 *  @glsymbols
	 *  @glfunref{GetStringi}
	 *  @gldefref{EXTENSIONS}
	 */
	static const GLubyte* Extensions(GLuint index)
	{
		const GLubyte* result = OGLPLUS_GLFUNC(GetStringi)(
			GL_EXTENSIONS,
			index
		);
		OGLPLUS_VERIFY_SIMPLE(GetStringi);
		return result;
	}
#endif

#if OGLPLUS_DOCUMENTATION_ONLY
	/// Returns a range of extension strings
	/**
	 *  @glsymbols
	 *  @glfunref{GetString}
	 *  @gldefref{EXTENSIONS}
	 */
	static Range<StrCRef> Extensions(void);
#elif GL_VERSION_2_0 || GL_ES_VERSION_3_0
	static aux::StrQueryRange Extensions(void)
	{
		return aux::StrQueryRange(
			NumExtensions(),
			GL_EXTENSIONS
		);
	}
#else
    static aux::StrQueryRange Extensions(void)
	{
		return aux::StrQueryRange(
			GL_EXTENSIONS
		);
	}
#endif
};

} // namespace context
} // namespace oglplus

#endif // include guard
