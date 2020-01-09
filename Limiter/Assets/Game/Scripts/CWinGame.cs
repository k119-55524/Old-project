
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CWinGame : CBaseWindow
{
	public Button m_ButtonExit;
	public Button m_ButtonHelp;
	public Button m_ButtonSetting;

	private PlayManager m_PlayManager;
	private UFOCreator m_Creator;

	private void Awake()
	{
		m_PlayManager = FindObjectOfType<PlayManager>();
		m_Creator = FindObjectOfType<UFOCreator>();
	}

	public void SetupUI()
	{
		m_PlayManager.ResetGame();
		m_Creator.ResetEnergy();
	}

	#region Events
	public override void LockInput()
	{
		m_ButtonExit.enabled = false;
		m_ButtonHelp.enabled = false;
		m_ButtonSetting.enabled = false;
	}

	public override void UnLockInput()
	{
		m_ButtonExit.enabled = true;
		m_ButtonHelp.enabled = true;
		m_ButtonSetting.enabled = true;
	}

	public override void StartShow()
	{
		m_PlayManager.StartPlay();
	}

	public override void EndShow()
	{
		if (m_PlayManager.e_PlayerState == PlayManager.EPlayerState.e_Pause)
			m_PlayManager.SetPause(false);
		//else
		//	m_PlayManager.StartPlay();
	}
	#endregion
}
