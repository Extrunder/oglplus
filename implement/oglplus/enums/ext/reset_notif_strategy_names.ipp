/*
 *  .file oglplus/enums/ext/reset_notif_strategy_names.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/oglplus/ext/reset_notif_strategy.txt' instead.
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
OGLPLUS_LIB_FUNC StrCRef ValueName_(
	ResetNotificationStrategyARB*,
	GLenum value
)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVN_RESETNOTIFICATIONSTRATEGYARB)
#define OGLPLUS_IMPL_EVN_RESETNOTIFICATIONSTRATEGYARB
{
switch(value)
{
#if defined GL_NO_RESET_NOTIFICATION_ARB
	case GL_NO_RESET_NOTIFICATION_ARB: return StrCRef("NO_RESET_NOTIFICATION_ARB");
#endif
#if defined GL_LOSE_CONTEXT_ON_RESET_ARB
	case GL_LOSE_CONTEXT_ON_RESET_ARB: return StrCRef("LOSE_CONTEXT_ON_RESET_ARB");
#endif
	default:;
}
OGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums

