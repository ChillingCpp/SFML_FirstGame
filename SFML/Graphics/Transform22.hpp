#pragma once

#include <SFML/Graphics/BoundingBox.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class Transform22
    {
    public:
        Transform22()
        {
            m_matrix[0] = 1.f;
            m_matrix[2] = 0.f;
            m_matrix[1] = 0.f;
            m_matrix[3] = 1.f;
        };
        Transform22(float a11, float a12, float a21, float a22)
        {
            m_matrix[0] = a11;
            m_matrix[2] = a12;
            m_matrix[1] = a21;
            m_matrix[3] = a22;
        };
        void setZeros()
        {
            m_matrix[0] = 0.f;
            m_matrix[2] = 0.f;
            m_matrix[1] = 0.f;
            m_matrix[3] = 0.f;
        }
        void setIdentity()
        {
            m_matrix[0] = 1.f;
            m_matrix[2] = 0.f;
            m_matrix[1] = 0.f;
            m_matrix[3] = 1.f;
        }
        void set(float a11, float a12, float a21, float a22)
        {
            m_matrix[0] = a11;
            m_matrix[2] = a12;
            m_matrix[1] = a21;
            m_matrix[3] = a22;
        }
        Transform22 getInverse() const 
        {
            float det = m_matrix[0] * m_matrix[3] - m_matrix[2] * m_matrix[1];
            det = det != 0.0f ? 1.0f / det : 0.0f;
            float a11 = det * m_matrix[3];
            float a12 = -det * m_matrix[2];
            float a21 = -det * m_matrix[1];
            float a22 = det * m_matrix[0];
            Transform22 matrix(a11, a12, a21, a22);
            return matrix;
        }
        friend Transform22 operator*(const Transform22 &left, const Transform22 &right);
        friend Transform22 &operator*(Transform22 &left, const Transform22 &right);
        friend Vector2f operator*(const Transform22 &left, const Vector2f &right);
        const float *getMatrix() const
        {
            return m_matrix;
        }

        Transform22 &combine(const Transform22 &right)
        {
            const float *a = m_matrix;
            const float *b = right.getMatrix();

            *this = Transform22(a[0] * b[0] + a[2] * b[1],
                                a[0] * b[2] + a[2] * b[3],
                                a[1] * b[0] + a[3] * b[1],
                                a[1] * b[2] + a[3] * b[3]);
            return *this;
        }
        Vector2f transformPoint(float x, float y) const
        {
            return Vector2f(m_matrix[0] * x + m_matrix[2] * y,
                            m_matrix[1] * x + m_matrix[3] * y);
        }
        Vector2f transformPoint(const Vector2f &point) const
        {
            return transformPoint(point.x, point.y);
        }
        /// Solve A * x = b, where b is a column vector. This is more efficient
	    /// than computing the inverse in one-shot cases.
        Vector2f Solve(const Vector2f &b) const
        {
            Transform22 transform = getInverse();
            return transform.transformPoint(b);

        }

    private:
        float m_matrix[4];
    };

    Transform22 operator*(const Transform22 &left, const Transform22 &right)
    {
        return Transform22(left).combine(right);
    }
    Transform22 &operator*=(Transform22 &left, const Transform22 &right)
    {
        return left.combine(right);
    }
    Vector2f operator*(const Transform22 &left, const Vector2f &right)
    {
        return left.transformPoint(right);
    }
};