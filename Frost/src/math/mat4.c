#include "mat4.h"

mat4 mat4_indentity()
{
	mat4 m;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m.values[i][j] = (i == j) ? 1.0f : 0.0f;

	return m;
}

mat4 mat4_translate(const mat4 m, vec3 v)
{
	mat4 result;

	result.values[0][0] = m.values[0][0];
	result.values[1][0] = m.values[1][0];
	result.values[2][0] = m.values[2][0];
	result.values[3][0] = m.values[3][0];
	result.values[0][1] = m.values[0][1];
	result.values[1][1] = m.values[1][1];
	result.values[2][1] = m.values[2][1];
	result.values[3][1] = m.values[3][1];
	result.values[0][2] = m.values[0][2];
	result.values[1][2] = m.values[1][2];
	result.values[2][2] = m.values[2][2];
	result.values[3][2] = m.values[3][2];
	result.values[0][3] = m.values[0][3] + v.x;
	result.values[1][3] = m.values[1][3] + v.y;
	result.values[2][3] = m.values[2][3] + v.z;
	result.values[3][3] = m.values[3][3];

	return result;
}

mat4 mat4_inverse(const mat4 mat)
{
	float t[6];
	float det;
	float a = mat.values[0][0], b = mat.values[0][1], c = mat.values[0][2], d = mat.values[0][3],
		e = mat.values[1][0], f = mat.values[1][1], g = mat.values[1][2], h = mat.values[1][3],
		i = mat.values[2][0], j = mat.values[2][1], k = mat.values[2][2], l = mat.values[2][3],
		m = mat.values[3][0], n = mat.values[3][1], o = mat.values[3][2], p = mat.values[3][3];

	t[0] = k * p - o * l; t[1] = j * p - n * l; t[2] = j * o - n * k;
	t[3] = i * p - m * l; t[4] = i * o - m * k; t[5] = i * n - m * j;

	mat4 result;
	result.values[0][0] = f * t[0] - g * t[1] + h * t[2];
	result.values[1][0] = -(e * t[0] - g * t[3] + h * t[4]);
	result.values[2][0] = e * t[1] - f * t[3] + h * t[5];
	result.values[3][0] = -(e * t[2] - f * t[4] + g * t[5]);

	result.values[0][1] = -(b * t[0] - c * t[1] + d * t[2]);
	result.values[1][1] = a * t[0] - c * t[3] + d * t[4];
	result.values[2][1] = -(a * t[1] - b * t[3] + d * t[5]);
	result.values[3][1] = a * t[2] - b * t[4] + c * t[5];

	t[0] = g * p - o * h; t[1] = f * p - n * h; t[2] = f * o - n * g;
	t[3] = e * p - m * h; t[4] = e * o - m * g; t[5] = e * n - m * f;

	result.values[0][2] = b * t[0] - c * t[1] + d * t[2];
	result.values[1][2] = -(a * t[0] - c * t[3] + d * t[4]);
	result.values[2][2] = a * t[1] - b * t[3] + d * t[5];
	result.values[3][2] = -(a * t[2] - b * t[4] + c * t[5]);

	t[0] = g * l - k * h; t[1] = f * l - j * h; t[2] = f * k - j * g;
	t[3] = e * l - i * h; t[4] = e * k - i * g; t[5] = e * j - i * f;

	result.values[0][3] = -(b * t[0] - c * t[1] + d * t[2]);
	result.values[1][3] = a * t[0] - c * t[3] + d * t[4];
	result.values[2][3] = -(a * t[1] - b * t[3] + d * t[5]);
	result.values[3][3] = a * t[2] - b * t[4] + c * t[5];

	det = 1.0f / (a * result.values[0][0] + b * result.values[1][0] + c * result.values[2][0] + d * result.values[3][0]);

	return mat4_scale(result, det);
}

mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far)
{
	mat4 m = mat4_indentity();

	float rl, tb, fn;

	rl = 1.0f / (right - left);
	tb = 1.0f / (top - bottom);
	fn = -1.0f / (far - near);

	m.values[0][0] = 2.0f * rl;
	m.values[1][1] = 2.0f * tb;
	m.values[2][2] = 2.0f * fn;
	m.values[3][0] = -(right + left) * rl;
	m.values[3][1] = -(top + bottom) * tb;
	m.values[3][2] = (far + near) * fn;
	m.values[3][3] = 1.0f;

	return m;
}

