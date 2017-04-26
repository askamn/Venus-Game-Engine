#include "Matrix4.h"

namespace Syndicate { namespace Maths {

Matrix4::Matrix4()
{
	memset(this->elements, 0.0f, 16 * sizeof(float));
}

Matrix4::Matrix4(float diagonal)
{
	for (int i = 0; i < 4 * 4; ++i)
		this->elements[i] = !( i % 4 - i / 4 ) ? diagonal : 0.0f;
}

Matrix4 Matrix4::Identity()
{
	return Matrix4(1.0f);
}

Matrix4& Matrix4::Multiply(const Matrix4& mat4)
{
	float data[4 * 4];

	for (int rows = 0; rows < 4; ++rows)
	{
		for (int cols = 0; cols < 4; ++cols)
		{
			float sum = 0.0f;
			for (int e = 0; e < 4; ++e)
			{
				sum += elements[e + rows * 4] * mat4.elements[cols + e * 4];
			}

			data[cols + rows * 4] = sum;
		}
	}

	memcpy(this->elements, data, 16 * sizeof(float));

	return *this;
}

Matrix4& Matrix4::Invert()
{
	float temp[16];

	temp[0] = elements[5] * elements[10] * elements[15] -
		elements[5] * elements[11] * elements[14] -
		elements[9] * elements[6] * elements[15] +
		elements[9] * elements[7] * elements[14] +
		elements[13] * elements[6] * elements[11] -
		elements[13] * elements[7] * elements[10];

	temp[4] = -elements[4] * elements[10] * elements[15] +
		elements[4] * elements[11] * elements[14] +
		elements[8] * elements[6] * elements[15] -
		elements[8] * elements[7] * elements[14] -
		elements[12] * elements[6] * elements[11] +
		elements[12] * elements[7] * elements[10];

	temp[8] = elements[4] * elements[9] * elements[15] -
		elements[4] * elements[11] * elements[13] -
		elements[8] * elements[5] * elements[15] +
		elements[8] * elements[7] * elements[13] +
		elements[12] * elements[5] * elements[11] -
		elements[12] * elements[7] * elements[9];

	temp[12] = -elements[4] * elements[9] * elements[14] +
		elements[4] * elements[10] * elements[13] +
		elements[8] * elements[5] * elements[14] -
		elements[8] * elements[6] * elements[13] -
		elements[12] * elements[5] * elements[10] +
		elements[12] * elements[6] * elements[9];

	temp[1] = -elements[1] * elements[10] * elements[15] +
		elements[1] * elements[11] * elements[14] +
		elements[9] * elements[2] * elements[15] -
		elements[9] * elements[3] * elements[14] -
		elements[13] * elements[2] * elements[11] +
		elements[13] * elements[3] * elements[10];

	temp[5] = elements[0] * elements[10] * elements[15] -
		elements[0] * elements[11] * elements[14] -
		elements[8] * elements[2] * elements[15] +
		elements[8] * elements[3] * elements[14] +
		elements[12] * elements[2] * elements[11] -
		elements[12] * elements[3] * elements[10];

	temp[9] = -elements[0] * elements[9] * elements[15] +
		elements[0] * elements[11] * elements[13] +
		elements[8] * elements[1] * elements[15] -
		elements[8] * elements[3] * elements[13] -
		elements[12] * elements[1] * elements[11] +
		elements[12] * elements[3] * elements[9];

	temp[13] = elements[0] * elements[9] * elements[14] -
		elements[0] * elements[10] * elements[13] -
		elements[8] * elements[1] * elements[14] +
		elements[8] * elements[2] * elements[13] +
		elements[12] * elements[1] * elements[10] -
		elements[12] * elements[2] * elements[9];

	temp[2] = elements[1] * elements[6] * elements[15] -
		elements[1] * elements[7] * elements[14] -
		elements[5] * elements[2] * elements[15] +
		elements[5] * elements[3] * elements[14] +
		elements[13] * elements[2] * elements[7] -
		elements[13] * elements[3] * elements[6];

	temp[6] = -elements[0] * elements[6] * elements[15] +
		elements[0] * elements[7] * elements[14] +
		elements[4] * elements[2] * elements[15] -
		elements[4] * elements[3] * elements[14] -
		elements[12] * elements[2] * elements[7] +
		elements[12] * elements[3] * elements[6];

	temp[10] = elements[0] * elements[5] * elements[15] -
		elements[0] * elements[7] * elements[13] -
		elements[4] * elements[1] * elements[15] +
		elements[4] * elements[3] * elements[13] +
		elements[12] * elements[1] * elements[7] -
		elements[12] * elements[3] * elements[5];

	temp[14] = -elements[0] * elements[5] * elements[14] +
		elements[0] * elements[6] * elements[13] +
		elements[4] * elements[1] * elements[14] -
		elements[4] * elements[2] * elements[13] -
		elements[12] * elements[1] * elements[6] +
		elements[12] * elements[2] * elements[5];

	temp[3] = -elements[1] * elements[6] * elements[11] +
		elements[1] * elements[7] * elements[10] +
		elements[5] * elements[2] * elements[11] -
		elements[5] * elements[3] * elements[10] -
		elements[9] * elements[2] * elements[7] +
		elements[9] * elements[3] * elements[6];

	temp[7] = elements[0] * elements[6] * elements[11] -
		elements[0] * elements[7] * elements[10] -
		elements[4] * elements[2] * elements[11] +
		elements[4] * elements[3] * elements[10] +
		elements[8] * elements[2] * elements[7] -
		elements[8] * elements[3] * elements[6];

	temp[11] = -elements[0] * elements[5] * elements[11] +
		elements[0] * elements[7] * elements[9] +
		elements[4] * elements[1] * elements[11] -
		elements[4] * elements[3] * elements[9] -
		elements[8] * elements[1] * elements[7] +
		elements[8] * elements[3] * elements[5];

	temp[15] = elements[0] * elements[5] * elements[10] -
		elements[0] * elements[6] * elements[9] -
		elements[4] * elements[1] * elements[10] +
		elements[4] * elements[2] * elements[9] +
		elements[8] * elements[1] * elements[6] -
		elements[8] * elements[2] * elements[5];

	float determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
	determinant = 1.0f / determinant;

	for (size_t i = 0; i < 4 * 4; i++)
	{
		elements[i] = temp[i] * determinant;
	}

	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& mat4)
{
	return this->Multiply(mat4);
}

Matrix4 operator*(Matrix4 a, const Matrix4& b)
{
	return a.Multiply(b);
}

Vector2 Matrix4::Multiply(const Vector2& vec3) const
{
	return Vector2(
		this->columns[0].x * vec3.x + this->columns[0].y * vec3.y + this->columns[0].z + this->columns[0].w,
		this->columns[1].x * vec3.x + this->columns[1].y * vec3.y + this->columns[1].z + this->columns[1].w
	);															
}

Vector3 Matrix4::Multiply(const Vector3& vec3) const
{
	return Vector3(
		this->columns[0].x * vec3.x + this->columns[0].y * vec3.y + this->columns[0].z * vec3.z + this->columns[0].w,
		this->columns[1].x * vec3.x + this->columns[1].y * vec3.y + this->columns[1].z * vec3.z + this->columns[1].w,
		this->columns[2].x * vec3.x + this->columns[2].y * vec3.y + this->columns[2].z * vec3.z + this->columns[2].w
	);
}

Vector4 Matrix4::Multiply(const Vector4& vec4) const
{
	return Vector4(
		this->columns[0].x * vec4.x + this->columns[0].y * vec4.y + this->columns[0].z * vec4.z + this->columns[0].w * vec4.w,
		this->columns[1].x * vec4.x + this->columns[1].y * vec4.y + this->columns[1].z * vec4.z + this->columns[1].w * vec4.w,
		this->columns[2].x * vec4.x + this->columns[2].y * vec4.y + this->columns[2].z * vec4.z + this->columns[2].w * vec4.w,
		this->columns[3].x * vec4.x + this->columns[3].y * vec4.y + this->columns[3].z * vec4.z + this->columns[3].w * vec4.w
	);
}

Vector2 operator*(const Matrix4& a, const Vector2& b)
{
	return a.Multiply(b);
}

Vector3 operator*(const Matrix4& a, const Vector3& b)
{
	return a.Multiply(b);
}

Vector4 operator*(const Matrix4& a, const Vector4& b)
{
	return a.Multiply(b);
}

std::ostream& operator<<(std::ostream& stream, const Matrix4& mat4)
{
	stream << "Matrix4(" << std::endl;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			stream << mat4.elements[i + j * 4] << " ";
		}

