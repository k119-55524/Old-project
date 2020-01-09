
using System;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;

using RedDev.Localization;

[RequireComponent(typeof(Camera))]
public class CUFOGameManager : MonoBehaviour
{
	public enum ELanguage
	{
		e_Russian = 0,
		e_English = 1,
		e_China = 2
	}

	private string[] langTypeInText = { "en_US", "ru_RU", "zh_CN" };

	[Range(0.1f, 3.0f)]
	public float m_TimeSwapWindow = 1.0f;
	public Text m_HiScore;

	[Header("Дочерние модальные окна")]
	public GameObject m_WinStartGame;
	public GameObject m_WinAbout;
	public GameObject m_WinAchievements;
	public GameObject m_WinSetting;
	public GameObject m_WinGame;
	public GameObject m_WinPlayExit;
	public GameObject m_WinHelp;
	public GameObject m_WinGameOver;
	public GameObject m_WinExitOfGame;
	
	[Header("Адреса страниц игры")]
	public string m_FacebookURL = "https://www.facebook.com/";
	public string m_TwitterURL = "https://twitter.com/";
	public string m_VKURL = "https://vk.com/";

	// Цвета фона устанавливаемые художником
	[Header("Задайте сменные цвета фона:")]
	private Color[] BackgroundColors =
	{
		new Color32(0x16, 0x0D, 0x1C, 0xff),
		new Color32(0x1C, 0x0D, 0x18, 0xff),
		new Color32(0x1E, 0x0A, 0x0A, 0xff),
		new Color32(0x1E, 0x15, 0x0D, 0xff),
		new Color32(0x1C, 0x1A, 0x0E, 0xff),
		new Color32(0x12, 0x12, 0x12, 0xff),
		new Color32(0x06, 0x17, 0x06, 0xff),
		new Color32(0x0A, 0x19, 0x18, 0xff),
		new Color32(0x0E, 0x15, 0x1E, 0xff),
		new Color32(0x13, 0x0D, 0x20, 0xff)
	};

	private int _curIndColFon = 0;
	private int _newIndColFon = 0;
	private float _swapTime;
	private Camera _backgroundCamera;
	private PlayManager m_PlayManager;
	private CADS m_ADS;

	public eTypeAnym e_TypeWinAnim { get; private set; }
	private eTypeColorAnym e_TypeColorAnim = eTypeColorAnym.e_No;

	public void SwapFonColor()
	{
		while(_newIndColFon == _curIndColFon)
			_newIndColFon = UnityEngine.Random.Range(0, BackgroundColors.Length);

		e_TypeColorAnim = eTypeColorAnym.e_SwapColor;
		_swapTime = 0;
	}

	void Start()
	{
		m_PlayManager = FindObjectOfType<PlayManager>();
		m_Achiv = FindObjectOfType<CAchievements>();
		m_ADS = FindObjectOfType<CADS>();

		LoadSetup();

		_curIndColFon = UnityEngine.Random.Range(0, BackgroundColors.Length);
		_newIndColFon = _curIndColFon;
		_backgroundCamera = GetComponent<Camera>();
		_backgroundCamera.backgroundColor = BackgroundColors[_curIndColFon];

		e_TypeWinAnim = eTypeAnym.e_No;

		if (m_LocManager == null)
			m_LocManager = FindObjectOfType<LocalizationManager>();
		StartCoroutine(m_LocManager.LoadLocalization(langTypeInText[(int)m_Language]));
	}

	private void OnDestroy()
	{
		SaveSetup();
	}

#region Animation show hide windows
	public enum eTypeAnym
	{
		e_No,
		e_ShowModalWindow,
		e_HideModalWindow
	};

	public enum eTypeColorAnym
	{
		e_No,
		e_SwapColor
	};

	private GameObject m_ParentWindow;
	private GameObject m_ChildWindow;
	private CAlphaManager c_pwBase;
	private CAlphaManager c_cwBase;

	private float m_AlfaWin;
	private float m_DT;