mat4 mat4_scale(const mat4 m, float s)
{
	mat4 scaled;

	scaled.values[0][0] = m.values[0][0] * s;
	scaled.values[0][1] = m.values[1][0] * s;
	scaled.values[0][2] = m.values[2][0] * s;
	scaled.values[0][3] = m.values[3][0] * s;
	scaled.values[1][0] = m.values[0][1] * s;
	scaled.values[1][1] = m.values[1][1] * s;
	scaled.values[1][2] = m.values[2][1] * s;
	scaled.values[1][3] = m.values[3][1] * s;
	scaled.values[2][0] = m.values[0][2] * s;
	scaled.values[2][1] = m.values[1][2] * s;
	scaled.values[2][2] = m.values[2][2] * s;
	scaled.values[2][3] = m.values[3][2] * s;
	scaled.values[3][0] = m.values[0][3] * s;
	scaled.values[3][1] = m.values[1][3] * s;
	scaled.values[3][2] = m.values[2][3] * s;
	scaled.values[3][3] = m.values[3][3] * s;

	return scaled;
}

mat4 mat4_multiply(const mat4 a, const mat4 b)
{
	mat4 m;
	m.values[0][0] = a.values[0][0] * b.values[0][0] + a.values[1][0] * b.values[0][1] + a.values[2][0] * b.values[0][2] + a.values[3][0] * b.values[0][3];
	m.values[0][1] = a.values[0][1] * b.values[0][0] + a.values[1][1] * b.values[0][1] + a.values[2][1] * b.values[0][2] + a.values[3][1] * b.values[0][3];
	m.values[0][2] = a.values[0][2] * b.values[0][0] + a.values[1][2] * b.values[0][1] + a.values[2][2] * b.values[0][2] + a.values[3][2] * b.values[0][3];
	m.values[0][3] = a.values[0][3] * b.values[0][0] + a.values[1][3] * b.values[0][1] + a.values[2][3] * b.values[0][2] + a.values[3][3] * b.values[0][3];
	m.values[1][0] = a.values[0][0] * b.values[1][0] + a.values[1][0] * b.values[1][1] + a.values[2][0] * b.values[1][2] + a.values[3][0] * b.values[1][3];
	m.values[1][1] = a.values[0][1] * b.values[1][0] + a.values[1][1] * b.values[1][1] + a.values[2][1] * b.values[1][2] + a.values[3][1] * b.values[1][3];
	m.values[1][2] = a.values[0][2] * b.values[1][0] + a.values[1][2] * b.values[1][1] + a.values[2][2] * b.values[1][2] + a.values[3][2] * b.values[1][3];
	m.values[1][3] = a.values[0][3] * b.values[1][0] + a.values[1][3] * b.values[1][1] + a.values[2][3] * b.values[1][2] + a.values[3][3] * b.values[1][3];
	m.values[2][0] = a.values[0][0] * b.values[2][0] + a.values[1][0] * b.values[2][1] + a.values[2][0] * b.values[2][2] + a.values[3][0] * b.values[2][3];
	m.values[2][1] = a.values[0][1] * b.values[2][0] + a.values[1][1] * b.values[2][1] + a.values[2][1] * b.values[2][2] + a.values[3][1] * b.values[2][3];
	m.values[2][2] = a.values[0][2] * b.values[2][0] + a.values[1][2] * b.values[2][1] + a.values[2][2] * b.values[2][2] + a.values[3][2] * b.values[2][3];
	m.values[2][3] = a.values[0][3] * b.values[2][0] + a.values[1][3] * b.values[2][1] + a.values[2][3] * b.values[2][2] + a.values[3][3] * b.values[2][3];
	m.values[3][0] = a.values[0][0] * b.values[3][0] + a.values[1][0] * b.values[3][1] + a.values[2][0] * b.values[3][2] + a.values[3][0] * b.values[3][3];
	m.values[3][1] = a.values[0][1] * b.values[3][0] + a.values[1][1] * b.values[3][1] + a.values[2][1] * b.values[3][2] + a.values[3][1] * b.values[3][3];
	m.values[3][2] = a.values[0][2] * b.values[3][0] + a.values[1][2] * b.values[3][1] + a.values[2][2] * b.values[3][2] + a.values[3][2] * b.values[3][3];
	m.values[3][3] = a.values[0][3] * b.values[3][0] + a.values[1][3] * b.values[3][1] + a.values[2][3] * b.values[3][2] + a.values[3][3] * b.values[3][3];
	return m;
}
