#pragma once

#include <boost/signals2.hpp>
#include <boost/weak_ptr.hpp>

// ハンドラーへ関数を登録
#define BIND_HANDLER( func, thiz )( boost::bind( (func), boost::ref( (thiz) ), _1, _2 ) )

namespace widget
{
	class WGWidgetBase; 
	class WGEventArgs; 
	
	// 最も基本的なイベントハンドラー
	typedef boost::signals2::signal< void ( boost::weak_ptr< WGWidgetBase >, WGEventArgs* ) > EventHandler;
}