
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CWinExitPlay : CBaseWindow
{
	public Button m_ButtonHideWnd;
	public Button m_ButtonYes;
	public Button m_ButtonNo;

	#region input
	public override void LockInput()
	{
		m_ButtonHideWnd.enabled = false;
		m_ButtonYes.enabled = false;
		m_ButtonNo.enabled = false;
	}

	public override void UnLockInput()
	{
		m_ButtonHideWnd.enabled = true;
		m_ButtonYes.enabled = true;
		m_ButtonNo.enabled = true;
	}
	#endregion
}
