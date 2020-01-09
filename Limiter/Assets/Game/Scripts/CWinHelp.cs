
using System;
using UnityEngine;
using UnityEngine.UI;

public class CWinHelp : CBaseWindow
{
	//private CUFOGameManager m_UFOManager;

	public Button m_ButtonHideWnd;

	private void Awake()
	{
	}

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
