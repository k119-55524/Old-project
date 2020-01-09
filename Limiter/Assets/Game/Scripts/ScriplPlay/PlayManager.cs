
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using RedDev.Localization;
using System.Collections.Generic;
using UnityEngine.SceneManagement;

public class PlayManager : MonoBehaviour
{
	public enum EPlayerState
	{
		e_Stop,
		e_Play,
		e_Pause,
	}

	private int m_NumKillLevel;
	public Text m_StartCurrentLevel;

    [Header("UFO prefabs")]
    public GameObject[] m_UfoPref;

    [Header("Barier prefabs")]
    public GameObject[] m_BarierPref;

	private UFOCreator m_Creator;
	private CUFOGameManager m_GameManager;
    private float m_TimeInst;
	private CADS c_ADS;

	[HideInInspector] public int enemyCount;
	[HideInInspector] public EPlayerState e_PlayerState { get; private set; }

    private void Awake()
    {
		e_PlayerState = EPlayerState.e_Stop;
		m_Creator = FindObjectOfType<UFOCreator>();
		m_GameManager = FindObjectOfType<CUFOGameManager>();
		c_ADS = FindObjectOfType<CADS>();
		m_Ach = FindObjectOfType<CAchievements>();
	}

	//private Vector3 GetRNDUFOPos()
	//{
	//	return new Vector3(Random.Range(-33.0f, 33.0f), Random.Range(-25.0f, 20.0f), 0.0f);
	//}

    private void FixedUpdate()
    {
		if (e_PlayerState == EPlayerState.e_Play)
		{
			InstantiateOverTime();

			if (enemyCount > m_GameManager.m_GlobalSetupData.maxUFOInLevel)
			{
				GameOver("wgo_res_1");

				return;
			}
		}
    }

	private void AllDestroy(string tag)
	{
		GameObject[] go = GameObject.FindGameObjectsWithTag(tag);

		foreach (GameObject obj in go)
			Destroy(obj);
	}

	private void AllPartDestroy()
	{
		GameObject[] go = GameObject.FindGameObjectsWithTag("particle");

		foreach (GameObject obj in go)
		{
			CDelUFOPart cl = obj.GetComponent<CDelUFOPart>();

			if (!cl.IsPlay)
				Destroy(obj);
		}
	}

#region playmanage
	// Выводим номер уровня при старте уровня
	private IEnumerator PrintNewLevel()
	{
		m_StartCurrentLevel.gameObject.SetActive(true);
		m_StartCurrentLevel.text = LocalizationManager.Instance.GetValue("wg_sgl") + m_Level.ToString();

		yield return new WaitForSeconds(3.0f);

		m_StartCurrentLevel.gameObject.SetActive(false);
	}

	private float percAddUFO;
	private float percDecInstUFO;

	public void ResetGame()
	{
		m_Level = 1;
		enemyCount = 0;
		percAddUFO = m_GameManager.m_GlobalSetupData.numKillUFOInLevel;
		percDecInstUFO = m_GameManager.m_GlobalSetupData.timeInstantUFO;
		m_ScoreText.text = m_Level.ToString();

		if (m_Creator == null)
			m_Creator = FindObjectOfType<UFOCreator>();
		m_Creator.ShowStatus();

		if (m_GameManager == null)
			m_GameManager = FindObjectOfType<CUFOGameManager>();
		m_LevelTime = m_GameManager.m_GlobalSetupData.levelTimeLimit;

		m_TimeText.text = m_LevelTime.ToString("F0");
		m_Creator.ResetEnergy();

		AllDestroy("enemy");
		AllDestroy("line");
		AllDestroy("particle");
	}

	public void ResetLevel()
	{
		enemyCount = 0;
		m_Creator.ShowStatus();
		m_LevelTime = m_GameManager.m_GlobalSetupData.levelTimeLimit;
		m_TimeText.text = m_LevelTime.ToString("F0");
		m_Creator.ResetEnergy();

		AllDestroy("enemy");
		AllDestroy("line");
		AllDestroy("particle");
	}

