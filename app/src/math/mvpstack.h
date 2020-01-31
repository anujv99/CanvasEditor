
#ifndef __MVP_STACK_H__
#define __MVP_STACK_H__

#include "matrixstack.h"

#include <utils/singleton.h>

namespace app { namespace math {

	class MVPStack : public utils::Singleton<MVPStack> {
		friend class utils::Singleton<MVPStack>;
	private:
		MVPStack() { m_Cache = m_Projection.Get() * m_View.Get() * m_Model.Get(); }
		~MVPStack() {}
	public:
		inline bool IsChanged() const { return (m_Model.IsChanged() || m_View.IsChanged() || m_Projection.IsChanged()); }
		prevmath::Mat4 GetMVP() const { if (IsChanged()) { RecalculateMVP(); } return m_Cache; }

		MatrixStack & Model() { return m_Model; };
		MatrixStack & View() { return m_View; };
		MatrixStack & Projection() { return m_Projection; };
	private:
		inline void RecalculateMVP() const {
			m_Cache = m_Projection.Get() * m_View.Get() * m_Model.Get();
		}
	private:
		mutable MatrixStack m_Model;
		mutable MatrixStack m_View;
		mutable MatrixStack m_Projection;

		mutable prevmath::Mat4 m_Cache;
	};

} }

#endif //__MVP_STACK_H__
