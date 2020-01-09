
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CAchievView : MonoBehaviour
{
	public Image image;
	public Text text;

	private Color enableAchColor;
	private Color disableAchColor;
	private CAchievements m_Ach;
	private int m_Index;

	private void Awake()
	{
		m_Ach = FindObjectOfType<CAchievements>();
		f_OK = false;
	}

	public void Setup(Color enab, Color disab, int i)
	{
		enableAchColor = enab;
		disableAchColor = disab;
		m_Index = i;
	}

	public bool f_OK { get; private set; }
	public void SetOK(bool f = true, bool fs = true)
	{
		if (m_Ach == null)
			m_Ach = FindObjectOfType<CAchievements>();

		if (f)
		{
			if (!f_OK)
				m_Ach.IncrementResult();

			f_OK = true;
			image.color = enableAchColor;
			text.color = enableAchColor;
		}
		else
		{
			if (f_OK)
				m_Ach.DecrementResult();

			f_OK = false;
			image.color = disableAchColor;
			text.color = disableAchColor;
		}

		if (fs)
			m_Ach.SetStatus(m_Index, f_OK);
	}
}
