#pragma once

#include <boost/weak_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <map>
#include <vector>

#include "WGStruct.h"
#include "WGEventHandler.h"

#define CREATE_WIDGET(T1,...) T1::Create<T1>(__VA_ARGS__)

namespace widget
{
	/**
	 * 全てのウィジェットの基底となるクラス
	 * TODO 座標変換周りが冗長な感じになってるの計算遅いかも。後で治す。
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
	public:
		template < typename T, typename... Args>
		static boost::weak_ptr< T > Create(boost::weak_ptr<WGWidgetBase> parent, Args... args) {
			if (!parent.lock()) {
				return boost::weak_ptr< T >();
			}
			auto p = boost::shared_ptr<T>(new T(args...));
			(boost::dynamic_pointer_cast<WGWidgetBase>(p))->Initialize();
			parent.lock()->AddWidget(p);
			return p;
		}
		static boost::shared_ptr<WGWidgetBase> Create() {
			return boost::shared_ptr<WGWidgetBase>(new WGWidgetBase());
		}

		/* 初期化 */
		virtual void Initialize();

		////
		// 基本機能
	public:
		/** 描画基準位置設定 */
		void SetPivotType( PivotType type ) { pivotType = type; };
		/** 描画基準位置取得 */
		PivotType GetPivotType() const { return pivotType; };
		/** ローカルの位置を設定 */
		void SetLocalPos( Vector2 pos );
		/** ローカルの位置を取得 */
		Vector2 GetLocalPos() const { return ( this->localPos); };
		/** ローカルスケールを設定 */
		void SetLocalScale( Vector2 scale );
		/** ローカルスケールを取得 */
		Vector2 GetLocalScale(Vector2 scale) { return localScale; };
		/** ローカルサイズを設定 */
		void SetLocalSize( Vector2 size ) { this->localSize = size; };
		/** ローカルサイズを設定 */
		Vector2 GetLocalSize(Vector2 size) { return localSize; };
		/** 位置を取得 */
		Vector2 GetPos();
		/* サイズを取得 */
		Vector2 GetSize();
		/** 左上の座標を取得 */
		Vector2 GetTopLeftPos();
		/** 変換済み行列を取得 */
		Matrix3x3 GetTransformMatrix() { return mat; }
	public:
		/** 更新処理 */
		virtual void Update( WGEventArgs* e );
		/** 描画処理 */
		virtual void Draw( WGEventArgs* e );
		
		void UpdateMatrix( Matrix3x3* matrix );

	private:
		/** 位置 */
		Vector2 localPos;
		/** 幅、高さ */
		Vector2 localSize;
		/* スケール */
		Vector2 localScale;
		/* 変換済み行列 */
		Matrix3x3 mat;

		PivotType pivotType;

		/////
		// イベント関連
	public:
		/** 描画イベント */
		EventHandler OnDrawHandle;
		/** 更新イベント */
		EventHandler OnUpdateHandle;

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
		void SendEvent( T1 handle, T2 sender, T3 e ){
			(*handle)( sender, e );
			this->occurEventList[ this->occurEventListIdxCurrent ][ (EventHandler*)handle ] = false;
		}
		/** 直前のフレームで当該イベントは発生していたか */
		bool IsPrevFrameOccurEvent( EventHandler* pEvent );
		/** 現在のフレームで当該イベントは発生していたか */
		bool IsCurrentFrameOccurEvent( EventHandler* pEvent );

		////
		// 階層構造関連
	private:
		/** 親ウィジェット */
		boost::weak_ptr< WGWidgetBase > parentWidget;
		/** 子ウィジェット */
		std::vector< boost::shared_ptr< WGWidgetBase > > childWidgets;
	public:
		/** 親ウィジェットを設定 */
		void SetParentWidget( boost::weak_ptr< WGWidgetBase > parent ){ this->parentWidget = parent; };
		/** 子ウィジェットを追加 */
		void AddWidget( boost::shared_ptr< WGWidgetBase > widget );
	};
}

