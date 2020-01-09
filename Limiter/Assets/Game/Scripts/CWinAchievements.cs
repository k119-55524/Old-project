
using System;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;

public class CWinAchievements : CBaseWindow
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
