#pragma once

#include <boost/weak_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <map>
#include <vector>

#include "WGStruct.h"
#include "WGEventHandler.h"

namespace widget
{
	class WGWidgetManager;

	/**
	 * 全てのウィジェットの基底となるクラス
	 */
	class WGWidgetBase : public boost::enable_shared_from_this< WGWidgetBase >
	{
		/////
		// コンストラクタ・デストラクタ
	protected:
		WGWidgetBase(void);
	public:
		virtual ~WGWidgetBase(void);
		
		/** widgetを生成 */
		template < typename T >
		static boost::weak_ptr< T > createWidget( boost::weak_ptr<WGWidgetBase> parent ){
			if (!parent.lock()) {
				return boost::weak_ptr< T >();
			}
			auto w = boost::shared_ptr< T >(new T);
			(boost::dynamic_pointer_cast<WGWidgetBase>(w))->Initialize();
			parent.lock()->addWidget(w);
			return w;
		}
		static boost::shared_ptr<WGWidgetBase> createRootWidget() {
			return boost::shared_ptr<WGWidgetBase>(new WGWidgetBase());
		}

		/* 初期化 */
		virtual void Initialize();

		////
		// 基本機能
	public:
		/** 位置を設定 */
		void setPos( float x, float y ){ this->pos.x = x; this->pos.y = y; };
		/** 位置を設定 */
		void setPos( Point pos ){ this->pos = pos; };
		/** 位置を取得 */
		Point getPos() const;
		/** 位置を取得[X] */
		float getPosX() const;
		/** 位置を取得[Y] */
		float getPosY() const;
		/** 基準位置を取得 */
		Point getExtraPos() const;
		/** 基準位置を取得[X] */
		float getExtraPosX() const;
		/** 基準位置を取得[Y] */
		float getExtraPosY() const;
		/** ローカルの位置を取得 */
		Point getLocalPos() const { return ( this->pos ); };
		/** ローカルの位置を取得[X] */
		float getLocalPosX() const { return ( this->pos.x ); };
		/** ローカルの位置を取得[Y] */
		float getLocalPosY() const { return ( this->pos.y ); };
		/** サイズを設定 */
		void setSize( float width, float height ) { this->size.width = width; this->size.height= height; };
		/** サイズを設定 */
		void setSize( Rect size ) { this->size = size; };
		/** サイズを取得 */
		Rect getSize() const { return ( this->size ); };
		/** サイズを取得[W] */
		float getWidth() const { return ( this->size.width ); };
		/** サイズを取得[H] */
		float getHeight() const { return ( this->size.height ); };
	public:
		/** 更新処理(WidgetManagerから呼ばれる) */
		virtual void update( WGEventArgs* e );
		/** 描画処理(WidgetManagerから呼ばれる) */
		virtual void draw( WGEventArgs* e );

	private:
		/** 位置 */
		Point pos;
		/** 幅、高さ */
		Rect size;

		/////
		// イベント関連
	public:
		/** 描画イベント */
		EventHandler onDrawHandle;
		/** 更新イベント */
		EventHandler onUpdateHandle;

	public:
		/** 直前のフレームで発生したイベントリストへのポインタ(アクセス用) */
		std::map< EventHandler*, bool >* pPrevOccurEventListAlias;
		/** 発生したイベントリスト */
		std::map< EventHandler*, bool > occurEventList[ 2 ];
	private:
		/** 現在を示すイベントリストのインデックス */
		int occurEventListIdxCurrent;
	public:
		/** イベントの呼び出し(イベントの呼び出しは必ずこのメソッドを介して行うルール) */
		template <typename T1, typename T2, typename T3>
		void sendEvent( T1 handle, T2 sender, T3 e ){
			(*handle)( sender, e );
			this->occurEventList[ this->occurEventListIdxCurrent ][ (EventHandler*)handle ] = false;
		}
		/** 直前のフレームで当該イベントは発生していたか */
		bool isPrevFrameOccurEvent( EventHandler* pEvent );
		/** 現在のフレームで当該イベントは発生していたか */
		bool isCurrentFrameOccurEvent( EventHandler* pEvent );

		////
		// 階層構造関連
	private:
		/** 親ウィジェット */
		boost::weak_ptr< WGWidgetBase > parentWidget;
		/** 子ウィジェット */
		std::vector< boost::shared_ptr< WGWidgetBase > > childWidgets;
	public:
		/** 親ウィジェットを設定 */
		void setParentWidget( boost::weak_ptr< WGWidgetBase > parent ){ this->parentWidget = parent; };
		/** 子ウィジェットを追加 */
		void addWidget( boost::shared_ptr< WGWidgetBase > widget );

	};
}

