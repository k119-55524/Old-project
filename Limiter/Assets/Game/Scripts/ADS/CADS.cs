
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
//using UnityEngine.Advertisements;

// Клас вывода рекламных блоков. Работает только под IOS и Android(пока).
public class CADS : MonoBehaviour
{
	#if UNITY_IOS
		private string gameId = "1566975";
	#elif UNITY_ANDROID
		private string gameId = "1566974";
	#endif

	#if UNITY_IOS || UNITY_ANDROID
		private UFOCreator m_Creator;
	#endif

	private PlayManager m_PlayManager = null;

#if UNITY_IOS || UNITY_ANDROID
	private string placementId = "rewardedVideo";
#endif

	void Start()
	{
		f_IsReady = false;
		m_PlayManager = FindObjectOfType<PlayManager>();

		#if UNITY_IOS || UNITY_ANDROID
			m_Creator = FindObjectOfType<UFOCreator>();

			if (Advertisement.isSupported)
				Advertisement.Initialize(gameId, true);
		#endif
	}

	public bool f_IsReady { get; private set; }

	public void Update()
	{
		#if UNITY_IOS || UNITY_ANDROID
			f_IsReady = Advertisement.IsReady(placementId);
		#endif
	}

	public void ShowOfPlayADS()
	{
		#if UNITY_IOS || UNITY_ANDROID
			if (!Advertisement.IsReady(placementId))
			{
				m_PlayManager.SetPause(false);

				return;
			}

			ShowOptions options = new ShowOptions();
			options.resultCallback = HandleShowResult;

			Advertisement.Show(placementId, options);
		#endif

		#if UNITY_STANDALONE_WIN
			m_PlayManager.SetPause(false);
		#endif
	}

	public void ShowOfWinGameOverPlayADS()
	{
		f_IsReady = false;

		#if UNITY_IOS || UNITY_ANDROID
			if (!Advertisement.IsReady(placementId))
			{
				#if UNITY_STANDALONE_WIN
					m_PlayManager.SetPause(false);
				#endif

				return;
			}

			ShowOptions options = new ShowOptions();
			options.resultCallback = HandleShowResultOfGameOver;

			Advertisement.Show(placementId, options);
		#endif

		#if UNITY_STANDALONE_WIN
			m_PlayManager.SetPause(false);
		#endif
	}

	#if UNITY_IOS || UNITY_ANDROID
		void HandleShowResult(ShowResult result)
		{
			if (result == ShowResult.Finished)
			{
				m_Creator.AddADSEnergy();
			}
			//else if (result == ShowResult.Skipped)
			//{
			//		Debug.LogWarning("Video was skipped - Do NOT reward the player");
			//}
			//else if (result == ShowResult.Failed)
			//{
			//	Debug.LogError("Video failed to show");
			//}

			m_PlayManager.SetPause(false);
		}

		void HandleShowResultOfGameOver(ShowResult result)
		{
			if (result == ShowResult.Finished)
			{
				m_PlayManager.ResetLevel();
				m_PlayManager.SetPause(false);
			}
			//else if (result == ShowResult.Skipped)
			//{
			//		Debug.LogWarning("Video was skipped - Do NOT reward the player");
			//}
			//else if (result == ShowResult.Failed)
			//{
			//	Debug.LogError("Video failed to show");
			//}

			m_PlayManager.SetPause(false);
		}
	#endif
}