	public void StartPlay()
	{
		ResetGame();
		m_GameManager.m_NewLevelMusic.Play();
		m_Ach.StartGame();

		e_PlayerState = EPlayerState.e_Play;
		m_NumKillLevel = m_GameManager.m_GlobalSetupData.numKillUFOInLevel;
		m_LevelTime = m_GameManager.m_GlobalSetupData.levelTimeLimit;

		// Добавляем барьеры после определённого уровня
		CreateBarrier();

		//for (int i = 0; i < _CountInitEnemy; i++)
		//{
			enemyCount++;
			int ran = Random.Range(0, m_UfoPref.Length);
		    Instantiate(m_UfoPref[ran], GetRNDPos(), Quaternion.identity);
		//}

		StartCoroutine(PrintNewLevel());
	}

	public void StopPlay()
	{
		ResetGame();

		AllDestroy("barrier");
		e_PlayerState = EPlayerState.e_Stop;
		m_Creator.StopPlayDrawEffect();
	}

	public void SetPause(bool f)
	{
		if (f)
			e_PlayerState = EPlayerState.e_Pause;
		else
			e_PlayerState = EPlayerState.e_Play;
	}

	private int m_ADS = 0;
	private CAchievements m_Ach;

	// Старт нового уровня
	private IEnumerator StartNewLevel()
	{
		m_Creator.StopDrawLine();
		m_GameManager.m_NewLevelMusic.Play();
		m_Ach.NewLevel();

		yield return new WaitForFixedUpdate();

		percAddUFO += percAddUFO / 100.0f * m_GameManager.m_GlobalSetupData.percentAddUFO;
		m_NumKillLevel = (int)percAddUFO;

		percDecInstUFO -= percDecInstUFO / 100.0f * m_GameManager.m_GlobalSetupData.percentDecInstUFO;

		m_Level++;
		StartCoroutine(PrintNewLevel());
		enemyCount = 0;
		m_ScoreText.text = m_Level.ToString();

		// Восстанавливаем время
		int s = m_Level;
		m_LevelTime = m_GameManager.m_GlobalSetupData.levelTimeLimit;
		while(--s != 0)
		{
			m_LevelTime -= m_LevelTime / 100.0f * (float)m_GameManager.m_GlobalSetupData.percentDecTime;
		}
		m_TimeText.text = ((int)m_LevelTime).ToString("F0");

		// Восстанавливаем энергию
		m_Creator.ResetEnergy();
		m_TimeInst = m_GameManager.m_GlobalSetupData.timeInstantUFO;
		m_GameManager.SwapFonColor();

		// Удаляем всех врагов
		AllDestroy("enemy");
		AllDestroy("line");
		AllPartDestroy();

		// Добавляем барьеры после определённого уровня
		CreateBarrier(true);

		m_ADS++;
		if (m_ADS >= m_GameManager.m_GlobalSetupData.advertisingCount)
		{
			m_ADS = 0;
			SetPause(true);
			c_ADS.ShowOfPlayADS();
		}
	}

	// Медленно скрываем и удаляем барьеры
	private IEnumerator HideBarrier()
	{
		float time = 0.0f;
		GameObject[] obj = GameObject.FindGameObjectsWithTag("barrier");
		List<CBarrier> barr = new List<CBarrier>();
		foreach(GameObject o in obj)
		{
			barr.Add(o.GetComponent<CBarrier>());

			o.GetComponent<PolygonCollider2D>().enabled = false;
		}

		yield return null;


		List<CBarrier> newbarr = new List<CBarrier>();
		foreach(GameObject o in objBarrier)
		{
			newbarr.Add(o.GetComponent<CBarrier>());
		}

		while (time < 1.0f)
		{
			time += Time.deltaTime;

			foreach (CBarrier cb in barr)
				cb.SetAlpha(1.0f - time);

			foreach (CBarrier cb in newbarr)
				cb.SetAlpha(time);

			yield return null;
		}

		foreach (GameObject o in obj)
			Destroy(o);
	}

