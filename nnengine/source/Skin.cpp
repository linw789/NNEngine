#include "Skin.h"
#include "Timer.h"
#include "String.h"

namespace NNE{

NNEvoid Skin::beginAnimation(const char *animName, NNEint numloop)
{
	Sid name = HashString(animName);
	m_currentAnim = m_skeleton.findAnim(name);
	m_loopNum = numloop;
}

NNEvoid Skin::animate()
{
	if (m_currentAnim != NULL && m_loopNum != 0)
	{
		NNEdouble elapse;

		if (0.0 == m_animStartTime)
		{
			m_animStartTime = gTimer.newTime();
			elapse = 0.0;

			m_skeleton.setPose(m_currentAnim, elapse);
		}
		else
		{
			elapse = gTimer.newTime() - m_animStartTime;

			if (elapse > m_currentAnim->getAnimDuration())
			{
				if (m_loopNum > 1)
				{
					m_animStartTime = gTimer.newTime();
					elapse = 0.0;
					m_loopNum--;

					m_skeleton.setPose(m_currentAnim, elapse);
				}
				else if (1 == m_loopNum)
				{
					m_loopNum--;
					m_currentAnim = NULL;
					m_animStartTime = 0.0;
				}
				else
				{
					m_animStartTime = gTimer.newTime();
					elapse = 0.0;

					m_skeleton.setPose(m_currentAnim, elapse);
				}
			}
			else
			{
				m_skeleton.setPose(m_currentAnim, elapse);
			}
		}
	}
}

} // namespace NNE