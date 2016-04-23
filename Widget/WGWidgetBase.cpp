#include "WGWidgetBase.h"
#include "WGEventArgs.h"

namespace widget
{

	WGWidgetBase::WGWidgetBase(void)
	{
	}

	WGWidgetBase::~WGWidgetBase(void)
	{
	}

	/** 
	 * 初期化 
	 */
	void WGWidgetBase::Initialize()
	{
		this->occurEventListIdxCurrent = 0;
		this->pPrevOccurEventListAlias = &this->occurEventList[1];
		this->pivotType = PivotType::TopLeft;
		mat.identity();
		localScale = Vector2(1.0f, 1.0f);
		localSize = Vector2(1.0f, 1.0f);
	}

	/** ローカルの位置を設定 */
	void WGWidgetBase::SetLocalPos( Vector2 pos )
	{
		localPos = pos;
		if (parentWidget.lock()) {
			Matrix3x3 parent = parentWidget.lock()->GetTransformMatrix();
			UpdateMatrix(&parent);
		}
		else {
			UpdateMatrix(nullptr);
		}	
	}

	/** ローカルスケールを設定 */
	void WGWidgetBase::SetLocalScale(Vector2 scale)
	{
		localScale = scale;
		if (parentWidget.lock()) {
			Matrix3x3 parent = parentWidget.lock()->GetTransformMatrix();
			UpdateMatrix(&parent);
		}
		else {
			UpdateMatrix(nullptr);
		}
	}

	/**
	* 位置を取得
	*/
	Vector2 WGWidgetBase::GetPos()
	{
		return (mat.transformPoint(Vector2()));
	}

	/* サイズを取得 */
	Vector2 WGWidgetBase::GetSize()
	{
		Matrix3x3 _mat = mat;
		_mat.mat(0, 2) = _mat.mat(1, 2) = 0.0f; // offset値は要らないので潰す
		return _mat.transformPoint(localSize);
	}

	/** 左上の座標を取得 */
	Vector2 WGWidgetBase::GetTopLeftPos()
	{
		// TODO PivotTypeによってずらしてあげる処理が必要

		Vector2 result;
		result.x = GetPos().x;
		result.y = GetPos().y;

		return result;
	}

	/** 
	 * 更新処理
	 */
	void WGWidgetBase::Update( WGEventArgs* e )
	{
		this->pPrevOccurEventListAlias = &this->occurEventList[ this->occurEventListIdxCurrent ];
		this->occurEventListIdxCurrent = (int)( this->occurEventListIdxCurrent == 0 );
		this->occurEventList[ this->occurEventListIdxCurrent ].clear();

		this->SendEvent( &this->OnUpdateHandle, shared_from_this(), e );

		for( unsigned int i = ( this->childWidgets.size() - 1 ); i <= 0; i++ ){
			this->childWidgets.at( i )->Update( e );
		}
	}

	/** 
	 * 描画処理
	 */
	void WGWidgetBase::Draw( WGEventArgs* e )
	{
		this->SendEvent( &this->OnDrawHandle, shared_from_this(), e );

		for( unsigned int i = 0; i < this->childWidgets.size(); i++ ){			
			this->childWidgets.at( i )->Draw( e );
		}
	}

	void WGWidgetBase::UpdateMatrix( Matrix3x3* matrix )
	{
		// 行列を作り直す
		mat.identity();
		mat.translate(localPos.x, localPos.y);
		mat.scale(localScale.x, localScale.y);

		if (matrix != nullptr) {
			mat = mat * *matrix;
		}
		for (unsigned int i = 0; i < this->childWidgets.size(); i++) {
			this->childWidgets.at(i)->UpdateMatrix( &mat );
		}

	}

	/** 
	 * 直前のフレームで当該イベントは発生していたか
	 */
	bool WGWidgetBase::IsPrevFrameOccurEvent( EventHandler* pEvent )
	{
		if( this->pPrevOccurEventListAlias == NULL ){
			return ( false );
		}

		std::map< EventHandler*, bool >::iterator ite;
		ite = this->pPrevOccurEventListAlias->find( pEvent );
		return ( ite != this->pPrevOccurEventListAlias->end() );
	}

	/** 
	 * 現在のフレームで当該イベントは発生していたか
	 */
	bool  WGWidgetBase::IsCurrentFrameOccurEvent( EventHandler* pEvent )
	{
		std::map< EventHandler*, bool >::iterator ite;
		ite = this->occurEventList[ this->occurEventListIdxCurrent ].find( pEvent );
		return ( ite != this->occurEventList[ this->occurEventListIdxCurrent ].end() );
	}

	/** 
	 * 子ウィジェットを追加
	 */
	void WGWidgetBase::AddWidget( boost::shared_ptr< WGWidgetBase > widget )
	{
		if( widget == nullptr ){
			return;
		}

		// 親を設定
		widget->SetParentWidget( shared_from_this() );

		this->childWidgets.push_back( widget );
	}
}