	private void ShowModalWindow(GameObject parentWin, GameObject childWin)
	{
		//m_DrawLineMusic.Stop();
		m_ParentWindow = parentWin;
		m_ChildWindow = childWin;
		e_TypeWinAnim = eTypeAnym.e_ShowModalWindow;
		m_AlfaWin = 0.0f;
		m_DT = 0.0f;

		m_ChildWindow.SetActive(true);
		m_ChildWindow.GetComponent<CBaseWindow>().StartShow();
		c_pwBase = m_ParentWindow.GetComponent<CAlphaManager>();
		c_cwBase = m_ChildWindow.GetComponent<CAlphaManager>();

		m_ParentWindow.GetComponent<CBaseWindow>().LockInput();
		m_ChildWindow.GetComponent<CBaseWindow>().LockInput();
	}

	private void HideModalWindow()
	{
		m_ParentWindow.SetActive(true);
		e_TypeWinAnim = eTypeAnym.e_HideModalWindow;
		m_AlfaWin = 0.0f;
		m_DT = 0.0f;

		m_ParentWindow.GetComponent<CBaseWindow>().LockInput();
		m_ChildWindow.GetComponent<CBaseWindow>().LockInput();
	}
#endregion

	private void LateUpdate()
	{
		// Если меняем фон окна
		if (e_TypeColorAnim == eTypeColorAnym.e_SwapColor)
		{
			_swapTime += Time.deltaTime;

			if (_swapTime >= 1.0f)
			{
				_swapTime = 1.0f;
				e_TypeColorAnim = eTypeColorAnym.e_No;
				_curIndColFon = _newIndColFon;
			}

			_backgroundCamera.backgroundColor = Color.Lerp( BackgroundColors[_curIndColFon],
															BackgroundColors[_newIndColFon],
															_swapTime);
		}

		if (e_TypeWinAnim != eTypeAnym.e_No)
		{
			m_DT += Time.deltaTime;

			m_AlfaWin = m_DT * (1.0f / m_TimeSwapWindow);

			if (m_DT < m_TimeSwapWindow)
			{
				if (e_TypeWinAnim == eTypeAnym.e_ShowModalWindow)
				{
					c_pwBase.SetAlpha(1.0f - m_AlfaWin);
					c_cwBase.SetAlpha(m_AlfaWin);
				}
				else
				{
					c_pwBase.SetAlpha(m_AlfaWin);
					c_cwBase.SetAlpha(1.0f - m_AlfaWin);
				}
			}
			else
			{
				if (e_TypeWinAnim == eTypeAnym.e_ShowModalWindow)
				{
					m_ParentWindow.SetActive(false);
					m_ChildWindow.SetActive(true);
					m_ParentWindow.GetComponent<CBaseWindow>().EndHide();
					m_ChildWindow.GetComponent<CBaseWindow>().EndShow();
				}
				else
				{
					m_ParentWindow.SetActive(true);
					m_ChildWindow.SetActive(false);
					m_ParentWindow.GetComponent<CBaseWindow>().EndShow();
					m_ParentWindow.GetComponent<CBaseWindow>().EndHide();
				}

				e_TypeWinAnim = eTypeAnym.e_No;
				m_ParentWindow.GetComponent<CBaseWindow>().UnLockInput();
				m_ChildWindow.GetComponent<CBaseWindow>().UnLockInput();
			}
		}
		else if (Input.GetKeyDown(KeyCode.Escape))
		{
			// Если находимся в стартовом окне
			if (m_WinStartGame.activeSelf)
			{
				m_GUIMusic.Play();
				StartCoroutine(CorShowWinGameEnd());

				return;
			}

			// Если играем
			if (m_WinGame.activeSelf)
			{
				StartCoroutine(CorShowWinPlayExit());

				return;
			}

			// Если находимся в окне About
			if (m_WinAbout.activeSelf)
			{
				OnPressBtn_HideAboutWnd();

				return;
			}

			// Если находимся в окне достижений
			if (m_WinAchievements.activeSelf)
			{
				OnPressBtn_HideAchievementsWnd();

				return;
			}

			// Если находимся в окне настроек
			if (m_WinSetting.activeSelf)
			{
				OnPressBtn_HideSettingWnd();

				return;
			}

			// Если находимся в окне выхода из игры
			if (m_WinPlayExit.activeSelf)
			{
				OnPressBtn_HidePlayExitWnd();

				return;
			}

			// Если находимся в окне окончания игры
			if (m_WinGameOver.activeSelf)
			{
				OnPressBtn_GameOver();

				return;
			}

			// Если находимся в окне помощи
			if (m_WinHelp.activeSelf)
			{
				OnPressBtn_HideHelpWnd();

				return;
			}

			// Если находимся в окне окончания игры
			if (m_WinExitOfGame.activeSelf)
			{
				m_GUIMusic.Play();
				HideModalWindow();

				return;
			}
		}
	}

