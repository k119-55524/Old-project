
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;

public class UFOCreator : MonoBehaviour
{
    public GameObject linePrefab;
	public bool destroyImediately;
	public ParticleSystem m_PSDrawLine;

	private int m_StartEnergy;

    [Header("UI")]
    public Text m_ShowPointsCount;

	[Header("Rect min/max points")]
	public GameObject m_LeftUp;
	public GameObject m_RughtDown;

    private Maker[] activeLine;
    private GameObject[] lineGO;
    private PlayManager m_PlayManager;
    private CUFOGameManager m_GameManager;
    private int count = -1;
    private int objectCount = 0;
	private CAchievements m_Ach;

	public GameObject m_ImageEnergyProgressBar;
	private float m_MaxPB = 150.0f;

    private void Start()
    {
		m_PSDrawLine.Stop();
		m_GameManager = FindObjectOfType<CUFOGameManager>();
		activeLine = new Maker[m_GameManager.m_GlobalSetupData.maxEnergy];
		lineGO = new GameObject[m_GameManager.m_GlobalSetupData.maxEnergy];
		m_StartEnergy = m_GameManager.m_GlobalSetupData.startEnergy;
		m_PlayManager = FindObjectOfType<PlayManager>();
		m_Ach = FindObjectOfType<CAchievements>();
	}

	public void ResetEnergy()
	{
		m_StartEnergy = m_GameManager.m_GlobalSetupData.startEnergy;
		ShowStatus();
	}

	void LateUpdate()
    {
		if (m_PlayManager.e_PlayerState != PlayManager.EPlayerState.e_Play)
			return;

		Vector3 mousePosToWorld = Camera.main.ScreenToWorldPoint(Input.mousePosition);

		// Если нажата левая кнопка
		if (Input.GetMouseButton(0))
		{
			// Если можно рисовать линию
			if (IsDrawLine(mousePosToWorld))
				DrawLine(mousePosToWorld);
			else
				StopDrawLine(true);
		}

		if (Input.GetMouseButtonUp(0) & count > -1)
			StopDrawLine();

        DestroyOverTime();

        ShowStatus();

        LineIsOver();
    }

	// Можно ли рисовать линию
	private bool IsDrawLine(Vector3 mousePosToWorld)
	{
		if (m_StartEnergy <= 0)
			return false;

		// Если попали в игровое поле
		if (mousePosToWorld.x > m_LeftUp.transform.position.x && mousePosToWorld.x < m_RughtDown.transform.position.x &&
			mousePosToWorld.y < m_LeftUp.transform.position.y && mousePosToWorld.y > m_RughtDown.transform.position.y)
		{
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			RaycastHit2D hit = Physics2D.Raycast(ray.origin, ray.direction);
			if (hit.collider)
			{
				if (/*hit.collider.gameObject.tag == "enemy" || */hit.collider.gameObject.tag == "barrier")
					return false;
				else
					return true;
			}

			return true;
		}

		return false;
	}

	private bool f_DrawLine = false;
	private float m_UpdTime = 0;
	private void DrawLine(Vector3 mousePosToWorld)
	{
		// Если уже рисуем линию
		if (f_DrawLine)
		{
			m_UpdTime += Time.deltaTime;
			if (m_UpdTime > 0.01f)
			{
				activeLine[count].UpdateLine(mousePosToWorld);
				m_StartEnergy--;
				m_Ach.SpentEnergy();

				m_UpdTime = 0.0f;
			}

			m_PSDrawLine.transform.position = mousePosToWorld;
		}
		// иначе начинаем рисовать линию
		else
		{
			count++;

			if (m_StartEnergy >= 0)
				lineGO[count] = Instantiate(linePrefab);

			activeLine[count] = lineGO[count].GetComponent<Maker>();
			objectCount++;
			m_GameManager.m_DrawLineMusic.Play();
			m_PSDrawLine.transform.position = mousePosToWorld;
			m_PSDrawLine.Play();
			f_DrawLine = true;
		}
	}

	public void StopDrawLine(bool f = false)
	{
		if (f_DrawLine && f)
			m_GameManager.m_ShortCircuit.Play();
		if (count > -1)
			activeLine[count] = null;

		if (destroyImediately)
			Destroy(lineGO[count]);

		StopPlayDrawEffect();
		f_DrawLine = false;
	}

	public void AddEnergy()
	{
		m_StartEnergy += m_GameManager.m_GlobalSetupData.energyAdd;

		if (m_StartEnergy > m_GameManager.m_GlobalSetupData.maxEnergy)
			m_StartEnergy = m_GameManager.m_GlobalSetupData.maxEnergy;
	}

	// Энергия за полный просмотр рекламы
	public void AddADSEnergy()
	{
		m_StartEnergy += m_GameManager.m_GlobalSetupData.energyAddADS;

		if (m_StartEnergy > m_GameManager.m_GlobalSetupData.maxEnergy)
			m_StartEnergy = m_GameManager.m_GlobalSetupData.maxEnergy;
	}

    int objectIndex = -1;
    float time;

    void DestroyOverTime()
    {
        if (count > objectIndex)
        {
            time += Time.deltaTime;
            if (time > m_GameManager.m_GlobalSetupData.timeToDestroyLine ||
				objectCount >= m_GameManager.m_GlobalSetupData.maxLineObjects)
            {
                objectIndex++;
                Destroy(lineGO[objectIndex]);
                time = 0;
                objectCount--;
			}
        }
    }

    public void ShowStatus()
    {
		m_ShowPointsCount.text = m_StartEnergy.ToString();
		Vector2 sd = m_ImageEnergyProgressBar.GetComponent<RectTransform>().rect.size;

		if (m_GameManager == null)
			m_GameManager = FindObjectOfType<CUFOGameManager>();

		sd.x = m_MaxPB / (float)m_GameManager.m_GlobalSetupData.maxEnergy * (float)m_StartEnergy;
		m_ImageEnergyProgressBar.GetComponent<RectTransform>().sizeDelta = sd;
	}

	public void StopPlayDrawEffect()
	{
		m_PSDrawLine.Stop();
		m_GameManager.m_DrawLineMusic.Stop();
	}

    void LineIsOver()
    {
        if (m_StartEnergy <= 0)
			m_PlayManager.GameOver("wgo_res_3");
    }
}
