
using System;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;

public class CWinSetting : CBaseWindow
{
	private CUFOGameManager m_UFOManager;

	public Button m_ButtonHideWnd;
	public Slider sl_VolumeMusic;
	public Slider sl_VolumeSound;
	public Button m_BtnLeftLang;
	public Button m_BtnRightLang;

	private void Awake()
	{
		m_UFOManager = GameObject.FindWithTag("MainCamera").GetComponent<CUFOGameManager>();
	}

	private void Start()
	{
		f_StartSound = true;
	}

	bool f_StartSound = false;
	public void SetupUI(float volSound, CUFOGameManager.ELanguage lang)
	{
		sl_VolumeMusic.enabled = true;
		sl_VolumeSound.enabled = true;

		sl_VolumeMusic.value = m_UFOManager.GetVolumeMusic();
		sl_VolumeSound.value = volSound;
	}

	#region input
	public override void LockInput()
	{
		m_ButtonHideWnd.enabled = false;
		//sl_VolumeMusic.enabled = false;
		//sl_VolumeSound.enabled = false;
		m_BtnLeftLang.enabled = false;
		m_BtnRightLang.enabled = false;
	}

	public override void UnLockInput()
	{
		m_ButtonHideWnd.enabled = true;
		sl_VolumeMusic.enabled = true;
		sl_VolumeSound.enabled = true;
		m_BtnLeftLang.enabled = true;
		m_BtnRightLang.enabled = true;
	}

	public void OnVolumeMusicChanged()
	{
		m_UFOManager.SetVolumeMusic(sl_VolumeMusic.value);
	}

	public void OnVolumeSoundChanged()
	{
		m_UFOManager.SetVolumeSound(sl_VolumeSound.value);

		if (f_StartSound)
			m_UFOManager.m_GUIMusic.Play();
	}
	#endregion
}