	private IEnumerator CorShowWinGameEnd()
	{
		yield return null;

		ShowModalWindow(m_WinStartGame, m_WinExitOfGame);
	}

	private IEnumerator CorShowWinPlayExit()
	{
		yield return null;

		OnPressBtn_ShowWndPlayExit();
	}

#region Input
	public void OnPressBtn_YesExitOfGame()
	{
		Application.Quit();
	}

	public void OnPressBtn_NoExitOfGame()
	{
		m_GUIMusic.Play();
		HideModalWindow();
	}

	public void OnPressBtn_StartGame()
	{
		m_GUIMusic.Play();
		ShowModalWindow(m_WinStartGame, m_WinGame);
	}

	public void OnPressBtn_ShowWndAbout()
	{
		m_GUIMusic.Play();
		ShowModalWindow(m_WinStartGame, m_WinAbout);
	}

	public void OnPressBtn_HideAboutWnd()
	{
		m_GUIMusic.Play();
		HideModalWindow();
	}

	public void OnPressBtn_ShowWndAchievements()
	{
		m_GUIMusic.Play();
		ShowModalWindow(m_WinStartGame, m_WinAchievements);
	}

	public void OnPressBtn_HideAchievementsWnd()
	{
		m_GUIMusic.Play();
		HideModalWindow();
	}

	public void OnPressBtn_ShowWndSetting()
	{
		m_GUIMusic.Play();
		ShowModalWindow(m_WinStartGame, m_WinSetting);
	}

	public void OnPressBtn_ShowWndSettingOfPlay()
	{
		m_GUIMusic.Play();
		m_PlayManager.SetPause(true);
		ShowModalWindow(m_WinGame, m_WinSetting);
	}

	public void OnPressBtn_HideSettingWnd()
	{
		m_GUIMusic.Play();
		HideModalWindow();
	}

	public void OnPressBtn_ShowWndPlayExit()
	{
		m_GUIMusic.Play();
		m_PlayManager.SetPause(true);
		ShowModalWindow(m_WinGame, m_WinPlayExit);
	}

	public void OnPressBtn_HidePlayExitWnd()
	{
		m_GUIMusic.Play();
		HideModalWindow();
	}

	public void OnPressBtn_PlayExitYes()
	{
		m_GUIMusic.Play();
		m_PlayManager.StopPlay();
		ShowModalWindow(m_WinPlayExit, m_WinStartGame);
	}

	public void OnPressBtn_ShowWndHelp()
	{
		m_GUIMusic.Play();
		m_PlayManager.SetPause(true);
		ShowModalWindow(m_WinGame, m_WinHelp);
	}

	public void OnPressBtn_HideHelpWnd()
	{
		m_GUIMusic.Play();
		HideModalWindow();
	}

	public void ShowWinGameOver(string whyFail, int level)
	{
		if (Convert.ToInt32(m_HiScore.text) < level)
			m_HiScore.text = level.ToString();

		m_GameOver.Play();
		m_WinGameOver.GetComponent<CWinGameOver>().SetupUI(whyFail, level);
		ShowModalWindow(m_WinGame, m_WinGameOver);
		//m_PlayManager.ResetGame();
	}

	public void OnPressBtn_ReStartGame()
	{
		m_GUIMusic.Play();
		m_PlayManager.StopPlay();
		ShowModalWindow(m_WinGameOver, m_WinGame);
	}

	public void OnPressBtn_ViewADS()
	{
		m_ADS.ShowOfWinGameOverPlayADS();
	}

	public void OnPressBtn_GameOver()
	{
		m_GUIMusic.Play();
		m_PlayManager.StopPlay();
		ShowModalWindow(m_WinGameOver, m_WinStartGame);
	}

	public void OnPressBtn_PatriotGame()
	{
		m_GUIMusic.Play();
		Application.OpenURL("http://www.patriot-game.ru/en/");
	}

