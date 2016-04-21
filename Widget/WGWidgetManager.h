#pragma once

#include <vector>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace widget
{
	class WGWidgetBase;
	class WGInputManager;

	class WGWidgetManager : public boost::enable_shared_from_this< WGWidgetManager >
	{
		/////
		// コンストラクタ・デストラクタ
	private:
		WGWidgetManager();
	public:
		~WGWidgetManager();

		/** WidgetManagerを生成 */
		static boost::shared_ptr< WGWidgetManager > createWidgetManager(){
			return ( boost::shared_ptr< WGWidgetManager >( new WGWidgetManager ) );
		}

		////
		// 配下Widget関連
	private:
		/** Widgetリスト */
		std::vector< boost::weak_ptr< WGWidgetBase > > widgetList;
	public:
		/** 配下にWidgetを追加 */
		bool addWidget( boost::weak_ptr< WGWidgetBase > widget );

	public:
		/** 更新処理 */
		void onUpdate();
		/** 描画処理 */
		void onDraw();
	};

}

