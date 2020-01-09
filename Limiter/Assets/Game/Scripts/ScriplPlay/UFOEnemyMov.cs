
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class UFOEnemyMov : MonoBehaviour
{
	public enum UFOType
	{
		Round,          // Уничтожено круглых кораблей за сессию
		Oval,           // Уничтожено овальных кораблей за сессию
		Square,         // Уничтожено квадратных кораблей за сессию	
		Triangular,     // Уничтожено треугольных кораблей за сессию
		Rectangular,    // Уничтожено прямоугольных кораблей за сессию
		Rhombic,        // Уничтожено ромбических кораблей за сессию
		Pentagonal,		// Уничтожено пятиугольных кораблей за сессию
		Hexagonal		// Уничтожено шестиугольных кораблей за сессию
	}

	private PlayManager m_PlayManager;
	private CUFOGameManager m_GameManager;

	public UFOType m_Type;
	public Rigidbody2D enemyRigibody;
    public SpriteRenderer enemySprite;
    public Color particleColor;
    public GameObject particle;
    public int enemyVelocity;

    float time;

	private SpriteRenderer m_SprRend;
    private void Start()
    {
		m_PlayManager = FindObjectOfType<PlayManager>();
		m_GameManager = FindObjectOfType<CUFOGameManager>();
		m_Achievements = FindObjectOfType<CAchievements>();

		particle = Instantiate(particle, Vector3.zero, Quaternion.identity);
		particle.GetComponent<CDelUFOPart>().SetColor(particleColor);

		m_SprRend = GetComponent<SpriteRenderer>();
	}

    private void FixedUpdate()
    {
		if (m_PlayManager.e_PlayerState != PlayManager.EPlayerState.e_Play)
		{
			enemyRigibody.simulated = false;

			return;
		}

		// переделать под единовременное обновление через sendmessage
		enemyRigibody.simulated = true;

        RandomMoviment();
        ChangeEnemyColor();
        DestroyEnemy();

		//if (enemyRigibody.velocity.magnitude > 50.0f)
		//	enemyRigibody.velocity = enemyRigibody.velocity.normalized * 50.0f;
    }

    void RandomMoviment()
    {
        int num = Random.Range(1, 5);

        switch (num)
        {
            case 1:
                enemyRigibody.AddForce(Vector2.down * enemyVelocity);
                break;
            case 2:
                enemyRigibody.AddForce(Vector2.up * enemyVelocity);
                break;
            case 3:
                enemyRigibody.AddForce(Vector2.right * enemyVelocity);
                break;
            case 4:
                enemyRigibody.AddForce(Vector2.left * enemyVelocity);
                break;
        }
    }

    void ChangeEnemyColor()
    {
        if (enemyRigibody.velocity.magnitude <= 4.5f)// 4.5
        {
            time += Time.deltaTime;

			float t = m_timeToUFODestroy - time;
			if (t < 1.0f)
			{
				if (t < 0.8f)
				{
					if (t < 0.5f)
					{
						if (t < 0.25f)
						{
							UFODim();
						}
						else
						{
							UFOBright();
						}
					}
					else
					{
						UFOBright();
					}
				}
				else
				{
					UFODim();
				}
			}
		}
        else
        {
			if (time > 0)
			{
				UFOBright();

			    time = 0;
			}
        }
    }

	private void UFOBright()
	{
		Color c = m_SprRend.color;
		c.r = 1.0f;
		c.g = 1.0f;
		c.b = 1.0f;
		c.a = 1.0f;
		m_SprRend.color = c;
	}

	private void UFODim()
	{
		Color c = m_SprRend.color;
		c.r = 0.5f;
		c.g = 0.5f;
		c.b = 0.5f;
		c.a = 1.0f;
		m_SprRend.color = c;
	}

	[Header("Время остановки UFO для уничтожения")]
	public float m_timeToUFODestroy = 3.0f;
	private CAchievements m_Achievements;

    void DestroyEnemy()
    {
        if (time > m_timeToUFODestroy)
        {
			m_PlayManager.Score();

            Vector3 particlePos = gameObject.transform.position;
            particle.transform.position = particlePos;
            particle.GetComponent<CDelUFOPart>().Play();

            Creator.isAddingLine = true;
            TimeLimited.addTime = true;

			m_PlayManager.enemyCount--;
			m_GameManager.m_KillUFO.Play();

            Debug.Log(m_PlayManager.enemyCount);

            Destroy(gameObject);
			m_Achievements.KillUFO(m_Type);
		}
    }
}