	List<GameObject> objBarrier = new List<GameObject>();
	private void CreateBarrier(bool f_hide = false)
	{
		if (f_hide)
			StartCoroutine(HideBarrier());
		else
			AllDestroy("barrier");

		if (m_Level < m_GameManager.m_GlobalSetupData.startLevelBarrier)
			return;

		GameObject obj;
		objBarrier = new List<GameObject>();

		// Планета
		Vector3 pos = new Vector3(Random.Range(-3.0f, 3.0f), Random.Range(1.0f, 6.0f), 0.0f);
		int side = Random.Range(0, 4);
		float dx = 35.0f;
		float dy = 20.0f;
		switch (side)
		{
			case 0:
				pos.x += dx;
				pos.y += dy;
				break;

			case 1:
				pos.x -= dx;
				pos.y += dy;
				break;

			case 2:
				pos.x += dx;
				pos.y -= dy;
				break;

			default:
				pos.x -= dx;
				pos.y -= dy;
				break;
		}

		obj = Instantiate(m_BarierPref[m_BarierPref.Length - 1], pos, Quaternion.identity);
		obj.transform.Rotate(new Vector3(0.0f, 0.0f, Random.Range(0.0f, 360.0f)));
		objBarrier.Add(obj);

		int count = Random.Range(0, 10);
		for (int i = 0; i < count; i++)
		{
			obj = Instantiate(m_BarierPref[Random.Range(0, m_BarierPref.Length - 1)],
				GetRNDPos(), Quaternion.identity);
			objBarrier.Add(obj);
			obj.transform.Rotate(new Vector3(0.0f, 0.0f, Random.Range(0.0f, 360.0f)));
		}

		if (f_hide)
			foreach (GameObject o in objBarrier)
				o.GetComponent<CBarrier>().SetAlpha(0.0f);
	}

	public Vector3 GetRNDPos()
	{
		lab:
		Vector3 pos = new Vector3(Random.Range(-33.0f, 33.0f), Random.Range(-25.0f, 20.0f), 0.0f);

		// ЧоБы не пересекались
		foreach (GameObject obj in objBarrier)
			if (Vector3.Distance(obj.transform.position, pos) < 15.0f)
				goto lab;

		return pos;
	}

	public void GameOver(string whyFail)
    {
		m_GameManager.ShowWinGameOver(whyFail, m_Level);
		SetPause(true);
		m_Creator.StopPlayDrawEffect();
		//StopPlay();
	}
#endregion

	void InstantiateOverTime()
    {
        m_TimeInst += Time.deltaTime;
		m_LevelTime -= Time.deltaTime;

		// GameOver
		if (m_LevelTime < 0.0f)
		{
			GameOver("wgo_res_2");

			return;
		}

		m_TimeText.text = m_LevelTime.ToString("F0");

		if (m_TimeInst > percDecInstUFO)//m_GameManager.m_GlobalSetupData.timeInstantUFO)
        {
            enemyCount++;
            int ran = Random.Range(0, m_UfoPref.Length);
            Instantiate(m_UfoPref[ran], GetRNDPos(), Quaternion.identity);

            m_TimeInst = 0;
        }
    }

    private int m_Level;
	private float m_LevelTime;
	public Text m_ScoreText;
    public Text m_TimeText;

	public void Score()
    {
		if (e_PlayerState == EPlayerState.e_Play)
		{
			m_NumKillLevel--;

			m_LevelTime += m_GameManager.m_GlobalSetupData.timeReward;

			// new Level
			if (m_NumKillLevel == 0)
				StartCoroutine(StartNewLevel());
			else
				m_Creator.AddEnergy();
		}
	}
}
