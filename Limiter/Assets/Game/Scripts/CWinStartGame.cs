
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CWinStartGame : CBaseWindow
{
	public Button m_BtnAbout;
	public Button m_BtnAchiev;
	public Button m_BtnSetting;
	public Button m_BtnPlay;

	#region input
	public override void LockInput()
	{
		m_BtnAbout.enabled = false;
		m_BtnAchiev.enabled = false;
		m_BtnSetting.enabled = false;
		m_BtnPlay.enabled = false;
	}

	public override void UnLockInput()
	{
		m_BtnAbout.enabled = true;
		m_BtnAchiev.enabled = true;
		m_BtnSetting.enabled = true;
		m_BtnPlay.enabled = true;
	}
#endregion
}
