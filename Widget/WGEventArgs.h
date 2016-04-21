#pragma once

#include <boost/weak_ptr.hpp>

namespace widget
{
	// [メモ]このソースにWGEventArgsの派生クラスも定義する予定

	class WGInputManager;

	 /////
	 // イベントデータが格納されている構造体の基底
	struct WGEventArgs{};

	//////
	//// 各種ユーザー入力情報を格納
	//struct WGInputEventArgs : public WGEventArgs
	//{
	//	boost::weak_ptr< WGInputManager > pInputMgr;
	//};
}

