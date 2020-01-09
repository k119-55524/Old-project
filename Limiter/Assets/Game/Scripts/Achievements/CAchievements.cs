
using System.Linq;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using RedDev.Localization;
using Facepunch.Steamworks;
using System.Collections.Generic;

public class CAchievements : MonoBehaviour
{
	const int countAch = 34;
	private int resultAch = 0;

	public Text caption;
	public UnityEngine.Color enableAchColor;
	public UnityEngine.Color disableAchColor;

	public CAchievView[] achView = new CAchievView[countAch];

	private PlayManager m_PlayManaer;

	void Awake()
	{
		// Не уничтожать при загрузке новых сцен
		DontDestroyOnLoad(gameObject);

		m_PlayManaer = FindObjectOfType<PlayManager>();

		StartCoroutine(IsLoadLockalize());

		Facepunch.Steamworks.Config.ForUnity(Application.platform.ToString());
		new Facepunch.Steamworks.Client(762620);

		// Проверяем, всё ли запущено правильно
		if (Client.Instance == null)
		{
			Debug.LogError("Error starting Steam!");
			return;
		}

		Client.Instance.Stats.StoreStats();

		// Печатаем базовую информацию
		Debug.Log("Steam connect.My Steam ID: " + Client.Instance.SteamId);
		//Debug.Log("My Steam Username: " + Client.Instance.Username);
		//Debug.Log("My Friend Count: " + Client.Instance.Friends.AllFriends.Count());

		StartCoroutine(LoadAsyncAch());
	}

	private IEnumerator LoadAsyncAch()
	{
		while (Client.Instance.Achievements.All.Length != countAch)
			yield return null;

		for (int i = 0; i < achView.Length; i++)
			achView[i].Setup(enableAchColor, disableAchColor, i);

		for (int i = 0; i < countAch; i++)
		{
			int ind = GetIndexAchByID("ACH_" + (i + 1).ToString());
			bool f = Client.Instance.Achievements.All[ind].State;
			achView[i].SetOK(f, false);
		}

		// Полный сброс
		//ResetAchievements();
	}

	private int GetIndexAchByID(string id)
	{
		int i = 0;
		while (Client.Instance.Achievements.All[i].Id != id)
			i++;

		return i;
	}

	public void SetStatus(int index, bool f)
	{
		if (Client.Instance != null)
		{
			if (Client.Instance.Achievements.All.Length == countAch)
			{
				if (f)
				{
					Client.Instance.Achievements.All[GetIndexAchByID("ACH_" + (index + 1).ToString())].Trigger();
				}
				else
				{
					Client.Instance.Achievements.All[GetIndexAchByID("ACH_" + (index + 1).ToString())].Reset();
				}
			}
		}
	}

	private void OnDestroy()
	{
		if (Client.Instance != null)
			// Надлежащим образом избавляемся от клиента, если этот объект уничтожен
			Client.Instance.Dispose();
	}

	private IEnumerator IsLoadLockalize()
	{
		while (!LocalizationManager.Instance._isReady)
			yield return null;

		PrintResult();
	}

	private float allTime = 0;

	private void Update()
	{
		if (Client.Instance != null)
			// Это необходимо вызывать Update для правильной работы библиотеки
			Client.Instance.Update();

		if (m_PlayManaer.e_PlayerState == PlayManager.EPlayerState.e_Play)
		{
			allTime += Time.deltaTime;

			// Если играли более 10 минут
			if (allTime >= 1.0f * 60.0f * 10.0f)
			{
				achView[0].SetOK();

				// Если играли более 1 часа
				if (allTime >= 1.0f * 60.0f * 60.0f)
				{
					achView[1].SetOK();

					// Если играли более 24 часов
					if (allTime >= 1.0f * 60.0f * 60.0f * 24.0f)
					{
						achView[2].SetOK();
					}
				}
			}
		}
	}

	public void SaveAchievements(ref Dictionary<string, object> gsetup)
	{
		//return;

		gsetup.Add("allTime", allTime);
		gsetup.Add("levelAllCount", levelAllCount);
		gsetup.Add("killAllUFO", killAllUFO);
		gsetup.Add("spentAllEnergy", spentAllEnergy);

		gsetup.Add("shipAll_round", shipAll_round);
		gsetup.Add("shipAll_oval", shipAll_oval);
		gsetup.Add("shipAll_square", shipAll_square);
		gsetup.Add("shipAll_triangular", shipAll_triangular);
		gsetup.Add("shipAll_rectangular", shipAll_rectangular);
		gsetup.Add("shipAll_rhombic", shipAll_rhombic);
		gsetup.Add("shipAll_pentagonal", shipAll_pentagonal);
		gsetup.Add("shipAll_hexagonal", shipAll_hexagonal);

		for (int i = 0; i < countAch; i++)
			gsetup.Add("ach_" + i.ToString(), achView[i].f_OK);
	}