		stream << std::endl;
	}

	stream << ")" << std::endl;

	return stream;
}

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float _near, float _far)
{
	Matrix4 result(1.0f);

	// Diagonal elements
	result.elements[0 + 0 * 4] = 2.0f / (right - left);
	result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
	result.elements[2 + 2 * 4] = -2.0f / (_far - _near);

	// Elements of the last column
	result.elements[3 + 0 * 4] = (left + right) / (left - right);
	result.elements[3 + 1 * 4] = (bottom + top) / (bottom - top);
	result.elements[3 + 2 * 4] = -(_far + _near) / (_far - _near);

	return result;
}

Matrix4 Matrix4::Perspective(float fov, float aspectRatio, float _near, float _far)
{
	Matrix4 result(1.0f);

	float q = 1.0f / Math::Tan(Math::toRadians(0.5f * fov));
	float a = 1 / aspectRatio;
	float b = (_near + _far) / (_near - _far);
	float c = (2.0f * _near * _far) / (_near - _far);

	result.elements[0 + 0 * 4] = a;
	result.elements[1 + 1 * 4] = q;
	result.elements[2 + 2 * 4] = b;
	result.elements[3 + 2 * 4] = -1.0f;
	result.elements[2 + 3 * 4] = c;

	return result;
}

Matrix4 Matrix4::Translation(const Vector3& translation)
{
	Matrix4 result(1.0f);

	result.elements[3 + 0 * 4] = translation.x;
	result.elements[3 + 1 * 4] = translation.y;
	result.elements[3 + 2 * 4] = translation.z;
	return result;
}

