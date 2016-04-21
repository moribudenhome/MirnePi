#pragma once

namespace widget
{
	/**
	 * ウィジェットシステム内で使用される構造体を定義
	 */

	/** 2D座標を示す構造体 */
	struct Point
	{
		// Xを示す値を指定
		float x;
		// Yを示す値を指定
		float y;
	};

	/** 範囲,サイズを示す構造体 */
	struct Rect
	{
		// 幅を示す値を指定
		float width;
		// 高さを示すあタイを指定
		float height;
	};

}