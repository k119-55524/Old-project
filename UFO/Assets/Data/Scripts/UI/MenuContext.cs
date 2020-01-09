using System.Linq;
using UI.Context;
using UnityDI;
using UnityEngine;
using UnityEngine.UI;

namespace CommonGames.Gameplay.UI
{
	public class MenuContext : BaseContext, IDependent
	{

		public bool VisibleBackButton { get; private set; }
		public bool VisibleButtonAbout { get; private set; }
		public bool VisibleButtonAchiev { get; private set; }
		public bool VisibleButtonSettings { get; private set; }
		public bool VisibleButtonUnified { get; private set; }

		public int VisiblePanel { get; private set; }

		[Dependency("ButtonUnified")]
		public Image ButtonUnified { get; private set; }

		private Container _containerUnified;

		public void OnInjected()
		{
			
		}

		void Awake()
		{
			_containerUnified = new Container();

			_containerUnified.RegisterSceneObject<Image>("TopPanel/ButtonUnified", "ButtonUnified");
			_containerUnified.BuildUp(this);
		}

		void Start()
		{
			VisibleBackButton = false;
			VisibleButtonAbout = true;
			VisibleButtonAchiev = true;
			VisibleButtonSettings = true;
			VisibleButtonUnified = false;

			VisiblePanel = 0;
		}

		void Update()
		{

		}

		public void ShowAboutPanel()
		{
			VisiblePanel = 1;
		}

		public void ShowAchievmentsPanel()
		{
			VisiblePanel = 2;
		}

		public void ShowSettingsPanel()
		{
			VisiblePanel = 3;
		}

		public void ClickBack()
		{
			VisiblePanel = 0;
		}

		public void ClickUnified()
		{
			
		}
	}
}