Matrix4 Matrix4::Scale(const Vector3& scale)
{
	Matrix4 result(1.0f);

	result.elements[0 + 0 * 4] = scale.x;
	result.elements[1 + 1 * 4] = scale.y;
	result.elements[2 + 2 * 4] = scale.z;

	return result;
}

Matrix4 Matrix4::Rotation(float angle, const Vector3& axis)
{
	Matrix4 result(1.0f);

	float c = Math::Cos(Math::toRadians(angle));
	float s = Math::Sin(Math::toRadians(angle));
	float lc = 1 - c;

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	result.elements[0 + 0 * 4] = x * x + c;
	result.elements[1 + 0 * 4] = y * x * lc + z * s;
	result.elements[2 + 0 * 4] = x * z * lc - y * s;

	result.elements[0 + 1 * 4] = x * y * lc - z * s;
	result.elements[1 + 1 * 4] = y * y + lc + c;
	result.elements[2 + 1 * 4] = y * z * lc + x * s;

	result.elements[0 + 2 * 4] = x * z * lc + y * s;
	result.elements[1 + 2 * 4] = y * z * lc - x * s;
	result.elements[2 + 2 * 4] = z * z + lc + c;

	return result;
}

Matrix4 Matrix4::Translate(Matrix4 matrix, const Matrix4& translationMatrix)
{
	return matrix.Multiply(translationMatrix);
}

Matrix4::~Matrix4()
{
}

}}