	public void OnPressBtn_Facebook()
	{
		m_GUIMusic.Play();
		Application.OpenURL(m_FacebookURL);
	}

	public void OnPressBtn_Twitter()
	{
		m_GUIMusic.Play();
		Application.OpenURL(m_TwitterURL);
	}

	public void OnPressBtn_VK()
	{
		m_GUIMusic.Play();
		Application.OpenURL(m_VKURL);
	}

	public void OnPressLeftSelLang()
	{
		int ind = (int)m_Language;
		ind--;
		if (ind < 0)
			ind = (int)ELanguage.e_China;

		m_Language = (ELanguage)ind;
		if (m_LocManager == null)
			m_LocManager = FindObjectOfType<LocalizationManager>();

		StartCoroutine(m_LocManager.LoadLocalization(langTypeInText[ind]));
	}

	public void OnPressRightSelLang()
	{
		int ind = (int)m_Language;
		ind++;
		if (ind > (int)ELanguage.e_China)
			ind = 0;

		m_Language = (ELanguage)ind;
		if (m_LocManager == null)
			m_LocManager = FindObjectOfType<LocalizationManager>();

		StartCoroutine(m_LocManager.LoadLocalization(langTypeInText[ind]));
	}
#endregion

#region LoadSaveData
	private string m_SaveFileName;
	private float m_VolumeSound;
	private ELanguage m_Language;
	private LocalizationManager m_LocManager;
	private CAchievements m_Achiv;

	public AudioSource m_FonMusic;
	public AudioSource m_NewLevelMusic;
	public AudioSource m_DrawLineMusic;
	public AudioSource m_GUIMusic;
	public AudioSource m_GameOver;
	public AudioSource m_KillUFO;
	public AudioSource m_ShortCircuit;

	public void LoadSetup()
	{
		m_Language = ELanguage.e_English;
		m_SaveFileName = Path.Combine(Application.persistentDataPath, "ufo.set");
		//m_SaveFileName = "file:/" + Application.streamingAssetsPath +  "/ufo.set";

		Dictionary<string, object> gsetup;
		FileStream file = null;

		try
		{
			BinaryFormatter formatter = new BinaryFormatter();
			file = File.Open(m_SaveFileName, FileMode.Open);
			gsetup = formatter.Deserialize(file) as Dictionary<string, object>;
			file.Close();

			m_FonMusic.volume = (float)gsetup["volumeMusic"];
			SetVolumeSound((float)gsetup["volumeSound"]);
			m_Language = (ELanguage)gsetup["Language"];
			m_HiScore.text = ((int)gsetup["bestScore"]).ToString();

			m_Achiv.LoadAchievements(gsetup);
		}
		catch (Exception /*e*/)
		{
			if (file != null)
				file.Close();

			m_FonMusic.volume = 1.0f;
			SetVolumeSound(1.0f);
			m_Language = ELanguage.e_English;
			m_HiScore.text = "1";
			m_Achiv.ResetAchievements();
		}

		StartCoroutine(CorUpdateUI());
		LoadJSON();

		//if (m_LocManager == null)
			m_LocManager = FindObjectOfType<LocalizationManager>();
		StartCoroutine(m_LocManager.LoadLocalization(langTypeInText[(int)m_Language]));
	}

	[Serializable]
	public struct sGobalSetupData
	{
		public int energyAddADS;		// Энергия, даваемая за просмотр рекламы
		public int advertisingCount;	// Через сколько уровней появляется реклама

		public int levelTimeLimit;		// Время до проигрыша уровня при старте уровня
		public int timeReward;			// Призовое время при уничтожении врага
		public int timeInstantUFO;		// Через какое время генерировать нового врага
		public int numKillUFOInLevel;   // Сколько надо уничтожить врагов на уровне
		public int maxUFOInLevel;       // Максимальное к-во врагов на уровне для 

		//public int barrierCount;		// К-во барьеров на уровне(с планетой + 1)
		public int startLevelBarrier;   // С какого уровня строить барьеры

		public int maxEnergy;			// Максимальное к-во энергии на уровне
		public int startEnergy;         // К-во энергии даваемое при старте уровня
		public int energyAdd;			// Энергия, даваемая за уничтожение врага
		public int maxLineObjects;		// Максимальное к-во линий на уровне
		public int timeToDestroyLine;   // Время жизни линии