	public void LoadAchievements(Dictionary<string, object> gsetup)
	{
		for (int i = 0; i < achView.Length; i++)
			achView[i].Setup(enableAchColor, disableAchColor, i);

        if (Client.Instance == null)
        {
            allTime = (float)gsetup["allTime"];
            levelAllCount = (int)gsetup["levelAllCount"];
            killAllUFO = (int)gsetup["killAllUFO"];
            spentAllEnergy = (int)gsetup["spentAllEnergy"];

            shipAll_round = (int)gsetup["shipAll_round"];
            shipAll_oval = (int)gsetup["shipAll_oval"];
            shipAll_square = (int)gsetup["shipAll_square"];
            shipAll_triangular = (int)gsetup["shipAll_triangular"];
            shipAll_rectangular = (int)gsetup["shipAll_rectangular"];
            shipAll_rhombic = (int)gsetup["shipAll_rhombic"];
            shipAll_pentagonal = (int)gsetup["shipAll_pentagonal"];
            shipAll_hexagonal = (int)gsetup["shipAll_hexagonal"];

			for (int i = 0; i < countAch; i++)
				achView[i].SetOK((bool)gsetup["ach_" + i.ToString()], false);
		}
	}

	#region AddAchievements
	public void IncrementResult()
	{
		resultAch++;
		PrintResult();
	}

	public void DecrementResult()
	{
		if (resultAch > 0)
		{
			resultAch--;

			PrintResult();
		}
	}

	private void PrintResult()
	{
		caption.text = LocalizationManager.Instance.GetValue("waс_caption") +
			"[" + resultAch.ToString() + "/" + countAch.ToString() + "]";
	}

	// Вызывается при (пере)старте игры для обнуления переменных сессии
	public void StartGame()
	{
		levelCurrentCount = 0;
		killCurrentUFO = 0;
		spentCurrentEnergy = 0;

		shipCurrent_round = 0;
		shipCurrent_oval = 0;
		shipCurrent_square = 0;
		shipCurrent_triangular = 0;
		shipCurrent_rectangular = 0;
		shipCurrent_rhombic = 0;
		shipCurrent_pentagonal = 0;
		shipCurrent_hexagonal = 0;
	}

	// Полный сброс всех достижений
	public void ResetAchievements()
	{
		allTime = 0.0f;
		levelAllCount = 0;
		killAllUFO = 0;
		spentAllEnergy = 0;

		shipAll_round = 0;
		shipAll_oval = 0;
		shipAll_square = 0;
		shipAll_triangular = 0;
		shipAll_rectangular = 0;
		shipAll_rhombic = 0;
		shipAll_pentagonal = 0;
		shipAll_hexagonal = 0;

		StartGame();

		foreach (CAchievView ach in achView)
			ach.SetOK(false);
	}

	private int levelCurrentCount = 0;	// Сколько уровней отыграли в текущей сессии
	private int levelAllCount = 0;		// Сколько всего уровней отыграли
	// Событие - начинаем новый уровень
	public void NewLevel()
	{
		levelCurrentCount++;
		levelAllCount++;

		// Отыграли за раз 1 уровень
		//if (levelCurrentCount > 1)
		{
			achView[3].SetOK();

			// Отыграли за раз 10 уровеней
			if (levelCurrentCount >=  10)
			{
				achView[4].SetOK();

				// Отыграли за раз 100 уровеней
				if (levelCurrentCount >= 100)
				{
					achView[5].SetOK();
				}
			}
		}

		// Всего отыграли 50 уровней
		if (levelAllCount >= 50)
		{
			achView[6].SetOK();

			// Всего отыграли 250 уровней
			if (levelAllCount >= 250)
			{
				achView[7].SetOK();

				// Всего отыграли 1000 уровней
				if (levelAllCount >= 1000)
				{
					achView[8].SetOK();
				}
			}
		}
	}

	private int killCurrentUFO = 0;				// Уничтожено кораблей за сесстию
	private int killAllUFO = 0;					// Уничтожено всего кораблей

