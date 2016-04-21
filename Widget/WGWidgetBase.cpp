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
		memset(&this->pos, 0, sizeof(this->pos));
		memset(&this->size, 0, sizeof(this->size));
		this->occurEventListIdxCurrent = 0;

		this->pPrevOccurEventListAlias = &this->occurEventList[1];
	}

	/** 
	 * 位置を取得
	 */
	Point WGWidgetBase::getPos() const 
	{ 
		Point p = { this->getPosX(), this->getPosY() };

		return ( p );
	}

	/** 
	 * 位置を取得[X]
	 */
	float WGWidgetBase::getPosX() const 
	{ 
		if( this->parentWidget.lock() ){
			
			return ( this->parentWidget.lock()->getPosX() + this->pos.x );
		}
		
		return ( this->pos.x );
	}

	/** 
	 * 位置を取得[Y]
	 */
	float WGWidgetBase::getPosY() const 
	{ 
		if( this->parentWidget.lock() ){
			
			return ( this->parentWidget.lock()->getPosY() + this->pos.y );
		}

		return ( this->pos.y );
	}

	/** 
	 * 基準位置を取得
	 */
	Point WGWidgetBase::getExtraPos() const
	{
		Point p;
		p.x = this->getExtraPosX();
		p.y = this->getExtraPosY();
		return ( p );
	}

	/** 
	 * 基準位置を取得[X]
	 */
	float WGWidgetBase::getExtraPosX() const
	{
		if( this->parentWidget.lock() ){
			
			return ( this->parentWidget.lock()->getPosX() );
		}

		return ( 0.0f );
	}

	/** 
	 * 基準位置を取得[Y]
	 */
	float WGWidgetBase::getExtraPosY() const
	{
		if( this->parentWidget.lock() ){
			
			return ( this->parentWidget.lock()->getPosY() );
		}

		return ( 0.0f );
	}

	/** 
	 * 指定座標と重なるか取得
	 */
	boost::weak_ptr< WGWidgetBase > WGWidgetBase::isHit( float x, float y )
	{
		if( this->getPosX() <= x && x < ( this->getPosX() + this->size.width )
			&& this->getPosY() <= y && y < (this->getPosY() + this->size.height ) )
		{
			for( unsigned int i = 0; i < this->childWidgets.size(); i++ ){
				
				boost::weak_ptr< WGWidgetBase > widget = this->childWidgets.at( i ).lock()->isHit( x, y );
				if( widget.lock() ){
					return ( widget );
				}
			}
			
			return ( shared_from_this() );
		}

		return ( boost::weak_ptr< WGWidgetBase >() );
	}


	/** 
	 * 更新処理(WidgetManagerから呼ばれる)
	 */
	void WGWidgetBase::update( WGEventArgs* e )
	{
		this->pPrevOccurEventListAlias = &this->occurEventList[ this->occurEventListIdxCurrent ];
		this->occurEventListIdxCurrent = (int)( this->occurEventListIdxCurrent == 0 );
		this->occurEventList[ this->occurEventListIdxCurrent ].clear();

		this->sendEvent( &this->onUpdateHandle, shared_from_this(), e );

		for( unsigned int i = ( this->childWidgets.size() - 1 ); i <= 0; i++ ){
			
			this->childWidgets.at( i ).lock()->update( e );
		}
	}

	/** 
	 * 描画処理(WidgetManagerから呼ばれる)
	 */
	void WGWidgetBase::draw( WGEventArgs* e )
	{
		this->sendEvent( &this->onDrawHandle, shared_from_this(), e );

		for( unsigned int i = 0; i < this->childWidgets.size(); i++ ){
			
			this->childWidgets.at( i ).lock()->draw( e );
		}
	}


	/** 
	 * 直前のフレームで当該イベントは発生していたか
	 */
	bool WGWidgetBase::isPrevFrameOccurEvent( EventHandler* pEvent )
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
	bool  WGWidgetBase::isCurrentFrameOccurEvent( EventHandler* pEvent )
	{
		std::map< EventHandler*, bool >::iterator ite;
		ite = this->occurEventList[ this->occurEventListIdxCurrent ].find( pEvent );
		return ( ite != this->occurEventList[ this->occurEventListIdxCurrent ].end() );
	}

	/** 
	 * 子ウィジェットを追加
	 */
	void WGWidgetBase::addWidget( boost::weak_ptr< WGWidgetBase > widget )
	{
		if( !widget.lock() ){
			return;
		}

		// 親を設定
		widget.lock()->setParentWidget( shared_from_this() );

		this->childWidgets.push_back( widget );
	}
}
