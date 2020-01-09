
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CWinAbout : CBaseWindow
{
	public Button m_ButtonHideWnd;

	#region input
	public override void LockInput()
	{
		m_ButtonHideWnd.enabled = false;
	}

	public override void UnLockInput()
	{
		m_ButtonHideWnd.enabled = true;
	}
	#endregion
}