	private int shipCurrent_round = 0;			// Уничтожено круглых кораблей за сессию
	private int shipCurrent_oval = 0;			// Уничтожено овальных кораблей за сессию
	private int shipCurrent_square = 0;			// Уничтожено квадратных кораблей за сессию	
	private int shipCurrent_triangular = 0;		// Уничтожено треугольных кораблей за сессию
	private int shipCurrent_rectangular = 0;	// Уничтожено прямоугольных кораблей за сессию
	private int shipCurrent_rhombic = 0;		// Уничтожено ромбических кораблей за сессию
	private int shipCurrent_pentagonal = 0;		// Уничтожено пятиугольных кораблей за сессию
	private int shipCurrent_hexagonal = 0;		// Уничтожено шестиугольных кораблей за сессию

	private int shipAll_round = 0;				// Уничтожено всего круглых кораблей
	private int shipAll_oval = 0;				// Уничтожено всего овальных кораблей
	private int shipAll_square = 0;				// Уничтожено всего квадратных кораблей
	private int shipAll_triangular = 0;			// Уничтожено всего треугольных кораблей
	private int shipAll_rectangular = 0;		// Уничтожено всего прямоугольных кораблей
	private int shipAll_rhombic = 0;			// Уничтожено всего ромбических кораблей
	private int shipAll_pentagonal = 0;			// Уничтожено всего пятиугольных кораблей
	private int shipAll_hexagonal = 0;			// Уничтожено всего шестиугольных кораблей

	public void KillUFO(UFOEnemyMov.UFOType type)
	{
		killCurrentUFO++;

		// Уничтожили 5 кораблей
		if (killCurrentUFO >= 5)
		{
			achView[9].SetOK();

			// Уничтожили 25 кораблей
			if (killCurrentUFO >= 25)
			{
				achView[10].SetOK();

				// Уничтожили 100 кораблей
				if (killCurrentUFO >= 100)
				{
					achView[11].SetOK();
				}
			}
		}

		killAllUFO++;
		// Уничтожили всего 50 кораблей
		if (killAllUFO >= 25)
		{
			achView[12].SetOK();

			// Уничтожили всего 500 кораблей
			if (killAllUFO >= 500)
			{
				achView[13].SetOK();

				// Уничтожили всего 1000 кораблей
				if (killAllUFO >= 1000)
				{
					achView[14].SetOK();
				}
			}
		}

		switch (type)
		{
			case UFOEnemyMov.UFOType.Round:
				shipCurrent_round++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_round >= 5)
				{
					achView[15].SetOK();
				}

				shipAll_round++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_round >= 50)
				{
					achView[16].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Oval:
				shipCurrent_oval++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_oval >= 5)
				{
					achView[17].SetOK();
				}

				shipAll_oval++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_oval >= 50)
				{
					achView[18].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Square:
				shipCurrent_square++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_square >= 5)
				{
					achView[19].SetOK();
				}

				shipAll_square++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_square >= 50)
				{
					achView[20].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Triangular:
				shipCurrent_triangular++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_triangular >= 5)
				{
					achView[21].SetOK();
				}

				shipAll_triangular++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_triangular >= 50)
				{
					achView[22].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Rectangular:
				shipCurrent_rectangular++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_rectangular >= 5)
				{
					achView[23].SetOK();
				}

				shipAll_rectangular++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_rectangular > 50)
				{
					achView[24].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Rhombic:
				shipCurrent_rhombic++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_rhombic >= 5)
				{
					achView[25].SetOK();
				}

				shipAll_rhombic++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_rhombic >= 50)
				{
					achView[26].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Pentagonal:
				shipCurrent_pentagonal++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_pentagonal >= 5)
				{
					achView[27].SetOK();
				}

				shipAll_pentagonal++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_pentagonal >= 50)
				{
					achView[28].SetOK();
				}
				break;

			case UFOEnemyMov.UFOType.Hexagonal:
				shipCurrent_hexagonal++;
				// Уничтожили 5 кораблей данного типа в текущей сесии
				if (shipCurrent_hexagonal >= 5)
				{
					achView[29].SetOK();
				}

				shipAll_hexagonal++;
				// Уничтожили всего 50 кораблей данного типа
				if (shipAll_hexagonal >= 50)
				{
					achView[30].SetOK();
				}
				break;
		}
	}

	private int spentCurrentEnergy = 0;
	private int spentAllEnergy = 0;
	public void SpentEnergy()
	{
		spentCurrentEnergy++;
		// Потратили более 500 энергии в текущей сессии
		if (spentCurrentEnergy > 500)
		{
			achView[31].SetOK();
		}

		spentAllEnergy++;
		// Всего потратили более 1000 энергии
		if (spentAllEnergy > 1000)
		{
			achView[32].SetOK();

			// Всего потратили более 10000 энергии
			if (spentAllEnergy > 10000)
			{
				achView[33].SetOK();
			}
		}
	}
	#endregion
}
