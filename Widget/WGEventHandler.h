#pragma once

#include <boost/signals2.hpp>
#include <boost/weak_ptr.hpp>

// �n���h���[�֊֐���o�^
#define BIND_HANDLER( func, thiz )( boost::bind( (func), boost::ref( (thiz) ), _1, _2 ) )

namespace widget
{
	class WGWidgetBase; 
	class WGEventArgs; 
	
	// �ł���{�I�ȃC�x���g�n���h���[
	typedef boost::signals2::signal< void ( boost::weak_ptr< WGWidgetBase >, WGEventArgs* ) > EventHandler;
}