/**
 *  @file oglplus/images/image.ipp
 *  @brief Implementation of images::Image
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace oglplus {
namespace images {

OGLPLUS_LIB_FUNC
bool Image::_is_initialized(void) const
OGLPLUS_NOEXCEPT(true)
{
	return (!_storage.empty()) && (_convert != nullptr);
}

OGLPLUS_LIB_FUNC
PixelDataFormat Image::_get_def_pdf(unsigned n)
OGLPLUS_NOEXCEPT(true)
{
#if GL_VERSION_2_0 || GL_ES_VERSION_3_0
	if(n == 1) return PixelDataFormat::Red;
	if(n == 2) return PixelDataFormat::RG;
	if(n == 3) return PixelDataFormat::RGB;
	if(n == 4) return PixelDataFormat::RGBA;
	OGLPLUS_ABORT("Invalid number of color channels!");
	return PixelDataFormat::Red;
#else
	if(n == 1) return PixelDataFormat::Luminance;
	if(n == 2) return PixelDataFormat::LuminanceAlpha;
	if(n == 3) return PixelDataFormat::RGB;
	if(n == 4) return PixelDataFormat::RGBA;
	assert(!"Invalid number of color channels!");
    return PixelDataFormat::Luminance;
#endif
}

OGLPLUS_LIB_FUNC
PixelDataInternalFormat Image::_get_def_pdif(unsigned n)
OGLPLUS_NOEXCEPT(true)
{
#if GL_VERSION_2_0 || GL_ES_VERSION_3_0
	if(n == 1) return PixelDataInternalFormat::Red;
	if(n == 2) return PixelDataInternalFormat::RG;
	if(n == 3) return PixelDataInternalFormat::RGB;
	if(n == 4) return PixelDataInternalFormat::RGBA;
	OGLPLUS_ABORT("Invalid number of color channels!");
	return PixelDataInternalFormat::Red;
#else
	if(n == 1) return PixelDataInternalFormat::Luminance;
	if(n == 2) return PixelDataInternalFormat::LuminanceAlpha;
	if(n == 3) return PixelDataInternalFormat::RGB;
	if(n == 4) return PixelDataInternalFormat::RGBA;
	assert(!"Invalid number of color channels!");
    return PixelDataInternalFormat::Luminance;
#endif
}

} // namespace images
} // namespace oglplus

