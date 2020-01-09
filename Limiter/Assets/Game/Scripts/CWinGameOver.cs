
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using RedDev.Localization;
using System.Collections.Generic;

public class CWinGameOver : CBaseWindow
{
	public Button _btnExit;
	public Button _btnYes;
	public Button _btnADS;
	public Button _btnNo;

	public Text _txtCaption;
	public Text _txtResult;

	private CADS m_ADS;

	void Start()
	{
		m_ADS = FindObjectOfType<CADS>();
	}

	private void Update()
	{
		_btnADS.interactable = m_ADS.f_IsReady;		
	}

	public void SetupUI(string whyFail, int level)
	{
		_txtCaption.text = LocalizationManager.Instance.GetValue(whyFail);
		_txtResult.text = LocalizationManager.Instance.GetValue("wgo_res_4") + level.ToString();
	}

#region input
	public override void LockInput()
	{
		_btnExit.enabled = false;
		_btnYes.enabled = false;
		_btnADS.enabled = false;
		_btnNo.enabled = false;
	}

	public override void UnLockInput()
	{
		_btnExit.enabled = true;
		_btnYes.enabled = true;
		_btnADS.enabled = true;
		_btnNo.enabled = true;
	}
#endregion
}
