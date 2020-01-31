
/*
 *  Matrix Stack is a stack of matrix lol
 */

#ifndef __MATRIX_STACK_H__
#define __MATRIX_STACK_H__

#include <stack>
#include <prevmath/matrix/mat4.hpp>

namespace app { namespace math {

    class MatrixStack {
    public:
        MatrixStack() : m_IsChanged(true) { m_Stack.push(prevmath::Mat4()); }
        ~MatrixStack() {}
    public:
        inline void Push(const prevmath::Mat4 & _mat) { m_Stack.push(_mat); m_IsChanged = true; }
        inline void Pop() { m_Stack.pop(); m_IsChanged = true; }
        inline prevmath::Mat4 Get() const { m_IsChanged = false; return m_Stack.top(); }

        inline size_t Size() const { return m_Stack.size(); }
        inline bool IsChanged() const { return m_IsChanged; }
    private:
        std::stack<prevmath::Mat4> m_Stack;
        mutable bool m_IsChanged;
    };

} }

#endif //__MATRIX_STACK_H__