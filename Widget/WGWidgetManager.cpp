#include "WGWidgetManager.h"
#include "WGWidgetBase.h"
#include "WGEventArgs.h"

#include <boost/weak_ptr.hpp>


namespace widget
{

	WGWidgetManager::WGWidgetManager()
	{
	}

	WGWidgetManager::~WGWidgetManager()
	{
	}

	/** 
	 * 配下にウィジェットを追加
	 */
	bool WGWidgetManager::addWidget( boost::weak_ptr< WGWidgetBase > widget )
	{
		if( widget.lock() == NULL ){
			return ( false );
		}
		
		widget.lock()->setRootWidgetManager( shared_from_this() );
		widget.lock()->setParentWidget( boost::weak_ptr< WGWidgetBase >() );

		this->widgetList.push_back( widget );

		return ( true );
	}

	/** 
	 * 更新処理
	 */
	void WGWidgetManager::onUpdate()
	{
		/////
		// 更新処理呼び出し
		for( unsigned int i = 0; i < this->widgetList.size(); i++ ){
			
			this->widgetList.at( i ).lock()->update( (WGEventArgs*)NULL );
		}
	}

	/** 
	 * 描画処理
	 */
	void WGWidgetManager::onDraw()
	{
		/////
		// 描画処理呼び出し
		for( unsigned int i = 0; i < this->widgetList.size(); i++ ){
			
			this->widgetList.at( i ).lock()->draw((WGEventArgs*)NULL);
		}
	}
}