		public int percentAddUFO;		// На сколько процентов бульше будет врагов в каждом последующем уровне
		public int percentDecInstUFO;   // На сколько процентов меньще времени будет меньше для появления нового врага в каждом последующем уровне
		public int percentDecTime;		// На сколько процентов уменьшится время следующего уровня

	}

	// Хранит к-во добавляемой призовой энергии, читаем из JSON-a
	public sGobalSetupData m_GlobalSetupData;// { get; private set; }

	private void LoadJSON()
	{
		string filePath = Path.Combine(Application.streamingAssetsPath, "ufo.json");
		//string filePath = "file://" + Application.streamingAssetsPath + "/ufo.json";

		try
		{
			if (File.Exists(filePath))
			{
				string td = File.ReadAllText(filePath);
				m_GlobalSetupData = JsonUtility.FromJson<sGobalSetupData>(td);
			}
		}
		catch (Exception /*e*/)
		{
			m_GlobalSetupData.energyAddADS		= 100;
			m_GlobalSetupData.advertisingCount	= 5;
			m_GlobalSetupData.levelTimeLimit	= 90;
	
			m_GlobalSetupData.timeReward		= 5;
			m_GlobalSetupData.timeInstantUFO	= 5;
			m_GlobalSetupData.numKillUFOInLevel	= 5;
			m_GlobalSetupData.maxUFOInLevel		= 10;
	
			//m_GlobalSetupData.barrierCount		= 3;
			m_GlobalSetupData.startLevelBarrier	= 0;
	
			m_GlobalSetupData.maxEnergy			= 500;
			m_GlobalSetupData.startEnergy		= 500;
			m_GlobalSetupData.energyAdd			= 30;
			m_GlobalSetupData.maxLineObjects	= 4;
			m_GlobalSetupData.timeToDestroyLine	= 5;
	
			m_GlobalSetupData.percentAddUFO		= 10;
			m_GlobalSetupData.percentDecInstUFO	= 5;
			m_GlobalSetupData.percentDecTime	= 5;
		}
	}

	// Так сложно стобы обновилось визуально UI
	private IEnumerator CorUpdateUI()
	{
		m_WinSetting.SetActive(true);
		m_WinGame.SetActive(true);
		m_WinSetting.GetComponent<CAlphaManager>().SetAlpha(0.0f);
		m_WinGame.GetComponent<CAlphaManager>().SetAlpha(0.0f);

		m_WinSetting.GetComponent<CWinSetting>().SetupUI(m_VolumeSound, m_Language);
		m_WinGame.GetComponent<CWinGame>().SetupUI();

		yield return null;

		m_WinSetting.GetComponent<CAlphaManager>().SetAlpha(1.0f);
		m_WinSetting.GetComponent<CAlphaManager>().SetAlpha(1.0f);
		m_WinSetting.SetActive(false);
		m_WinGame.SetActive(false);
	}

	private void SaveSetup()
	{
		Dictionary<string, object> gsetup = new Dictionary<string, object>();
		FileStream file = null;

		gsetup.Add("volumeMusic", m_FonMusic.volume);
		gsetup.Add("volumeSound", m_VolumeSound);
		gsetup.Add("Language", m_Language);
		gsetup.Add("bestScore", Convert.ToInt32(m_HiScore.text));

		m_Achiv.SaveAchievements(ref gsetup);

		try
		{
			file = File.Create(m_SaveFileName);
			BinaryFormatter formatter = new BinaryFormatter();
			formatter.Serialize(file, gsetup);
			file.Close();
		}
		catch (Exception /* e */)
		{
			if (file != null)
				file.Close();
		}
	}
#endregion

#region Sound
	public void SetVolumeMusic(float v)
	{
		m_FonMusic.volume = v;
	}

	public void SetVolumeSound(float v)
	{
		m_VolumeSound = v;

		m_NewLevelMusic.volume = v;
		m_DrawLineMusic.volume = v;
		m_GUIMusic.volume = v;
		m_GameOver.volume = v;
		m_KillUFO.volume = v;
		m_ShortCircuit.volume = v;
	}

	public float GetVolumeMusic()
	{
		return m_FonMusic.volume;
	}
#endregion
}
