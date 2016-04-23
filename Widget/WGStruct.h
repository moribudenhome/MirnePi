#pragma once

#include <boost/numeric/ublas/matrix.hpp>

namespace widget
{
	/**
	 * ウィジェットシステム内で使用される構造体を定義
	 */

	/* 描画基準位置種別 */
	enum PivotType
	{
		BottomLeft,
		Left,
		TopLeft,
		BottomRight,
		Right,
		TopRight
	};

	struct Vector2 {
		Vector2(float x, float y) {
			this->x = x; this->y = y;
		}
		Vector2() {
			x = y = 0.0f;
		}
		float x, y;
	};

	struct Matrix3x3 {
		typedef boost::numeric::ublas::matrix<float> Matrixf;
		Matrix3x3() {
			mat = Matrixf(3, 3);
			identity();
		}
		Matrix3x3 operator *(Matrix3x3& mat) {
			Matrix3x3 result;
			result.mat = boost::numeric::ublas::prod(mat.mat, this->mat);
			return result;
		}

		void identity() {
			mat(0, 0) = mat(1, 1) = mat(2, 2) = 1.0f;
			mat(0, 1) = mat(0, 2) = mat(1, 0) = mat(1, 2) = mat(2, 0) = mat(2, 1) = 0.0f;
		}

		void translate(float tx, float ty) {
			Matrix3x3 _mat;
			_mat.mat(0, 2) = tx;
			_mat.mat(1, 2) = ty;
			mat = boost::numeric::ublas::prod(_mat.mat, mat);
		}

		void scale(float sx, float sy) {
			Matrix3x3 _mat;
			_mat.mat(0, 0) = sx;
			_mat.mat(1, 1) = sy;
			mat = boost::numeric::ublas::prod(_mat.mat, mat);
		}

		Vector2 transformPoint(const Vector2& point) {
			Vector2 result;
			result.x = mat(0, 0) * point.x + mat(0, 1) * point.y + mat(0, 2);
			result.y = mat(1, 0) * point.x + mat(1, 1) * point.y + mat(1, 2);
			return result;
		}

		void dump() {
			printf("------------------------------\n");
			for (int i = 0; i < 3; i++) {
				for (int h = 0; h < 3; h++) {
					printf("[%d][%d]=%f ", i, h, mat(i, h));
				}
				printf("\n");
			}
			printf("------------------------------\n");
		}

		Matrixf mat;
	};

}